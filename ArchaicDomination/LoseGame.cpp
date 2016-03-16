#include "LoseGame.h"


#include "SGD Wrappers\CSGD_TextureManager.h"
#include "BitmapFont.h"
#include "MainMenu.h"
#include "SGD Wrappers\CSGD_DirectInput.h"
#include "Game.h"


////////////////////////////////////////////////////
//Singleton
CLoseGame* CLoseGame::GetInstance( void )
{
	//Global Instance
	static CLoseGame s_Instance;
	return &s_Instance;
}






//Functions for every state
bool	CLoseGame::Input			( )					 
{

	CSGD_DirectInput*	pDI		=	CSGD_DirectInput::GetInstance();

	///Escape to exit
	if (pDI->JoystickButtonPressed( 3, 0 ) || pDI->JoystickButtonPressed( 3, 1 ) )
	{
		CGame::GetInstance()->ChangeState( CMainMenu::GetInstance() );
		return true;
	}
	else if (pDI->KeyPressed( DIK_ESCAPE ) == true || pDI->JoystickButtonPressed( 9, JOYSTICKID1 ) == true)
	{
			CGame::GetInstance()->ChangeState( CMainMenu::GetInstance() );
			return true;
	}

	// Keep running
	return true;
}
void	CLoseGame::Update			( float fElapsedTime ) 
{
	m_fDeltaTime += fElapsedTime;
	if (m_fDeltaTime >= 4.0f)
	{
		CGame::GetInstance()->ChangeState( CMainMenu::GetInstance() );
		m_fDeltaTime = 0;
	}
}
void	CLoseGame::Render			( )			
{
	
	CSGD_TextureManager * pTM = CSGD_TextureManager::GetInstance();
	CBitmapFont* pFont = CGame::GetInstance()->GetFont();

	pTM->Draw( m_nBackgroundImageID, 0, 0, 1.5f, 1.5f );
	
	pFont->Draw( _T("Archaic Domination"), (int)((640 - (10*32*0.85f)) / 2), 20, 2.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );


	//Developers
	pFont->Draw( _T("Player Two Wins!"), (640 - (11*32)) / 2, 100, 1.5f, D3DCOLOR_ARGB(255, 255, 0, 0) );

	

	

}
void	CLoseGame::Enter			( )		
{
	m_nBackgroundImageID =  CSGD_TextureManager::GetInstance()->LoadTexture( _T( "resources/graphics/CreditsScreen.jpg") );

}
void	CLoseGame::Exit			( )	
{

}
