#include "Acheivements.h"

#include "SGD Wrappers\CSGD_TextureManager.h"
#include "BitmapFont.h"
#include "MainMenu.h"
#include "SGD Wrappers\CSGD_DirectInput.h"
#include "Game.h"
#include "GamePlay.h"
#include "MainMenu.h"
#include "ArcadeGamePlay.h"
#include "WorldPlay.h"




////////////////////////////////////////////////////
//Singleton
CAcheivements* CAcheivements::GetInstance( void )
{
	//Global Instance
	static CAcheivements s_Instance;
	return &s_Instance;
}






//Functions for every state
bool	CAcheivements::Input			( )					 
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
void	CAcheivements::Update			( float fElapsedTime ) 
{

}
void	CAcheivements::Render			( )			
{
	
	CSGD_TextureManager * pTM = CSGD_TextureManager::GetInstance();
	CBitmapFont* pFont = CGame::GetInstance()->GetFont();
	CSGD_DirectInput*	pDI		=	CSGD_DirectInput::GetInstance();
	int playerTurn;

	
	if (CMainMenu::GetInstance()->GetCampaign())
	{
		playerTurn = CGamePlay::GetInstance()->GetTurnOrder();
	}
	else if (CMainMenu::GetInstance()->GetVersus() == true || CMainMenu::GetInstance()->GetQuickMatch() == true)
	{
		playerTurn = CWorldPlay::GetInstance()->GetTurnOrder();
	}

	int ownerOne, ownerTwo;

	pTM->Draw( m_nBackgroundImageID, 0, 0, 1.0f, 1.0f );
	
	if ( CMainMenu::GetInstance()->GetArcade() == true )
	{
		ownerOne = CArcadeGamePlay::GetInstance()->GetCountryOneOwner();
		ownerTwo = CArcadeGamePlay::GetInstance()->GetCountryTwoOwner();
	}
	else if (CMainMenu::GetInstance()->GetCampaign())
	{
		ownerOne = CGamePlay::GetInstance()->GetCountryOneOwner();
		ownerTwo = CGamePlay::GetInstance()->GetCountryTwoOwner();
	}
	else if (CMainMenu::GetInstance()->GetVersus() == true || CMainMenu::GetInstance()->GetQuickMatch() == true)
	{
		ownerOne = CWorldPlay::GetInstance()->GetCountryOneOwner();
		ownerTwo = CWorldPlay::GetInstance()->GetCountryTwoOwner();
	}
	
	///Player one acheivements
	pFont->Draw( _T("Player One Acheivements"), (int)((640 - (10*32*0.85f)) / 2), 100, 2.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );

	if (ownerOne == 1)

	{
		pFont->Draw( _T("Player one owns Country One!!!"), (int)((640 - (10*32*0.85f)) / 2), 150, 2.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );
	}
	if( ownerOne == 1)
	{
		pFont->Draw( _T("Player one owns Country Two!!!"), (int)((640 - (10*32*0.85f)) / 2), 150, 2.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );
	}


	///Player two acheivements
	pFont->Draw( _T("Player Two Acheivements"), (int)((640 - (10*32*0.85f)) / 2), 300, 2.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );

	if (ownerTwo == 2)
	{
		pFont->Draw( _T("Player two owns Country One!!!"), (int)((640 - (10*32*0.85f)) / 2), 150, 2.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );
	}
	if (ownerTwo == 2)
	{
		pFont->Draw( _T("Player two owns Country Two!!!"), (int)((640 - (10*32*0.85f)) / 2), 150, 2.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );
	}

	if (pDI->JoystickIsUnplugged( JOYSTICKID1 ))
	{
		pFont->Draw( _T("Press Escape to return to previous menu"), (640 - (5 * 32)) / 2, 550, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );
	}
	else
	{
		pFont->Draw( _T("Press Start to return to previous menu"), (640 - (5 * 32)) / 2, 550, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 255) );
	}



}
void	CAcheivements::Enter			( )		
{
	m_nBackgroundImageID =  CSGD_TextureManager::GetInstance()->LoadTexture( _T( "resources/graphics/Background_MainMenu.png") );

}
void	CAcheivements::Exit			( )	
{

}
