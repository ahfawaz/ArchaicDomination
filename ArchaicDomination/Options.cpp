////////////////////////////////////////////////////
// File:	COptions.cpp
// Purpose:	Handles all the options screen information
//			
//
// Author:	Joshua Ford (joshua.c.ford@gmail.com)
// Course:	SGP 1310
////////////////////////////////////////////////////

#include "Options.h"
#include "SGD Wrappers\CSGD_XAudio2.h"
#include "SGD Wrappers\CSGD_TextureManager.h"
#include "SGD Wrappers\CSGD_DirectInput.h"
#include "SGD Wrappers\CSGD_Direct3D.h"
#include "SGD Wrappers\SGD_String.h"
#include "BitmapFont.h"
#include "GamePlay.h"
#include "MainMenu.h"
#include "Acheivements.h"
#include <fstream>
#include <iostream>
#include "Game.h"
#include <Shlobj.h>

////////////////////////////////////////////////////
//Singleton
COptions* COptions::GetInstance( void )
{
	//Global Instance
	static COptions s_Instance;
	return &s_Instance;
}






//Functions for every state
bool	COptions::Input			( )					 
{
	CSGD_DirectInput*	pDI		=	CSGD_DirectInput::GetInstance();
	CSGD_Direct3D*		p3D		=	CSGD_Direct3D::GetInstance();

	// Navigate the menu
	if(   (pDI->KeyPressed( DIK_S )  || pDI->JoystickGetLStickDirPressed( DIR_DOWN, JOYSTICKID1 ) )  || ( pDI->KeyPressed( DIK_DOWN ) || pDI->JoystickGetLStickDirPressed( DIR_DOWN, JOYSTICKID2 )  ) )
	{
		++m_nCursor;
		CGame::GetInstance()->PlayMoveCursorSFX();

		if (CMainMenu::GetInstance()->GetInGame() == true)
		{
			// Stop at the bottom
			if( m_nCursor > 4 )		// EXIT
				m_nCursor = 0;

		}
		else
		{
			// Stop at the bottom
			if( m_nCursor > 3 )		// EXIT
				m_nCursor = 0;

		}

	}
	else if(  (pDI->KeyPressed( DIK_W )  || pDI->JoystickGetLStickDirPressed( DIR_UP, JOYSTICKID1 ) )  || ( pDI->KeyPressed( DIK_UP ) || pDI->JoystickGetLStickDirPressed( DIR_UP, JOYSTICKID2 )  ) )
	{
		--m_nCursor;

		CGame::GetInstance()->PlayMoveCursorSFX();
		if (CMainMenu::GetInstance()->GetInGame() == true)
		{
			// Stop at the bottom
			if( m_nCursor < 0 )		// EXIT
				m_nCursor = 4;

		}
		else
		{
			// Stop at the top
			if( m_nCursor < 0 )		// EXIT
				m_nCursor = 3;

		}
	}


	if ((  ( (pDI->KeyDown( DIK_D )  || pDI->JoystickGetLStickDirDown( DIR_RIGHT, JOYSTICKID1 ) ) ) || ( pDI->KeyDown( DIK_RIGHT ) || pDI->JoystickGetLStickDirDown( DIR_RIGHT, JOYSTICKID2 ) )  ) && m_nCursor == 0)
	{
		if (m_nMasterMusicVolume < 1.0f)
		{
			m_nMasterMusicVolume += 0.0005f;
		}
		else
			m_nMasterMusicVolume = 1.0f;
		CGame::GetInstance()->SetVolumeLevel( m_nMasterMusicVolume );

	}
	else if ((  ( (pDI->KeyDown( DIK_D )  || pDI->JoystickGetLStickDirDown( DIR_RIGHT, JOYSTICKID1 ) ) ) || ( pDI->KeyDown( DIK_RIGHT ) || pDI->JoystickGetLStickDirDown( DIR_RIGHT, JOYSTICKID2 ) )  ) && m_nCursor == 1)
	{
		if (m_nMasterSFXVolume < 1.0f)
		{
			m_nMasterSFXVolume += 0.0005f;
		}
		else
			m_nMasterSFXVolume = 1.0f;


		CGame::GetInstance()->SetSFXVolumeLevel( m_nMasterSFXVolume );


	}

	if ((  ( (pDI->KeyDown( DIK_A )  || pDI->JoystickGetLStickDirDown( DIR_LEFT, JOYSTICKID1 ) ) ) || ( pDI->KeyDown( DIK_LEFT ) || pDI->JoystickGetLStickDirDown( DIR_LEFT, JOYSTICKID2 ) )  ) && m_nCursor == 0)
	{
		if (m_nMasterMusicVolume > 0.0f)
		{
			m_nMasterMusicVolume -= 0.0005f;
		}
		else
			m_nMasterMusicVolume = 0.0f;

		CGame::GetInstance()->SetVolumeLevel( m_nMasterMusicVolume );

	}
	else if ((  ( (pDI->KeyDown( DIK_A )  || pDI->JoystickGetLStickDirDown( DIR_LEFT, JOYSTICKID1 ) ) ) || ( pDI->KeyDown( DIK_LEFT ) || pDI->JoystickGetLStickDirDown( DIR_LEFT, JOYSTICKID2 ) )  ) && m_nCursor == 1)
	{
		if (m_nMasterSFXVolume > 0.0f)
		{
			m_nMasterSFXVolume -= 0.0005f;
		}
		else
			m_nMasterSFXVolume = 0.0f;

		CGame::GetInstance()->SetSFXVolumeLevel( m_nMasterSFXVolume );


	}
	// Handle the Enter key
	if( ( pDI->KeyPressed( DIK_TAB ) || pDI->JoystickButtonPressed( 1, JOYSTICKID1 ) ) || (pDI->KeyPressed( DIK_RETURN ) || pDI->JoystickButtonPressed( 1, JOYSTICKID2 )  ) )
	{
		// What selection?

		if (CMainMenu::GetInstance()->GetInGame() == true)
		{
			switch( m_nCursor )
			{

			case 2:
				{
					m_bIsWindowed = !m_bIsWindowed;
					CGame::GetInstance()->SetIsWindowed(m_bIsWindowed);
					if (m_bIsWindowed)
					{
						p3D->Resize( m_nScreenWidth, m_nScreenHeight, m_bIsWindowed );
					}
					else 
					{
						p3D->Resize( m_nScreenWidth, m_nScreenHeight, m_bIsWindowed );
					}
				}
				break;
			case 3:
				{
					CGame::GetInstance()->PushState( CAcheivements::GetInstance() );
					return true;
				}
				break;
			case 4:
				{
					CGame::GetInstance()->PopState();
					return true;

				}
				break;
			}
		}
		else
		{
			switch( m_nCursor )
			{

			case 2:
				{
					m_bIsWindowed = !m_bIsWindowed;
					CGame::GetInstance()->SetIsWindowed(m_bIsWindowed);
					if (m_bIsWindowed)
					{
						p3D->Resize( m_nScreenWidth, m_nScreenHeight, m_bIsWindowed );
					}
					else 
					{
						p3D->Resize( m_nScreenWidth, m_nScreenHeight, m_bIsWindowed );
					}
				}
				break;
			case 3:
				{
					CGame::GetInstance()->PopState();
					return true;

				}
				break;
			}
		}
	}

	///Escape to exit
	if (pDI->JoystickButtonPressed( 3, 0 ) || pDI->JoystickButtonPressed( 3, 1 ) )
	{
		if (CMainMenu::GetInstance()->GetInGame() == true)
		{
			// Stop at the bottom
			if( m_nCursor < 0 )		// EXIT
				m_nCursor = 3;
			return true;
		}
		else
		{
			// Stop at the top
			if( m_nCursor < 0 )		// EXIT
				m_nCursor = 4;
			return true;
		}
	}
	else if (pDI->KeyPressed( DIK_ESCAPE ) == true || pDI->JoystickButtonPressed( 2, JOYSTICKID1 ) == true)
	{
		if (CMainMenu::GetInstance()->GetInGame() == true)
		{

			m_nCursor = 4;
			CGame::GetInstance()->PlayCancelSFX();
			return true;
		}
		else
		{

			m_nCursor = 3;
			CGame::GetInstance()->PlayCancelSFX();
			return true;
		}
	}
	// Keep running
	return true;


}
void	COptions::Update			( float fElapsedTime ) 
{
	CSGD_XAudio2* pXA =  CSGD_XAudio2::GetInstance();

	//if (m_nTime <= 2.0f)
	//{
	//	m_nTime += fElapsedTime;
	//}
	//else
	//{
	//	m_nTime = 0.0f;
	//	if (!pXA->SFXIsSoundPlaying( CGame::GetInstance()->GetSFXID() ) && m_nCursor == 1 )
	//	{
	//		pXA->SFXPlaySound( CGame::GetInstance()->GetSFXID() );
	//	}
	//}


}
void	COptions::Render( )			
{
	CSGD_TextureManager * pTM = CSGD_TextureManager::GetInstance();
	CBitmapFont* pFont = CGame::GetInstance()->GetFont();

	pTM->Draw( m_nBackgroundImageID, 0, 0, 1.0f, 1.0f );


	pFont->Draw( _T("Options"), (int)((640 - (7*32*0.85f)) / 2), 100, 1.85f, D3DCOLOR_ARGB(255, 0, 0, 255) );

	if(m_nCursor == 0)
		pTM->Draw(SelectedButtonID,(640 - (15*32)) / 2, 184, 1.0f, 0.7f);
	else
		pTM->Draw(nonSelectedButtonID,(640 - (15*32)) / 2, 184, 1.0f, 0.7f);
	pFont->Draw( _T("Music Volume"), (640 - (10*32)) / 2, 200, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );

	if(m_nCursor == 1)
		pTM->Draw(SelectedButtonID,(640 - (15*32)) / 2, 234, 1.0f, 0.7f);
	else
		pTM->Draw(nonSelectedButtonID,(640 - (15*32)) / 2, 234, 1.0f, 0.7f);
	pFont->Draw( _T("SFX Volume"), (640 - (10*32)) / 2, 250, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );

	if(m_nCursor == 2)
		pTM->Draw(SelectedButtonID,(640 - (15*32)) / 2, 284, 1.0f, 0.7f);
	else
		pTM->Draw(nonSelectedButtonID,(640 - (15*32)) / 2, 284, 1.0f, 0.7f);
	pFont->Draw( _T("Full Screen" ), (640 - (10*32)) / 2, 300, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );

	if (CMainMenu::GetInstance()->GetInGame() == true)
	{

		if(m_nCursor == 3)
			pTM->Draw(SelectedButtonID,(640 - (15*32)) / 2, 334, 1.0f, 0.7f);
		else
			pTM->Draw(nonSelectedButtonID,(640 - (15*32)) / 2, 334, 1.0f, 0.7f);
		pFont->Draw( _T("Acheivements"), (640 - (10*32)) / 2, 350, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );

		if(m_nCursor == 4)
			pTM->Draw(SelectedButtonID,(640 - (15*32)) / 2, 384, 1.0f, 0.7f);
		else
			pTM->Draw(nonSelectedButtonID,(640 - (15*32)) / 2, 384, 1.0f, 0.7f);
		pFont->Draw( _T("Exit"), (640 - (10*32)) / 2, 400, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );

	}
	else
	{
		if(m_nCursor == 3)
			pTM->Draw(SelectedButtonID,(640 - (15*32)) / 2, 334, 1.0f, 0.7f);
		else
			pTM->Draw(nonSelectedButtonID,(640 - (15*32)) / 2, 334, 1.0f, 0.7f);
		pFont->Draw( _T("Exit"), (640 - (10*32)) / 2, 350, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );

	}

	//Render Cursor Position
	//pTM->Draw(cursorImageID, (640 - (18*32)) / 2 , 200 + (m_nCursor *50), 0.3f, 0.3f, nullptr, 35.0f / 2, 72.0f / 2, D3DX_PI / -2);

	RECT MusicVolumeBoundries = { 400, 200, 500, 225 };

	CSGD_Direct3D::GetInstance()->DrawHollowRect( MusicVolumeBoundries, D3DCOLOR_XRGB( 255, 0, 0 ) );

	RECT RectFillMusic = { (long)(399), (long)(201), (long)(m_nMasterMusicVolume * 100 + 400)  , (long)(224) };

	CSGD_Direct3D::GetInstance()->DrawRect( RectFillMusic, D3DCOLOR_XRGB( 0, 0, 255 ) );




	RECT SFXVolumeBoundries		=	{ 400, 250, 500, 275 };

	CSGD_Direct3D::GetInstance()->DrawHollowRect( SFXVolumeBoundries, D3DCOLOR_XRGB( 255, 0, 0 ) );

	RECT RectFillSFX = { 399, 251, (long)(m_nMasterSFXVolume * 100 + 400)  , 274 };

	CSGD_Direct3D::GetInstance()->DrawRect( RectFillSFX, D3DCOLOR_XRGB( 0, 0, 255 ) );





	RECT FullScreenHollow		=	{ 400, 300, 500, 325 };

	CSGD_Direct3D::GetInstance()->DrawHollowRect( FullScreenHollow, D3DCOLOR_XRGB( 0, 0, 255 ) );

	if (m_bIsWindowed)
	{
		RECT RectFullScreenoff =  { 450, 301, 499 , 324 };

		CSGD_Direct3D::GetInstance()->DrawRect( RectFullScreenoff, D3DCOLOR_XRGB( 255, 0, 0 ) );

	}
	else
	{
		RECT RectFullScreenOn = { 401, 301, 450 , 324 };

		CSGD_Direct3D::GetInstance()->DrawRect( RectFullScreenOn, D3DCOLOR_XRGB( 0, 255, 0 ) );

	}



	TOSTRINGSTREAM outputMusic;
	outputMusic << static_cast<int>(m_nMasterMusicVolume * 100);

	pFont->Draw( outputMusic.str().c_str(), 550 , 200, 1.0f, D3DCOLOR_ARGB( 255, 0, 0, 255 ) ); 

	TOSTRINGSTREAM outputSFX;
	outputSFX << static_cast<int>(m_nMasterSFXVolume * 100);

	pFont->Draw( outputSFX.str().c_str(), 550 , 250, 1.0f, D3DCOLOR_ARGB( 255, 0, 0, 255 ) ); 

}
void	COptions::Enter			( )		
{
	//LoadOptions();

	//CGame::GetInstance()->SetVolumeLevel( m_nMasterMusicVolume );
	//CGame::GetInstance()->SetSFXVolumeLevel( m_nMasterSFXVolume );
	m_nTime = 0.0f;

	nonSelectedButtonID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/Option_Background.png"));
	SelectedButtonID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/Selected_Option_Background.png"));
	m_nBackgroundImageID =  CSGD_TextureManager::GetInstance()->LoadTexture( _T( "resources/graphics/Background_MainMenu.png") );
	//Cursor Image ID Set

	m_nMasterMusicVolume	=	CGame::GetInstance()->GetVolumeLevel();
	m_nMasterSFXVolume		=	CGame::GetInstance()->GetSFXVolumeLevel();
	m_bIsWindowed			=	CGame::GetInstance()->GetIsWindowed();
	m_nScreenWidth			=	CGame::GetInstance()->GetScreenWidth();
	m_nScreenHeight			=	CGame::GetInstance()->GetScreenHeight();


	m_nCursor = 0;
}
void	COptions::Exit			( )	
{
	CSGD_TextureManager::GetInstance()->UnloadTexture(nonSelectedButtonID);
	CSGD_TextureManager::GetInstance()->UnloadTexture(SelectedButtonID);
	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nBackgroundImageID);
	SaveOptions();
}

void COptions::SaveOptions() const
{
	TCHAR * wszPath = NULL;


	HRESULT hr = SHGetKnownFolderPath( FOLDERID_LocalAppData, 0, 0, &wszPath);

	if(SUCCEEDED(hr))
	{
		wstring name = wszPath;
		name += L"\\ArchaicDomination\\Volume.txt";

		std::fstream fs;
		fs.open(name, std::ios_base::out | std::ios_base::trunc);
		if (fs.is_open())
		{

			fs << m_nMasterMusicVolume << '\n' << m_nMasterSFXVolume << '\n';
			if(m_bIsWindowed)
				fs << "true";
			else
				fs << "false";

		}
		fs.close();
	}			
}

void COptions::LoadOptions( void )
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
