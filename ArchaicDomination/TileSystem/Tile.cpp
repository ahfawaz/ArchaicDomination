#include "Tile.h"


CTile::CTile(int X, int Y, int W, int H, const char* ID, const char* o, int lX, int lY, int T)
{
	m_nPosX = X;
	m_nPosY = Y;
	m_nWidth = W;
	m_nHeight = H;
	m_cImageID = ID;
	m_cObj = o;
	m_eType =(TileType) T;
	m_nLocX = lX;
	m_nLocY = lY;
	m_bIsOccupied = false;

}


CTile::~CTile(void)
{
}

CTile::CTile(void)
{

}