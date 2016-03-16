////////////////////////////////////////////////////
// File:	CTitleScreen.cpp
// Purpose:	Handles all the title screen information
//			
//
// Author:	Joshua Ford (joshua.c.ford@gmail.com)
// Course:	SGP 1310
////////////////////////////////////////////////////


#include "TitleScreen.h"
#include "SGD Wrappers\CSGD_DirectInput.h"
#include "Game.h"
#include "MainMenu.h"
#include "SGD Wrappers\CSGD_TextureManager.h"
#include "BitmapFont.h"

////////////////////////////////////////////////////
//Singleton
CTitleScreen* CTitleScreen::GetInstance( void )
{
	//Global Instance
	static CTitleScreen s_Instance;
	return &s_Instance;
}






//Functions for every state
bool	CTitleScreen::Input			( )					 
{
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();


	///Escape to exit
	if (pDI->JoystickButtonPressed( 6, 0 ) || pDI->JoystickButtonPressed( 6, 1 ) )
	{
		CGame::GetInstance()->ChangeState( CMainMenu::GetInstance() );
		return true;
	}
	else if (pDI->KeyPressed( DIK_RETURN ) == true || pDI->JoystickButtonPressed( 9, JOYSTICKID1 ) == true)
	{
			CGame::GetInstance()->ChangeState( CMainMenu::GetInstance() );
			return true;
	}

	return true;
}
void	CTitleScreen::Update			( float fElapsedTime ) 
{

}
void	CTitleScreen::Render			( )			
{
	CSGD_TextureManager * pTM = CSGD_TextureManager::GetInstance();
	
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();



	pTM->Draw( m_nBackgroundID, 0, 0, 1.0f, 1.0f );

	CBitmapFont* pFont = CGame::GetInstance()->GetFont();

	//Render the title
	pFont->Draw( _T("Archaic Domination"), 
		(int)((640 - (7*32*2.0f)) / 2), 
		100, 3.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );

	pFont->Draw( _T("By  Iron Clad"), 
		(int)((640 - (1*32*1.0f)) / 2), 
		300, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );

	if (pDI->JoystickIsUnplugged( JOYSTICKID1 ))
	{
		pFont->Draw( _T("Enter to Continue"), (int)((640 - (2*32*1.0f)) / 2), 450, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );
	}
	else
	{
		pFont->Draw( _T("Start to Continue"), (int)((640 - (2*32*1.0f)) / 2), 450, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );
	}


}
void	CTitleScreen::Enter			( )		
{
	m_nBackgroundID = -1;
	
	m_nBackgroundID =  CSGD_TextureManager::GetInstance()->LoadTexture( _T( "resources/graphics/Background_MainMenu.png"), D3DCOLOR_XRGB(255,255,255) );
}
void	CTitleScreen::Exit			( )	
{

}