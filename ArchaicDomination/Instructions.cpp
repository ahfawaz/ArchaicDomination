#include "Instructions.h"

#include"SGD Wrappers\CSGD_XAudio2.h"
#include "SGD Wrappers\CSGD_DirectInput.h"
#include "SGD Wrappers\CSGD_TextureManager.h"
#include "SGD Wrappers\CSGD_Direct3D.h"
#include "BitmapFont.h"
#include "Game.h"


CInstructions* CInstructions::GetInstance( void )
{
	//Global Instance
	static CInstructions s_Instance;
	return &s_Instance;
}

CInstructions::CInstructions(void)
{
}



//Functions for every state
bool	CInstructions::Input			( )					 
{

	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();


	// Navigate the menu
	if(   (pDI->KeyPressed( DIK_D )  || pDI->JoystickGetLStickDirPressed( DIR_RIGHT, JOYSTICKID1 ) )  || ( pDI->KeyPressed( DIK_RIGHT ) || pDI->JoystickGetLStickDirPressed( DIR_RIGHT, JOYSTICKID2 )  ) )
	{
		++m_nCPos;
		if(m_nCPos > 2)
			m_nCPos = 2;

		CGame::GetInstance()->PlayMoveCursorSFX();
	}
	else if(  (pDI->KeyPressed( DIK_A )  || pDI->JoystickGetLStickDirPressed( DIR_LEFT, JOYSTICKID1 ) )  || ( pDI->KeyPressed( DIK_LEFT ) || pDI->JoystickGetLStickDirPressed( DIR_LEFT, JOYSTICKID2 )  ) )
	{
		--m_nCPos;
		if(m_nCPos < 0)
			m_nCPos = 0;

		CGame::GetInstance()->PlayMoveCursorSFX();
	}

	//Exiting out of the game with escape
	if( (pDI->KeyPressed( DIK_ESCAPE ) || pDI->JoystickButtonPressed( 2, JOYSTICKID1 ) ) ||  ( pDI->JoystickButtonPressed( 3, JOYSTICKID1 )  || pDI->JoystickButtonPressed( 3, JOYSTICKID2 ) ) )
	{
		CGame::GetInstance()->PopState();
		return true;
	}

	// Keep running
	return true;
}
void	CInstructions::Update			( float fElapsedTime ) 
{

}
void	CInstructions::Render			( )			
{
	CSGD_TextureManager * pTM = CSGD_TextureManager::GetInstance();


	pTM->Draw( m_nBackgroundID, 0, 0, 1.0f, 1.0f );

	CBitmapFont* pFont = CGame::GetInstance()->GetFont();

	RECT box = {50,50,750,550};
	CSGD_Direct3D::GetInstance()->DrawRect(box,D3DCOLOR_XRGB(0,0,0));



	//Render Pages
	switch(m_nCPos)
	{
	case 0:
		Render0();
		break;
	case 1:
		Render1();
		break;
	case 2:
		Render2();
		break;
	case 3:
		Render3();
		break;
	case 4:
		break;
	case 5:
		break;
	case 6:
		break;
	}





}
void	CInstructions::Enter			( )		
{
	m_nBackgroundID = -1;

	m_nBackgroundID =  CSGD_TextureManager::GetInstance()->LoadTexture( _T( "resources/graphics/Background_MainMenu.png") );

	m_nCursor = 0;

	m_nCPos = 0;


	//Images
	m_nArcade = CSGD_TextureManager::GetInstance()->LoadTexture( _T( "resources/graphics/instructions/arcade.png") );
	m_nController = CSGD_TextureManager::GetInstance()->LoadTexture( _T( "resources/graphics/instructions/controls.png") );
	m_nTurn =  CSGD_TextureManager::GetInstance()->LoadTexture( _T( "resources/graphics/instructions/turn.png") );
	m_nUnits =  CSGD_TextureManager::GetInstance()->LoadTexture( _T( "resources/graphics/instructions/units.png") );
	m_nResources =  CSGD_TextureManager::GetInstance()->LoadTexture( _T( "resources/graphics/instructions/resources.png") );
	m_nRecruit =  CSGD_TextureManager::GetInstance()->LoadTexture( _T( "resources/graphics/instructions/recruit.png") );
	m_nCommands =  CSGD_TextureManager::GetInstance()->LoadTexture( _T( "resources/graphics/instructions/commands.png") );
	m_nBuildings =  CSGD_TextureManager::GetInstance()->LoadTexture( _T( "resources/graphics/instructions/buildings.png") );
}
void	CInstructions::Exit			( )	
{

}





