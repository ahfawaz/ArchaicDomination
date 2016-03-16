#include "BattleGrid.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../BattlePlay.h"
#include "../SGD Wrappers\CSGD_TextureManager.h"
#include "../BattlePlay.h"
#include "../Game.h"

vector<vector<CTile*>> CBattleGrid::GetTiles(void)
{
	return BTile;
}


CBattleGrid::CBattleGrid(void)
{

}


CBattleGrid::~CBattleGrid(void)
{
	for(unsigned int i = 0; i < 32; i++)
	{
		for(unsigned int j = 0; j < BTile[i].size(); j++)
		{
			delete BTile[i][j];
		}
		BTile[i].clear();
	}
	BTile.clear();
}


CBattleGrid::CBattleGrid(vector<vector<CTile*>>& tiles)
{
	BTile = tiles;
	CBattlePlay::GetInstance()->SetGrid(this);
}

CTile* CBattleGrid::GetATile(int posX, int posY) const 
{
	return BTile[posX][posY];
}



void CBattleGrid::Update()
{

}

void CBattleGrid::Render()
{
	offsetX = CBattlePlay::GetInstance()->GetOffX();
	offsetY = CBattlePlay::GetInstance()->GetOffY();


	for(unsigned int i = 0; i < BTile.size(); i++)
	{

		for(unsigned int j = 0; j < BTile[i].size(); j++)
		{
			if(BTile[i][j]->GetLocX() < CGame::GetInstance()->GetScreenWidth() + CBattlePlay::GetInstance()->GetOffX() && BTile[i][j]->GetLocY() < CGame::GetInstance()->GetScreenHeight() + CBattlePlay::GetInstance()->GetOffY())
			{
				RECT rect = {BTile[i][j]->GetPosX(), BTile[i][j]->GetPosY(), 
					BTile[i][j]->GetPosX()+ BTile[i][j]->GetWidth(), BTile[i][j]->GetPosY()+BTile[i][j]->GetHeight()};

				RECT rect2 = {BTile[i][j]->GetLocX()-offsetX, BTile[i][j]->GetLocY()-offsetY, 
					BTile[i][j]->GetLocX()+ BTile[i][j]->GetWidth(), BTile[i][j]->GetLocY()+BTile[i][j]->GetHeight()};

				TileSet = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/SCA_WorldTileSet.PNG"));
				CSGD_TextureManager::GetInstance()->Draw(TileSet,BTile[i][j]->GetLocX()-offsetX,BTile[i][j]->GetLocY()-offsetY,1.0f,1.0f,&rect);
				CSGD_Direct3D::GetInstance()->DrawHollowRect(rect2, D3DCOLOR_ARGB(255, 0, 0, 0),1 );
			}
		}
	}
}

void CBattleGrid::ChangeCurrent(int x, int y)
{
	m_pCurrent = BTile[y/32][x/32];
}

