#include "WorldGrid.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../GamePlay.h"
#include "../MainMenu.h"
#include "../ArcadeGamePlay.h"
#include "../WorldPlay.h"

CWorldGrid::CWorldGrid(int mode)
{
	playMode = mode;
	m_nMapID = CSGD_TextureManager::GetInstance()->LoadTexture( _T("Resources/Graphics/IC_MapB.png" ) );


	//Set Up Country Vectors
	////////////////////////
	vector<CWorldTile*> cntry0;
	WTile.push_back(cntry0);
	vector<CWorldTile*> cntry1;
	WTile.push_back(cntry1);
	vector<CWorldTile*> cntry2;
	WTile.push_back(cntry2);
	vector<CWorldTile*> cntry3;
	WTile.push_back(cntry3);
	////////////////////////



	if(playMode == 0)
	{
		/*WTile.resize(4);
		for(unsigned int i = 0; i < 4; i++)
		{
		vector<CWorldTile*> temp;
		WTile.push_back(temp);

		if(i < 2)
		{
		CWorldTile* temptile = new CWorldTile(0,i,300+(i*200),200,"none");
		temptile->SetTerritory( 0 );
		WTile[i].push_back(temptile);


		}
		else
		{
		CWorldTile* temptile = new CWorldTile(0,i,300+((i-1)*200)-200,400,"none");
		temptile->SetTerritory( 0 );

		WTile[i].push_back(temptile);
		}
		}

		*/
		CWorldTile* ter00 = new CWorldTile(0,0,190,170,"none");
		ter00->SetTerritory( 0 );
		WTile[0].push_back(ter00);

		CWorldTile* ter10 = new CWorldTile(0,1,410,160,"none");
		ter10->SetTerritory( 0 );
		WTile[1].push_back(ter10);

		CWorldTile* ter20 = new CWorldTile(0,2,280,420,"none");
		ter20->SetTerritory( 0 );
		WTile[2].push_back(ter20);

		CWorldTile* ter30 = new CWorldTile(0,3,520,380,"none");
		ter30->SetTerritory( 0 );
		WTile[3].push_back(ter30);





		//Adjacencies
		WTile[0][0]->SetAdj(WTile[0][0]);
		WTile[0][0]->SetAdj(WTile[1][0]);
		WTile[0][0]->SetAdj(WTile[2][0]);
		WTile[0][0]->SetAdj(WTile[0][0]);
									 
		WTile[1][0]->SetAdj(WTile[1][0]);
		WTile[1][0]->SetAdj(WTile[3][0]);
		WTile[1][0]->SetAdj(WTile[2][0]);
		WTile[1][0]->SetAdj(WTile[0][0]);
									 
		WTile[2][0]->SetAdj(WTile[1][0]);
		WTile[2][0]->SetAdj(WTile[3][0]);
		WTile[2][0]->SetAdj(WTile[2][0]);
		WTile[2][0]->SetAdj(WTile[0][0]);
									 
		WTile[3][0]->SetAdj(WTile[1][0]);
		WTile[3][0]->SetAdj(WTile[3][0]);
		WTile[3][0]->SetAdj(WTile[3][0]);
		WTile[3][0]->SetAdj(WTile[2][0]);



	}
	else if( playMode == 1)
	{

		//Country 0 Territories
		////////////////////////
		CWorldTile* ter00 = new CWorldTile(0,0,120,160,"none");
		ter00->SetTerritory( 0 );
		WTile[0].push_back(ter00);

		CWorldTile* ter01 = new CWorldTile(0,0,220,90,"none");
		ter01->SetTerritory( 1 );
		WTile[0].push_back(ter01);

		CWorldTile* ter02 = new CWorldTile(0,0,190,180,"none");
		ter02->SetTerritory( 2 );
		WTile[0].push_back(ter02);

		CWorldTile* ter03 = new CWorldTile(0,0,220,230,"none");
		ter03->SetTerritory( 3 );
		WTile[0].push_back(ter03);
		////////////////////////



		//Country 1 Territories
		////////////////////////
		CWorldTile* ter10 = new CWorldTile(0,1,340,130,"none");
		ter10->SetTerritory( 0 );
		WTile[1].push_back(ter10);

		CWorldTile* ter11 = new CWorldTile(0,1,330,200,"none");
		ter11->SetTerritory( 1 );
		WTile[1].push_back(ter11);

		CWorldTile* ter12 = new CWorldTile(0,1,400,160,"none");
		ter12->SetTerritory( 2 );
		WTile[1].push_back(ter12);

		CWorldTile* ter13 = new CWorldTile(0,1,470,120,"none");
		ter13->SetTerritory( 3 );
		WTile[1].push_back(ter13);
		////////////////////////



		//Country 2 Territories
		////////////////////////
		CWorldTile* ter20 = new CWorldTile(0,2,320,330,"none");
		ter20->SetTerritory( 0 );
		WTile[2].push_back(ter20);

		CWorldTile* ter21 = new CWorldTile(0,2,270,420,"none");
		ter21->SetTerritory( 1 );
		WTile[2].push_back(ter21);

		CWorldTile* ter22 = new CWorldTile(0,2,365,455,"none");
		ter22->SetTerritory( 2 );
		WTile[2].push_back(ter22);

		CWorldTile* ter23 = new CWorldTile(0,2,250,530,"none");
		ter23->SetTerritory( 3 );
		WTile[2].push_back(ter23);
		////////////////////////




		//Country 3 Territories
		////////////////////////
		CWorldTile* ter30 = new CWorldTile(0,3,435,360,"none");
		ter30->SetTerritory( 0 );
		WTile[3].push_back(ter30);

		CWorldTile* ter31 = new CWorldTile(0,3,515,370,"none");
		ter31->SetTerritory( 1 );
		WTile[3].push_back(ter31);

		CWorldTile* ter32 = new CWorldTile(0,3,580,270,"none");
		ter32->SetTerritory( 2 );
		WTile[3].push_back(ter32);

		CWorldTile* ter33 = new CWorldTile(0,3,575,450,"none");
		ter33->SetTerritory( 3 );
		WTile[3].push_back(ter33);
		////////////////////////




		//Country 0 Adjacencies
		////////////////////////
		WTile[0][0]->SetAdj(WTile[0][1]);
		WTile[0][0]->SetAdj(WTile[0][2]);
		WTile[0][0]->SetAdj(WTile[2][1]);
		WTile[0][0]->SetAdj(WTile[0][0]);

		WTile[0][1]->SetAdj(WTile[0][1]);
		WTile[0][1]->SetAdj(WTile[1][0]);
		WTile[0][1]->SetAdj(WTile[0][2]);
		WTile[0][1]->SetAdj(WTile[0][0]);

		WTile[0][2]->SetAdj(WTile[0][1]);
		WTile[0][2]->SetAdj(WTile[0][3]);
		WTile[0][2]->SetAdj(WTile[0][3]);
		WTile[0][2]->SetAdj(WTile[0][0]);

		WTile[0][3]->SetAdj(WTile[0][2]);
		WTile[0][3]->SetAdj(WTile[1][1]);
		WTile[0][3]->SetAdj(WTile[2][0]);
		WTile[0][3]->SetAdj(WTile[0][0]);
		////////////////////////



		//Country 1 Adjacencies
		////////////////////////
		WTile[1][0]->SetAdj(WTile[1][0]);
		WTile[1][0]->SetAdj(WTile[1][2]);
		WTile[1][0]->SetAdj(WTile[1][1]);
		WTile[1][0]->SetAdj(WTile[0][1]);

		WTile[1][1]->SetAdj(WTile[1][0]);
		WTile[1][1]->SetAdj(WTile[1][2]);
		WTile[1][1]->SetAdj(WTile[2][0]);
		WTile[1][1]->SetAdj(WTile[0][3]);

		WTile[1][2]->SetAdj(WTile[1][0]);
		WTile[1][2]->SetAdj(WTile[1][3]);
		WTile[1][2]->SetAdj(WTile[1][2]);
		WTile[1][2]->SetAdj(WTile[1][1]);

		WTile[1][3]->SetAdj(WTile[1][3]);
		WTile[1][3]->SetAdj(WTile[3][2]);
		WTile[1][3]->SetAdj(WTile[3][0]);
		WTile[1][3]->SetAdj(WTile[1][2]);
		////////////////////////



		//Country 2 Adjacencies
		////////////////////////
		WTile[2][0]->SetAdj(WTile[1][1]);
		WTile[2][0]->SetAdj(WTile[2][0]);
		WTile[2][0]->SetAdj(WTile[2][1]);
		WTile[2][0]->SetAdj(WTile[0][3]);

		WTile[2][1]->SetAdj(WTile[2][0]);
		WTile[2][1]->SetAdj(WTile[3][0]);
		WTile[2][1]->SetAdj(WTile[2][2]);
		WTile[2][1]->SetAdj(WTile[0][0]);
		WTile[2][1]->SetAdj(WTile[2][3]);


		WTile[2][2]->SetAdj(WTile[2][1]);
		WTile[2][2]->SetAdj(WTile[3][3]);
		WTile[2][2]->SetAdj(WTile[2][2]);
		WTile[2][2]->SetAdj(WTile[2][3]);

		WTile[2][3]->SetAdj(WTile[2][1]);
		WTile[2][3]->SetAdj(WTile[2][2]);
		WTile[2][3]->SetAdj(WTile[2][3]);
		WTile[2][3]->SetAdj(WTile[2][3]);
		////////////////////////



		//Country 3 Adjacencies
		////////////////////////
		WTile[3][0]->SetAdj(WTile[1][3]);
		WTile[3][0]->SetAdj(WTile[3][1]);
		WTile[3][0]->SetAdj(WTile[3][0]);
		WTile[3][0]->SetAdj(WTile[2][1]);

		WTile[3][1]->SetAdj(WTile[3][2]);
		WTile[3][1]->SetAdj(WTile[3][2]);
		WTile[3][1]->SetAdj(WTile[3][3]);
		WTile[3][1]->SetAdj(WTile[3][0]);

		WTile[3][2]->SetAdj(WTile[1][3]);
		WTile[3][2]->SetAdj(WTile[3][2]);
		WTile[3][2]->SetAdj(WTile[3][1]);
		WTile[3][2]->SetAdj(WTile[3][1]);

		WTile[3][3]->SetAdj(WTile[3][1]);
		WTile[3][3]->SetAdj(WTile[3][3]);
		WTile[3][3]->SetAdj(WTile[3][3]);
		WTile[3][3]->SetAdj(WTile[2][2]);
		////////////////////////

	}

	m_pCurrent = WTile[0][0];


}


