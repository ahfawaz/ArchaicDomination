/**************************************************
|	File:		Game.cpp
|	Author:		Joshua Ford(joshuacford@fullsail.edu
|	Course:		SGD 1308
|	Purpose:	CGame class initializes SGD wrappers
|				& handles the game state machine
**************************************************/


#include "Game.h"

#include "SGD Wrappers/CSGD_Direct3D.h"
#include "SGD Wrappers/CSGD_DirectInput.h"
#include "SGD Wrappers/CSGD_TextureManager.h"
#include "SGD Wrappers/CSGD_XAudio2.h"
#include "SGD Wrappers/SGD_String.h"
#include "SaveGame.h"
#include "IGameState.h"
#include "MainMenu.h"
#include "TitleScreen.h"
#include "Pause.h"
#include "Animation System\AnimationSystem.h"

#include <fstream>
#include <cassert>
#include <ctime>


#include "SGD Wrappers\CSGD_XAudio2.h"

#include <Shlobj.h>

/*************************************************/
// SINGLETON:

// Instantiate the static member
/*static*/ CGame* CGame::s_pInstance = nullptr;

// GetInstance
//	- returns the ONE singleton object
/*static*/ CGame* CGame::GetInstance( void )
{
	// Allocate the static pointer if necessary
	if( s_pInstance == nullptr )
		s_pInstance = new CGame;

	// Return the singleton
	return s_pInstance;
}

// DeleteInstance
//	- deallocate the singleton object
/*static*/ void CGame::DeleteInstance( void )
{
	delete s_pInstance;
	s_pInstance = nullptr;
}


/*************************************************/
// Constructor
//	- set all the data members to 'clean' values
CGame::CGame(void)
{
	m_pD3D			= nullptr;
	m_pDI			= nullptr;
	m_pTM			= nullptr;
	m_pXA			= nullptr;

	m_pFont			= nullptr;

	m_nFullScreenHeight = 600;
	m_nFullScreenWidth = 800;
	m_nScreenWidth	= 1;
	m_nScreenHeight	= 1;
	m_bIsWindowed	= true;

	m_pCurrState.clear();

	m_dwCurrTime	= 0;

	m_nbackgroundmusicID = -1;
	m_nSFXMusicID		= -1;

	m_nMasterMusicVolume = 0.75f;
	m_nMasterSFXVolume = 0.75f;

	// seed the random number generator
	srand( (unsigned int)time( nullptr ) );


	m_nFPS			= 0;
	m_nFrames		= 0;
	m_fFPSTimer		= 0;
}

/*************************************************/
// Destructor
CGame::~CGame(void)
{
}


