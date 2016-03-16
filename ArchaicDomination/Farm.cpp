#include "Farm.h"


Farm::Farm(int o, int x, int y)
{
	m_nWoodCost = 20;
	m_nSteelCost = 0;
	
	m_eType = FARM;

	posX = x;
	posY = y;
	
	owner = o;
	ImgID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("resources/graphics/farm.png"));

	Foodpt = 10;
	Steelpt = 0;
	Woodpt = 0;

}


Farm::~Farm(void)
{
}



