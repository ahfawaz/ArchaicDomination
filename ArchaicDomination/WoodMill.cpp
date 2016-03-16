#include "WoodMill.h"


WoodMill::WoodMill(int o, int x, int y)
{
	m_nWoodCost = 20;
	m_nSteelCost = 5;
	
	m_eType = WOOD;

	posX = x;
	posY = y;
	
	owner = o;

	ImgID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("resources/graphics/lumbermill.png"));

	Foodpt = 0;
	Woodpt = 10;
	Steelpt = 0;
}


WoodMill::~WoodMill(void)
{
}


