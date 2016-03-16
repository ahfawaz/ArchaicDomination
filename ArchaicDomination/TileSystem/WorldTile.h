#pragma once
#include "tile.h"
#include <vector>
using namespace std;
#include "../Entity.h"
#include "../BaseBuilding.h"


class CWorldTile
{

private:

	int							m_nOwner;
	int							m_nBuildingID;
	int							m_nCountry;
	int							posX, posY;
	const char*					ImageID;
	bool						m_bIsOccupied;
	bool						m_bHasAttacked;
	bool						m_bHasMoved;
	bool						m_bHasBuilt;
	int							m_nTerritory;
	vector<CWorldTile*>			m_vAdjacents;



	
public:
	CWorldTile(int own,int cntry, int X, int Y, const char* ID);

	~CWorldTile(void);

	CWorldTile( int X, int Y );

	int			GetOwner			( void )			{ return m_nOwner; }
	int			GetBuilding			( void )			{ return m_nBuildingID; }
	int			GetCountry			( void )			{ return m_nCountry; }
	int			GetPosX				( void )			{ return posX; }
	int			GetPosY				( void )			{ return posY; }
	bool		GetOccupied			( void )			{ return m_bIsOccupied; }
	bool		GetAttacked			( void )			{ return m_bHasAttacked; }
	bool		GetMoved			( void )			{ return m_bHasMoved; }
	bool		GetBuilt			( void )			{ return m_bHasBuilt; }



	vector<CWorldTile*>				GetAdj(void)		{ return m_vAdjacents; }


	void		SetOwner			( int o	)			{ m_nOwner = o; }
	void		SetBuilding			( int b	)			{ m_nBuildingID = b; }
	void		SetCountry			( int c	)			{ m_nCountry = c; }
	void		SetPosX				( int x	)			{ posX = x; }
	void		SetPosY				( int y	)			{ posY = y; }
	void		SetOccupied			( bool temp )		{ m_bIsOccupied = temp; }
	void		SetAttacked			( bool temp )		{ m_bHasAttacked = temp; }
	void		SetMoved			( bool temp )		{ m_bHasMoved = temp; }
	void		SetBuilt			( bool temp )		{ m_bHasBuilt = temp; }
	void		SetTerritory		( int T )			{ m_nTerritory = T; }
	int			GetTerritory		( void )			{ return m_nTerritory; }

	void		SetAdj(CWorldTile* adj);

	vector<CEntity*>			m_vUnits;

	void AddUnit(CEntity* u);
	void DeleteUnit(CEntity* u);



};

