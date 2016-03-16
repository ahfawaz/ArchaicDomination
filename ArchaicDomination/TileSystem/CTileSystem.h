#pragma once
#include "../TinyXML/tinyxml.h"
#include <string>
using namespace std;
#include "Tile.h"
#include "BattleGrid.h"
#include "../BattlePlay.h"





class CTileSystem
{
private:
	//CBattleGrid* bg;
	string		path;
	//CBattleGrid* grid;
public:
	CTileSystem();

	~CTileSystem();
	bool Load(const char* p, vector<vector<CTile*>>& v);

	
};






