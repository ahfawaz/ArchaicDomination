////////////////////////////////////////////////////
// File:	CMainMenu.cpp
// Purpose:	Handles all input for the main menu
//			screen
//
// Author:	Joshua Ford (joshua.c.ford@gmail.com)
// Course:	SGP 1310
////////////////////////////////////////////////////

#include "MainMenu.h"
#include "SGD Wrappers\CSGD_TextureManager.h"
#include "Game.h"
#include "GamePlay.h"
#include "Options.h"
#include "LoadGame.h"
#include "SaveGame.h"
#include "Credits.h"
#include "TitleScreen.h"
#include "SGD Wrappers\CSGD_DirectInput.h"
#include "BattlePlay.h"
#include "ArcadeGamePlay.h"
#include "WorldPlay.h"
#include "Instructions.h"




////////////////////////////////////////////////////
//Singleton
CMainMenu* CMainMenu::GetInstance( void )
{
	//Global Instance
	static CMainMenu s_Instance;
	return &s_Instance;
}






//Functions for every state
bool	CMainMenu::Input			( )					 
{

	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();


	// Navigate the menu
	if(   (pDI->KeyPressed( DIK_S )  || pDI->JoystickGetLStickDirPressed( DIR_DOWN, JOYSTICKID1 ) )  || ( pDI->KeyPressed( DIK_DOWN ) || pDI->JoystickGetLStickDirPressed( DIR_DOWN, JOYSTICKID2 )  ) )
	{
		++m_nCursor;


		// Stop at the bottom
		if( m_nCursor > 7 )		
			m_nCursor = 0;
		
		CGame::GetInstance()->PlayMoveCursorSFX();
	}
	else if(  (pDI->KeyPressed( DIK_W )  || pDI->JoystickGetLStickDirPressed( DIR_UP, JOYSTICKID1 ) )  || ( pDI->KeyPressed( DIK_UP ) || pDI->JoystickGetLStickDirPressed( DIR_UP, JOYSTICKID2 )  ) )
	{
		--m_nCursor;


		// Stop at the top
		if( m_nCursor < 0 )		
			m_nCursor = 7;

		CGame::GetInstance()->PlayMoveCursorSFX();
	}


	// Handle the Enter key
	if( ( pDI->KeyPressed( DIK_TAB ) || pDI->JoystickButtonPressed( 1, JOYSTICKID1 ) ) || (pDI->KeyPressed( DIK_RETURN ) || pDI->JoystickButtonPressed( 1, JOYSTICKID2 )  ) )
	{
	// What selection?
		switch( m_nCursor )
		{
		case 0:			// Campaign Option
			{

				// Change the state to CGameplay Campaign Option
				m_bCampaign = true;
				m_bInGame = true;
				//CGamePlay::GetInstance();
				if (m_bArcadeMode == true)
				{
					CGame::GetInstance()->ChangeState( CArcadeGamePlay::GetInstance() );
					return true;	
				}
				else
				{
					CGame::GetInstance()->ChangeState( CGamePlay::GetInstance() );
					return true;	
				}
				
				

			}
			break;


		case 1:    //Versus
			{
				
				// Change the state to CGameplay Versus Option
				m_bInGame = true;
				m_bVersus = true;
				if (m_bArcadeMode == true)
				{
					CGame::GetInstance()->ChangeState( CArcadeGamePlay::GetInstance() );
					return true;	
				}
				else
				{
					CGame::GetInstance()->ChangeState( CWorldPlay::GetInstance() );
					return true;	
				}

			}
			break;

		case 2:    //Quick Match
			{

				// Change the state to CGameplay Quick Match option
				m_bQuickMatch = true;
				m_bInGame = true;
				if (m_bArcadeMode == true)
				{
					CGame::GetInstance()->ChangeState( CArcadeGamePlay::GetInstance() );
					return true;	
				}
				else
				{
					CGame::GetInstance()->ChangeState( CWorldPlay::GetInstance() );
					return true;	
				}

			}
			break;

		case 3:    //Load
			{

                CGame::GetInstance()->PushState( CLoadGame::GetInstance() );
				return true;
				
			}
			break;

		case 4:    //Instructions
			{

				CGame::GetInstance()->PushState( CInstructions::GetInstance() );
				return true;

			}
			break;
		case 5:   //Credits
			{
				CGame::GetInstance()->PushState( CCredits::GetInstance() );
				return true;
			}
			break;
		case 6:  //options
			{
				CGame::GetInstance()->PushState( COptions::GetInstance() );
			}
			break; //Exit
		case 7:
			{
				return false;
			}
			break;
		}
	}
	
	///Escape to exit
	if (pDI->JoystickButtonPressed( 3, 0 ) || pDI->JoystickButtonPressed( 3, 1 ) )
	{
		m_nCursor = 7;
		CGame::GetInstance()->PlayCancelSFX();
		return true;
	}
	else if (pDI->KeyPressed( DIK_ESCAPE ) == true || pDI->JoystickButtonPressed( 2, JOYSTICKID1 ) == true)
	{
		m_nCursor = 7;
		CGame::GetInstance()->PlayCancelSFX();
		return true;
	}
	// Keep running
	return true;
}
void	CMainMenu::Update			( float fElapsedTime ) 
{

}
void	CMainMenu::Render			( )			
{
	CSGD_TextureManager * pTM = CSGD_TextureManager::GetInstance();
	

	pTM->Draw( m_nBackgroundID, 0, 0, 1.0f, 1.0f );

	CBitmapFont* pFont = CGame::GetInstance()->GetFont();

	//Render the title
	pFont->Draw( _T("Archaic Domination"), 
		(int)((640 - (5*32*2.0f)) / 2), 
		70, 2.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );
	

	//Render the 
	if(m_nCursor == 0)
		pTM->Draw(SelectedButtonID,(640 - (15*32)) / 2, 164, 1.0f, 0.7f);
	else
		pTM->Draw(nonSelectedButtonID,(640 - (15*32)) / 2, 164, 1.0f, 0.7f);
	pFont->Draw( _T("Campaign"), (640 - (10*32)) / 2, 180, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );

	if(m_nCursor == 1)
		pTM->Draw(SelectedButtonID,(640 - (15*32)) / 2, 214, 1.0f, 0.7f);
	else
		pTM->Draw(nonSelectedButtonID,(640 - (15*32)) / 2, 214, 1.0f, 0.7f);
	pFont->Draw( _T("Versus"), (640 - (10*32)) / 2, 230, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );

	if(m_nCursor == 2)
		pTM->Draw(SelectedButtonID,(640 - (15*32)) / 2, 264, 1.0f, 0.7f);
	else
		pTM->Draw(nonSelectedButtonID,(640 - (15*32)) / 2, 264, 1.0f, 0.7f);
	pFont->Draw( _T("Quick Match"), (640 - (10*32)) / 2, 280, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );

	if(m_nCursor == 3)
		pTM->Draw(SelectedButtonID,(640 - (15*32)) / 2, 314, 1.0f, 0.7f);
	else
		pTM->Draw(nonSelectedButtonID,(640 - (15*32)) / 2, 314, 1.0f, 0.7f);
	pFont->Draw( _T("Load"), (640 - (10*32)) / 2, 330, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );

	if(m_nCursor == 4)
		pTM->Draw(SelectedButtonID,(640 - (15*32)) / 2, 364, 1.0f, 0.7f);
	else
		pTM->Draw(nonSelectedButtonID,(640 - (15*32)) / 2, 364, 1.0f, 0.7f);
	pFont->Draw( _T("Instructions"), (640 - (10*32)) / 2, 380, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );

	if(m_nCursor == 5)
		pTM->Draw(SelectedButtonID,(640 - (15*32)) / 2, 414, 1.0f, 0.7f);
	else
		pTM->Draw(nonSelectedButtonID,(640 - (15*32)) / 2, 414, 1.0f, 0.7f);
	pFont->Draw( _T("Credits"), (640 - (10*32)) / 2, 430, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );

	if(m_nCursor == 6)
		pTM->Draw(SelectedButtonID,(640 - (15*32)) / 2, 464, 1.0f, 0.7f);
	else
		pTM->Draw(nonSelectedButtonID,(640 - (15*32)) / 2, 464, 1.0f, 0.7f);
	pFont->Draw( _T("Options"), (640 - (10*32)) / 2, 480, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );

	if(m_nCursor == 7)
		pTM->Draw(SelectedButtonID,(640 - (15*32)) / 2, 514, 1.0f, 0.7f);
	else
		pTM->Draw(nonSelectedButtonID,(640 - (15*32)) / 2, 514, 1.0f, 0.7f);;
	pFont->Draw( _T("Exit"), (640 - (10*32)) / 2, 530, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );
	

	//Render Cursor Position
	//pTM->Draw(cursorImageID, (640 - (12*32)) / 2 , 200 + (m_nCursor *50), 0.3f, 0.3f, nullptr, 35.0f / 2, 72.0f / 2, D3DX_PI / -2);
}

void	CMainMenu::Enter			( )		
{

	m_nCursor = 0;
	m_nBackgroundID = -1;
	
	nonSelectedButtonID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/Option_Background.png"));
	SelectedButtonID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/Selected_Option_Background.png"));
	m_nBackgroundID =  CSGD_TextureManager::GetInstance()->LoadTexture( _T( "resources/graphics/Background_MainMenu.png"), D3DCOLOR_XRGB(255,255,255) );
	//Cursor Image ID Set
	cursorImageID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/Cursor_Battle.png"));
	m_bCampaign			= false;
	m_bQuickMatch		= false;
	m_bInGame			= false;
	m_bArcadeMode       = false;

	
}
void	CMainMenu::Exit			( )	
{
	
}