/*************************************************/
// Initialize
//	- setup the SGD wrappers
void CGame::Initialize( HWND hWnd, HINSTANCE hInstance,
					   int nWidth, int nHeight,
					   bool bIsWindowed )
{
	CreateAppFiles();
	// Store the SGD Wrapper singletons
	m_pD3D			= CSGD_Direct3D::GetInstance();
	m_pDI			= CSGD_DirectInput::GetInstance();
	m_pTM			= CSGD_TextureManager::GetInstance();
	m_pXA			= CSGD_XAudio2::GetInstance();

	//Loading Saved Settings
	LoadSettings();

	// Store the parameters
	m_nScreenWidth	= nWidth;
	m_nScreenHeight	= nHeight;
	//m_bIsWindowed	= bIsWindowed;


	// Initialize the wrappers
	m_pD3D->Initialize( hWnd, m_nScreenWidth, m_nScreenHeight, m_bIsWindowed, false );
	m_pDI->Initialize( hWnd, hInstance, DI_KEYBOARD | DI_MOUSE | DI_JOYSTICKS, DI_MOUSE );
	m_pTM->Initialize( m_pD3D->GetDirect3DDevice(),	m_pD3D->GetSprite() );
	m_pXA->Initialize();


	//Initialize the Font
	m_pFont = new CBitmapFont;
	m_pFont->Initialize();


	//Music Initialized and playing
	m_nbackgroundmusicID	= m_pXA->MusicLoadSong( _T("Resources/Sounds/GameMusic.xwm" ) );

	m_nOverworldMusicID		= m_pXA->MusicLoadSong( _T("Resources/Sounds/GameMusic.xwm" ) );
	m_nBattleMusicID		= m_pXA->MusicLoadSong( _T("Resources/Sounds/GameMusic.xwm" ) );

	//SFX Initialized
	SFXCursorMoveID			= m_pXA->SFXLoadSound(_T("Resources/Sounds/cursor_move.wav"));
	SFXCancelID				= m_pXA->SFXLoadSound(_T("Resources/Sounds/exit_option.wav"));
	SFXInvalidInputID		= m_pXA->SFXLoadSound(_T("Resources/Sounds/AD_Error_Final.wav"));
	SFXUndeadMoveID			= m_pXA->SFXLoadSound(_T("Resources/Sounds/AD_DeadWalk_Final.wav"));
	SFXHumanMoveID			= m_pXA->SFXLoadSound(_T("Resources/Sounds/AD_HumanWalk_Final.wav"));

	//m_nSFXMusicID		= m_pXA->SFXLoadSound( _T("resource/sounds/JCFDie.wav" ) );

	//Animation System Initializing
	animeSystem = new CAnimationSystem();
	animeSystem->LoadedAnimation("Resources/XMLFiles/Player_Animation.xml");
	animeSystem->LoadedAnimation("Resources/XMLFiles/Enemy_Animation.xml");
	animeSystem->LoadedAnimation("Resources/XMLFiles/Player_Move_Animation.xml");
	animeSystem->LoadedAnimation("Resources/XMLFiles/Enemy_Move_Animation.xml");

	m_pXA->MusicPlaySong( m_nbackgroundmusicID , true);
	
	m_pXA->MusicSetMasterVolume( m_nMasterMusicVolume );

	// Start in the Main Menu state
	PushState( CTitleScreen::GetInstance() );

	// Store the current time
	m_dwCurrTime	= GetTickCount();
}

/*************************************************/
// Main
//	- run one frame of the gameplay
bool CGame::Main( void )
{
	// Check input
	if( Input() == false )
		return false;

	// Update & render
	Update();
	Render();
	return true;
}

/*************************************************/
// Terminate
//	- shutdown the SGD wrappers (in REVERSE ORDER)
void CGame::Terminate( void )
{
	// Exit the current state
	ClearStates();

	m_pXA->SFXUnloadSound(SFXCursorMoveID);		
	m_pXA->SFXUnloadSound(SFXCancelID);	
	m_pXA->SFXUnloadSound(SFXInvalidInputID);	
	m_pXA->MusicUnloadSong(m_nbackgroundmusicID);
	m_pXA->SFXUnloadSound(SFXUndeadMoveID);
	m_pXA->SFXUnloadSound(SFXHumanMoveID);

	//Animation System Shutdown
	delete animeSystem;
	animeSystem = nullptr;

	// Shutdown the font
	if( m_pFont != nullptr )
	{
		m_pFont->Terminate();
		delete m_pFont;
		m_pFont = nullptr;
	}


	// Shutdown the wrappers
	m_pXA->Terminate();
	m_pTM->Terminate();
	m_pDI->Terminate();
	m_pD3D->Terminate();


	// Reset the data members to 'clean' values
	m_pD3D			= nullptr;
	m_pDI			= nullptr;
	m_pTM			= nullptr;
	m_pXA			= nullptr;

	m_nScreenWidth	= 1;
	m_nScreenHeight	= 1;
	m_bIsWindowed	= true;
}


