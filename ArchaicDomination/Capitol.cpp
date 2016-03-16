#include "Capitol.h"


Capitol::Capitol(int o, int x, int y)
{
	m_nWoodCost = 0;
	m_nSteelCost = 0;
	
	m_eType = CAPITOL;

	owner = o;

	posX = x;
	posY = y;
	
	ImgID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("resources/graphics/capitol.png"));


	Woodpt = 10;
	Steelpt = 5;
	Foodpt = 10;
}


Capitol::~Capitol(void)
{
}



