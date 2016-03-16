////////////////////////////////////////////////////
// File:	CGamePlay.cpp
// Purpose:	Handles all game logic for the WorldMap
//			
//
// Author:	Joshua Ford (joshua.c.ford@gmail.com)
// Course:	SGP 1310
////////////////////////////////////////////////////

#include "GamePlay.h"

#include "SGD Wrappers\CSGD_DirectInput.h"
#include "SGD Wrappers\CSGD_TextureManager.h"
#include "MainMenu.h"
#include "Game.h"
#include "Pause.h"
#include "Player.h"
#include "Enemy.h"
#include "BattlePlay.h"
#include "SGD Wrappers\CSGD_XAudio2.h"
#include "TileSystem\WorldGrid.h"
#include "BasePlayer.h"
#include "WinGame.h"
#include "LoseGame.h"
#include "Barracks.h"
#include "Farm.h"
#include "WarFactory.h"
#include "Mine.h"
#include "WoodMill.h"
#include "Capitol.h"
#include "Particle System\ParticleSystem.h"
#include "TileSystem\WorldTile.h"




CGamePlay* CGamePlay::GetInstance( void )
{

	//Global Instance
	static CGamePlay s_Instance;
	return &s_Instance;

}


//Functions for every state
bool CGamePlay::Input			( void )	
{
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();

	//Checking the cheat input to see if the input is done correctly
	CheatCheck();

	///Pause the Game

	if (pDI->JoystickButtonPressed( 6, 0 ) || pDI->JoystickButtonPressed( 6, 1 ) )
	{
		CGame::GetInstance()->PushState( CPause::GetInstance() );
		return true;
	}
	else if (pDI->KeyPressed( DIK_ESCAPE ) == true || pDI->JoystickButtonPressed( 9, JOYSTICKID1 ) == true)
	{
		CGame::GetInstance()->PushState( CPause::GetInstance() );
		return true;
	}



	if (m_bBuildInterface == false && m_bAMMode == false && m_bTerritoryMenuUp == false && m_bRecruitInterface == false)
	{
		HandleMovingOnTerritories();
		HandleEnterForClickingOnTerritories();
	}
	else if (m_bBuildInterface == true)
	{
		//Handle all movement for the cursor on the Build interface
		HandleMovingOnBuildInterface();
		HandleEnterOnBuildInterface();
	}
	else if (m_bRecruitInterface == true)
	{
		//Handle all movement for the cursor on the Recruit interface
		HandleMovingOnRecruitInterface();
		HandleEnterOnRecruitInterface();
	}
	else if (m_bTerritoryMenuUp == true)
	{
		//Handling the territory Menu when you click on what Territory you want to access
		HandleMovingOnTerritoryMenu();
		HandleEnterOnTerritoryMenu();
	}
	else if (m_bAMMode == true)
	{
		//Hanlding the moving and attack from Territory to Territory
		HandleInputForMoveAndAttack();
		HandleEnterForMoveAndAttack();
	}



	// Keep running
	return true;
}
void CGamePlay::Update			( float fElapsedTime )	
{
	/*if (m_ncurrentPlayer->GetType() == PLAYER)
	{
	CheckIsBuilt( 1 );
	}
	else
	CheckIsBuilt( 2 );*/

	//Updating AI
	//WinCheck();
	if (m_ncurrentPlayer->GetType() == ENEMY)
	{
		UpdateAI();
	}
	

	//Updating Tile Cursor;
	m_fHoverTimer += fElapsedTime;
	if(m_fHoverTimer >= 0.06f)
	{
		if(m_bHoverUp == true)
		{
			--m_nHoverOffset;
			if(m_nHoverOffset < -5)
				m_bHoverUp = false;
		}
		else
		{
			++m_nHoverOffset;
			if(m_nHoverOffset > 5)
				m_bHoverUp = true;
		}

		m_fHoverTimer = 0;
	}


	//Particle Timer Updates
	if(m_fCTimer <= 10.0f)
	{
		m_fCTimer += fElapsedTime;
		m_psConstruction->Update(fElapsedTime);
	}
	if(m_fRTimer <= 10.0f)
	{
		m_fRTimer += fElapsedTime;
		m_psRecruit->Update(fElapsedTime);
	}
	if(m_fTurnTimer <= 10.0f)
	{
		m_fTurnTimer += fElapsedTime;
		m_psTurn->Update(fElapsedTime);
	}

	m_fTime += fElapsedTime;

	if (WinCheck() == true )
	{
		if (m_bPlayerOneWins == 4)
		{
			CGame::GetInstance()->ChangeState( CWinGame::GetInstance() );
		}
		else if (m_bPlayerTwoWins == 4)
		{
			CGame::GetInstance()->ChangeState( CLoseGame::GetInstance() );
		}
	}

}
void CGamePlay::Render			( void )		
{
	CSGD_TextureManager::GetInstance()->Draw(m_nBackgroundID,0,0);

	//Renders army roster for currently highlighted territory
	if(m_cWorld->GetCurrent()->GetOwner() != 0)
	{
		RenderUnits();
	}


	//Render Campaign 2x2 World Map
	m_cWorld->Render();
	RenderOwnership();

	//Render the Player Turn
	TurnRender();


	//Renders rest of interface
	InterfaceRender();


	//Renders all buildings on the Map
	RenderBuildings();


	//Renders Menu for purchasing buildings
	if(m_bBuildInterface && m_cWorld->GetCurrent()->GetBuilding() == -1)
		BuildRender();

	//Renders Menu for purchasing units from buildings
	if(m_bRecruitInterface)
		RecruitRender();





	//Resource Allocation at beginning of turn
	if(m_fTime <= 3.5f && m_nTurnNumber != 0)
	{
		RenderResources();
	}

	//Render The Cursor here
	if (m_bTerritoryMenuUp == true)
	{
		ShowTerritoryOptions();
	}
	else 
		RenderTileCursor();


	//Renders the move units to territory window
	if (m_bAMMode == true)
	{
		RenderOptionsForMove();
	}

	//Render Timers for particles
	if(m_fCTimer >= 0.5f && m_fCTimer <= 1.0f)
	{
		m_psConstruction->Render();
	}


	if(m_fRTimer >= 0.5f && m_fRTimer <= 1.0f)
	{
		m_psRecruit->Render();
	}

	if(m_fTurnTimer <= 2.0f)
	{
		m_psTurn->Render();
	}

	if (m_nTurnNumber < 4 && m_ncurrentPlayer->GetType() == PLAYER)
	{
		ShowTutorial();
	}

}
void CGamePlay::Enter			( void )	
{
	m_nBackgroundID			= -1;

	curX = 2;
	curY = 0;

	m_bPlayerOneWins = 0;
	m_bPlayerTwoWins = 0;

	m_nBackgroundID =  CSGD_TextureManager::GetInstance()->LoadTexture( _T( "resources/graphics/campaign.png") );

	m_nFarm = CSGD_TextureManager::GetInstance()->LoadTexture(_T("resources/graphics/farm.png"));
	m_nMine = CSGD_TextureManager::GetInstance()->LoadTexture(_T("resources/graphics/mine.png"));
	m_nLumber = CSGD_TextureManager::GetInstance()->LoadTexture(_T("resources/graphics/lumbermill.png"));
	m_nBarracks = CSGD_TextureManager::GetInstance()->LoadTexture(_T("resources/graphics/barracks.png"));
	m_nFactory = CSGD_TextureManager::GetInstance()->LoadTexture(_T("resources/graphics/factory.png"));

	m_nFoot = CSGD_TextureManager::GetInstance()->LoadTexture(_T("resources/graphics/swordsman.png"));
	m_nPriest = CSGD_TextureManager::GetInstance()->LoadTexture(_T("resources/graphics/priest.png"));
	m_nWizard = CSGD_TextureManager::GetInstance()->LoadTexture(_T("resources/graphics/wizard.png"));
	m_nSkeleton = CSGD_TextureManager::GetInstance()->LoadTexture(_T("resources/graphics/SkeletalWarrior.png"));
	m_nSPriest = CSGD_TextureManager::GetInstance()->LoadTexture(_T("resources/graphics/shadow_priest.png"));
	m_nNecro = CSGD_TextureManager::GetInstance()->LoadTexture(_T("resources/graphics/Necromancer.png"));
	m_nCavalry = CSGD_TextureManager::GetInstance()->LoadTexture(_T("resources/graphics/calvary.png"));
	m_nCatapult = CSGD_TextureManager::GetInstance()->LoadTexture(_T("resources/graphics/catapult.png"));
	m_nDK = CSGD_TextureManager::GetInstance()->LoadTexture(_T("resources/graphics/death_knight.png"));
	m_nWM = CSGD_TextureManager::GetInstance()->LoadTexture(_T("resources/graphics/war_machine.png"));


	m_nHuman = CSGD_TextureManager::GetInstance()->LoadTexture(_T("resources/graphics/humans.png"));
	m_nUndead = CSGD_TextureManager::GetInstance()->LoadTexture(_T("resources/graphics/Undead_Icon.png"));
	m_nTurnBox = CSGD_TextureManager::GetInstance()->LoadTexture(_T("resources/graphics/turn.png"));
	m_nResourceBox = CSGD_TextureManager::GetInstance()->LoadTexture(_T("resources/graphics/resources.png"));
	m_nFoodIcon = CSGD_TextureManager::GetInstance()->LoadTexture(_T("resources/graphics/food.png"));
	m_nWoodIcon = CSGD_TextureManager::GetInstance()->LoadTexture(_T("resources/graphics/wood.png"));
	m_nSteelIcon = CSGD_TextureManager::GetInstance()->LoadTexture(_T("resources/graphics/steel.png"));
	m_nBlankBar = CSGD_TextureManager::GetInstance()->LoadTexture(_T("resources/graphics/blankbar.png"));
	m_nBlankBox = CSGD_TextureManager::GetInstance()->LoadTexture(_T("resources/graphics/blankbox.png"));
	m_nBlankPanel = CSGD_TextureManager::GetInstance()->LoadTexture(_T("resources/graphics/backpanel.png"));
	commandImageID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/Command_box.png"));
	m_nMoveBox = CSGD_TextureManager::GetInstance()->LoadTexture(_T("resources/graphics/unitMove.png"));

	//Cursor Image ID Set
	cursorImageID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/Cursor_Battle.png"));


	m_nResources = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("resources/sounds/allocation.wav"));

	m_nPlaceBuild = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("resources/sounds/placebuilding.wav"));

	m_nPlaceBuild = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("resources/sounds/placebuilding.wav"));
	m_nConquer =CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("resources/sounds/CountryTakeover.wav"));

	m_nTurnSound = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("resources/sounds/AD_EndTurn_Final.wav"));

	

	m_cWorld = new CWorldGrid(0);

	m_cPlayerOne = CPlayer::GetInstance();

	m_cPlayerTwo = CEnemy::GetInstance();

	m_ncurrentPlayer = m_cPlayerOne;

	m_nTurnOrder = 0;

	m_nWidth = m_cWorld->GetGrid()[2][0]->GetPosX();

	m_nHeight = m_cWorld->GetGrid()[2][0]->GetPosY();

	m_nInOptions = false;

	m_nCursor	 = 0;

	m_nTerritoryOptionsCursor	 = 0;

	m_nBuildCursor = 0;

	m_bIsInWorld = true;

	m_bIsMoving = false;

	m_cWorld->ChangeCurrent( m_cWorld->GetGrid()[2][0]);

	m_bBuildMode = true;

	m_bMoveandAttackMode = false;

	m_bBuildInterface = false;

	//Set occupied for the territories that are occupied in the beginning
	m_cWorld->GetGrid()[2][0]->SetOccupied( true );
	m_cWorld->GetGrid()[1][0]->SetOccupied( true );

	//m_cPlayerOne->AddBuilding(new Capitol(1,m_cWorld->GetGrid()[2][0]->GetPosX(),m_cWorld->GetGrid()[2][0]->GetPosY()));
	//m_cPlayerTwo->AddBuilding(new Capitol(1,m_cWorld->GetGrid()[1][0]->GetPosX(),m_cWorld->GetGrid()[1][0]->GetPosY()));



	//Setting owner for both territories off the bat
	m_cWorld->GetGrid()[2][0]->SetOwner( 1 );
	m_cWorld->GetGrid()[1][0]->SetOwner( 2 );


	////Checking win condition for both players
	//m_cWorld->GetGrid()[3][0]->SetOwner( 1 );
	//m_cWorld->GetGrid()[0][0]->SetOwner( 1 );

	//Set the buildings in the first two starting Zones
	m_cWorld->GetGrid()[2][0]->SetBuilding( -1 );
	m_cWorld->GetGrid()[1][0]->SetBuilding( -1 );


	//Greying out for the buttons after their phase is done
	m_nGreyedOut = 30;
	m_cWorld->GetCurrent()->SetOwner( m_cWorld->GetGrid()[2][0]->GetOwner() );

	m_cWorld->GetCurrent()->SetOccupied( m_cWorld->GetGrid()[2][0]->GetOccupied() );

	m_cWorld->GetCurrent()->SetBuilding(-1);

	//m_bPlayerOneWins = false;

	//m_bPlayerTwoWins = false;

	m_nTurnNumber = 0;

	//Loading Particle Emitters
	m_psConstruction = new CParticleSystem;
	m_psConstruction->LoadEffect("ConstructionZone", "Resources/XMLFiles/BuildParticle.xml");
	m_psConstruction->CreateEmitter("ConstructionZone");
	m_fCTimer = 40;

	m_psRecruit = new CParticleSystem;
	m_psRecruit->LoadEffect("Recruit", "Resources/XMLFiles/RecruitParticle.xml");
	m_psRecruit->CreateEmitter("Recruit");
	m_fRTimer = 40;


	m_psTurn = new CParticleSystem;
	m_psTurn->LoadEffect("Recruit", "Resources/XMLFiles/TurnChangeParticle.xml");
	m_psTurn->CreateEmitter("Recruit");
	m_fTurnTimer = 40;
	m_psTurn->GetEffect(0)->SetEmitterPosX(75.0f);
	m_psTurn->GetEffect(0)->SetEmitterPosY(50.0f);

	//Unit Display variables
	soldierCount = 0;
	priestCount = 0;
	wizardCount = 0;
	cavalryCount = 0;
	catapultCount = 0;
	generalPresent = false;

	skeletonCount = 0;
	spriestCount = 0;
	necroCount = 0;
	dkCount = 0;
	wmCount = 0;
	lichPresent = false;


	m_MovesoldierCount		  = 0;
	m_MovepriestCount		  = 0;
	m_MovewizardCount		  = 0;
	m_MovecavalryCount		  = 0;
	m_MovecatapultCount		  = 0;
	m_MovegeneralCount		  = 0;

	m_MoveskeletonCount		  = 0;
	m_MovespriestCount		  = 0;
	m_MovenecroCount		  = 0;
	m_MovedkCount			  = 0;
	m_MovewmCount			  = 0;
	m_MovelichCount			  = 0;


	if (CMainMenu::GetInstance()->GetQuickMatch() == true)
	{
		m_nTurnCap = 10;
	}
	else
	{
		m_nTurnCap = -1;
	}


	m_bSelectTerritoryToMove = false;
	m_bAIEnabled = true;
	m_cPlayerTwo->SetAI(m_bAIEnabled);

	m_bTerritoryMenuUp = false;
	m_bAMMode = false;

	for(unsigned int i = 0; i < m_cPlayerTwo->GetUnits().size();i++)
	{
		m_cPlayerTwo->GetUnits()[i]->SetCountry(1);
		m_cPlayerTwo->GetUnits()[i]->SetTerritory(0);

	}

}
void CGamePlay::Exit( void )	
{
	delete m_cWorld;
	delete m_psConstruction;
	delete m_psRecruit;
	delete m_psTurn;

	m_cPlayerOne->Reset();
	m_cPlayerTwo->Reset();
}
bool CGamePlay::Move( CWorldTile * Tiletomove, int temp )
{

	m_cWorld->ChangeCurrent( Tiletomove->GetAdj()[temp] );


	return true;

}
void CGamePlay::Build( CBasePlayer* TempPlayer )
{
	m_bBuildInterface = true;

	if(m_cWorld->GetCurrent()->GetBuilding() == BARRACKS || m_cWorld->GetCurrent()->GetBuilding() == WARFACTORY)
	{
		m_bRecruitInterface = true;
		m_bBuildInterface = false;
	}
}
void CGamePlay::CheckIsBuilt	( int count )
{
	for (unsigned int i = 0; i < m_cWorld->GetGrid().size(); i++)
	{
		for (unsigned int j = 0; j < m_cWorld->GetGrid()[i].size(); j++)
		{
			if (m_cWorld->GetGrid()[i][j]->GetOwner() == count)
			{
				if (m_cWorld->GetGrid()[i][j]->GetBuilding() != 1 && m_cWorld->GetGrid()[i][j]->GetBuilt() == true)
				{
					m_bBuildMode = false;
				}
				else
				{
					m_bBuildMode = true;
				}
			}

		}
	}
}
void CGamePlay::InterfaceRender ( void )
{
	//Renders army roster for currently highlighted territory
	if(m_cWorld->GetCurrent()->GetOwner() != 0)
	{
		RenderUnits();
	}


	//Render the Player Turn
	TurnRender();


	PlayerResources();
}
void CGamePlay::BuildRender		( void )
{
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();
	CBitmapFont* pFont = CGame::GetInstance()->GetFont();

	pFont->Draw(_T("Select A Building:"),150,420,1.0f,D3DCOLOR_ARGB(255,0,0,0));

	pTM->Draw(m_nBlankPanel,150,450);


	for(int i = 0; i < 5; i++)
	{
		RECT border = {150+(i*102),450,250+(i*102),575};
		CSGD_Direct3D::GetInstance()->DrawHollowRect(border,D3DCOLOR_ARGB(255,0,0,0),2);
	}

	RECT ex = {625,575,800,648};
	CSGD_Direct3D::GetInstance()->DrawHollowRect(ex,D3DCOLOR_ARGB(255,0,0,0),2);
	pFont->Draw(_T("Exit Menu"),635,577,0.75f,D3DCOLOR_ARGB(255,255,255,255));



	CSGD_TextureManager::GetInstance()->Draw(m_nFarm,160,450,1.0,1.0,0);
	pFont->Draw(_T("Farm"),160,510,0.60f,D3DCOLOR_ARGB(255,0,0,0));
	pFont->Draw(_T("Wood:  20"),160,530,0.60f,D3DCOLOR_ARGB(255,0,0,0));


	CSGD_TextureManager::GetInstance()->Draw(m_nMine,260,450,1.0,1.0,0);
	pFont->Draw(_T("Mine"),260,510,0.60f,D3DCOLOR_ARGB(255,0,0,0));
	pFont->Draw(_T("Wood:  15"),260,530,0.60f,D3DCOLOR_ARGB(255,0,0,0));
	pFont->Draw(_T("Steel:  10"),260,550,0.60f,D3DCOLOR_ARGB(255,0,0,0));



	CSGD_TextureManager::GetInstance()->Draw(m_nLumber,360,450,1.0,1.0,0);
	pFont->Draw(_T("Lumber Mill"),360,510,0.60f,D3DCOLOR_ARGB(255,0,0,0));
	pFont->Draw(_T("Wood:  20"),360,530,0.60f,D3DCOLOR_ARGB(255,0,0,0));
	pFont->Draw(_T("Steel:  5"),360,550,0.60f,D3DCOLOR_ARGB(255,0,0,0));


	CSGD_TextureManager::GetInstance()->Draw(m_nBarracks,460,450,1.0,1.0,0);
	pFont->Draw(_T("Barracks"),460,510,0.60f,D3DCOLOR_ARGB(255,0,0,0));
	pFont->Draw(_T("Wood:  30"),460,530,0.60f,D3DCOLOR_ARGB(255,0,0,0));
	pFont->Draw(_T("Steel:  10"),460,550,0.60f,D3DCOLOR_ARGB(255,0,0,0));


	CSGD_TextureManager::GetInstance()->Draw(m_nFactory,560,450,1.0,1.0,0);
	pFont->Draw(_T("War Factory"),560,510,0.60f,D3DCOLOR_ARGB(255,0,0,0));
	pFont->Draw(_T("Wood:  20"),560,530,0.60f,D3DCOLOR_ARGB(255,0,0,0));
	pFont->Draw(_T("Steel:  40"),560,550,0.60f,D3DCOLOR_ARGB(255,0,0,0));


	//Cursor for the Building interface
	if (m_nBuildCursor != 5)
	{
		pTM->Draw(cursorImageID, 160 + (m_nBuildCursor * 100) , 420, 0.3f, 0.3f, nullptr, 35.0f / 2, 72.0f / 2, 0, D3DCOLOR_XRGB(255,0,0));
	}
	else if(m_nBuildCursor == 5)
	{
		pTM->Draw(cursorImageID, 600 , 577, 0.3f, 0.3f, nullptr, 35.0f / 2, 72.0f / 2,0,D3DCOLOR_XRGB(255,0,0));
	}
}
void CGamePlay::RecruitRender	( void )
{
	CBitmapFont* pFont = CGame::GetInstance()->GetFont();

	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();

	pFont->Draw(_T("Select A Unit:"),150,420,1.0f,D3DCOLOR_ARGB(255,0,0,0));

	pTM->Draw(m_nBlankPanel,150,450);




	if(m_cWorld->GetCurrent()->GetBuilding() == BARRACKS)
	{

		for(int i = 0; i < 3; i++)
		{
			RECT border = {150+(i*102),450,250+(i*102),575};
			CSGD_Direct3D::GetInstance()->DrawHollowRect(border,D3DCOLOR_ARGB(255,0,0,0),2);
		}


		pFont->Draw(_T("Exit Menu"),560,500,0.75f,D3DCOLOR_ARGB(255,255,255,255));



		if(m_nTurnOrder == 0)
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nFoot,160,450,1.0,1.0,0);
			pFont->Draw(_T("Footsoldier"),160,500,0.75f,D3DCOLOR_ARGB(255,255,255,255));
			pFont->Draw(_T("Food:  10"),160,530,0.75f,D3DCOLOR_ARGB(255,255,255,255));


			CSGD_TextureManager::GetInstance()->Draw(m_nPriest,260,450,1.0,1.0,0);
			pFont->Draw(_T("Priest"),260,500,0.75f,D3DCOLOR_ARGB(255,255,255,255));
			pFont->Draw(_T("Food:  25"),260,530,0.75f,D3DCOLOR_ARGB(255,255,255,255));



			CSGD_TextureManager::GetInstance()->Draw(m_nWizard,360,450,1.0,1.0,0);
			pFont->Draw(_T("Wizard"),360,500,0.75f,D3DCOLOR_ARGB(255,255,255,255));
			pFont->Draw(_T("Food:  50"),360,530,0.75f,D3DCOLOR_ARGB(255,255,255,255));

		}
		else if(m_nTurnOrder == 1)
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nSkeleton,160,450,1.0,1.0,0);
			pFont->Draw(_T("Skeletal Warrior"),160,500,0.75f,D3DCOLOR_ARGB(255,255,255,255));
			pFont->Draw(_T("Food:  10"),160,530,0.75f,D3DCOLOR_ARGB(255,255,255,255));


			CSGD_TextureManager::GetInstance()->Draw(m_nSPriest,260,450,1.0,1.0,0);
			pFont->Draw(_T("Shadow Priest"),260,500,0.75f,D3DCOLOR_ARGB(255,255,255,255));
			pFont->Draw(_T("Food:  25"),260,530,0.75f,D3DCOLOR_ARGB(255,255,255,255));



			CSGD_TextureManager::GetInstance()->Draw(m_nNecro,360,450,1.0,1.0,0);
			pFont->Draw(_T("Necromancer"),360,500,0.75f,D3DCOLOR_ARGB(255,255,255,255));
			pFont->Draw(_T("Food:  50"),360,530,0.75f,D3DCOLOR_ARGB(255,255,255,255));
		}
		//Cursor for the Building interface
		if (m_nBuildCursor != 3)
		{
			pTM->Draw(cursorImageID, 160 + (m_nBuildCursor * 100) , 420, 0.3f, 0.3f, nullptr, 35.0f / 2, 72.0f / 2, 0, D3DCOLOR_XRGB(255,0,0));
		}
		else if(m_nBuildCursor == 3)
		{
			pTM->Draw(cursorImageID, 500,500, 0.3f, 0.3f, nullptr, 35.0f / 2, 72.0f / 2, 0,D3DCOLOR_XRGB(255,0,0));
		}


	}
	else if(m_cWorld->GetCurrent()->GetBuilding() == WARFACTORY)
	{
		for(int i = 0; i < 2; i++)
		{
			RECT border = {150+(i*102),450,250+(i*102),575};
			CSGD_Direct3D::GetInstance()->DrawHollowRect(border,D3DCOLOR_ARGB(255,0,0,0),2);
		}



		pFont->Draw(_T("Exit Menu"),560,500,0.75f,D3DCOLOR_ARGB(255,255,255,255));



		if(m_nTurnOrder == 0)
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCavalry,160,450,1.0,1.0,0);
			pFont->Draw(_T("Cavalry"),160,500,0.60f,D3DCOLOR_ARGB(255,255,255,255));
			pFont->Draw(_T("Food:  50"),160,520,0.60f,D3DCOLOR_ARGB(255,255,255,255));
			pFont->Draw(_T("Steel:  10"),160,540,0.60f,D3DCOLOR_ARGB(255,255,255,255));



			CSGD_TextureManager::GetInstance()->Draw(m_nCatapult,260,450,1.0,1.0,0);
			pFont->Draw(_T("Catapult"),260,500,0.60f,D3DCOLOR_ARGB(255,255,255,255));
			pFont->Draw(_T("Wood:  100"),260,520,0.60f,D3DCOLOR_ARGB(255,255,255,255));
			pFont->Draw(_T("Steel:  100"),260,540,0.60f,D3DCOLOR_ARGB(255,255,255,255));


		}
		else if(m_nTurnOrder == 1)
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nDK,160,450,1.0,1.0,0);
			pFont->Draw(_T("Death Knight"),160,500,0.60f,D3DCOLOR_ARGB(255,255,255,255));
			pFont->Draw(_T("Food:  50"),160,520,0.60f,D3DCOLOR_ARGB(255,255,255,255));
			pFont->Draw(_T("Steel:  10"),160,540,0.60f,D3DCOLOR_ARGB(255,255,255,255));


			CSGD_TextureManager::GetInstance()->Draw(m_nWM,260,450,1.0,1.0,0);
			pFont->Draw(_T("War Machine"),260,500,0.60f,D3DCOLOR_ARGB(255,255,255,255));
			pFont->Draw(_T("Wood:  100"),260,520,0.60f,D3DCOLOR_ARGB(255,255,255,255));
			pFont->Draw(_T("Steel:  100"),260,540,0.60f,D3DCOLOR_ARGB(255,255,255,255));


		}
		if (m_nBuildCursor != 2)
		{
			pTM->Draw(cursorImageID, 160 + (m_nBuildCursor * 100) , 420, 0.3f, 0.3f, nullptr, 35.0f / 2, 72.0f / 2, 0,D3DCOLOR_XRGB(255,0,0));
		}
		else if(m_nBuildCursor == 2)
		{
			pTM->Draw(cursorImageID, 500,500, 0.3f, 0.3f, nullptr, 35.0f / 2, 72.0f / 2,0,D3DCOLOR_XRGB(255,0,0));
		}

	}
}
void CGamePlay::RenderBuildings ( void )
{
	for(unsigned int i = 0; i < m_cPlayerOne->GetBuildings().size();i++)
	{
		if (m_cPlayerOne->GetBuildings()[i]->GetPosBuildX() == m_cWorld->GetCurrent()->GetPosX() && m_cPlayerOne->GetBuildings()[i]->GetPosBuildY() == m_cWorld->GetCurrent()->GetPosY())
		{
			m_cPlayerOne->GetBuildings()[i]->Render();
		}
	}


	for(unsigned int i = 0; i < m_cPlayerTwo->GetBuildings().size();i++)
	{
		if (m_cPlayerTwo->GetBuildings()[i]->GetPosBuildX() == m_cWorld->GetCurrent()->GetPosX() && m_cPlayerTwo->GetBuildings()[i]->GetPosBuildY() == m_cWorld->GetCurrent()->GetPosY())
		{
			m_cPlayerTwo->GetBuildings()[i]->Render();
		}

	}
}
void CGamePlay::RenderUnits		( void )
{
	CBitmapFont* pFont = CGame::GetInstance()->GetFont();
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();


	pTM->Draw(m_nBlankPanel,150,450);


	if(m_cWorld->GetCurrent()->GetOwner() == 1)
	{

		soldierCount = 0;
		priestCount = 0;
		wizardCount = 0;
		cavalryCount = 0;
		catapultCount = 0;
		generalPresent = false;

		for(unsigned int i = 0; i < m_cPlayerOne->GetUnits().size();i++)
		{
			if(m_cPlayerOne->GetUnits()[i]->GetCountry() == m_cWorld->GetCurrent()->GetCountry() && m_cPlayerOne->GetUnits()[i]->GetTerritory() == m_cWorld->GetCurrent()->GetTerritory())
			{
				switch(m_cPlayerOne->GetUnits()[i]->GetUnitID())
				{
				case FOOTSOLDIERS:
					{
						soldierCount += 1;
					}
					break;
				case PRIEST:
					{
						priestCount += 1;
					}
					break;
				case WIZARD:
					{
						wizardCount += 1;
					}
					break;
				case CALVARY:
					{
						cavalryCount += 1;
					}
					break;
				case CATAPULT:
					{
						catapultCount += 1;
					}
					break;
				case GENERAL:
					{
						generalPresent = true;
					}
					break;
				case SKELETALWARRIOR:
				case NECROMANCER:
				case DEATHKNIGHT:
				case SHADOWPRIEST:
				case WARMACHINE:
				case LICH:
				case VOLATILEGHOUL:
				default:
					break;
				}
			}
		}


		TOSTRINGSTREAM fs, p, w, cv, cp;
		fs << soldierCount;
		p << priestCount;
		w << wizardCount;
		cv << cavalryCount;
		cp << catapultCount;

		pFont->Draw(_T("Units in Territory: "),300,450,0.75f,D3DCOLOR_ARGB(255,0,0,0));

		if(generalPresent)
		{
			pFont->Draw(_T("General in Command"),275,465,0.75f,D3DCOLOR_ARGB(255,0,0,0));
		}

		pFont->Draw(_T("Foot Soldiers: "),200,490,0.65f,D3DCOLOR_ARGB(255,0,0,0));
		pFont->Draw(fs.str().c_str(),300,490,0.65f,D3DCOLOR_ARGB(255,0,0,0));

		pFont->Draw(_T("Priests: "),200,505,0.65f,D3DCOLOR_ARGB(255,0,0,0));
		pFont->Draw(p.str().c_str(),300,505,0.65f,D3DCOLOR_ARGB(255,0,0,0));

		pFont->Draw(_T("Wizards: "),200,520,0.65f,D3DCOLOR_ARGB(255,0,0,0));
		pFont->Draw(w.str().c_str(),300,520,0.65f,D3DCOLOR_ARGB(255,0,0,0));

		pFont->Draw(_T("Catapults: "),350,490,0.65f,D3DCOLOR_ARGB(255,0,0,0));
		pFont->Draw(cp.str().c_str(),450,490,0.65f,D3DCOLOR_ARGB(255,0,0,0));

		pFont->Draw(_T("Cavalry: "),350,505,0.65f,D3DCOLOR_ARGB(255,0,0,0));
		pFont->Draw(cv.str().c_str(),450,505,0.65f,D3DCOLOR_ARGB(255,0,0,0));










	}
	else if(m_cWorld->GetCurrent()->GetOwner() == 2)
	{

		skeletonCount = 0;
		spriestCount = 0;
		necroCount = 0;
		dkCount = 0;
		wmCount = 0;
		lichPresent = false;

		for(unsigned int i = 0; i < m_cPlayerTwo->GetUnits().size();i++)
		{
			if(m_cPlayerTwo->GetUnits()[i]->GetCountry() == m_cWorld->GetCurrent()->GetCountry() && m_cPlayerTwo->GetUnits()[i]->GetTerritory() == m_cWorld->GetCurrent()->GetTerritory())
			{
				switch(m_cPlayerTwo->GetUnits()[i]->GetUnitID())
				{
				case SKELETALWARRIOR:
					{
						skeletonCount += 1;
					}
					break;
				case SHADOWPRIEST:
					{
						spriestCount += 1;
					}
					break;
				case NECROMANCER:
					{
						necroCount += 1;
					}
					break;
				case DEATHKNIGHT:
					{
						dkCount += 1;
					}
					break;
				case WARMACHINE:
					{
						wmCount += 1;
					}
					break;
				case LICH:
					{
						lichPresent = true;
					}
					break;
				default:
					break;
				}
			}
		}



		TOSTRINGSTREAM sw, sp, n, dk, wm;
		sw << skeletonCount;
		sp << spriestCount;
		n << necroCount;
		dk << dkCount;
		wm << wmCount;

		pFont->Draw(_T("Units in Territory: "),300,450,0.75f,D3DCOLOR_ARGB(255,0,0,0));

		if(lichPresent)
		{
			pFont->Draw(_T("Lich in Command"),275,465,0.75f,D3DCOLOR_ARGB(255,0,0,0));
		}

		pFont->Draw(_T("Skeletal Warriors: "),200,490,0.65f,D3DCOLOR_ARGB(255,0,0,0));
		pFont->Draw(sw.str().c_str(),315,490,0.65f,D3DCOLOR_ARGB(255,0,0,0));

		pFont->Draw(_T("Shadow Priests: "),200,505,0.65f,D3DCOLOR_ARGB(255,0,0,0));
		pFont->Draw(sp.str().c_str(),300,505,0.65f,D3DCOLOR_ARGB(255,0,0,0));

		pFont->Draw(_T("Necromancers: "),200,520,0.65f,D3DCOLOR_ARGB(255,0,0,0));
		pFont->Draw(n.str().c_str(),300,520,0.65f,D3DCOLOR_ARGB(255,0,0,0));

		pFont->Draw(_T("War Machines: "),350,490,0.65f,D3DCOLOR_ARGB(255,0,0,0));
		pFont->Draw(wm.str().c_str(),450,490,0.65f,D3DCOLOR_ARGB(255,0,0,0));

		pFont->Draw(_T("Death Knights: "),350,505,0.65f,D3DCOLOR_ARGB(255,0,0,0));
		pFont->Draw(dk.str().c_str(),450,505,0.65f,D3DCOLOR_ARGB(255,0,0,0));






	}

}
void CGamePlay::AllocateResources(void )
{
	m_fTime = 0;

	food = 0;
	wood = 0;
	steel = 0;

	if(m_nTurnOrder == 0)
	{
		for(unsigned int i = 0; i < m_cPlayerOne->GetBuildings().size();i++)
		{
			food += m_cPlayerOne->GetBuildings()[i]->GetFood();
			wood += m_cPlayerOne->GetBuildings()[i]->GetWood();
			steel += m_cPlayerOne->GetBuildings()[i]->GetSteel();
		}

		if (m_nCountryOneOwner == 1)
		{
			food +=10;
			steel +=10;
			wood += 10;
		}
		if (m_nCountryTwoOwner == 1)
		{
			food +=10;
			steel +=10;
			wood += 10;
		}
		m_cPlayerOne->SetFood(m_cPlayerOne->GetFood() + food);
		m_cPlayerOne->SetWood(m_cPlayerOne->GetWood() + wood);
		m_cPlayerOne->SetSteel(m_cPlayerOne->GetSteel() + steel);


	}
	else
	{
		for(unsigned int i = 0; i < m_cPlayerTwo->GetBuildings().size();i++)
		{
			food += m_cPlayerTwo->GetBuildings()[i]->GetFood();
			wood += m_cPlayerTwo->GetBuildings()[i]->GetWood();
			steel += m_cPlayerTwo->GetBuildings()[i]->GetSteel();
		}
		if (m_nCountryOneOwner == 2)
		{
			food +=10;
			steel +=10;
			wood += 10;
		}
		if (m_nCountryTwoOwner == 2)
		{
			food +=10;
			steel +=10;
			wood += 10;
		}
		m_cPlayerTwo->SetFood(m_cPlayerTwo->GetFood() + food);
		m_cPlayerTwo->SetWood(m_cPlayerTwo->GetWood() + wood);
		m_cPlayerTwo->SetSteel(m_cPlayerTwo->GetSteel() + steel);
	}


}
bool CGamePlay::WinCheck		( void )
{
	m_bPlayerOneWins = 0;
	
	
	
	m_bPlayerOneWins = 0;

	if ( m_nTurnNumber == m_nTurnCap )
	{
		int playeroneowned = 0;
		int playertwoowned = 0;
		for (unsigned int i = 0; i < m_cWorld->GetGrid().size(); i++)
		{
			for (unsigned int j = 0; j < m_cWorld->GetGrid()[i].size(); j++)
			{
				if (m_cWorld->GetGrid()[i][j]->GetOwner() == 2)//enemy
				{
					playertwoowned++;
				}				
			}
		}
		for (unsigned int i = 0; i < m_cWorld->GetGrid().size(); i++)
		{
			for (unsigned int j = 0; j < m_cWorld->GetGrid()[i].size(); j++)
			{
				if (m_cWorld->GetGrid()[i][j]->GetOwner() == 1)//player
				{
					playeroneowned++;
				}				
			}
		}
		if (playeroneowned > playertwoowned)
		{
			CGame::GetInstance()->ChangeState( CWinGame::GetInstance() );
		}
		else if ( playertwoowned > playeroneowned )
		{
			CGame::GetInstance()->ChangeState( CLoseGame::GetInstance() );
		}

	}

	for (unsigned int i = 0; i < m_cWorld->GetGrid().size(); i++)
	{
		for (unsigned int j = 0; j < m_cWorld->GetGrid()[i].size(); j++)
		{
			if (m_cWorld->GetGrid()[i][j]->GetOwner() == 1) // player
			{
				++m_bPlayerOneWins;
			}
			else
			{
				--m_bPlayerOneWins;
			}
		}
	}
	if (m_bPlayerOneWins != 4)
	{
		for (unsigned int i = 0; i < m_cWorld->GetGrid().size(); i++)
		{
			for (unsigned int j = 0; j < m_cWorld->GetGrid()[i].size(); j++)
			{
				if (m_cWorld->GetGrid()[i][j]->GetOwner() == 2) //enemy
				{
					++m_bPlayerTwoWins;
				}
				else
				{
					--m_bPlayerTwoWins;
				}

			}
		}
	}
	
	if (m_bPlayerOneWins == 4		|| m_cPlayerTwo->GetUnits().size() == 0)
	{
		return true;
	}
	else if (m_bPlayerTwoWins == 4 || m_cPlayerOne->GetUnits().size() == 0)
	{
		return true;
	}
	else if (m_bPlayerOneWins != 4 || m_cPlayerTwo->GetUnits().size() != 0)
	{
		return false;
	}
	else if (m_bPlayerTwoWins != 4 || m_cPlayerOne->GetUnits().size() != 0)
	{
		return false;
	}

	

	return true;
	
}
bool CGamePlay::SellBuilding	( void )
{
	int wood;
	int steel;
	wood = m_ncurrentPlayer->GetWood();
	steel = m_ncurrentPlayer->GetSteel();
	vector<CBaseBuilding*> temp = m_cPlayerOne->GetBuildings();
	if (m_ncurrentPlayer->GetType() == PLAYER)
	{


		for (unsigned int i = 0; i < m_cPlayerOne->GetBuildings().size(); i++)
		{
			if (m_cPlayerOne->GetBuildings()[i]->GetPosBuildX() == (int)m_cWorld->GetCurrent()->GetPosX()  )
			{

				switch (m_cPlayerOne->GetBuildings()[i]->GetBuildingID())
				{
				case FARM:
					m_cPlayerOne->SetWood( wood + 10 );
					break;
				case WOOD:
					m_cPlayerOne->SetWood( wood +10 );
					break;
				case MINE:
					m_cPlayerOne->SetWood( wood + 5 );
					m_cPlayerOne->SetSteel( steel + 5 );
					break;
				case BARRACKS:
					m_cPlayerOne->SetWood( wood + 15 );
					m_cPlayerOne->SetSteel( steel +5 );
					break;
				case WARFACTORY:
					m_cPlayerOne->SetWood( wood +10 );
					m_cPlayerOne->SetSteel( steel + 20 );
					break;
				default:
					break;
				}
				m_cPlayerOne->DeleteBuilding( i );
				m_cWorld->GetCurrent()->SetBuilding( -1 );
				return true;
			}
		}
	}
	else if ( m_ncurrentPlayer->GetType() == ENEMY )
	{
		for (unsigned int i = 0; i < m_cPlayerTwo->GetBuildings().size(); i++)
		{
			if (m_cPlayerTwo->GetBuildings()[i]->GetPosBuildX() == (int)m_cWorld->GetCurrent()->GetPosX()  )
			{

				switch (m_cPlayerTwo->GetBuildings()[i]->GetBuildingID())
				{
				case FARM:
					m_cPlayerTwo->SetWood( wood + 10 );
					break;
				case WOOD:
					m_cPlayerTwo->SetWood( wood +10 );
					break;
				case MINE:
					m_cPlayerTwo->SetWood( wood + 5 );
					m_cPlayerTwo->SetSteel( steel + 5 );
					break;
				case BARRACKS:
					m_cPlayerTwo->SetWood( wood + 15 );
					m_cPlayerTwo->SetSteel( steel +5 );
					break;
				case WARFACTORY:
					m_cPlayerTwo->SetWood( wood +10 );
					m_cPlayerTwo->SetSteel( steel + 20 );
					break;
				default:
					break;
				}
				m_cPlayerTwo->DeleteBuilding( i );
				m_cWorld->GetCurrent()->SetBuilding( -1 );
				return true;
			}
		}
	}


	return false;

}
void CGamePlay::RenderOptionsForMove( void )
{
	CBitmapFont* pFont = CGame::GetInstance()->GetFont();


	CSGD_TextureManager::GetInstance()->Draw(m_nMoveBox,50,50);


	//pFont->Draw( _T("->"), 100, 200 + (m_nMoveCursor * 50), 1.0f, D3DCOLOR_ARGB(255, 255, 0, 0) );

	CSGD_TextureManager::GetInstance()->Draw(cursorImageID, 100 , 200 + (m_nMoveCursor * 50), 0.3f, 0.3f, nullptr, 35.0f / 2, 72.0f / 2, D3DX_PI / -2);


	if(m_cWorld->GetPrevious()->GetOwner() == 1)
	{

		soldierCount = 0;
		priestCount = 0;
		wizardCount = 0;
		cavalryCount = 0;
		catapultCount = 0;
		generalPresent = false;



		for(unsigned int i = 0; i < m_cPlayerOne->GetUnits().size();i++)
		{
			if(m_cPlayerOne->GetUnits()[i]->GetCountry() == m_cWorld->GetPrevious()->GetCountry() && m_cPlayerOne->GetUnits()[i]->GetTerritory() == m_cWorld->GetPrevious()->GetTerritory())
			{
				switch(m_cPlayerOne->GetUnits()[i]->GetUnitID())
				{
				case FOOTSOLDIERS:
					{
						soldierCount += 1;
					}
					break;
				case PRIEST:
					{
						priestCount += 1;
					}
					break;
				case WIZARD:
					{
						wizardCount += 1;
					}
					break;
				case CALVARY:
					{
						cavalryCount += 1;
					}
					break;
				case CATAPULT:
					{
						catapultCount += 1;
					}
					break;
				case GENERAL:
					{
						generalPresent = true;
					}
					break;
				}
			}
		}


		TOSTRINGSTREAM fs, p, w, cv, cp;
		fs <<		soldierCount;
		p <<		priestCount;
		w <<		wizardCount;
		cv <<		cavalryCount;
		cp <<		catapultCount;


		TOSTRINGSTREAM Mfs, Mp, Mw, Mcv, Mcp, MG;
		Mfs <<		m_MovesoldierCount;
		Mp <<		m_MovepriestCount;
		Mw <<		m_MovewizardCount;
		Mcv <<		m_MovecavalryCount;
		Mcp <<		m_MovecatapultCount;
		MG <<		m_MovegeneralCount;



		pFont->Draw(_T("Units in Territory: "),250,100,1.5f,D3DCOLOR_ARGB(255,255,255,255));

		if(generalPresent)
		{
			pFont->Draw(_T("General in Command"),200,150,1.0f,D3DCOLOR_ARGB(255,255,255,255));
		}

		pFont->Draw(_T("Foot Soldiers "),200,200,1.0f,D3DCOLOR_ARGB(255,255,255,255));
		pFont->Draw(fs.str().c_str(),150,    200,1.0f,D3DCOLOR_ARGB(255,255,255,255));
		pFont->Draw(Mfs.str().c_str(),650,   200,1.0f,D3DCOLOR_ARGB(255,255,255,255));


		pFont->Draw(_T("Priests "),200,250,1.0f,D3DCOLOR_ARGB(255,255,255,255));
		pFont->Draw(p.str().c_str(),150,250,1.0f,D3DCOLOR_ARGB(255,255,255,255));
		pFont->Draw(Mp.str().c_str(),650,250,1.0f,D3DCOLOR_ARGB(255,255,255,255));

		pFont->Draw(_T("Wizards "),200,300,1.0f,D3DCOLOR_ARGB(255,255,255,255));
		pFont->Draw(w.str().c_str(),150,300,1.0f,D3DCOLOR_ARGB(255,255,255,255));
		pFont->Draw(Mw.str().c_str(),650,300,1.0f,D3DCOLOR_ARGB(255,255,255,255));

		pFont->Draw(_T("Cavalry "),200,350,1.0f,D3DCOLOR_ARGB(255,255,255,255));
		pFont->Draw(cv.str().c_str(),150,350,1.0f,D3DCOLOR_ARGB(255,255,255,255));
		pFont->Draw(Mcv.str().c_str(),650,350,1.0f,D3DCOLOR_ARGB(255,255,255,255));

		pFont->Draw(_T("Catapults "),200,400,1.0f,D3DCOLOR_ARGB(255,255,255,255));
		pFont->Draw(cp.str().c_str(),150,400,1.0f,D3DCOLOR_ARGB(255,255,255,255));
		pFont->Draw(Mcp.str().c_str(),650,400,1.0f,D3DCOLOR_ARGB(255,255,255,255));


		pFont->Draw(_T("How Many to Move? "),400,200,1.0f,D3DCOLOR_ARGB(255,255,255,255));
		pFont->Draw(_T("How Many to Move? "),400,250,1.0f,D3DCOLOR_ARGB(255,255,255,255));
		pFont->Draw(_T("How Many to Move? "),400,300,1.0f,D3DCOLOR_ARGB(255,255,255,255));
		pFont->Draw(_T("How Many to Move? "),400,350,1.0f,D3DCOLOR_ARGB(255,255,255,255));
		pFont->Draw(_T("How Many to Move? "),400,400,1.0f,D3DCOLOR_ARGB(255,255,255,255));


		pFont->Draw(_T("Move To Territory! "),300,450,1.0f,D3DCOLOR_ARGB(255,255,255,255));

		pFont->Draw(_T("Exit Move"),300,500,1.0f,D3DCOLOR_ARGB(255,255,255,255));





	}
	else if(m_cWorld->GetPrevious()->GetOwner() == 2)
	{

		skeletonCount = 0;
		spriestCount = 0;
		necroCount = 0;
		dkCount = 0;
		wmCount = 0;
		lichPresent = false;



		for(unsigned int i = 0; i < m_cPlayerTwo->GetUnits().size();i++)
		{
			if(m_cPlayerTwo->GetUnits()[i]->GetCountry() == m_cWorld->GetPrevious()->GetCountry() && m_cPlayerTwo->GetUnits()[i]->GetTerritory() == m_cWorld->GetPrevious()->GetTerritory())
			{
				switch(m_cPlayerTwo->GetUnits()[i]->GetUnitID())
				{
				case SKELETALWARRIOR:
					{
						skeletonCount += 1;
					}
					break;
				case SHADOWPRIEST:
					{
						spriestCount += 1;
					}
					break;
				case NECROMANCER:
					{
						necroCount += 1;
					}
					break;
				case DEATHKNIGHT:
					{
						dkCount += 1;
					}
					break;
				case WARMACHINE:
					{
						wmCount += 1;
					}
					break;
				case LICH:
					{
						lichPresent = true;
					}
					break;

				default:
					break;
				}
			}
		}



		TOSTRINGSTREAM sw, sp, n, dk, wm;
		sw <<		skeletonCount;
		sp <<		spriestCount;
		n <<		necroCount;
		dk <<		dkCount;
		wm <<		wmCount;


		TOSTRINGSTREAM Msw, Msp, Mn, Mdk, Mwm, Ml;
		Msw <<		m_MoveskeletonCount;
		Msp <<		m_MovespriestCount;
		Mn <<		m_MovenecroCount;
		Mdk <<		m_MovedkCount;
		Mwm <<		m_MovewmCount;
		Ml <<		m_MovelichCount;





		pFont->Draw(_T("Units in Territory: "),350,100,1.5f,D3DCOLOR_ARGB(255,255,255,255));


		if(lichPresent)
		{
			pFont->Draw(_T("Lich in Command"),200,150,1.0f,D3DCOLOR_ARGB(255,255,255,255));
		}

		pFont->Draw(_T("Skeletal Warriors "),200,200,1.0f,D3DCOLOR_ARGB(255,255,255,255));
		pFont->Draw(sw.str().c_str(),150,200,1.0f,D3DCOLOR_ARGB(255,255,255,255));
		pFont->Draw(Msw.str().c_str(),650,200,1.0f,D3DCOLOR_ARGB(255,255,255,255));


		pFont->Draw(_T("Shadow Priests "),200,250,1.0f,D3DCOLOR_ARGB(255,255,255,255));
		pFont->Draw(sp.str().c_str(),150,250,1.0f,D3DCOLOR_ARGB(255,255,255,255));
		pFont->Draw(Msp.str().c_str(),650,250,1.0f,D3DCOLOR_ARGB(255,255,255,255));


		pFont->Draw(_T("Necromancers "),200,300,1.0f,D3DCOLOR_ARGB(255,255,255,255));
		pFont->Draw(n.str().c_str(),150,300,1.0f,D3DCOLOR_ARGB(255,255,255,255));
		pFont->Draw(Mn.str().c_str(),650,300,1.0f,D3DCOLOR_ARGB(255,255,255,255));

		pFont->Draw(_T("Death Knights "),200,350,1.0f,D3DCOLOR_ARGB(255,255,255,255));
		pFont->Draw(dk.str().c_str(),150,350,1.0f,D3DCOLOR_ARGB(255,255,255,255));
		pFont->Draw(Mdk.str().c_str(),650,350,1.0f,D3DCOLOR_ARGB(255,255,255,255));

		pFont->Draw(_T("War Machines "),200,400,1.0f,D3DCOLOR_ARGB(255,255,255,255));
		pFont->Draw(wm.str().c_str(),150,400,1.0f,D3DCOLOR_ARGB(255,255,255,255));
		pFont->Draw(Mwm.str().c_str(),650,400,1.0f,D3DCOLOR_ARGB(255,255,255,255));

		pFont->Draw(_T("How Many to Move? "),400,200,1.0f,D3DCOLOR_ARGB(255,255,255,255));
		pFont->Draw(_T("How Many to Move? "),400,250,1.0f,D3DCOLOR_ARGB(255,255,255,255));
		pFont->Draw(_T("How Many to Move? "),400,300,1.0f,D3DCOLOR_ARGB(255,255,255,255));
		pFont->Draw(_T("How Many to Move? "),400,350,1.0f,D3DCOLOR_ARGB(255,255,255,255));
		pFont->Draw(_T("How Many to Move? "),400,400,1.0f,D3DCOLOR_ARGB(255,255,255,255));


		pFont->Draw(_T("Move To Territory! "),300,450,1.0f,D3DCOLOR_ARGB(255,255,255,255));

		pFont->Draw(_T("Exit Move"),300,500,1.0f,D3DCOLOR_ARGB(255,255,255,255));


	}
}
bool CGamePlay::HandleInputForMoveAndAttack( void )   //done
{


	CSGD_DirectInput*	pDI		=	CSGD_DirectInput::GetInstance();


	// Navigate the menu
	if(  ( (pDI->KeyPressed( DIK_S )  || pDI->JoystickGetLStickDirPressed( DIR_DOWN, JOYSTICKID1 ) ) && m_ncurrentPlayer->GetType() == PLAYER ) || ( pDI->KeyPressed( DIK_DOWN ) || pDI->JoystickGetLStickDirPressed( DIR_DOWN, JOYSTICKID2 ) && m_ncurrentPlayer->GetType() == ENEMY )  )
	{
		++m_nMoveCursor;



		// Stop at the bottom
		if( m_nMoveCursor > 6 )		// EXIT
			m_nMoveCursor = 0;



	}
	else if(  ( (pDI->KeyPressed( DIK_W )  || pDI->JoystickGetLStickDirPressed( DIR_UP, JOYSTICKID1 ) ) && m_ncurrentPlayer->GetType() == PLAYER ) || ( pDI->KeyPressed( DIK_UP ) || pDI->JoystickGetLStickDirPressed( DIR_UP, JOYSTICKID2 ) && m_ncurrentPlayer->GetType() == ENEMY )  )
	{
		--m_nMoveCursor;



		// Stop at the top
		if( m_nMoveCursor < 0 )		// EXIT
			m_nMoveCursor = 6;


	}

	if(  ( (pDI->KeyPressed( DIK_D )  || pDI->JoystickGetLStickDirPressed( DIR_RIGHT, JOYSTICKID1 ) ) && m_ncurrentPlayer->GetType() == PLAYER ) || ( pDI->KeyPressed( DIK_RIGHT ) || pDI->JoystickGetLStickDirPressed( DIR_RIGHT, JOYSTICKID2 ) && m_ncurrentPlayer->GetType() == ENEMY )  )
	{
		switch (m_nMoveCursor)
		{
		case 0:
			{
				if (m_nTurnOrder == 1 && skeletonCount > 0)
				{
					if (m_MoveskeletonCount < skeletonCount )
					{
						++m_MoveskeletonCount;
					}


					if (m_MoveskeletonCount < 0)
					{
						m_MoveskeletonCount = 0;
					}

				}
				else if( m_nTurnOrder == 0 && soldierCount > 0 )	
				{
					if (m_MovesoldierCount < soldierCount )
					{
						++m_MovesoldierCount;
					}

					if (m_MovesoldierCount < 0)
					{
						m_MovesoldierCount = 0;
					}
				}
			}
			break;
		case 1:
			{
				if (m_nTurnOrder == 1 && spriestCount > 0)
				{
					if (m_MovespriestCount < spriestCount )
					{
						++m_MovespriestCount;
					}
					if (m_MovespriestCount < 0)
					{
						m_MovespriestCount = 0;
					}

				}
				else if( m_nTurnOrder == 0 && priestCount > 0 )	
				{
					if (m_MovepriestCount < priestCount )
					{
						++m_MovepriestCount;
					}
					if (m_MovepriestCount < 0)
					{
						m_MovepriestCount = 0;
					}
				}
			}
			break;
		case 2:
			{
				if (m_nTurnOrder == 1 && necroCount > 0)
				{
					if (m_MovenecroCount < necroCount )
					{
						++m_MovenecroCount;
					}
					if (m_MovenecroCount < 0)
					{
						m_MovenecroCount = 0;
					}

				}
				else if( m_nTurnOrder == 0 && wizardCount > 0 )	
				{
					if (m_MovewizardCount < wizardCount )
					{
						++m_MovewizardCount;
					}
					if (m_MovewizardCount < 0)
					{
						m_MovewizardCount = 0;
					}
				}
			}
			break;
		case 3:
			{
				if (m_nTurnOrder == 1 && dkCount > 0)
				{
					if (m_MovedkCount < dkCount )
					{
						++m_MovedkCount;
					}
					if (m_MovedkCount < 0)
					{
						m_MovedkCount = 0;
					}

				}
				else if( m_nTurnOrder == 0 && cavalryCount > 0 )	
				{
					if (m_MovecavalryCount < cavalryCount )
					{
						++m_MovecavalryCount;
					}
					if (m_MovecavalryCount < 0)
					{
						m_MovecavalryCount = 0;
					}
				}
			}
			break;
		case 4:
			{
				if (m_nTurnOrder == 1 && wmCount > 0)
				{
					if (m_MovewmCount < wmCount )
					{
						++m_MovewmCount;
					}
					if (m_MovewmCount < 0)
					{
						m_MovewmCount = 0;
					}

				}
				else if( m_nTurnOrder == 0 && catapultCount > 0 )	
				{
					if (m_MovecatapultCount < catapultCount )
					{
						++m_MovecatapultCount;
					}
					if (m_MovecatapultCount < 0)
					{
						m_MovecatapultCount = 0;
					}
				}
			}
			break;

		}


	}
	if(  ( (pDI->KeyPressed( DIK_A )  || pDI->JoystickGetLStickDirPressed( DIR_LEFT, JOYSTICKID1 ) ) && m_ncurrentPlayer->GetType() == PLAYER ) || ( pDI->KeyPressed( DIK_LEFT ) || pDI->JoystickGetLStickDirPressed( DIR_LEFT, JOYSTICKID2 ) && m_ncurrentPlayer->GetType() == ENEMY )  )
	{
		switch (m_nMoveCursor)
		{
		case 0:
			{
				if (m_nTurnOrder == 1 && skeletonCount > 0)
				{
					--m_MoveskeletonCount;

					if (m_MoveskeletonCount < 0)
					{
						m_MoveskeletonCount = 0;
					}

				}
				else if( m_nTurnOrder == 0 && soldierCount > 0 )	
				{
					--m_MovesoldierCount;

					if (m_MovesoldierCount < 0)
					{
						m_MovesoldierCount = 0;
					}
				}
			}
			break;
		case 1:
			{
				if (m_nTurnOrder == 1 && spriestCount > 0)
				{
					--m_MovespriestCount;

					if (m_MovespriestCount < 0)
					{
						m_MovespriestCount = 0;
					}

				}
				else if( m_nTurnOrder == 0 && priestCount > 0 )	
				{
					--m_MovepriestCount;

					if (m_MovepriestCount < 0)
					{
						m_MovepriestCount = 0;
					}
				}
			}
			break;
		case 2:
			{
				if (m_nTurnOrder == 1 && necroCount > 0)
				{
					--m_MovenecroCount;

					if (m_MovenecroCount < 0)
					{
						m_MovenecroCount = 0;
					}

				}
				else if( m_nTurnOrder == 0 && wizardCount > 0 )	
				{
					--m_MovewizardCount;

					if (m_MovewizardCount < 0)
					{
						m_MovewizardCount = 0;
					}
				}
			}
			break;
		case 3:
			{
				if (m_nTurnOrder == 1 && dkCount > 0)
				{
					--m_MovedkCount;

					if (m_MovedkCount < 0)
					{
						m_MovedkCount = 0;
					}

				}
				else if( m_nTurnOrder == 0 && cavalryCount > 0 )	
				{
					--m_MovecavalryCount;

					if (m_MovecavalryCount < 0)
					{
						m_MovecavalryCount = 0;
					}
				}
			}
			break;
		case 4:
			{
				if (m_nTurnOrder == 1 && wmCount > 0)
				{
					--m_MovewmCount;

					if (m_MovewmCount < 0)
					{
						m_MovewmCount = 0;
					}

				}
				else if( m_nTurnOrder == 0 && catapultCount > 0 )	
				{
					--m_MovecatapultCount;

					if (m_MovecatapultCount < 0)
					{
						m_MovecatapultCount = 0;
					}
				}
			}
			break;

		}

	}

	return false;
}    //Done
void CGamePlay::CheckMovement ( void )
{

	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();

	if( ( pDI->KeyPressed( DIK_TAB ) || pDI->JoystickButtonPressed( 1, JOYSTICKID1 ) && m_ncurrentPlayer->GetType() == PLAYER ) || (pDI->KeyPressed( DIK_RETURN ) || pDI->JoystickButtonPressed( 1, JOYSTICKID2 ) && m_ncurrentPlayer->GetType() == ENEMY ) )
	{
		for(int i = 0; i < 4; i++)
		{
			if(m_cWorld->GetCurrent() == m_cWorld->GetPrevious()->GetAdj()[i])
				m_bAMMode = true;
		}

	}


}
bool CGamePlay::MoveUnitsBetweenTerritories ( void )
{
	if (m_nTurnOrder == 0)
	{
		if(m_MovesoldierCount == 0)
			if(m_MovepriestCount == 0)
				if(m_MovewizardCount == 0)
					if(m_MovecavalryCount == 0)
						if(m_MovecatapultCount == 0)
							return false;

		for(unsigned int i = 0; i < m_cPlayerOne->GetUnits().size();i++)
		{
			if(m_cPlayerOne->GetUnits()[i]->GetCountry() == m_cWorld->GetPrevious()->GetCountry() && m_cPlayerOne->GetUnits()[i]->GetTerritory() == m_cWorld->GetPrevious()->GetTerritory())
			{
				switch(m_cPlayerOne->GetUnits()[i]->GetUnitID())
				{
				case FOOTSOLDIERS:
					{
						if(m_MovesoldierCount > 0)
						{
							m_cPlayerOne->GetUnits()[i]->SetCountry(m_cWorld->GetCurrent()->GetCountry());
							m_cPlayerOne->GetUnits()[i]->SetTerritory(m_cWorld->GetCurrent()->GetTerritory());

							m_MovesoldierCount -= 1;
						}
					}
					break;
				case PRIEST:
					{
						if(m_MovepriestCount > 0)
						{
							m_cPlayerOne->GetUnits()[i]->SetCountry(m_cWorld->GetCurrent()->GetCountry());
							m_cPlayerOne->GetUnits()[i]->SetTerritory(m_cWorld->GetCurrent()->GetTerritory());

							m_MovepriestCount -= 1;
						}
					}
					break;
				case WIZARD:
					{
						if(m_MovewizardCount > 0)
						{
							m_cPlayerOne->GetUnits()[i]->SetCountry(m_cWorld->GetCurrent()->GetCountry());
							m_cPlayerOne->GetUnits()[i]->SetTerritory(m_cWorld->GetCurrent()->GetTerritory());

							m_MovewizardCount -= 1;
						}
					}
					break;
				case CALVARY:
					{
						if(m_MovecavalryCount > 0)
						{
							m_cPlayerOne->GetUnits()[i]->SetCountry(m_cWorld->GetCurrent()->GetCountry());
							m_cPlayerOne->GetUnits()[i]->SetTerritory(m_cWorld->GetCurrent()->GetTerritory());

							m_MovecavalryCount -= 1;
						}
					}
					break;
				case CATAPULT:
					{
						if(m_MovecatapultCount > 0)
						{
							m_cPlayerOne->GetUnits()[i]->SetCountry(m_cWorld->GetCurrent()->GetCountry());
							m_cPlayerOne->GetUnits()[i]->SetTerritory(m_cWorld->GetCurrent()->GetTerritory());

							m_MovecatapultCount -= 1;
						}
					}
					break;

				default:
					break;
				}
			}
		}
		return true;
	}
	else if (m_nTurnOrder == 1)
	{
		if(m_MoveskeletonCount == 0)
			if(m_MovespriestCount == 0)
				if(m_MovenecroCount == 0)
					if(m_MovedkCount == 0)
						if(m_MovewmCount == 0)
							return false;

		for(unsigned int i = 0; i < m_cPlayerTwo->GetUnits().size();i++)
		{
			if(m_cPlayerTwo->GetUnits()[i]->GetCountry() == m_cWorld->GetPrevious()->GetCountry() )
			{
				switch(m_cPlayerTwo->GetUnits()[i]->GetUnitID())
				{
				case SKELETALWARRIOR:
					{
						if(m_MoveskeletonCount > 0)
						{
							m_cPlayerTwo->GetUnits()[i]->SetCountry(m_cWorld->GetCurrent()->GetCountry());

							m_MoveskeletonCount -= 1;
						}
					}
					break;
				case SHADOWPRIEST:
					{
						if(m_MovespriestCount > 0)
						{
							m_cPlayerTwo->GetUnits()[i]->SetCountry(m_cWorld->GetCurrent()->GetCountry());

							m_MovespriestCount -= 1;
						}
					}
					break;
				case NECROMANCER:
					{
						if(m_MovenecroCount > 0)
						{
							m_cPlayerTwo->GetUnits()[i]->SetCountry(m_cWorld->GetCurrent()->GetCountry());

							m_MovenecroCount -= 1;
						}
					}
					break;
				case DEATHKNIGHT:
					{
						if(m_MovedkCount > 0)
						{
							m_cPlayerTwo->GetUnits()[i]->SetCountry(m_cWorld->GetCurrent()->GetCountry());

							m_MovedkCount -= 1;
						}
					}
					break;
				case WARMACHINE:
					{
						if(m_MovewmCount > 0)
						{
							m_cPlayerTwo->GetUnits()[i]->SetCountry(m_cWorld->GetCurrent()->GetCountry());

							m_MovewmCount -= 1;
						}
					}
					break;

				default:
					break;

				}
			}
		}

		return true;

	}

	return false;
}
void CGamePlay::RenderResources ( void )
{

	CBitmapFont* pFont = CGame::GetInstance()->GetFont();
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();



	TOSTRINGSTREAM f, w, s;

	f << food;
	w << wood;
	s << steel;


	pTM->Draw(m_nBlankBox,250,200);


	/*	RECT resource = {CGame::GetInstance()->GetScreenWidth()/2 -100, CGame::GetInstance()->GetScreenHeight()/2 -100,CGame::GetInstance()->GetScreenWidth()/2 +100,CGame::GetInstance()->GetScreenHeight()/2 +100};
	CSGD_Direct3D::GetInstance()->DrawRect(resource, D3DCOLOR_ARGB(200,0,0,0));*/

	pFont->Draw(_T("Resources this Turn"),CGame::GetInstance()->GetScreenWidth()/2 -100, 275, 0.75f,D3DCOLOR_ARGB(255,255,255,255));

	pFont->Draw(_T("Food:"),CGame::GetInstance()->GetScreenWidth()/2 -100, 300, 0.75f,D3DCOLOR_ARGB(255,255,255,255));
	pFont->Draw(f.str().c_str(),CGame::GetInstance()->GetScreenWidth()/2, 300, 0.75f,D3DCOLOR_ARGB(255,255,255,255));

	pFont->Draw(_T("Wood:"),CGame::GetInstance()->GetScreenWidth()/2 -100, 350 , 0.75f,D3DCOLOR_ARGB(255,255,255,255));
	pFont->Draw(w.str().c_str(),CGame::GetInstance()->GetScreenWidth()/2, 350 , 0.75f,D3DCOLOR_ARGB(255,255,255,255));

	pFont->Draw(_T("Steel:"),CGame::GetInstance()->GetScreenWidth()/2 -100,400, 0.75f,D3DCOLOR_ARGB(255,255,255,255));
	pFont->Draw(s.str().c_str(),CGame::GetInstance()->GetScreenWidth()/2, 400, 0.75f,D3DCOLOR_ARGB(255,255,255,255));
}
void CGamePlay::TurnRender( void )
{

	CSGD_TextureManager * pTM = CSGD_TextureManager::GetInstance();
	CBitmapFont* pFont = CGame::GetInstance()->GetFont();

	pTM->Draw(m_nBlankBar,40,30,1.0f,1.0f);

	if (m_nTurnOrder == 0)
	{
		pTM->Draw(m_nHuman, 85,45,1.0f,1.0f);
		pFont->Draw( _T("Human's Turn"), 120, 55, 0.75f, D3DCOLOR_ARGB(255, 255, 255, 255) );

	}
	else
	{
		pTM->Draw(m_nUndead, 65,30,1.0f,1.0f);
		pFont->Draw( _T("Undead's Turn"), 120, 55, 0.75f, D3DCOLOR_ARGB(255, 0, 0, 0) );

	}
}
void CGamePlay::PlayerResources(void)
{
	CBitmapFont* pFont = CGame::GetInstance()->GetFont();
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();

	pTM->Draw(m_nResourceBox,520,30);

	pTM->Draw(m_nFoodIcon,545,40);
	pTM->Draw(m_nWoodIcon,620,40);
	pTM->Draw(m_nSteelIcon,695,40);

	TOSTRINGSTREAM FoodResource;
	TOSTRINGSTREAM WoodResource;
	TOSTRINGSTREAM SteelResource;




	if (m_nTurnOrder == 0)
	{
		FoodResource << m_cPlayerOne->GetInstance()->GetFood();

		pFont->Draw( FoodResource.str().c_str(), 580, 50, 0.60f, D3DCOLOR_ARGB( 255, 0, 0, 255 ) ); 

		WoodResource << m_cPlayerOne->GetInstance()->GetWood();

		pFont->Draw( WoodResource.str().c_str(), 655, 50, 0.60f, D3DCOLOR_ARGB( 255, 0, 0, 255 ) ); 

		SteelResource << m_cPlayerOne->GetInstance()->GetSteel();

		pFont->Draw( SteelResource.str().c_str(), 730, 50, 0.60f, D3DCOLOR_ARGB( 255, 0, 0, 255 ) ); 

	}
	else
	{
		FoodResource << m_cPlayerTwo->GetInstance()->GetFood();

		pFont->Draw( FoodResource.str().c_str(),  580, 50, 0.60f, D3DCOLOR_ARGB( 255, 0, 0, 255 ) ); 

		WoodResource << m_cPlayerTwo->GetInstance()->GetWood();

		pFont->Draw( WoodResource.str().c_str(),  655, 50, 0.60f, D3DCOLOR_ARGB( 255, 0, 0, 255 ) ); 

		SteelResource << m_cPlayerTwo->GetInstance()->GetSteel();

		pFont->Draw( SteelResource.str().c_str(),  730, 50, 0.60f, D3DCOLOR_ARGB( 255, 0, 0, 255 ) ); 
	}
}
void CGamePlay::LoadBackFromBattle()
{

	if (m_cWorld->GetCurrent() != nullptr)
	{
		if (m_ncurrentPlayer->GetType() == PLAYER && m_cWorld->GetCurrent()->GetOwner() == 2 && m_cWorld->GetCurrent()->GetBuilding() != -1)
		{
			m_bChooseToSellBack = true;
		}
		else if (m_ncurrentPlayer->GetType() == ENEMY && m_cWorld->GetCurrent()->GetOwner() == 1 && m_cWorld->GetCurrent()->GetBuilding() != -1)
		{
			m_bChooseToSellBack = true;
		}


		int count = 0;

		for(unsigned int i = 0; i < m_cPlayerOne->GetUnits().size(); i++)
		{
			if(m_cPlayerOne->GetUnits()[i]->GetCountry() ==  m_cWorld->GetCurrent()->GetCountry() && m_cPlayerOne->GetUnits()[i]->GetTerritory() == m_cWorld->GetCurrent()->GetTerritory())
			{

				++count;

			}
		}
		if (count == 0)
		{
			m_cWorld->GetGrid()[m_cWorld->GetCurrent()->GetCountry()][m_cWorld->GetCurrent()->GetTerritory()]->SetOwner( 2 );
		}

		count = 0;

		for(unsigned int i = 0; i < m_cPlayerTwo->GetUnits().size(); i++)
		{
			if(m_cPlayerTwo->GetUnits()[i]->GetCountry() ==  m_cWorld->GetCurrent()->GetCountry() && m_cPlayerTwo->GetUnits()[i]->GetTerritory() == m_cWorld->GetCurrent()->GetTerritory())
			{

				++count;

			}
		}
		if (count == 0)
		{
			m_cWorld->GetGrid()[m_cWorld->GetCurrent()->GetCountry()][m_cWorld->GetCurrent()->GetTerritory()]->SetOwner ( 1 );
		}
	}

}
void CGamePlay::RenderTileCursor(void)
{
	CSGD_TextureManager * pTM = CSGD_TextureManager::GetInstance();

	if(m_ncurrentPlayer->GetType() == PLAYER)
		pTM->Draw(cursorImageID, m_cWorld->GetCurrent()->GetPosX() - 22, m_cWorld->GetCurrent()->GetPosY() - ( 44 + m_nHoverOffset ), 0.5f, 0.5f, nullptr, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(50, 50, 255 ) );
	else
		pTM->Draw(cursorImageID,  m_cWorld->GetCurrent()->GetPosX() - 22, m_cWorld->GetCurrent()->GetPosY() - ( 44 + m_nHoverOffset ), 0.5f, 0.5f, nullptr, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(255, 10, 10 ) );
}
void CGamePlay::ShowTerritoryOptions(void)
{
	CBitmapFont* pFont = CGame::GetInstance()->GetFont();
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();
	int posX = m_cWorld->GetCurrent()->GetPosX() + 40;
	int posY = m_cWorld->GetCurrent()->GetPosY() - 50;

	if(posY + 192 > 570)
		posY -= 100;

	pTM->Draw(commandImageID,posX, posY, 0.75f, 0.75f);
	pFont->Draw(_T("Commands"), posX + 42, posY+5, 0.9f, D3DCOLOR_XRGB(255, 255, 255));

	if ((m_bMoveandAttackMode == false && m_bBuildMode == false))
	{
		pFont->Draw(_T("End Turn"), posX + 32, posY+50, 0.75f, D3DCOLOR_XRGB(255, 255, 255));
		pFont->Draw(_T("Exit"), posX + 32, posY+95, 0.75f, D3DCOLOR_XRGB(255, 255, 255));
	}
	else if (m_bBuildMode == true && m_cWorld->GetCurrent()->GetBuilding() != CAPITOL && m_cWorld->GetCurrent()->GetBuilding() == -1)
	{
		pFont->Draw(_T("Build"), posX + 32, posY+50, 0.75f, D3DCOLOR_XRGB(255, 255, 255));
		pFont->Draw(_T("End Phase"), posX + 32, posY+95, 0.75f, D3DCOLOR_XRGB(255, 255, 255));
		pFont->Draw(_T("Exit"), posX + 32, posY+140, 0.75f, D3DCOLOR_XRGB(255, 255, 255));
	}
	else if (m_bBuildMode == true && (m_cWorld->GetCurrent()->GetBuilding() == BARRACKS || m_cWorld->GetCurrent()->GetBuilding() == WARFACTORY ))
	{
		pFont->Draw(_T("Recruit"), posX + 32, posY+50, 0.75f, D3DCOLOR_XRGB(255, 255, 255));
		pFont->Draw(_T("End Phase"), posX + 32, posY+95, 0.75f, D3DCOLOR_XRGB(255, 255, 255));
		pFont->Draw(_T("Exit"), posX + 32, posY+140, 0.75f, D3DCOLOR_XRGB(255, 255, 255));
	}
	else if (m_bMoveandAttackMode == true)
	{
		pFont->Draw(_T("Move/Attack"), posX + 32, posY+50, 0.75f, D3DCOLOR_XRGB(255, 255, 255));
		pFont->Draw(_T("End Phase"), posX + 32, posY+95, 0.75f, D3DCOLOR_XRGB(255, 255, 255));
		pFont->Draw(_T("Exit"), posX + 32, posY+140, 0.75f, D3DCOLOR_XRGB(255, 255, 255));
	}
	else if (m_bBuildMode == true && ( m_cWorld->GetCurrent()->GetBuilding() == FARM || m_cWorld->GetCurrent()->GetBuilding() == WOOD || m_cWorld->GetCurrent()->GetBuilding() == MINE ))
	{
		pFont->Draw(_T("Sell"), posX + 32, posY+50, 0.75f, D3DCOLOR_XRGB(255, 255, 255));
		pFont->Draw(_T("End Phase"), posX + 32, posY+95, 0.75f, D3DCOLOR_XRGB(255, 255, 255));
		pFont->Draw(_T("Exit"), posX + 32, posY+140, 0.75f, D3DCOLOR_XRGB(255, 255, 255));
	}





	//Render Cursor Position
	pTM->Draw(cursorImageID, posX , (posY + 55) + (m_nTerritoryOptionsCursor *45), 0.3f, 0.3f, nullptr, 35.0f / 2, 72.0f / 2, D3DX_PI / -2);
	pTM->Draw(cursorImageID, posX + 180, (posY + 45) + (m_nTerritoryOptionsCursor *45), 0.3f, 0.3f, nullptr, 35.0f / 2, 72.0f / 2, D3DX_PI / 2);


}
bool CGamePlay::CheatCheck(void)
{

	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();




	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_W) == true)
	{
		cheatstring += "W";
		if(cheatstring.size() == 2 || cheatstring.size() == 3)
		{
			cheatstring.clear();

		}
	}
	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_I) == true)
	{
		cheatstring += "I";
		if(cheatstring.size() == 1 || cheatstring.size() == 3)
		{
			cheatstring.clear();	
		}
	}
	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_N) == true)
	{
		cheatstring += "N";
		std::string test = "WIN";
		if(!std::strcmp(cheatstring.c_str(),test.c_str() ))
		{
			if(m_nTurnOrder == 0)
			{
				m_cPlayerOne->SetWood(1000000000);
				m_cPlayerOne->SetFood(1000000000);
				m_cPlayerOne->SetSteel(1000000000);
			}
			else if(m_nTurnOrder == 1)
			{
				m_cPlayerTwo->SetWood(1000000000);
				m_cPlayerTwo->SetFood(1000000000);
				m_cPlayerTwo->SetSteel(1000000000);
			}
		}
		else
			cheatstring.clear();

	}
	return true;
}
bool CGamePlay::HandleMovingOnTerritoryMenu( void )
{
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();

	// Navigate the Territory menu
	if(  ( (pDI->KeyPressed( DIK_S )  || pDI->JoystickGetLStickDirPressed( DIR_DOWN, JOYSTICKID1 ) ) && m_ncurrentPlayer->GetType() == PLAYER ) || ( pDI->KeyPressed( DIK_DOWN ) || pDI->JoystickGetLStickDirPressed( DIR_DOWN, JOYSTICKID2 ) && m_ncurrentPlayer->GetType() == ENEMY )  )
	{

		++m_nTerritoryOptionsCursor;
		if (m_bMoveandAttackMode == false && m_bBuildMode == false)
		{
			if( m_nTerritoryOptionsCursor > 1 )		
				m_nTerritoryOptionsCursor = 0;  
		}
		// Stop at the bottom
		else if( m_nTerritoryOptionsCursor > 2 )		
			m_nTerritoryOptionsCursor = 0;     //Move/Build/Recruit

	}
	else if(  ( pDI->KeyPressed( DIK_W )  || pDI->JoystickGetLStickDirPressed( DIR_UP, JOYSTICKID1 )  && m_ncurrentPlayer->GetType() == PLAYER ) || ( pDI->KeyPressed( DIK_UP ) || pDI->JoystickGetLStickDirPressed( DIR_UP, JOYSTICKID2 ) && m_ncurrentPlayer->GetType() == ENEMY )  )
	{

		--m_nTerritoryOptionsCursor;
		if (m_bMoveandAttackMode == false && m_bBuildMode == false)
		{
			if( m_nTerritoryOptionsCursor < 0 )		
				m_nTerritoryOptionsCursor = 1;  
		}
		// Stop at the top
		if( m_nTerritoryOptionsCursor < 0 )		
			m_nTerritoryOptionsCursor = 2; //Exit each option

	}
	return true;
}
bool CGamePlay::HandleMovingOnBuildInterface( void )
{
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();

	// Navigate the Building menu
	if(  ( (pDI->KeyPressed( DIK_D )  || pDI->JoystickGetLStickDirPressed( DIR_DOWN, JOYSTICKID1 ) ) && m_ncurrentPlayer->GetType() == PLAYER ) || ( pDI->KeyPressed( DIK_RIGHT ) || pDI->JoystickGetLStickDirPressed( DIR_DOWN, JOYSTICKID2 ) && m_ncurrentPlayer->GetType() == ENEMY )  )
	{

		++m_nBuildCursor;

		// Stop at the bottom
		if( m_nBuildCursor > 5 )		
			m_nBuildCursor = 0;     //Move/Build/Recruit

	}
	else if(  ( (pDI->KeyPressed( DIK_A )  || pDI->JoystickGetLStickDirPressed( DIR_LEFT, JOYSTICKID1 ) ) && m_ncurrentPlayer->GetType() == PLAYER ) || ( pDI->KeyPressed( DIK_LEFT ) || pDI->JoystickGetLStickDirPressed( DIR_LEFT, JOYSTICKID2 ) && m_ncurrentPlayer->GetType() == ENEMY )  )
	{

		--m_nBuildCursor;

		// Stop at the top
		if( m_nBuildCursor < 0 )		
			m_nBuildCursor = 5; //Exit each option

	}
	return true;

}
bool CGamePlay::HandleMovingOnRecruitInterface( void )
{
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();

	// Navigate the Building menu
	if(  ( (pDI->KeyPressed( DIK_D )  || pDI->JoystickGetLStickDirPressed( DIR_DOWN, JOYSTICKID1 ) ) && m_ncurrentPlayer->GetType() == PLAYER ) || ( pDI->KeyPressed( DIK_RIGHT ) || pDI->JoystickGetLStickDirPressed( DIR_DOWN, JOYSTICKID2 ) && m_ncurrentPlayer->GetType() == ENEMY )  )
	{

		++m_nBuildCursor;

		// Stop at the bottom
		if( m_nBuildCursor > 3 )		
			m_nBuildCursor = 0;     //Move/Build/Recruit

	}
	else if(  ( (pDI->KeyPressed( DIK_A )  || pDI->JoystickGetLStickDirPressed( DIR_LEFT, JOYSTICKID1 ) ) && m_ncurrentPlayer->GetType() == PLAYER ) || ( pDI->KeyPressed( DIK_LEFT ) || pDI->JoystickGetLStickDirPressed( DIR_LEFT, JOYSTICKID2 ) && m_ncurrentPlayer->GetType() == ENEMY )  )
	{

		--m_nBuildCursor;

		// Stop at the top
		if( m_nBuildCursor < 0 )		
			m_nBuildCursor = 3; //Exit each option

	}

	return true;
}
bool CGamePlay::HandleMovingOnTerritories( void )
{
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();

	// Navigate the Territory menu
	if(  ( (pDI->KeyPressed( DIK_S )  || pDI->JoystickGetLStickDirPressed( DIR_DOWN, JOYSTICKID1 ) ) && m_ncurrentPlayer->GetType() == PLAYER ) || ( pDI->KeyPressed( DIK_DOWN ) || pDI->JoystickGetLStickDirPressed( DIR_DOWN, JOYSTICKID2 ) && m_ncurrentPlayer->GetType() == ENEMY )  )
	{

		Move(m_cWorld->GetCurrent(), 2);

	}
	else if(  ( (pDI->KeyPressed( DIK_W )  || pDI->JoystickGetLStickDirPressed( DIR_UP, JOYSTICKID1 ) ) && m_ncurrentPlayer->GetType() == PLAYER ) || ( pDI->KeyPressed( DIK_UP ) || pDI->JoystickGetLStickDirPressed( DIR_UP, JOYSTICKID2 ) && m_ncurrentPlayer->GetType() == ENEMY )  )
	{

		Move(m_cWorld->GetCurrent(), 0);

	}
	else if(  ( (pDI->KeyPressed( DIK_D )  || pDI->JoystickGetLStickDirPressed( DIR_RIGHT, JOYSTICKID1 ) ) && m_ncurrentPlayer->GetType() == PLAYER ) || ( pDI->KeyPressed( DIK_RIGHT ) || pDI->JoystickGetLStickDirPressed( DIR_RIGHT, JOYSTICKID2 ) && m_ncurrentPlayer->GetType() == ENEMY )  )
	{
		Move(m_cWorld->GetCurrent(), 1);
	}
	else if(  ( (pDI->KeyPressed( DIK_A )  || pDI->JoystickGetLStickDirPressed( DIR_LEFT, JOYSTICKID1 ) ) && m_ncurrentPlayer->GetType() == PLAYER ) || ( pDI->KeyPressed( DIK_LEFT ) || pDI->JoystickGetLStickDirPressed( DIR_LEFT, JOYSTICKID2 ) && m_ncurrentPlayer->GetType() == ENEMY )  )
	{
		Move(m_cWorld->GetCurrent(), 3);
	}
	return true;
}
bool CGamePlay::HandleEnterForMoveAndAttack( void )
{
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();

	if( ( pDI->KeyPressed( DIK_TAB ) || pDI->JoystickButtonPressed( 1, JOYSTICKID1 ) && m_ncurrentPlayer->GetType() == PLAYER ) || (pDI->KeyPressed( DIK_RETURN ) || pDI->JoystickButtonPressed( 1, JOYSTICKID2 ) && m_ncurrentPlayer->GetType() == ENEMY ) )
	{
		switch (m_nMoveCursor)
		{
		case 5:
			{
				if (m_nTurnOrder == 0)
				{
					if (m_cWorld->GetCurrent()->GetOwner() == 2)
					{
						m_bAMMode = false;
						m_bSelectTerritoryToMove = false;
						m_bIsMoving = false;
						if(MoveUnitsBetweenTerritories())
						{
							m_nMoveCursor = 0;

							m_bIsInWorld = false;


							curX = m_cWorld->GetCurrent()->GetCountry();
							CGame::GetInstance()->PushState( CBattlePlay::GetInstance() );
						}
					}
					else if (m_cWorld->GetCurrent()->GetOwner() == 0)
					{
						m_bAMMode = false;
						m_bIsMoving = false;
						m_bSelectTerritoryToMove = false;
						if(MoveUnitsBetweenTerritories())
						{
							m_cWorld->GetGrid()[m_cWorld->GetCurrent()->GetCountry()][m_cWorld->GetCurrent()->GetTerritory()]->SetOwner( 1 );

							m_nBuildCursor = 0;
							m_nCursor = 1;
							m_nMoveCursor = 0;
							m_cWorld->GetGrid()[m_cWorld->GetCurrent()->GetCountry()][m_cWorld->GetCurrent()->GetTerritory()]->SetOccupied( true );
							CSGD_XAudio2::GetInstance()->SFXPlaySound( m_nConquer );
							if (m_cWorld->GetCurrent()->GetCountry() == 0)
							{
								m_nCountryOneOwner = 1;
							}
							else if (m_cWorld->GetCurrent()->GetCountry() == 3)
							{
								m_nCountryOneOwner = 1;
							}

						}
					}
					else if (m_cWorld->GetCurrent()->GetOwner() == 1)
					{
						m_bAMMode = false;
						m_bSelectTerritoryToMove = false;
						m_bIsMoving = false;
						if(MoveUnitsBetweenTerritories())
						{
							m_nBuildCursor = 0;
							m_nCursor = 1;
							m_nMoveCursor = 0;
						}

					}
				}
				else if (m_nTurnOrder == 1)
				{
					if (m_cWorld->GetCurrent()->GetOwner() == 1)
					{
						m_bAMMode = false;
						m_bIsMoving = false;
						m_bSelectTerritoryToMove = false;
						if(MoveUnitsBetweenTerritories())
						{
							m_nMoveCursor = 0;


							m_bIsInWorld = false;
							curX = m_cWorld->GetCurrent()->GetCountry();
							CGame::GetInstance()->PushState( CBattlePlay::GetInstance() );
						}
					}
					else if (m_cWorld->GetCurrent()->GetOwner() == 0)
					{
						m_bAMMode = false;
						m_bIsMoving = false;
						m_bSelectTerritoryToMove = false;

						if(MoveUnitsBetweenTerritories())
						{

							m_cWorld->GetGrid()[m_cWorld->GetCurrent()->GetCountry()][m_cWorld->GetCurrent()->GetTerritory()]->SetOwner( 2 );
							m_nBuildCursor = false;
							m_nWidth = (800 - ( 5 * 32 ) );
							m_nInOptions = true;
							m_nCursor = 1;
							m_nMoveCursor = 0;
							m_cWorld->GetGrid()[m_cWorld->GetCurrent()->GetCountry()][m_cWorld->GetCurrent()->GetTerritory()]->SetOccupied( true );
							CSGD_XAudio2::GetInstance()->SFXPlaySound( m_nConquer );
							if (m_cWorld->GetCurrent()->GetCountry() == 0)
							{
								m_nCountryOneOwner = 2;
							}
							else if (m_cWorld->GetCurrent()->GetCountry() == 3)
							{
								m_nCountryOneOwner = 2;
							}
						}

					}
					else if (m_cWorld->GetCurrent()->GetOwner() == 2)
					{
						m_bAMMode = false;
						m_bIsMoving = false;
						m_bSelectTerritoryToMove = false;

						if(MoveUnitsBetweenTerritories())
						{
							m_nBuildCursor = 0;
							m_nCursor = 1;
							m_nMoveCursor = 0;
						}

					}
				}

			}
			break;
		case 6:
			{
				m_bAMMode = false;
				m_nBuildCursor = 0;
			}
			break;
		default:
			break;
		}
	}

	return true;

}
bool CGamePlay::HandleEnterForClickingOnTerritories( void )
{
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();

	if( ( pDI->KeyPressed( DIK_TAB ) || pDI->JoystickButtonPressed( 1, JOYSTICKID1 ) && m_ncurrentPlayer->GetType() == PLAYER ) || (pDI->KeyPressed( DIK_RETURN ) || pDI->JoystickButtonPressed( 1, JOYSTICKID2 ) && m_ncurrentPlayer->GetType() == ENEMY ) )
	{
		if (m_bIsMoving == false)
		{
			if (m_cWorld->GetCurrent()->GetOwner() == 1 && m_ncurrentPlayer->GetType() == PLAYER)
			{
				m_bTerritoryMenuUp =true;
			}
			else if (m_cWorld->GetCurrent()->GetOwner() == 2 && m_ncurrentPlayer->GetType() == ENEMY )
			{
				m_bTerritoryMenuUp =true;
			}
		}
		else if (m_bIsMoving == true)
		{
			CheckMovement();
		}

	}

	return true;
}
bool CGamePlay::HandleEnterOnTerritoryMenu( void )
{
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();

	if( ( pDI->KeyPressed( DIK_TAB ) || pDI->JoystickButtonPressed( 1, JOYSTICKID1 ) && m_ncurrentPlayer->GetType() == PLAYER ) || (pDI->KeyPressed( DIK_RETURN ) || pDI->JoystickButtonPressed( 1, JOYSTICKID2 ) && m_ncurrentPlayer->GetType() == ENEMY ) )
	{
		if (m_bBuildMode == false && m_bMoveandAttackMode == false)
		{
			if (m_nTerritoryOptionsCursor == 0)
			{
				if (m_ncurrentPlayer->GetType() == PLAYER)
				{
					m_nTurnOrder = 1;
					m_bBuildMode = true;
					CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nTurnSound);
					m_nWidth = m_cWorld->GetGrid()[1][0]->GetPosX();
					m_nHeight = m_cWorld->GetGrid()[1][0]->GetPosY();
					m_cWorld->ChangeCurrent( m_cWorld->GetGrid()[1][0] );
					curX = 1;
					AllocateResources();
					m_cPlayerOne->SetMoved ( false  ) ;
					m_ncurrentPlayer = m_cPlayerTwo;
					m_bTerritoryMenuUp = false;
					m_nTerritoryOptionsCursor = 0;
					return true;
				}
				else if ( m_ncurrentPlayer->GetType() == ENEMY )
				{
					m_nTurnOrder = 0;
					m_bBuildMode = true;
					CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nTurnSound);
					m_nWidth = m_cWorld->GetGrid()[2][0]->GetPosX();
					m_nHeight = m_cWorld->GetGrid()[2][0]->GetPosY();
					m_cWorld->ChangeCurrent( m_cWorld->GetGrid()[2][0] ); 
					m_cPlayerTwo->SetMoved ( false  ) ;
					m_nTurnNumber++;
					curX = 2;
					AllocateResources();
					m_ncurrentPlayer = m_cPlayerOne;
					m_bTerritoryMenuUp = false;
					m_nTerritoryOptionsCursor = 0;
					return true;
				}
			}
			else if (m_nTerritoryOptionsCursor == 1)
			{
				m_bTerritoryMenuUp = false;
				m_nTerritoryOptionsCursor = 0;
				return true;
			}
		}
		else if (m_bBuildMode == true && m_cWorld->GetCurrent()->GetBuilding() != CAPITOL && m_cWorld->GetCurrent()->GetBuilding() == -1)  // Build Mode Buildings and End Build Phase as well as Exit
		{
			if (m_nTerritoryOptionsCursor == 0)
			{
				Build( m_ncurrentPlayer );
				m_bTerritoryMenuUp = false;
				m_nTerritoryOptionsCursor = 0;
				return true;
			}
			else if (m_nTerritoryOptionsCursor == 1)
			{
				m_bBuildMode = false;
				m_bMoveandAttackMode = true;
				m_bTerritoryMenuUp = false;
				m_nTerritoryOptionsCursor = 0;
				return true;
			}
			else
			{
				m_bTerritoryMenuUp = false;
				m_nTerritoryOptionsCursor = 0;
				return true;
			}
		}
		else if (m_bBuildMode == true && (m_cWorld->GetCurrent()->GetBuilding() == BARRACKS || m_cWorld->GetCurrent()->GetBuilding() == WARFACTORY) )// Build Mode Recruit phase and End Build Phase as well as Exit
		{
			if (m_nTerritoryOptionsCursor == 0)
			{
				Build( m_ncurrentPlayer );
				m_bTerritoryMenuUp = false;
				m_nTerritoryOptionsCursor = 0;
				return true;
			}
			else if (m_nTerritoryOptionsCursor == 1)
			{
				m_bBuildMode = false;
				m_bMoveandAttackMode = true;
				m_bTerritoryMenuUp = false;
				m_nTerritoryOptionsCursor = 0;
				return true;
			}
			else
			{
				m_bTerritoryMenuUp = false;
				m_nTerritoryOptionsCursor = 0;
				return true;

			}
		}
		else if (m_bMoveandAttackMode == true)   //Move and attack phase as well as end phase for move and attack and exit
		{
			if (m_nTerritoryOptionsCursor == 0)
			{
				m_bIsMoving = true;
				m_bTerritoryMenuUp = false;
				m_nTerritoryOptionsCursor = 0;
				m_cWorld->ChangePrevious( m_cWorld->GetCurrent() );
				return true;
			}
			else if (m_nTerritoryOptionsCursor == 1)
			{
				m_bMoveandAttackMode = false;
				m_bTerritoryMenuUp = false;
				m_nTerritoryOptionsCursor = 0;
				return true;
			}
			else
			{
				m_bTerritoryMenuUp = false;
				m_nTerritoryOptionsCursor = 0;
				return true;
			}
		}
		else if (m_bBuildMode == true && ( m_cWorld->GetCurrent()->GetBuilding() == FARM || m_cWorld->GetCurrent()->GetBuilding() == WOOD || m_cWorld->GetCurrent()->GetBuilding() == MINE ))
		{
			if (m_nTerritoryOptionsCursor == 0)
			{
				m_bTerritoryMenuUp = false;
				m_nTerritoryOptionsCursor = 0;
				SellBuilding();
				return true;
			}
			else if (m_nTerritoryOptionsCursor == 1)
			{
				m_bMoveandAttackMode = true;
				m_bBuildMode = true;
				m_bTerritoryMenuUp = false;
				m_nTerritoryOptionsCursor = 0;
				return true;
			}
			else
			{
				m_bTerritoryMenuUp = false;
				m_nTerritoryOptionsCursor = 0;
				return true;
			}
		}

	}
	return true;
}
bool CGamePlay::HandleEnterOnBuildInterface( void )
{
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();
	int wood, steel;
	wood = m_ncurrentPlayer->GetWood();
	steel = m_ncurrentPlayer->GetSteel();

	//Build interface
	if( ( pDI->KeyPressed( DIK_TAB ) || pDI->JoystickButtonPressed( 1, JOYSTICKID1 ) && m_ncurrentPlayer->GetType() == PLAYER ) || (pDI->KeyPressed( DIK_RETURN ) || pDI->JoystickButtonPressed( 1, JOYSTICKID2 ) && m_ncurrentPlayer->GetType() == ENEMY ) )
	{		
		int x = m_cWorld->GetCurrent()->GetPosX();
		int y = m_cWorld->GetCurrent()->GetPosY();

		if(m_cWorld->GetCurrent()->GetOwner() == 1)
		{


			switch(m_nBuildCursor)
			{
			case 0:			//Farm
				if(wood >= 20)
				{
					m_cPlayerOne->AddBuilding(new Farm(1,x,y));
					m_cPlayerOne->SetWood(wood-20);

					m_cWorld->GetGrid()[curX][0]->SetBuilding(FARM);
					m_cWorld->GetCurrent()->SetBuilding( FARM );
					CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nPlaceBuild);

					m_fCTimer = 0.5f;
					m_psConstruction->GetEffect(0)->KillParticles();
					m_psConstruction->GetEffect(0)->SetEmitterPosX((float)m_cWorld->GetGrid()[m_cWorld->GetCurrent()->GetCountry()][m_cWorld->GetCurrent()->GetTerritory()]->GetPosX()-32);
					m_psConstruction->GetEffect(0)->SetEmitterPosY((float)m_cWorld->GetGrid()[m_cWorld->GetCurrent()->GetCountry()][m_cWorld->GetCurrent()->GetTerritory()]->GetPosY()+32);

				}
				break;
			case 1:			//Mine
				if(wood >= 15 && steel >= 10)
				{
					m_cPlayerOne->AddBuilding(new Mine(1,x,y));
					m_cPlayerOne->SetWood(wood-15);
					m_cPlayerOne->SetSteel(steel-10);
					m_cWorld->GetGrid()[curX][0]->SetBuilding(MINE);
					m_cWorld->GetCurrent()->SetBuilding( MINE );

					CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nPlaceBuild);

					m_fCTimer = 0.5f;
					m_psConstruction->GetEffect(0)->KillParticles();
					m_psConstruction->GetEffect(0)->SetEmitterPosX((float)m_cWorld->GetGrid()[m_cWorld->GetCurrent()->GetCountry()][m_cWorld->GetCurrent()->GetTerritory()]->GetPosX()-32);
					m_psConstruction->GetEffect(0)->SetEmitterPosY((float)m_cWorld->GetGrid()[m_cWorld->GetCurrent()->GetCountry()][m_cWorld->GetCurrent()->GetTerritory()]->GetPosY()+32);

				}
				break;
			case 2:			//Lumber Mill
				if(wood >= 20 && steel >= 5)
				{
					m_cPlayerOne->AddBuilding(new WoodMill(1,x,y));
					m_cPlayerOne->SetWood(wood-20);
					m_cPlayerOne->SetSteel(steel-5);
					m_cWorld->GetGrid()[curX][0]->SetBuilding(WOOD);
					m_cWorld->GetCurrent()->SetBuilding( WOOD );
					CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nPlaceBuild);

					m_fCTimer = 0.5f;
					m_psConstruction->GetEffect(0)->KillParticles();
					m_psConstruction->GetEffect(0)->SetEmitterPosX((float)m_cWorld->GetGrid()[m_cWorld->GetCurrent()->GetCountry()][m_cWorld->GetCurrent()->GetTerritory()]->GetPosX()-32);
					m_psConstruction->GetEffect(0)->SetEmitterPosY((float)m_cWorld->GetGrid()[m_cWorld->GetCurrent()->GetCountry()][m_cWorld->GetCurrent()->GetTerritory()]->GetPosY()+32);


				}
				break;
			case 3:			//Barracks
				if(wood >= 30 && steel >= 10)
				{
					m_cPlayerOne->AddBuilding(new Barracks(1,x,y));
					m_cPlayerOne->SetWood(wood-30);
					m_cPlayerOne->SetSteel(steel-10);
					m_cWorld->GetGrid()[curX][0]->SetBuilding(BARRACKS);
					m_cWorld->GetCurrent()->SetBuilding( BARRACKS );

					CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nPlaceBuild);

					m_fCTimer = 0.5f;
					m_psConstruction->GetEffect(0)->KillParticles();
					m_psConstruction->GetEffect(0)->SetEmitterPosX((float)m_cWorld->GetGrid()[m_cWorld->GetCurrent()->GetCountry()][m_cWorld->GetCurrent()->GetTerritory()]->GetPosX()-32);
					m_psConstruction->GetEffect(0)->SetEmitterPosY((float)m_cWorld->GetGrid()[m_cWorld->GetCurrent()->GetCountry()][m_cWorld->GetCurrent()->GetTerritory()]->GetPosY()+32);

				}
				break;
			case 4:			//War Factory
				if(wood >= 20 && steel >= 40)
				{
					m_cPlayerOne->AddBuilding(new WarFactory(1,x,y));
					m_cPlayerOne->SetWood(wood-20);
					m_cPlayerOne->SetSteel(steel-40);
					m_cWorld->GetGrid()[curX][0]->SetBuilding(WARFACTORY);
					m_cWorld->GetCurrent()->SetBuilding( WARFACTORY );

					CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nPlaceBuild);

					m_fCTimer = 0.5f;
					m_psConstruction->GetEffect(0)->KillParticles();
					m_psConstruction->GetEffect(0)->SetEmitterPosX((float)m_cWorld->GetGrid()[m_cWorld->GetCurrent()->GetCountry()][m_cWorld->GetCurrent()->GetTerritory()]->GetPosX()-32);
					m_psConstruction->GetEffect(0)->SetEmitterPosY((float)m_cWorld->GetGrid()[m_cWorld->GetCurrent()->GetCountry()][m_cWorld->GetCurrent()->GetTerritory()]->GetPosY()+32);

				}
				break;
			case 5:			//Exit
				m_bBuildInterface = false;
				m_nBuildCursor = 0;
				break;
			}
		}
		else if(m_cWorld->GetCurrent()->GetOwner() == 2)
		{

			switch(m_nBuildCursor)
			{
			case 0:			//Farm
				if(wood >= 20)
				{
					m_cPlayerTwo->AddBuilding(new Farm(2,x,y));
					m_cPlayerTwo->SetWood(wood-20);
					m_cWorld->GetGrid()[curX][0]->SetBuilding(FARM);
					m_cWorld->GetCurrent()->SetBuilding( FARM );
					CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nPlaceBuild);

					m_fCTimer = 0.5f;
					m_psConstruction->GetEffect(0)->KillParticles();
					m_psConstruction->GetEffect(0)->SetEmitterPosX((float)m_cWorld->GetGrid()[m_cWorld->GetCurrent()->GetCountry()][m_cWorld->GetCurrent()->GetTerritory()]->GetPosX()-32);
					m_psConstruction->GetEffect(0)->SetEmitterPosY((float)m_cWorld->GetGrid()[m_cWorld->GetCurrent()->GetCountry()][m_cWorld->GetCurrent()->GetTerritory()]->GetPosY()+32);

				}
				break;
			case 1:			//Mine
				if(wood >= 15 && steel >= 10)
				{
					m_cPlayerTwo->AddBuilding(new Mine(2,x,y));
					m_cPlayerTwo->SetWood(wood-15);
					m_cPlayerTwo->SetSteel(steel-10);
					m_cWorld->GetGrid()[curX][0]->SetBuilding(MINE);
					m_cWorld->GetCurrent()->SetBuilding( MINE );
					CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nPlaceBuild);

					m_fCTimer = 0.5f;
					m_psConstruction->GetEffect(0)->KillParticles();
					m_psConstruction->GetEffect(0)->SetEmitterPosX((float)m_cWorld->GetGrid()[m_cWorld->GetCurrent()->GetCountry()][m_cWorld->GetCurrent()->GetTerritory()]->GetPosX()-32);
					m_psConstruction->GetEffect(0)->SetEmitterPosY((float)m_cWorld->GetGrid()[m_cWorld->GetCurrent()->GetCountry()][m_cWorld->GetCurrent()->GetTerritory()]->GetPosY()+32);

				}

				break;
			case 2:			//Lumber Mill
				if(wood >= 20 && steel >= 5)
				{
					m_cPlayerTwo->AddBuilding(new WoodMill(2,x,y));
					m_cPlayerTwo->SetWood(wood-20);
					m_cPlayerTwo->SetSteel(steel-5);
					m_cWorld->GetGrid()[curX][0]->SetBuilding( WOOD );
					m_cWorld->GetCurrent()->SetBuilding( WOOD );

					CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nPlaceBuild);

					m_fCTimer = 0.5f;
					m_psConstruction->GetEffect(0)->KillParticles();
					m_psConstruction->GetEffect(0)->SetEmitterPosX((float)m_cWorld->GetGrid()[m_cWorld->GetCurrent()->GetCountry()][m_cWorld->GetCurrent()->GetTerritory()]->GetPosX()-32);
					m_psConstruction->GetEffect(0)->SetEmitterPosY((float)m_cWorld->GetGrid()[m_cWorld->GetCurrent()->GetCountry()][m_cWorld->GetCurrent()->GetTerritory()]->GetPosY()+32);

				}

				break;
			case 3:			//Barracks
				if(wood >= 30 && steel >= 10)
				{
					m_cPlayerTwo->AddBuilding(new Barracks(2,x,y));
					m_cPlayerTwo->SetWood(wood-30);
					m_cPlayerTwo->SetSteel(steel-10);
					m_cWorld->GetGrid()[curX][0]->SetBuilding(BARRACKS);
					m_cWorld->GetCurrent()->SetBuilding( BARRACKS );
					CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nPlaceBuild);

					m_fCTimer = 0.5f;
					m_psConstruction->GetEffect(0)->KillParticles();
					m_psConstruction->GetEffect(0)->SetEmitterPosX((float)m_cWorld->GetGrid()[m_cWorld->GetCurrent()->GetCountry()][m_cWorld->GetCurrent()->GetTerritory()]->GetPosX()-32);
					m_psConstruction->GetEffect(0)->SetEmitterPosY((float)m_cWorld->GetGrid()[m_cWorld->GetCurrent()->GetCountry()][m_cWorld->GetCurrent()->GetTerritory()]->GetPosY()+32);

				}

				break;
			case 4:			//War Factory
				if(wood >= 20 && steel >= 40)
				{
					m_cPlayerTwo->AddBuilding(new WarFactory(2,x,y));
					m_cPlayerTwo->SetWood(wood-20);
					m_cPlayerTwo->SetSteel(steel-40);
					m_cWorld->GetGrid()[curX][0]->SetBuilding(WARFACTORY);
					m_cWorld->GetCurrent()->SetBuilding( WARFACTORY );
					CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nPlaceBuild);

					m_fCTimer = 0.5f;
					m_psConstruction->GetEffect(0)->KillParticles();
					m_psConstruction->GetEffect(0)->SetEmitterPosX((float)m_cWorld->GetGrid()[m_cWorld->GetCurrent()->GetCountry()][m_cWorld->GetCurrent()->GetTerritory()]->GetPosX()-32);
					m_psConstruction->GetEffect(0)->SetEmitterPosY((float)m_cWorld->GetGrid()[m_cWorld->GetCurrent()->GetCountry()][m_cWorld->GetCurrent()->GetTerritory()]->GetPosY()+32);

				}
				break;
			case 5:			//Exit
				m_bBuildInterface = false;
				m_nBuildCursor = 0;
				break;
			}
		}

		m_bBuildInterface = false;
		m_nBuildCursor = 0;

	}


	return true;
}
bool CGamePlay::HandleEnterOnRecruitInterface( void )
{

	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();

	//Recruit Interface
	if( ( pDI->KeyPressed( DIK_TAB ) || pDI->JoystickButtonPressed( 1, JOYSTICKID1 ) && m_ncurrentPlayer->GetType() == PLAYER ) || (pDI->KeyPressed( DIK_RETURN ) || pDI->JoystickButtonPressed( 1, JOYSTICKID2 ) && m_ncurrentPlayer->GetType() == ENEMY ) )
	{
		int x = m_cWorld->GetCurrent()->GetPosX();
		int y = m_cWorld->GetCurrent()->GetPosY();


		int food, wood, steel;



		if(m_cWorld->GetCurrent()->GetOwner() == 1)
		{
			food = m_cPlayerOne->GetFood();
			wood = m_cPlayerOne->GetWood();
			steel = m_cPlayerOne->GetSteel();

			if(m_cWorld->GetCurrent()->GetBuilding() == BARRACKS)
			{
				switch(m_nBuildCursor)
				{
				case 0:			//Foot Soldier
					if(food >= 10)
					{
						CBaseUnit* foot = new CBaseUnit();
						foot->CreateUnit(FOOTSOLDIERS, curX, curY);
						m_cPlayerOne->AddUnit(foot);
						m_cPlayerOne->SetFood(food -10);
						m_bBuildMode = false;
						m_bMoveandAttackMode = true;
						CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nPlaceBuild);
						m_bRecruitInterface = false;
						m_nBuildCursor = 0;

						m_fRTimer = 0.5f;
						m_psRecruit->GetEffect(0)->KillParticles();
						m_psRecruit->GetEffect(0)->SetEmitterPosX((float)m_cWorld->GetGrid()[curX][0]->GetPosX()-32);
						m_psRecruit->GetEffect(0)->SetEmitterPosY((float)m_cWorld->GetGrid()[curX][0]->GetPosY()+32);
					}
					break;
				case 1:			//Priest
					if(food >= 25)
					{
						CBaseUnit* priest = new CBaseUnit();
						priest->CreateUnit(PRIEST, curX, curY);
						m_cPlayerOne->AddUnit(priest);
						m_cPlayerOne->SetFood(food -25);
						m_bBuildMode = false;
						m_bMoveandAttackMode = true;
						CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nPlaceBuild);
						m_bRecruitInterface = false;
						m_nBuildCursor = 0;

						m_fRTimer = 0.5f;
						m_psRecruit->GetEffect(0)->KillParticles();
						m_psRecruit->GetEffect(0)->SetEmitterPosX((float)m_cWorld->GetGrid()[curX][0]->GetPosX()-32);
						m_psRecruit->GetEffect(0)->SetEmitterPosY((float)m_cWorld->GetGrid()[curX][0]->GetPosY()+32);

					}
					break;
				case 2:			//Wizard
					if(food >= 50)
					{
						CBaseUnit* wizard = new CBaseUnit();
						wizard->CreateUnit(WIZARD, curX, curY);
						m_cPlayerOne->AddUnit(wizard);
						m_cPlayerOne->SetFood(food -50);
						m_bBuildMode = false;
						m_bMoveandAttackMode = true;

						CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nPlaceBuild);
						m_bRecruitInterface = false;
						m_nBuildCursor = 0;
						m_fRTimer = 0.5f;
						m_psRecruit->GetEffect(0)->KillParticles();
						m_psRecruit->GetEffect(0)->SetEmitterPosX((float)m_cWorld->GetGrid()[curX][0]->GetPosX()-32);
						m_psRecruit->GetEffect(0)->SetEmitterPosY((float)m_cWorld->GetGrid()[curX][0]->GetPosY()+32);
					}
					break;
				case 3:			//Exit
					m_bRecruitInterface = false;
					m_nBuildCursor = 0;
					break;

				}
			}
			else if(m_cWorld->GetCurrent()->GetBuilding()== WARFACTORY)
			{
				switch(m_nBuildCursor)
				{
				case 0:			//Cavalry
					if(food >= 50 && steel >= 10)
					{
						CBaseUnit* cavalry = new CBaseUnit();
						cavalry->CreateUnit(CALVARY, curX, curY);
						m_cPlayerOne->AddUnit(cavalry);
						m_cPlayerOne->SetFood(food -50);
						m_cPlayerOne->SetSteel(steel -10);
						m_bRecruitInterface = false;
						m_nBuildCursor = 0;
						m_bBuildMode = false;
						m_bMoveandAttackMode = true;
						CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nPlaceBuild);

						m_fRTimer = 0.5f;
						m_psRecruit->GetEffect(0)->KillParticles();
						m_psRecruit->GetEffect(0)->SetEmitterPosX((float)m_cWorld->GetGrid()[curX][0]->GetPosX()-32);
						m_psRecruit->GetEffect(0)->SetEmitterPosY((float)m_cWorld->GetGrid()[curX][0]->GetPosY()+32);
					}
					break;
				case 1:			//Catapult
					if(wood >= 100 && steel >= 100)
					{
						CBaseUnit* catapult = new CBaseUnit();
						catapult->CreateUnit(CATAPULT, curX, curY);
						m_cPlayerOne->AddUnit(catapult);
						m_cPlayerOne->SetWood(wood -100);
						m_cPlayerOne->SetSteel(steel -100);
						m_bRecruitInterface = false;
						m_nBuildCursor = 0;
						m_bBuildMode = false;
						m_bMoveandAttackMode = true;
						CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nPlaceBuild);

						m_fRTimer = 0.5f;
						m_psRecruit->GetEffect(0)->KillParticles();
						m_psRecruit->GetEffect(0)->SetEmitterPosX((float)m_cWorld->GetGrid()[curX][0]->GetPosX()-32);
						m_psRecruit->GetEffect(0)->SetEmitterPosY((float)m_cWorld->GetGrid()[curX][0]->GetPosY()+32);

					}
					break;
				case 2:			//Exit
					m_bRecruitInterface = false;
					m_nBuildCursor = 0;
					break;
				}
			}


		}
		else if(m_cWorld->GetCurrent()->GetOwner() == 2)
		{
			food = m_cPlayerTwo->GetFood();
			wood = m_cPlayerTwo->GetWood();
			steel = m_cPlayerTwo->GetSteel();

			if(m_cWorld->GetCurrent()->GetBuilding() == BARRACKS)
			{
				switch(m_nBuildCursor)
				{
				case 0:			//Foot Soldier
					if(food >= 10)
					{
						CBaseUnit* foot = new CBaseUnit();
						foot->CreateUnit(SKELETALWARRIOR, curX, curY);
						m_cPlayerTwo->AddUnit(foot);
						m_cPlayerTwo->SetFood(food -10);
						m_bBuildMode = false;
						m_bMoveandAttackMode = true;
						CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nPlaceBuild);
						m_bRecruitInterface = false;
						m_nBuildCursor = 0;

						m_fRTimer = 0.5f;
						m_psRecruit->GetEffect(0)->KillParticles();
						m_psRecruit->GetEffect(0)->SetEmitterPosX((float)m_cWorld->GetGrid()[curX][0]->GetPosX()-32);
						m_psRecruit->GetEffect(0)->SetEmitterPosY((float)m_cWorld->GetGrid()[curX][0]->GetPosY()+32);
					}
					break;
				case 1:			//Priest
					if(food >= 25)
					{
						CBaseUnit* priest = new CBaseUnit();
						priest->CreateUnit(SHADOWPRIEST, curX, curY);
						m_cPlayerTwo->AddUnit(priest);
						m_cPlayerTwo->SetFood(food -25);
						m_bBuildMode = false;
						m_bMoveandAttackMode = true;
						CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nPlaceBuild);
						m_bRecruitInterface = false;
						m_nBuildCursor = 0;

						m_fRTimer = 0.5f;
						m_psRecruit->GetEffect(0)->KillParticles();
						m_psRecruit->GetEffect(0)->SetEmitterPosX((float)m_cWorld->GetGrid()[curX][0]->GetPosX()-32);
						m_psRecruit->GetEffect(0)->SetEmitterPosY((float)m_cWorld->GetGrid()[curX][0]->GetPosY()+32);
					}
					break;
				case 2:			//Wizard
					if(food >= 50)
					{
						CBaseUnit* wizard = new CBaseUnit();
						wizard->CreateUnit(NECROMANCER, curX, curY);
						m_cPlayerTwo->AddUnit(wizard);
						m_cPlayerTwo->SetFood(food -50);
						m_bBuildMode = false;
						m_bMoveandAttackMode = true;

						CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nPlaceBuild);
						m_bRecruitInterface = false;
						m_nBuildCursor = 0;

						m_fRTimer = 0.5f;
						m_psRecruit->GetEffect(0)->KillParticles();
						m_psRecruit->GetEffect(0)->SetEmitterPosX((float)m_cWorld->GetGrid()[curX][0]->GetPosX()-32);
						m_psRecruit->GetEffect(0)->SetEmitterPosY((float)m_cWorld->GetGrid()[curX][0]->GetPosY()+32);
					}
					break;
				case 3:			//Exit
					m_bRecruitInterface = false;
					m_nBuildCursor = 0;
					break;

				}
			}
			else if(m_cWorld->GetCurrent()->GetBuilding()== WARFACTORY)
			{
				switch(m_nBuildCursor)
				{
				case 0:			//Death Knight
					if(food >= 50 && steel >= 10)
					{
						CBaseUnit* dk = new CBaseUnit();
						dk->CreateUnit(DEATHKNIGHT, curX, curY);
						m_cPlayerTwo->AddUnit(dk);
						m_cPlayerTwo->SetFood(food -50);
						m_cPlayerTwo->SetSteel(steel -10);
						m_bRecruitInterface = false;
						m_nBuildCursor = 0;

						m_bBuildMode = false;
						m_bMoveandAttackMode = true;
						CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nPlaceBuild);

						m_fRTimer = 0.5f;
						m_psRecruit->GetEffect(0)->KillParticles();
						m_psRecruit->GetEffect(0)->SetEmitterPosX((float)m_cWorld->GetGrid()[curX][0]->GetPosX()-32);
						m_psRecruit->GetEffect(0)->SetEmitterPosY((float)m_cWorld->GetGrid()[curX][0]->GetPosY()+32);
					}
					break;
				case 1:			//War Machine
					if(wood >= 100 && steel >= 100)
					{
						CBaseUnit* wm = new CBaseUnit();
						wm->CreateUnit(WARMACHINE, curX, curY);
						m_cPlayerTwo->AddUnit(wm);
						m_cPlayerTwo->SetWood(wood -100);
						m_cPlayerTwo->SetSteel(steel -100);
						m_bRecruitInterface = false;
						m_nBuildCursor = 0;
						m_bBuildMode = false;
						m_bMoveandAttackMode = true;

						CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nPlaceBuild);

						m_fRTimer = 0.5f;
						m_psRecruit->GetEffect(0)->KillParticles();
						m_psRecruit->GetEffect(0)->SetEmitterPosX((float)m_cWorld->GetGrid()[curX][0]->GetPosX()-32);
						m_psRecruit->GetEffect(0)->SetEmitterPosY((float)m_cWorld->GetGrid()[curX][0]->GetPosY()+32);

					}
					break;
				case 2:			//Exit
					m_bRecruitInterface = false;
					m_nBuildCursor = 0;
					break;

				}
			}
		}
		m_bRecruitInterface = false;
		m_nBuildCursor = 0;
	}
	return true;
}
void CGamePlay::UpdateAI(void)
{
	if(m_bBuildMode)
	{
		if(m_cWorld->GetCurrent()->GetBuilding() == -1 && m_cWorld->GetCurrent()->GetBuilding() != CAPITOL && m_cWorld->GetGrid()[1][0]->GetOwner() == 2 )
		{
			if(m_cPlayerTwo->GetWood() >= 30 && m_cPlayerTwo->GetSteel() >= 10)
			{
				m_cPlayerTwo->AddBuilding(new Barracks(1,m_cWorld->GetCurrent()->GetPosX(),m_cWorld->GetCurrent()->GetPosY()));
				m_cPlayerTwo->SetWood(m_cPlayerTwo->GetWood()-30);
				m_cWorld->GetGrid()[curX][0]->SetBuilding(BARRACKS);

				CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nPlaceBuild);

				m_fCTimer = 0.5f;
				m_psConstruction->GetEffect(0)->KillParticles();
				m_psConstruction->GetEffect(0)->SetEmitterPosX((float)m_cWorld->GetGrid()[curX][0]->GetPosX()-32);
				m_psConstruction->GetEffect(0)->SetEmitterPosY((float)m_cWorld->GetGrid()[curX][0]->GetPosY()+32);
				m_bBuildMode = false;
				m_bMoveandAttackMode = true;

			}
		}
		else if(m_cWorld->GetGrid()[0][0]->GetOwner() == 2 && m_cWorld->GetGrid()[0][0]->GetBuilding() == -1)
		{
			if(m_cPlayerTwo->GetWood() >= 30 && m_cPlayerTwo->GetSteel() >= 10)
			{
				m_cPlayerTwo->AddBuilding(new Barracks(1,m_cWorld->GetGrid()[0][0]->GetPosX(),m_cWorld->GetGrid()[0][0]->GetPosY()));
				m_cPlayerTwo->SetWood(m_cPlayerTwo->GetWood()-30);
				m_cPlayerTwo->SetSteel(m_cPlayerTwo->GetSteel()-10);
				m_cWorld->GetGrid()[0][0]->SetBuilding(BARRACKS);

				CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nPlaceBuild);

				m_fCTimer = 0.5f;
				m_psConstruction->GetEffect(0)->KillParticles();
				m_psConstruction->GetEffect(0)->SetEmitterPosX((float)m_cWorld->GetGrid()[0][0]->GetPosX()-32);
				m_psConstruction->GetEffect(0)->SetEmitterPosY((float)m_cWorld->GetGrid()[0][0]->GetPosY()+32);
				m_bBuildMode = false;
				m_bMoveandAttackMode = true;

			}
		}
		m_bBuildMode = false;
		m_bMoveandAttackMode = true;
	}
	else if(m_bMoveandAttackMode)
	{
		if(m_cWorld->GetGrid()[0][0]->GetBuilding() == BARRACKS && m_cWorld->GetGrid()[0][0]->GetOwner() == 2)
		{

			if(m_cPlayerTwo->GetFood() >= 10)
			{
				CBaseUnit* foot = new CBaseUnit();
				foot->CreateUnit(SKELETALWARRIOR, curX, curY);
				m_cPlayerTwo->AddUnit(foot);
				m_cPlayerTwo->SetFood(m_cPlayerTwo->GetFood()-10);

				CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nPlaceBuild);

				m_fRTimer = 0.5f;
				m_psRecruit->GetEffect(0)->KillParticles();
				m_psRecruit->GetEffect(0)->SetEmitterPosX((float)m_cWorld->GetGrid()[0][0]->GetPosX()-32);
				m_psRecruit->GetEffect(0)->SetEmitterPosY((float)m_cWorld->GetGrid()[0][0]->GetPosY()+32);
			}
		}
		else if(m_cWorld->GetGrid()[3][0]->GetBuilding() == BARRACKS && m_cWorld->GetGrid()[3][0]->GetOwner() == 2)
		{

			if(m_cPlayerTwo->GetFood() >= 10)
			{
				CBaseUnit* foot = new CBaseUnit();
				foot->CreateUnit(SKELETALWARRIOR, curX, curY);
				m_cPlayerTwo->AddUnit(foot);
				m_cPlayerTwo->SetFood(m_cPlayerTwo->GetFood()-10);

				CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nPlaceBuild);

				m_fRTimer = 0.5f;
				m_psRecruit->GetEffect(0)->KillParticles();
				m_psRecruit->GetEffect(0)->SetEmitterPosX((float)m_cWorld->GetGrid()[3][0]->GetPosX()-32);
				m_psRecruit->GetEffect(0)->SetEmitterPosY((float)m_cWorld->GetGrid()[3][0]->GetPosY()+32);
			}
		}

		
			skeletonCount = 0;
			spriestCount = 0;
			necroCount = 0;
			dkCount = 0;
			wmCount = 0;
			m_cWorld->ChangePrevious( m_cWorld->GetGrid()[1][0] );


			for(unsigned int i = 0; i < m_cPlayerTwo->GetUnits().size();i++)
			{
				if(m_cPlayerTwo->GetUnits()[i]->GetCountry() == m_cWorld->GetPrevious()->GetCountry())
				{
					switch(m_cPlayerTwo->GetUnits()[i]->GetUnitID())
					{
					case SKELETALWARRIOR:
						{
							skeletonCount += 1;
						}
						break;
					case SHADOWPRIEST:
						{
							spriestCount += 1;
						}
						break;
					case NECROMANCER:
						{
							necroCount += 1;
						}
						break;
					case DEATHKNIGHT:
						{
							dkCount += 1;
						}
						break;
					case WARMACHINE:
						{
							wmCount += 1;
						}
						break;

					default:
						break;
					}
				}
			}

			if(skeletonCount != 0)
				m_MoveskeletonCount = rand() % (skeletonCount + 1);
			if(spriestCount != 0)
				m_MovespriestCount = rand() % (spriestCount + 1);
			if(necroCount != 0)
				m_MovenecroCount = rand() % (necroCount + 1);
			if(dkCount != 0)
				m_MovedkCount = rand() % (dkCount + 1);
			if(wmCount != 0)
				m_MovewmCount = rand() % (wmCount + 1);



			if(m_MoveskeletonCount == 0 && skeletonCount != 0)
				m_MoveskeletonCount = 1;

			if(m_MovespriestCount == 0 && spriestCount != 0 && m_MoveskeletonCount == 0 && skeletonCount != 0)
				m_MovespriestCount = 1;


			

			if(m_cWorld->GetGrid()[0][0]->GetOwner() == 0)
			{
				m_cWorld->GetGrid()[0][0]->SetOwner(2);
				m_cWorld->GetGrid()[0][0]->SetOccupied( true );
				m_cWorld->ChangePrevious( m_cWorld->GetGrid()[1][0] );
				m_cWorld->ChangeCurrent( m_cWorld->GetGrid()[0][0] );

				MoveUnitsBetweenTerritories();


			}
			else if (m_cWorld->GetGrid()[3][0]->GetOwner() == 0)
			{
				m_cWorld->GetGrid()[3][0]->SetOwner(2);
				m_cWorld->GetGrid()[3][0]->SetOccupied( true );
				m_cWorld->ChangePrevious( m_cWorld->GetGrid()[1][0] );
				m_cWorld->ChangeCurrent( m_cWorld->GetGrid()[3][0] );

				MoveUnitsBetweenTerritories();

			}
			else if(m_cWorld->GetGrid()[0][0]->GetOwner() == 1)
			{
				m_cWorld->ChangePrevious( m_cWorld->GetGrid()[1][0] );
				m_cWorld->ChangeCurrent( m_cWorld->GetGrid()[0][0] );
				MoveUnitsBetweenTerritories();
				SetIsInWorld(false);
				curX = m_cWorld->GetCurrent()->GetCountry();
				CGame::GetInstance()->PushState( CBattlePlay::GetInstance() );


			}
			else if (m_cWorld->GetGrid()[2][0]->GetOwner() == 1)
			{
				m_cWorld->ChangePrevious( m_cWorld->GetGrid()[0][0] );
				m_cWorld->ChangeCurrent( m_cWorld->GetGrid()[2][0] );
				MoveUnitsBetweenTerritories();
				SetIsInWorld(false);
				curX = m_cWorld->GetCurrent()->GetCountry();
				CGame::GetInstance()->PushState( CBattlePlay::GetInstance() );
			}



		
		m_bMoveandAttackMode = false;

	}
	else if(m_bMoveandAttackMode == false && m_bBuildMode == false)
	{

		m_nTurnOrder = 0;
		m_bBuildMode = true;
		m_nInOptions = false;
		m_cWorld->ChangeCurrent( m_cWorld->GetGrid()[2][0] ); 
		m_cPlayerTwo->SetMoved ( false  ) ;
		m_ncurrentPlayer = m_cPlayerOne;
		m_nTurnNumber++;
		AllocateResources();


	}





}
void CGamePlay::RenderOwnership	( void )
{
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();

	for (unsigned int i = 0; i < m_cWorld->GetGrid().size(); i++)
	{
		for (unsigned int j = 0; j < m_cWorld->GetGrid()[i].size(); j++)
		{
			if(m_cWorld->GetGrid()[i][j]->GetOwner() == 1)
			{
				pTM->Draw(m_nHuman,m_cWorld->GetGrid()[i][j]->GetPosX(),m_cWorld->GetGrid()[i][j]->GetPosY());
			}
			else if(m_cWorld->GetGrid()[i][j]->GetOwner() == 2)
			{
				pTM->Draw(m_nUndead,m_cWorld->GetGrid()[i][j]->GetPosX()- 32,m_cWorld->GetGrid()[i][j]->GetPosY() - 32);
			}

		}
	}
}
void CGamePlay::RenderSellOrKeep( void )
{
	CBitmapFont* pFont = CGame::GetInstance()->GetFont();

	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();



	int posX = m_cWorld->GetCurrent()->GetPosX() + 40;
	int posY = m_cWorld->GetCurrent()->GetPosY() - 50;


	pTM->Draw(commandImageID,300, 200, 0.75f, 0.75f);
	pFont->Draw(_T("You Won!"), 300 + 42, 200, 0.9f, D3DCOLOR_XRGB(255, 255, 255));

	pFont->Draw(_T("Choose Option:"), 300 + 32, 200+50, 0.75f, D3DCOLOR_XRGB(255, 255, 255));


	pFont->Draw(_T("Keep Building"), 300 + 32, 200+95, 0.75f, D3DCOLOR_XRGB(255, 255, 255));
	pFont->Draw(_T("Sell Building"), 300 + 32, 200+140, 0.75f, D3DCOLOR_XRGB(255, 255, 255));


	//Render Cursor Position
	pTM->Draw(cursorImageID, 300 , (245 + 55) + (m_nMoveOrSellOptions *45), 0.3f, 0.3f, nullptr, 35.0f / 2, 72.0f / 2, D3DX_PI / -2);
	pTM->Draw(cursorImageID, 480, (245 + 45) + (m_nMoveOrSellOptions *45), 0.3f, 0.3f, nullptr, 35.0f / 2, 72.0f / 2, D3DX_PI / 2);



}
bool CGamePlay::HandleAllInputForSellOrKeep( void )
{
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();

	// Navigate the Territory menu
	if(  ( (pDI->KeyPressed( DIK_S )  || pDI->JoystickGetLStickDirPressed( DIR_DOWN, JOYSTICKID1 ) ) && m_ncurrentPlayer->GetType() == PLAYER ) || ( pDI->KeyPressed( DIK_DOWN ) || pDI->JoystickGetLStickDirPressed( DIR_DOWN, JOYSTICKID2 ) && m_ncurrentPlayer->GetType() == ENEMY )  )
	{

		++m_nMoveOrSellOptions;

		// Stop at the bottom
		if( m_nMoveOrSellOptions > 1 )		
			m_nMoveOrSellOptions = 0;     //Sell 
		return true;

	}
	else if(  ( pDI->KeyPressed( DIK_W )  || pDI->JoystickGetLStickDirPressed( DIR_UP, JOYSTICKID1 )  && m_ncurrentPlayer->GetType() == PLAYER ) || ( pDI->KeyPressed( DIK_UP ) || pDI->JoystickGetLStickDirPressed( DIR_UP, JOYSTICKID2 ) && m_ncurrentPlayer->GetType() == ENEMY )  )
	{

		--m_nMoveOrSellOptions;

		// Stop at the top
		if( m_nMoveOrSellOptions < 0 )		
			m_nMoveOrSellOptions = 1; //Keep
		return true;

	}
	if( ( pDI->KeyPressed( DIK_TAB ) || pDI->JoystickButtonPressed( 1, JOYSTICKID1 ) && m_ncurrentPlayer->GetType() == PLAYER ) || (pDI->KeyPressed( DIK_RETURN ) || pDI->JoystickButtonPressed( 1, JOYSTICKID2 ) && m_ncurrentPlayer->GetType() == ENEMY ) )
	{
		switch (m_nMoveOrSellOptions)
		{
		case 0:   //Keep building
			{
				SellOrKeepBuilding( 0 );
			}
			break;
		case 1:  //sell Building
			{
				SellOrKeepBuilding( 1 );
			}
			break;
		default:
			break;
		}
	}

	return true;
}
void CGamePlay::SellOrKeepBuilding( int temp )
{
	if (temp == 1)   //Selling buildings that other people have
	{
		int wood;
		int steel;
		wood = m_ncurrentPlayer->GetWood();
		steel = m_ncurrentPlayer->GetSteel();
		vector<CBaseBuilding*> temp = m_cPlayerOne->GetBuildings();
		if (m_ncurrentPlayer->GetType() == PLAYER)
		{


			for (unsigned int i = 0; i < m_cPlayerOne->GetBuildings().size(); i++)
			{
				if (m_cPlayerOne->GetBuildings()[i]->GetPosBuildX() == m_cWorld->GetCurrent()->GetPosX() && m_cPlayerOne->GetBuildings()[i]->GetPosBuildY() == m_cWorld->GetCurrent()->GetPosY() )
				{

					switch (m_cPlayerOne->GetBuildings()[i]->GetBuildingID())
					{
					case FARM:
						m_ncurrentPlayer->SetWood( wood + 10 );
						break;
					case WOOD:
						m_ncurrentPlayer->SetWood( wood +10 );
						break;
					case MINE:
						m_ncurrentPlayer->SetWood( wood + 5 );
						m_ncurrentPlayer->SetSteel( steel + 5 );
						break;
					case BARRACKS:
						m_ncurrentPlayer->SetWood( wood + 15 );
						m_ncurrentPlayer->SetSteel( steel +5 );
						break;
					case WARFACTORY:
						m_ncurrentPlayer->SetWood( wood +10 );
						m_ncurrentPlayer->SetSteel( steel + 20 );
						break;
					default:
						break;
					}
					m_cPlayerOne->DeleteBuilding( i );
					m_cWorld->GetCurrent()->SetBuilding( -1 );

				}
			}
			m_bChooseToSellBack = false;
		}
		else if ( m_ncurrentPlayer->GetType() == ENEMY )
		{
			for (unsigned int i = 0; i < m_cPlayerTwo->GetBuildings().size(); i++)
			{
				if (m_cPlayerTwo->GetBuildings()[i]->GetPosBuildX() == m_cWorld->GetCurrent()->GetPosX() && m_cPlayerTwo->GetBuildings()[i]->GetPosBuildY() == m_cWorld->GetCurrent()->GetPosY() )
				{

					switch (m_cPlayerTwo->GetBuildings()[i]->GetBuildingID())
					{
					case FARM:
						m_ncurrentPlayer->SetWood( wood + 10 );
						break;
					case WOOD:
						m_ncurrentPlayer->SetWood( wood +10 );
						break;
					case MINE:
						m_ncurrentPlayer->SetWood( wood + 5 );
						m_ncurrentPlayer->SetSteel( steel + 5 );
						break;
					case BARRACKS:
						m_ncurrentPlayer->SetWood( wood + 15 );
						m_ncurrentPlayer->SetSteel( steel +5 );
						break;
					case WARFACTORY:
						m_ncurrentPlayer->SetWood( wood +10 );
						m_ncurrentPlayer->SetSteel( steel + 20 );
						break;
					default:
						break;
					}
					m_cPlayerTwo->DeleteBuilding( i );
					m_cWorld->GetCurrent()->SetBuilding( -1 );
				}
			}
			m_bChooseToSellBack = false;
		}

	}
	else if (temp == 0)// Trading ownership
	{
		if (m_ncurrentPlayer->GetType() == ENEMY)
		{


			for (unsigned int i = 0; i < m_cPlayerOne->GetBuildings().size(); i++)
			{
				if (m_cPlayerOne->GetBuildings()[i]->GetPosBuildX() == m_cWorld->GetCurrent()->GetPosX() && m_cPlayerOne->GetBuildings()[i]->GetPosBuildY() == m_cWorld->GetCurrent()->GetPosY() )
				{

					switch (m_cPlayerOne->GetBuildings()[i]->GetBuildingID())
					{
					case FARM:
						m_cPlayerTwo->AddBuilding( new Farm( 1, m_cPlayerOne->GetBuildings()[i]->GetPosBuildX(), m_cPlayerOne->GetBuildings()[i]->GetPosBuildY() ) );
						break;
					case WOOD:
						m_cPlayerTwo->AddBuilding( new WoodMill( 1, m_cPlayerOne->GetBuildings()[i]->GetPosBuildX(), m_cPlayerOne->GetBuildings()[i]->GetPosBuildY() ) );
						break;
					case MINE:
						m_cPlayerTwo->AddBuilding( new Mine( 1, m_cPlayerOne->GetBuildings()[i]->GetPosBuildX(), m_cPlayerOne->GetBuildings()[i]->GetPosBuildY() ) );		
						break;
					case BARRACKS:
						m_cPlayerTwo->AddBuilding( new Barracks( 1, m_cPlayerOne->GetBuildings()[i]->GetPosBuildX(), m_cPlayerOne->GetBuildings()[i]->GetPosBuildY() ) );
						break;
					case WARFACTORY:
						m_cPlayerTwo->AddBuilding( new WarFactory( 1, m_cPlayerOne->GetBuildings()[i]->GetPosBuildX(), m_cPlayerOne->GetBuildings()[i]->GetPosBuildY() ) );						
						break;
					default:
						break;
					}
					m_cPlayerOne->DeleteBuilding( i );
				}
			}
			m_bChooseToSellBack = false;
		}
		else if ( m_ncurrentPlayer->GetType() == PLAYER )
		{
			for (unsigned int i = 0; i < m_cPlayerTwo->GetBuildings().size(); i++)
			{
				if (m_cPlayerTwo->GetBuildings()[i]->GetPosBuildX() == m_cWorld->GetCurrent()->GetPosX() && m_cPlayerTwo->GetBuildings()[i]->GetPosBuildY() == m_cWorld->GetCurrent()->GetPosY() )
				{

					switch (m_cPlayerTwo->GetBuildings()[i]->GetBuildingID())
					{
					case FARM:
						m_cPlayerOne->AddBuilding( new Farm( 1, m_cPlayerTwo->GetBuildings()[i]->GetPosBuildX(), m_cPlayerTwo->GetBuildings()[i]->GetPosBuildY() ) );
						break;
					case WOOD:
						m_cPlayerOne->AddBuilding( new WoodMill( 1, m_cPlayerTwo->GetBuildings()[i]->GetPosBuildX(), m_cPlayerTwo->GetBuildings()[i]->GetPosBuildY() ) );
						break;
					case MINE:
						m_cPlayerOne->AddBuilding( new Mine( 1, m_cPlayerTwo->GetBuildings()[i]->GetPosBuildX(), m_cPlayerTwo->GetBuildings()[i]->GetPosBuildY() ) );		
						break;
					case BARRACKS:
						m_cPlayerOne->AddBuilding( new Barracks( 1, m_cPlayerTwo->GetBuildings()[i]->GetPosBuildX(), m_cPlayerTwo->GetBuildings()[i]->GetPosBuildY() ) );
						break;
					case WARFACTORY:
						m_cPlayerOne->AddBuilding( new WarFactory( 1, m_cPlayerTwo->GetBuildings()[i]->GetPosBuildX(), m_cPlayerTwo->GetBuildings()[i]->GetPosBuildY() ) );						
						break;
					default:
						break;
					}
					m_cPlayerTwo->DeleteBuilding( i );
				}
			}
			m_bChooseToSellBack = false;
		}
	}
}
void CGamePlay::ShowTutorial(void)
{
	

	

	TOSTRINGSTREAM output;

	if(m_bMoveandAttackMode)
	{
		
		if(m_bIsMoving)
			output << "Select a territory to move units into";
		else if(m_nInOptions || m_bTerritoryMenuUp)
			output << "";
		else if(m_bMoveandAttackMode)
		output << "select a territory to move units from";
	}
	else if(m_bAMMode)
		output << "bAMMode";
	else if(m_bBuildMode)
	{
		
		if(m_nInOptions || m_bTerritoryMenuUp)
			output << "";
		else if(m_bBuildInterface)
			output << "Select a type of building to make";
		else if(m_bRecruitInterface)
			output << "Select which unit you want to purchase";
		else if(!m_bRecruitInterface && !m_bBuildInterface)
		output << "	Select one of your territories to build/sell";
		}
	
	RECT box = { 200, 484, 600, 505};

	if(output.str().size() > 0)	
	CSGD_Direct3D::GetInstance()->DrawRect(box, D3DCOLOR_ARGB(255,0,0, 0));

	CSGD_Direct3D::GetInstance()->DrawText(output.str().c_str(), 205, 485);


}
