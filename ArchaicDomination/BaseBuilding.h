#pragma once
#include "entity.h"
#include "SGD Wrappers\CSGD_TextureManager.h"


enum BuildType
{
	FARM, WOOD, MINE, BARRACKS, WARFACTORY, CAPITOL
};

class CBaseBuilding :
	public CEntity
{
public:
	CBaseBuilding(void);
	virtual ~CBaseBuilding(void);


	///////////////////////////////////////////////
	// IEntity Interface:
	virtual void		Update				( float fElapsedTime )		override;
	virtual void		Render				( void )					override;


	int GetWood()		{return Woodpt;}
	int GetSteel()		{return Steelpt;}
	int GetFood()		{return Foodpt;}


	int GetPosBuildX()	{return posX; }
	int GetPosBuildY()  {return posY; }

	void SetPosBuildX(int px) { posX = px;}
	void SetPosBuildY(int py) { posY = py;}

	int GetBuildingID() {return m_eType; }


protected:

	int			m_nWoodCost;
	int			m_nSteelCost;

	int			m_eType;

	int			ImgID;

	int			owner;

	int			posX, posY;

		

	int			Woodpt;
	int			Steelpt;
	int			Foodpt;

};

