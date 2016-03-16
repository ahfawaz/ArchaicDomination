#pragma once
#include "WorldTile.h"
#include <vector>
using namespace std;

class CWorldGrid
{
private:

	CWorldTile*						m_pCurrent;
	CWorldTile*						m_pPrevious;
	vector<vector<CWorldTile*>>		WTile;
	int								playMode;
	int								m_nMapID;
	

public:

	CWorldGrid(int mode);
	
	~CWorldGrid(void);

	vector<vector<CWorldTile*>> GetGrid()		{return WTile;}

	void Update();
	void Render();
	void ChangeCurrent(CWorldTile* wtile);
	void ChangePrevious(CWorldTile* current);
	CWorldTile* GetCurrent( void )			{ return m_pCurrent; }
	CWorldTile* GetPrevious( void )			{ return m_pPrevious; }
	void Highlight();
	void UnHighlight();
};

