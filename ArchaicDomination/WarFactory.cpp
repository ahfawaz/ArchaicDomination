#include "WarFactory.h"


WarFactory::WarFactory(int o, int x, int y)
{
	m_nWoodCost = 20;
	m_nSteelCost = 40;

	m_eType = WARFACTORY;

	owner = o;

	posX = x;
	posY = y;

	ImgID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("resources/graphics/factory.png"));

	Foodpt = 0;
	Steelpt = 0;
	Woodpt = 0;

}


WarFactory::~WarFactory(void)
{
}


