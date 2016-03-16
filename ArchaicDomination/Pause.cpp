#include "Pause.h"
#include "SaveGame.h"
#include"SGD Wrappers\CSGD_XAudio2.h"
#include "SGD Wrappers\CSGD_DirectInput.h"
#include "SGD Wrappers\CSGD_TextureManager.h"
#include "BitmapFont.h"
#include "Game.h"
#include "Options.h"
#include "MainMenu.h"
#include "SaveGame.h"
#include "Instructions.h"


CPause* CPause::GetInstance( void )
{
	//Global Instance
	static CPause s_Instance;
	return &s_Instance;
}

CPause::CPause(void)
{
}



//Functions for every state
bool	CPause::Input			( )					 
{
	
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();


	// Navigate the menu
	if(   (pDI->KeyPressed( DIK_S )  || pDI->JoystickGetLStickDirPressed( DIR_DOWN, JOYSTICKID1 ) )  || ( pDI->KeyPressed( DIK_DOWN ) || pDI->JoystickGetLStickDirPressed( DIR_DOWN, JOYSTICKID2 )  ) )
	{
		++m_nCursor;


		// Stop at the bottom
		if( m_nCursor > 4 )		// EXIT
			m_nCursor = 0;
		CGame::GetInstance()->PlayMoveCursorSFX();
	}
	else if(  (pDI->KeyPressed( DIK_W )  || pDI->JoystickGetLStickDirPressed( DIR_UP, JOYSTICKID1 ) )  || ( pDI->KeyPressed( DIK_UP ) || pDI->JoystickGetLStickDirPressed( DIR_UP, JOYSTICKID2 )  ) )
	{
		--m_nCursor;


		// Stop at the top
		if( m_nCursor < 0 )		// PLAY
			m_nCursor = 4;
		CGame::GetInstance()->PlayMoveCursorSFX();
	}


	// Handle the Enter key
	if( pDI->KeyPressed( DIK_RETURN ) == true || pDI->JoystickButtonPressed( 1, JOYSTICKID1 ) == true || pDI->JoystickButtonPressed( 1, JOYSTICKID2 ) == true  )
	{
	// What selection?
		switch( m_nCursor )
		{
		case 0:    //return to play
			{

				CGame::GetInstance()->PopState();
				return true;

			}
			break;
		case 1:			// Save
			{

				// Change the state to CGameplayState
				CGame::GetInstance()->PushState( CSaveGame::GetInstance() );
				return true;	// MUST LEAVE THIS FUNCTION!

			}
			break;

		case 2:    //Options
			{
				
				CGame::GetInstance()->PushState( COptions::GetInstance() );
				return true;

			}
			break;

		case 3:
			{
				CGame::GetInstance()->PushState( CInstructions::GetInstance() );
			}
			break;

		case 4:    //Quit Game
			{

				CGame::GetInstance()->PlayCancelSFX();
				CGame::GetInstance()->ChangeState( CMainMenu::GetInstance() );
				return true;

			}
			break;

		}
	}
	
	//Exiting out of the game with escape
	if( (pDI->KeyPressed( DIK_ESCAPE ) || pDI->JoystickButtonPressed( 2, JOYSTICKID1 ) ) ||  ( pDI->JoystickButtonPressed( 3, JOYSTICKID1 )  || pDI->JoystickButtonPressed( 3, JOYSTICKID2 ) ) )
	{
		m_nCursor = 4;
		CGame::GetInstance()->PlayCancelSFX();
		return true;
	}

	// Keep running
	return true;
}
void	CPause::Update			( float fElapsedTime ) 
{

}
void	CPause::Render			( )			
{
	CSGD_TextureManager * pTM = CSGD_TextureManager::GetInstance();
	

	pTM->Draw( m_nBackgroundID, 0, 0, 1.0f, 1.0f );

	CBitmapFont* pFont = CGame::GetInstance()->GetFont();

	
	//Render the menu
	pFont->Draw( _T("Paused"), 640 / 2, 100, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );
	
	
	//Render the menu
	if(m_nCursor == 0)
		pTM->Draw(SelectedButtonID,(640 - (15*32)) / 2, 134, 1.0f, 0.7f);
	else
		pTM->Draw(nonSelectedButtonID,(640 - (15*32)) / 2, 134, 1.0f, 0.7f);
	pFont->Draw( _T("Return to Game"), (640 - (10*32)) / 2, 150, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );

	if(m_nCursor == 1)
		pTM->Draw(SelectedButtonID,(640 - (15*32)) / 2, 184, 1.0f, 0.7f);
	else
		pTM->Draw(nonSelectedButtonID,(640 - (15*32)) / 2, 184, 1.0f, 0.7f);
	pFont->Draw( _T("Save"), (640 - (10*32)) / 2, 200, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );

	if(m_nCursor == 2)
		pTM->Draw(SelectedButtonID,(640 - (15*32)) / 2, 234, 1.0f, 0.7f);
	else
		pTM->Draw(nonSelectedButtonID,(640 - (15*32)) / 2, 234, 1.0f, 0.7f);
	pFont->Draw( _T("Options"), (640 - (10*32)) / 2, 250, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );

	if(m_nCursor == 3)
		pTM->Draw(SelectedButtonID,(640 - (15*32)) / 2, 284, 1.0f, 0.7f);
	else
		pTM->Draw(nonSelectedButtonID,(640 - (15*32)) / 2, 284, 1.0f, 0.7f);
	pFont->Draw( _T("Instructions"), (640 - (10*32)) / 2, 300, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );

	if(m_nCursor == 4)
		pTM->Draw(SelectedButtonID,(640 - (15*32)) / 2, 334, 1.0f, 0.7f);
	else
		pTM->Draw(nonSelectedButtonID,(640 - (15*32)) / 2, 334, 1.0f, 0.7f);
	pFont->Draw( _T("Quit Game"), (640 - (10*32)) / 2, 350, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );
	
	
	//Render Cursor Position
	//pTM->Draw(cursorImageID, (640 - (12*32)) / 2 , 150 + (m_nCursor *50), 0.3f, 0.3f, nullptr, 35.0f / 2, 72.0f / 2, D3DX_PI / -2);

}
void	CPause::Enter			( )		
{
	m_nBackgroundID = -1;
	//Cursor Image ID Set
	nonSelectedButtonID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/Option_Background.png"));
	SelectedButtonID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/Selected_Option_Background.png"));
	m_nBackgroundID =  CSGD_TextureManager::GetInstance()->LoadTexture( _T( "resources/graphics/Background_MainMenu.png") );
	MusicVolume = CGame::GetInstance()->GetVolumeLevel();
	CGame::GetInstance()->SetVolumeLevel( 0.01f );
	m_nCursor = 0;
}
void	CPause::Exit			( )	
{
	CGame::GetInstance()->SetVolumeLevel( MusicVolume );
	CSGD_TextureManager::GetInstance()->UnloadTexture(nonSelectedButtonID);
	CSGD_TextureManager::GetInstance()->UnloadTexture(SelectedButtonID);
	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nBackgroundID);
}