/*************************************************/
// Input
//	- handle user input
bool CGame::Input( void )
{
	// Read the devices
	m_pDI->ReadDevices();

	if (m_pDI->KeyDown( DIK_LALT ) == true && m_pDI->KeyDown( DIK_RETURN ) == true)
	{
		//Redundant code here
		m_bIsWindowed = !m_bIsWindowed;
		if (m_bIsWindowed)
		{
			m_pD3D->Resize( m_nScreenWidth, m_nScreenHeight, m_bIsWindowed );
		}
		else 
		{
			m_pD3D->Resize( m_nScreenWidth, m_nScreenHeight, m_bIsWindowed );
		}

	}
	if (  m_pDI->KeyDown( DIK_LALT ) == true && m_pDI->KeyPressed( DIK_TAB ) == true )
	{
		CGame::GetInstance()->PushState( CPause::GetInstance() );
		return true; 
	}
	if ( ( m_pDI->KeyDown( DIK_LALT ) == true || m_pDI->KeyDown( DIK_RALT ) == true )&& m_pDI->KeyDown( DIK_F4 ) == true )
	{
		return false;
	}
	return m_pCurrState[m_pCurrState.size()- 1]->Input();	// current state handles input
}

////////////////////////////////////////////////////////////////////////////
// Update
//	- update the game
void CGame::Update( void )
{
	// Update the audio
	m_pXA->Update();


	// Calculate the elapsed time between frames
	DWORD dwNow = GetTickCount();
	float fElapsedTime = (dwNow - m_dwCurrTime) / 1000.0f;
	m_dwCurrTime = dwNow;

	// Update the FPS
	++m_nFrames;
	m_fFPSTimer += fElapsedTime;

	if( m_fFPSTimer >= 1.0f )
	{
		m_nFPS		= m_nFrames;
		m_nFrames	= 0;
		m_fFPSTimer = 0.0f;
	}


	// Cap the elapsed time to 1/8th
	if( fElapsedTime > 0.125f )
		fElapsedTime = 0.125f;


	m_pXA->MusicSetMasterVolume( m_nMasterMusicVolume );
	m_pXA->SFXSetMasterVolume( m_nMasterSFXVolume );
	// Current state updates
	m_pCurrState[m_pCurrState.size()- 1]->Update( fElapsedTime );
}

/*************************************************/
// Render
//	- render the game
void CGame::Render( void )
{
	// Clear the backbuffer
	m_pD3D->Clear( D3DCOLOR_XRGB( 50, 50, 50 ) );	// Dark gray

	// Begin rendering
	m_pD3D->DeviceBegin();
	m_pD3D->SpriteBegin();
	{
		// Current state renders
		for (unsigned int i = 0; i < m_pCurrState.size(); i++)
		{
			m_pCurrState[i]->Render();
		}

	}
	//	// Draw the FPS
	//TOSTRINGSTREAM output;
	//output << _T("FPS: ") << m_nFPS;
	//m_pFont->Draw( output.str().c_str(), 0, 0, 2.0f, D3DCOLOR_XRGB(255, 255, 255) );	// white

	m_pD3D->SpriteEnd();
	m_pD3D->DeviceEnd();




	// Present the backbuffer to the screen
	m_pD3D->Present();
}


/*************************************************/
// ChangeState
//	- switch between states
void CGame::ChangeState( IGameState* pState )
{

	ClearStates();
	PushState( pState );
}

void CGame::PushState	( IGameState* pState )
{
	m_pCurrState.push_back( pState );

	m_pCurrState[m_pCurrState.size() - 1]->Enter();


}
void CGame::PopState	( void )
{
	if (m_pCurrState.size() != 0)
	{
		m_pCurrState[m_pCurrState.size() -1]->Exit();
		m_pCurrState.pop_back();
	}

}
void CGame::ClearStates( void )
{
	for (unsigned int i = 0; i < m_pCurrState.size(); i++)
	{
		m_pCurrState[i]->Exit();
	}
	m_pCurrState.clear();
}