void CBattleGrid::Highlight()
{
	int offsetX = CBattlePlay::GetInstance()->GetOffX();
	int offsetY = CBattlePlay::GetInstance()->GetOffY();

	CBattlePlay* battle = CBattlePlay::GetInstance();
	CBaseUnit* unit = battle->GetCurrentUnit();

	RECT cursor = {(long)(m_pCurrent->GetLocX() + 1.5 - offsetX), (long)(m_pCurrent->GetLocY()+1.5-battle->GetOffY()),(long)(m_pCurrent->GetLocX()+m_pCurrent->GetWidth()-2-battle->GetOffX()), (long)(m_pCurrent->GetLocY()+m_pCurrent->GetHeight()-2-battle->GetOffY())};
	CSGD_Direct3D::GetInstance()->DrawRect(cursor, D3DCOLOR_ARGB(255, 255, 255, 255));

	int onTree = 1;
	if(m_pCurrent->GetType() == TileType::TREE)
		onTree = 2;

	if(unit != nullptr && !CBattlePlay::GetInstance()->GetShowUnitOptions() && !unit->GetIsMoving())
	{
		if(unit->IsAttacking() == false)
		for(int i = 1; i <= unit->GetMovement(); i++)//drawing white movement range grid
		{
			RECT right = {(long)(unit->GetPosX()+(i*32)  - offsetX), (long)(unit->GetPosY() - offsetY), (long)(unit->GetPosX()+(i*32)+32 - offsetX), (long)(unit->GetPosY()+32 - offsetY)};
			RECT left = {(long)(unit->GetPosX()-((i-1)*32)-32 - offsetX), (long)(unit->GetPosY() - offsetY), (long)(unit->GetPosX()-((i-1)*32) - offsetX), (long)(unit->GetPosY()+32 - offsetY)};
			RECT down = {(long)(unit->GetPosX() - offsetX), (long)(unit->GetPosY()+(i*32)+32 - offsetY), (long)(unit->GetPosX()+32 - offsetX), (long)(unit->GetPosY()+(i*32) - offsetY)};
			RECT up = {(long)(unit->GetPosX() - offsetX), (long)(unit->GetPosY()-((i-1)*32) - offsetY), (long)(unit->GetPosX()+32 - offsetX), (long)(unit->GetPosY()-((i-1)*32)-32 - offsetY)};

			RECT diagonalOne = { right.left, right.top+32, right.right, right.bottom+32};
			RECT diagonalTwo = { right.left, right.top-32, right.right, right.bottom-32};

			RECT diagonalThree = { left.left, left.top+32, left.right, left.bottom+32};
			RECT diagonalFour = { left.left, left.top-32, left.right, left.bottom-32};

			RECT diagonalFive = { down.left+32, down.top, down.right+32, down.bottom};
			RECT diagonalSix = { down.left-32, down.top, down.right-32, down.bottom};

			RECT diagonalSeven =  { up.left+32, up.top, up.right+32, up.bottom};
			RECT diagonalEight = { up.left-32, up.top, up.right-32, up.bottom};

			if(i == 2 && unit->GetMovement() > 3)
			{
				RECT diagonalNine = { right.left, right.top+64, right.right, right.bottom+64};
				RECT diagonalTen = { right.left, right.top-64, right.right, right.bottom-64};

				RECT diagonalEleven = { left.left, left.top+64, left.right, left.bottom+64};
				RECT diagonalTwelve = { left.left, left.top-64, left.right, left.bottom-64};

				RECT diagonalThirteen = { down.left+64, down.top, down.right+64, down.bottom};
				RECT diagonalFourteen = { down.left-64, down.top, down.right-64, down.bottom};

				RECT diagonalFifteen =  { up.left+64, up.top, up.right+64, up.bottom};
				RECT diagonalSixteen = { up.left-64, up.top, up.right-64, up.bottom};

				CSGD_Direct3D::GetInstance()->DrawHollowRect(diagonalNine, D3DCOLOR_ARGB(255, 255, 255, 255));
				CSGD_Direct3D::GetInstance()->DrawHollowRect(diagonalTen, D3DCOLOR_ARGB(255, 255, 255, 255));
				CSGD_Direct3D::GetInstance()->DrawHollowRect(diagonalEleven, D3DCOLOR_ARGB(255, 255, 255, 255));
				CSGD_Direct3D::GetInstance()->DrawHollowRect(diagonalTwelve, D3DCOLOR_ARGB(255, 255, 255, 255));
				CSGD_Direct3D::GetInstance()->DrawHollowRect(diagonalThirteen, D3DCOLOR_ARGB(255, 255, 255, 255));
				CSGD_Direct3D::GetInstance()->DrawHollowRect(diagonalFourteen, D3DCOLOR_ARGB(255, 255, 255, 255));
				CSGD_Direct3D::GetInstance()->DrawHollowRect(diagonalFifteen, D3DCOLOR_ARGB(255, 255, 255, 255));
				CSGD_Direct3D::GetInstance()->DrawHollowRect(diagonalSixteen, D3DCOLOR_ARGB(255, 255, 255, 255));
			}

			CSGD_Direct3D::GetInstance()->DrawHollowRect(right, D3DCOLOR_ARGB(255, 255, 255, 255));
			CSGD_Direct3D::GetInstance()->DrawHollowRect(left, D3DCOLOR_ARGB(255, 255, 255, 255));
			CSGD_Direct3D::GetInstance()->DrawHollowRect(up, D3DCOLOR_ARGB(255, 255, 255, 255));
			CSGD_Direct3D::GetInstance()->DrawHollowRect(down, D3DCOLOR_ARGB(255, 255, 255, 255));

			//draw diagonals
			if(i < unit->GetMovement())
			{
				CSGD_Direct3D::GetInstance()->DrawHollowRect(diagonalOne, D3DCOLOR_ARGB(255, 255, 255, 255));
				CSGD_Direct3D::GetInstance()->DrawHollowRect(diagonalTwo, D3DCOLOR_ARGB(255, 255, 255, 255));
				CSGD_Direct3D::GetInstance()->DrawHollowRect(diagonalThree, D3DCOLOR_ARGB(255, 255, 255, 255));
				CSGD_Direct3D::GetInstance()->DrawHollowRect(diagonalFour, D3DCOLOR_ARGB(255, 255, 255, 255));
				CSGD_Direct3D::GetInstance()->DrawHollowRect(diagonalFive, D3DCOLOR_ARGB(255, 255, 255, 255));
				CSGD_Direct3D::GetInstance()->DrawHollowRect(diagonalSix, D3DCOLOR_ARGB(255, 255, 255, 255));
				CSGD_Direct3D::GetInstance()->DrawHollowRect(diagonalSeven, D3DCOLOR_ARGB(255, 255, 255, 255));
				CSGD_Direct3D::GetInstance()->DrawHollowRect(diagonalEight, D3DCOLOR_ARGB(255, 255, 255, 255));
			}


		}
		else
		{
			cursor.left = long(unit->GetPosX() - offsetX);
			cursor.top = long(unit->GetPosY() - offsetY);
			cursor.right = long(unit->GetPosX() +32 - offsetX);
			cursor.bottom = long(unit->GetPosY()+32 - offsetY);
		}
		
	
		for(int i = 1; i <= unit->GetAttackRange()/onTree; i++)//drawing red attack range grid
		{
			RECT right = {(long)(cursor.left+(i*32)+1.5), (long)(cursor.top+1.5), (long)(cursor.right+(i*32)-1.5), (long)(cursor.bottom-1.5)};
			RECT left = {(long)(cursor.left-((i-1)*32)-32+1.5), (long)(cursor.top+1.5), (long)(cursor.right-((i-1)*32)-32-1.5), (long)(cursor.bottom-1.5)};
			RECT down = {(long)(cursor.left+1.5), (long)(cursor.top+(i*32)+1.5), (long)(cursor.right-1.5), (long)(cursor.bottom+(i*32)-1.5)};
			RECT up = {(long)(cursor.left+1.5), (long)(cursor.top-((i-1)*32)-32+1.5), (long)(cursor.right-1.5), (long)(cursor.bottom-((i-1)*32)-32-1.5)};

			CSGD_Direct3D::GetInstance()->DrawHollowRect(right, D3DCOLOR_ARGB(255, 255, 0, 0));
			CSGD_Direct3D::GetInstance()->DrawHollowRect(left, D3DCOLOR_ARGB(255, 255, 0, 0));
			CSGD_Direct3D::GetInstance()->DrawHollowRect(up, D3DCOLOR_ARGB(255, 255, 0, 0));
			CSGD_Direct3D::GetInstance()->DrawHollowRect(down, D3DCOLOR_ARGB(255, 255, 0, 0));



			RECT diagonalOne = { right.left, right.top+32, right.right, right.bottom+32};
			RECT diagonalTwo = { right.left, right.top-32, right.right, right.bottom-32};

			RECT diagonalThree = { left.left, left.top+32, left.right, left.bottom+32};
			RECT diagonalFour = { left.left, left.top-32, left.right, left.bottom-32};

			RECT diagonalFive = { down.left+32, down.top, down.right+32, down.bottom};
			RECT diagonalSix = { down.left-32, down.top, down.right-32, down.bottom};

			RECT diagonalSeven =  { up.left+32, up.top, up.right+32, up.bottom};
			RECT diagonalEight = { up.left-32, up.top, up.right-32, up.bottom};

			if(i == 2 && unit->GetAttackRange()/onTree > 3)
			{
				RECT diagonalNine = { right.left, right.top+64, right.right, right.bottom+64};
				RECT diagonalTen = { right.left, right.top-64, right.right, right.bottom-64};

				RECT diagonalEleven = { left.left, left.top+64, left.right, left.bottom+64};
				RECT diagonalTwelve = { left.left, left.top-64, left.right, left.bottom-64};

				RECT diagonalThirteen = { down.left+64, down.top, down.right+64, down.bottom};
				RECT diagonalFourteen = { down.left-64, down.top, down.right-64, down.bottom};

				RECT diagonalFifteen =  { up.left+64, up.top, up.right+64, up.bottom};
				RECT diagonalSixteen = { up.left-64, up.top, up.right-64, up.bottom};

				CSGD_Direct3D::GetInstance()->DrawHollowRect(diagonalNine, D3DCOLOR_ARGB(255, 255, 0, 0));
				CSGD_Direct3D::GetInstance()->DrawHollowRect(diagonalTen, D3DCOLOR_ARGB(255, 255, 0, 0));
				CSGD_Direct3D::GetInstance()->DrawHollowRect(diagonalEleven, D3DCOLOR_ARGB(255, 255, 0, 0));
				CSGD_Direct3D::GetInstance()->DrawHollowRect(diagonalTwelve, D3DCOLOR_ARGB(255, 255, 0, 0));
				CSGD_Direct3D::GetInstance()->DrawHollowRect(diagonalThirteen, D3DCOLOR_ARGB(255, 255, 0, 0));
				CSGD_Direct3D::GetInstance()->DrawHollowRect(diagonalFourteen, D3DCOLOR_ARGB(255, 255, 0, 0));
				CSGD_Direct3D::GetInstance()->DrawHollowRect(diagonalFifteen, D3DCOLOR_ARGB(255, 255, 0, 0));
				CSGD_Direct3D::GetInstance()->DrawHollowRect(diagonalSixteen, D3DCOLOR_ARGB(255, 255, 0, 0));
			}

			//draw diagonals
			if(i < unit->GetAttackRange()/onTree)
			{
				CSGD_Direct3D::GetInstance()->DrawHollowRect(diagonalOne, D3DCOLOR_ARGB(255, 255,0, 0));
				CSGD_Direct3D::GetInstance()->DrawHollowRect(diagonalTwo, D3DCOLOR_ARGB(255, 255,0, 0));
				CSGD_Direct3D::GetInstance()->DrawHollowRect(diagonalThree, D3DCOLOR_ARGB(255, 255, 0, 0));
				CSGD_Direct3D::GetInstance()->DrawHollowRect(diagonalFour, D3DCOLOR_ARGB(255, 255, 0, 0));
				CSGD_Direct3D::GetInstance()->DrawHollowRect(diagonalFive, D3DCOLOR_ARGB(255, 255, 0, 0));
				CSGD_Direct3D::GetInstance()->DrawHollowRect(diagonalSix, D3DCOLOR_ARGB(255, 255, 0, 0));
				CSGD_Direct3D::GetInstance()->DrawHollowRect(diagonalSeven, D3DCOLOR_ARGB(255, 255, 0, 0));
				CSGD_Direct3D::GetInstance()->DrawHollowRect(diagonalEight, D3DCOLOR_ARGB(255, 255, 0, 0));
			}
		}


	}
}

void CBattleGrid::UnHighlight()
{

}