CWorldGrid::~CWorldGrid(void)
{
	for(unsigned int i = 0; i < WTile.size(); i++)
	{
		for(unsigned int j = 0; j < WTile[i].size();j++)
		{
			delete WTile[i][j];
		}
		WTile[i].clear();

	}
	WTile.clear();

	
}



void CWorldGrid::Update()
{

}

void CWorldGrid::Render()
{
	CWorldGrid* temp;
	if ( CMainMenu::GetInstance()->GetArcade() == true )
	{
		temp = CArcadeGamePlay::GetInstance()->GetWorld();
	}
	else if (CMainMenu::GetInstance()->GetCampaign())
	{
		temp = CGamePlay::GetInstance()->GetWorld();
	}
	else if (CMainMenu::GetInstance()->GetVersus() == true || CMainMenu::GetInstance()->GetQuickMatch() == true)
	{
		temp = CWorldPlay::GetInstance()->GetWorld();
	}



	//CSGD_TextureManager::GetInstance()->Draw( m_nMapID, 0, 0 );

}

void CWorldGrid::ChangeCurrent(CWorldTile* wtile)
{
	m_pCurrent = wtile;
	m_pCurrent->SetCountry(wtile->GetCountry());
	m_pCurrent->SetTerritory(wtile->GetTerritory());

}

void CWorldGrid::Highlight()
{

}

void CWorldGrid::UnHighlight()
{

}


void CWorldGrid::ChangePrevious(CWorldTile* current)
{
	m_pPrevious = current;
}
