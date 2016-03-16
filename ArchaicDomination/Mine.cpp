#include "Mine.h"


Mine::Mine(int o, int x, int y)
{
	m_nWoodCost = 15;
	m_nSteelCost = 10;
	
	m_eType = MINE;
	
	posX = x;
	posY = y;

	owner = o;

	ImgID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("resources/graphics/mine.png"));

	Foodpt = 0;
	Steelpt = 10;
	Woodpt = 0;

}


Mine::~Mine(void)
{
}