void	CInstructions::Render0( )
{
	CSGD_TextureManager * pTM = CSGD_TextureManager::GetInstance();
	CBitmapFont* pFont = CGame::GetInstance()->GetFont();


	pFont->Draw( _T("Keyboard  Controls:"), 250, 100, 1.0f, D3DCOLOR_ARGB(255, 255, 255, 255) );



	pFont->Draw(_T("Player 1 :"),100,150,0.75f,D3DCOLOR_XRGB(255,255,255));

	pFont->Draw(_T("Tab :  Confirm  Selection"),100,200,0.75f,D3DCOLOR_XRGB(255,255,255));
	pFont->Draw(_T("WASD :  Move  Selection  Cursor"),100,250,0.75f,D3DCOLOR_XRGB(255,255,255));
	pFont->Draw(_T("ESC :  Pause / Options"),100,300,0.75f,D3DCOLOR_XRGB(255,255,255));
	pFont->Draw(_T("1  &  2 :  Use  Unit  Abilities"),100,350,0.75f,D3DCOLOR_XRGB(255,255,255));











	pFont->Draw(_T("Player 2:"),450,150,0.75f,D3DCOLOR_XRGB(255,255,255));

	pFont->Draw(_T("Enter :  Confirm  Selection"),450,200,0.75f,D3DCOLOR_XRGB(255,255,255));
	pFont->Draw(_T("Arrow  Keys :  Move  Selection  Cursor"),450,250,0.75f,D3DCOLOR_XRGB(255,255,255));
	pFont->Draw(_T("ESC :  Pause / Options"),450,300,0.75f,D3DCOLOR_XRGB(255,255,255));
	pFont->Draw(_T("Numpad  1  &  2 :  Use  Unit  Abilities"),450,350,0.75f,D3DCOLOR_XRGB(255,255,255));

	pFont->Draw( _T("ESC  to  return  to  Previous  Menu"), 250, 500, 0.75f, D3DCOLOR_ARGB(255, 255, 255, 255) );

	pFont->Draw( _T("Next  Page  ->"), 600, 500, 0.75f, D3DCOLOR_ARGB(255, 255, 255, 255) );

}





void	CInstructions::Render1( )
{
	CSGD_TextureManager * pTM = CSGD_TextureManager::GetInstance();
	CBitmapFont* pFont = CGame::GetInstance()->GetFont();
	CSGD_Direct3D* pD = CSGD_Direct3D::GetInstance();



	pFont->Draw( _T("Controller  Controls:"), 300, 100, 1.0f, D3DCOLOR_ARGB(255, 255, 255, 255) );


	pTM->Draw(m_nController,150,200,0.3f,0.3f);


	pFont->Draw( _T("<-  Previous  Page"), 100, 500, 0.75f, D3DCOLOR_ARGB(255, 255, 255, 255) );

	pFont->Draw( _T("ESC  to  return  to  Previous  Menu"), 250, 500, 0.75f, D3DCOLOR_ARGB(255, 255, 255, 255) );

	pFont->Draw( _T("Next  Page  ->"), 600, 500, 0.75f, D3DCOLOR_ARGB(255, 255, 255, 255) );
}




void	CInstructions::Render2( )
{
	CSGD_TextureManager * pTM = CSGD_TextureManager::GetInstance();
	CBitmapFont* pFont = CGame::GetInstance()->GetFont();
	CSGD_Direct3D* pD = CSGD_Direct3D::GetInstance();

	pFont->Draw( _T("Arcade  Controls:"), 300, 100, 1.0f, D3DCOLOR_ARGB(255, 255, 255, 255) );


	pTM->Draw(m_nArcade,150,200,0.5f,0.5f);


	pFont->Draw( _T("<-  Previous  Page"), 100, 500, 0.75f, D3DCOLOR_ARGB(255, 255, 255, 255) );

	pFont->Draw( _T("ESC  to  return  to  Previous  Menu"), 250, 500, 0.75f, D3DCOLOR_ARGB(255, 255, 255, 255) );

	//pFont->Draw( _T("Next  Page  ->"), 600, 500, 0.75f, D3DCOLOR_ARGB(255, 255, 255, 255) );
}




void	CInstructions::Render3( )
{
	CSGD_TextureManager * pTM = CSGD_TextureManager::GetInstance();
	CBitmapFont* pFont = CGame::GetInstance()->GetFont();
	CSGD_Direct3D* pD = CSGD_Direct3D::GetInstance();


	pFont->Draw( _T("INSTRUCTIONS:"), 300, 100, 1.0f, D3DCOLOR_ARGB(255, 255, 255, 255) );



	pFont->Draw( _T("Archaic  Domination  is  a  turn-based  strategy  game.  Move  your  armies  across  the\n continent  to  claim  ownership  of  every  territory.\n\nControl  the  kingdom  of  Humans  and  rid  the  land  of  the  Undead  in  Campaign  Mode.\n\nGo  head  to  head  against  a  friend  in  Versus  Mode  as  either  Human  or  Undead"), 100, 150, 0.75f, D3DCOLOR_ARGB(255, 255, 255, 255) );

	pFont->Draw( _T("Over  World:"), 300, 100, 1.0f, D3DCOLOR_ARGB(255, 255, 255, 255) );

	pTM->Draw(m_nTurn,100,175,0.75f,0.75f);


	pFont->Draw( _T("<-  Previous  Page"), 100, 500, 0.75f, D3DCOLOR_ARGB(255, 255, 255, 255) );

	pFont->Draw( _T("ESC  to  return  to  Previous  Menu"), 250, 500, 0.75f, D3DCOLOR_ARGB(255, 255, 255, 255) );

	pFont->Draw( _T("Next  Page  ->"), 600, 500, 0.75f, D3DCOLOR_ARGB(255, 255, 255, 255) );
}
