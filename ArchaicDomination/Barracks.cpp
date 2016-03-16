#include "Barracks.h"
#include "BaseUnit.h"
#include "SGD Wrappers\CSGD_TextureManager.h"





Barracks::Barracks(int o, int x, int y)
{
	m_nWoodCost = 30;
	m_nSteelCost = 10;
	
	m_eType = BARRACKS;

	posX = x;
	posY = y;

	owner = o;
	
	ImgID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("resources/graphics/barracks.png"));


	Woodpt = 0;
	Steelpt = 0;
	Foodpt = 0;


	vector<CBaseUnit*> vec1;
	m_vUnits.push_back(vec1);

	vector<CBaseUnit*> vec2;
	m_vUnits.push_back(vec2);



	//CBaseUnit* tempunit = new CBaseUnit();
	//tempunit-> CreateUnit( FOOTSOLDIERS );
	//m_vUnits[0].push_back(tempunit);


	//CBaseUnit* tempunit1 = new CBaseUnit();
	//tempunit1-> CreateUnit( FOOTSOLDIERS );
	//m_vUnits[0].push_back(tempunit1);


	//CBaseUnit* tempunit2 = new CBaseUnit();
	//tempunit2-> CreateUnit( FOOTSOLDIERS );
	//m_vUnits[0].push_back(tempunit2);








	//CBaseUnit* skeleton = new CBaseUnit();
	//skeleton-> CreateUnit( SKELETALWARRIOR );
	//m_vUnits[1].push_back(skeleton);


	//CBaseUnit* necro = new CBaseUnit();
	//necro-> CreateUnit( NECROMANCER );
	//m_vUnits[1].push_back(necro);



	//CBaseUnit* shadowPriest = new CBaseUnit();
	//shadowPriest-> CreateUnit( SHADOWPRIEST );
	//m_vUnits[1].push_back(shadowPriest);
	//
}


Barracks::~Barracks(void)
{
}


