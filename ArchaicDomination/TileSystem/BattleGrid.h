#pragma once
#include "Tile.h"
#include <vector>
using namespace std;


class CBattleGrid
{
private:

	CTile*						m_pCurrent;
	vector<vector<CTile*>>		BTile; 
	int							TileSet;

	int offsetX;
	int offsetY;



public:
	CBattleGrid(void);
	CBattleGrid(vector<vector<CTile*>>& tiles);
	~CBattleGrid(void);


	vector<vector<CTile*>> GetTiles(void);

	void Update();
	void Render();
	void ChangeCurrent(int x, int y);
	CTile* GetCurrent(void) {return m_pCurrent;}
	CTile* GetATile(int posX, int posY) const;
	void Highlight();
	void UnHighlight();


};