//Play Move Cursor SFX
void CGame::PlayMoveCursorSFX(void)
{
	if(m_pXA->SFXIsSoundPlaying(SFXCursorMoveID))
		m_pXA->SFXStopSound(SFXCursorMoveID);
	m_pXA->SFXPlaySound(SFXCursorMoveID);
}

//Play Cancel Cursor SFX
void CGame::PlayCancelSFX(void)
{
	if(m_pXA->SFXIsSoundPlaying(SFXCancelID))
		m_pXA->SFXStopSound(SFXCancelID);
	m_pXA->SFXPlaySound(SFXCancelID);
}

void CGame::PlayInvalidInputSFX(void)
{
	if(m_pXA->SFXIsSoundPlaying(SFXInvalidInputID))
		m_pXA->SFXStopSound(SFXInvalidInputID);
	m_pXA->SFXPlaySound(SFXInvalidInputID);
}

void CGame::PlayUndeadMoveSFX(void)
{
	if(m_pXA->SFXIsSoundPlaying(SFXUndeadMoveID))
		m_pXA->SFXStopSound(SFXUndeadMoveID);
	m_pXA->SFXPlaySound(SFXUndeadMoveID);
}

void CGame::PlayHumanMoveSFX(void)
{
	if(m_pXA->SFXIsSoundPlaying(SFXHumanMoveID))
		m_pXA->SFXStopSound(SFXHumanMoveID);
	m_pXA->SFXPlaySound(SFXHumanMoveID);
}

wstring CGame::SaveFolder()
{
	TCHAR * wszPath = NULL;


	HRESULT hr = SHGetKnownFolderPath( FOLDERID_LocalAppData, 0, 0, &wszPath);

	if(SUCCEEDED(hr))
	{
		wstring name = wszPath;
		name += L"\\ArchaicDomination";


		DWORD path = GetFileAttributes(name.c_str());

		if(path == INVALID_FILE_ATTRIBUTES)
		{
			hr = CreateDirectory(name.c_str(),0);
			if(!SUCCEEDED(hr))
			{
				assert(false && "did not create the folder successfully");

			}
		}

		return name;
	}
	else
	{
		assert(false && "Could not find ApData Folder");

	}
	return L"";


}

void CGame::CreateAppFiles()
{
	//TCHAR * wszPath = NULL;
	//HRESULT hr = SHGetKnownFolderPath( FOLDERID_LocalAppData, 0, 0, &wszPath);

	wstring foldername = SaveFolder();
	

		wstring saveslot = foldername + L"\\Slots";


		DWORD path = GetFileAttributes(saveslot.c_str());

		if(path == INVALID_FILE_ATTRIBUTES)
		{

			vector<tPlayerSaveInfo>vplayer1;

			tPlayerSaveInfo p1 = {};

			vplayer1.push_back(p1);


			//TODO: create save slots
			string slotname(saveslot.begin(), saveslot.end());




		}




	



	//Todo: Save Options




}

void CGame::LoadSettings(void)
{
	TCHAR * wszPath = NULL;


	HRESULT hr = SHGetKnownFolderPath( FOLDERID_LocalAppData, 0, 0, &wszPath);

	if(SUCCEEDED(hr))
	{
		wstring name = wszPath;
		name += L"\\ArchaicDomination\\Volume.txt";

		std::ifstream ifs;
		ifs.open(name);
		float tempMusicVolume;
		float tempSFXVolume;
		std::string tempIsWindowed;

		if (ifs.is_open())
		{

			ifs >> tempMusicVolume;
			//ifs.ignore(INT_MAX, '\n');
			ifs >> tempSFXVolume;
			ifs >> tempIsWindowed;

			m_nMasterMusicVolume = tempMusicVolume;
			m_nMasterSFXVolume = tempSFXVolume;
			if(tempIsWindowed == "true")
				m_bIsWindowed = true;
			else
				m_bIsWindowed = false;

		}

		ifs.close();


	}
}
