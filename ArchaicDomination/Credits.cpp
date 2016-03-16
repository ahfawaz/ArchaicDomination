////////////////////////////////////////////////////
// File:	CCredits.cpp
// Purpose:	Handles all the credits information
//			
//
// Author:	Joshua Ford (joshua.c.ford@gmail.com)
// Course:	SGP 1310
////////////////////////////////////////////////////

#include "Credits.h"
#include "SGD Wrappers\CSGD_TextureManager.h"
#include "BitmapFont.h"
#include "MainMenu.h"
#include "SGD Wrappers\CSGD_DirectInput.h"
#include "Game.h"


////////////////////////////////////////////////////
//Singleton
CCredits* CCredits::GetInstance( void )
{
	//Global Instance
	static CCredits s_Instance;
	return &s_Instance;
}






//Functions for every state
bool	CCredits::Input			( )					 
{

	CSGD_DirectInput*	pDI		=	CSGD_DirectInput::GetInstance();

	///Escape to exit
	if (pDI->JoystickButtonPressed( 3, 0 ) || pDI->JoystickButtonPressed( 3, 1 ) )
	{
		CGame::GetInstance()->PopState();
		return true;
	}
	else if (pDI->KeyPressed( DIK_ESCAPE ) == true || pDI->JoystickButtonPressed( 9, JOYSTICKID1 ) == true)
	{
			CGame::GetInstance()->PopState();
			return true;
	}

	// Keep running
	return true;
}
void	CCredits::Update			( float fElapsedTime ) 
{

}
void	CCredits::Render			( )			
{
	
	CSGD_TextureManager * pTM = CSGD_TextureManager::GetInstance();
	CBitmapFont* pFont = CGame::GetInstance()->GetFont();
	CSGD_DirectInput*	pDI		=	CSGD_DirectInput::GetInstance();



	pTM->Draw( m_nBackgroundImageID, 0, 0, 1.0f, 1.0f );
	
	pFont->Draw( _T("Archaic Domination"), (int)((640 - (10*32*0.85f)) / 2), 20, 2.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );


	//Developers
	pFont->Draw( _T("Developers"), (640 - (11*32)) / 2, 100, 1.5f, D3DCOLOR_ARGB(255, 255, 0, 0) );
	pFont->Draw( _T("Joshua Ford"), (640 - (11*32)) / 2, 150, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );
	pFont->Draw( _T("Andrew Pugliese" ), (640 - (11*32)) / 2, 200, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );
	pFont->Draw( _T("Ahmad Fawaz"), (640 - (11*32)) / 2, 250, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );
	pFont->Draw( _T("Evan Murray"), (640 - (11*32)) / 2, 300, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );
	pFont->Draw( _T("Roberto Crespo"), (640 - (11*32)) / 2, 350, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );
	
	//Producers
	pFont->Draw( _T("Producer"), (640 + (8*32)) / 2, 100, 1.5f, D3DCOLOR_ARGB(255, 255, 0, 0) );
	pFont->Draw( _T("John OLeske"), (640 + (8*32)) / 2, 150, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );
	pFont->Draw( _T("Assistant Producer"), (640 + (8*32)) / 2, 200, 1.0f, D3DCOLOR_ARGB(255, 255, 0, 0) );
	pFont->Draw( _T("Sean Hathaway"), (640 + (8*32)) / 2, 250, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );
	
	
	//Artist
	pFont->Draw( _T("Lead Artist"), (640 - (32)) / 2, 450, 1.5f, D3DCOLOR_ARGB(255, 255, 0, 0) );
	pFont->Draw( _T("Sagrillo Maher"), (640 - (32)) / 2, 500, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );

	if (pDI->JoystickIsUnplugged( JOYSTICKID1 ))
	{
		pFont->Draw( _T("Press Escape to return to Main Menu"), (640 - (5 * 32)) / 2, 550, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );
	}
	else
	{
		pFont->Draw( _T("Press Start to return to Main Menu"), (640 - (5 * 32)) / 2, 550, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );
	}



}
void	CCredits::Enter			( )		
{
	m_nBackgroundImageID =  CSGD_TextureManager::GetInstance()->LoadTexture( _T( "resources/graphics/Background_MainMenu.png") );

}
void	CCredits::Exit			( )	
{

}
