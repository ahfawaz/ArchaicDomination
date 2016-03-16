#include "WorldTile.h"


CWorldTile::CWorldTile(int own,int cntry, int X, int Y, const char* ID)
{

	m_nOwner = own;
	m_nCountry = cntry;
	SetPosX(X);
	SetPosY(Y);
	ImageID = ID;
	m_bHasAttacked = false;
	m_bHasMoved = false;
	m_bIsOccupied = false;
	m_bHasBuilt = false;
	m_nBuildingID = -1;

}

CWorldTile::CWorldTile( int X, int Y )
{
	SetPosX( X );
	SetPosY( Y );
}

CWorldTile::~CWorldTile(void)
{
}



void CWorldTile::AddUnit(CEntity* u)
{
	m_vUnits.push_back(u);
}

void CWorldTile::DeleteUnit(CEntity* u)
{

}



void CWorldTile::SetAdj(CWorldTile* adj)
{
	m_vAdjacents.push_back(adj);
}
