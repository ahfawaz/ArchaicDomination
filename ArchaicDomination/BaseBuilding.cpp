#include "BaseBuilding.h"
#include "BitmapFont.h"
#include <cassert>
#include "Game.h"

CBaseBuilding::CBaseBuilding(void)
{
	owner = 0;
}


CBaseBuilding::~CBaseBuilding(void)
{
	
}


void CBaseBuilding::Update( float fElapsedTime )
{
	
}



void CBaseBuilding::Render( void )
{
	CBitmapFont* pFont = CGame::GetInstance()->GetFont();

	switch (m_eType)
	{
	case FARM:
		pFont->Draw( _T("Farm"), 560, 450, 0.65f, D3DCOLOR_XRGB( 0, 0, 0 ) );
		break;
	case WOOD:
		pFont->Draw( _T("Lumber Mill"), 550, 450, 0.65f, D3DCOLOR_XRGB( 0, 0, 0 ) );
		break;
	case MINE:
		pFont->Draw( _T("Mine"), 560, 450, 0.65f, D3DCOLOR_XRGB( 0, 0, 0 ) );
		break;
	case BARRACKS:
		pFont->Draw( _T("Barracks"), 550, 450, 0.65f, D3DCOLOR_XRGB( 0, 0, 0 ) );
		break;
	case WARFACTORY:
		pFont->Draw( _T("Warfactory"), 550, 450, 0.65f, D3DCOLOR_XRGB( 0, 0, 0 ) );
		break;
	case CAPITOL:
		pFont->Draw( _T("Capitol"), 550, 450, 0.65f, D3DCOLOR_XRGB( 0, 0, 0 ) );
		break;
	default:
		break;
	}
	CSGD_TextureManager::GetInstance()->Draw(ImgID, 550, 480,1.0f,1.0f,0);
}






