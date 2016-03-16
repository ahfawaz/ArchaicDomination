#include "ArcadeGamePlay.h"


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




////////////////////////////////////////////////////
//Singleton
CArcadeGamePlay* CArcadeGamePlay::GetInstance( void )
{

	//Global Instance
	static CArcadeGamePlay s_Instance;
	return &s_Instance;

}



//Functions for every state
bool CArcadeGamePlay::Input ( void )					 
{
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();


	if (m_bAMMode == false)
	{

	

	if (pDI->JoystickGetLStickDirPressed( DIR_RIGHT, JOYSTICKID1) == true )
	{
		if (m_nInOptions == true && m_bBuildInterface == false)
		{
			if (m_nCursor == 1 )
			{
				m_nWidth = (800 - (5*32));
			}

		}
		else
		{
			Move( 2 );
		}

	}

	if ( pDI->JoystickGetLStickDirPressed( DIR_LEFT, JOYSTICKID1) == true )
	{
		if (m_nInOptions == true && m_bBuildInterface == false)
		{
			if (m_bBuildMode && m_cCurrentTile->GetCurrent()->GetBuilding() != CAPITOL && m_cCurrentTile->GetCurrent()->GetBuilding() != -1 && m_nCursor == 1)  //handling sell
			{
				m_nWidth = (800 - (9*32));
			}
			else if( m_nCursor == 1 && m_bBuildMode == false) 
			{
				m_nWidth = (800 - (9*32));
			}

		}
		else
		{			
			Move( 2 );
		}
	}


	// Navigate the menu
	if( (pDI->JoystickGetLStickDirPressed( DIR_DOWN, JOYSTICKID1) == true )  )
	{
		if (m_nInOptions == true && m_bBuildInterface == false && m_bRecruitInterface == false)
		{
			if (m_bBuildMode == false && m_bMoveandAttackMode == false)
			{

			}
			else if( m_bMoveandAttackMode == true && m_nWidth !=  (800 - ( 5 * 32 ) ) )
			{
				++m_nCursor;
			}
			else if( m_bBuildMode == true )
			{
				if(m_nWidth = (800 - (5*32)))
					++m_nCursor;
			}


			// Stop at the bottom
			if( m_nCursor > 1 )		// EXIT
				m_nCursor = 0;
		}
		else if(m_bBuildInterface == true)
		{
			++m_nBuildCursor;
			if(m_nBuildCursor > 5)
				m_nBuildCursor = 0;
		}
		else if(m_bRecruitInterface == true)
		{
			++m_nBuildCursor;
			if(m_nBuildCursor > 3)
				m_nBuildCursor = 0;
		}

		else
		{
			Move( 1 );
		}

	}
	else if( ( pDI->JoystickGetLStickDirPressed( DIR_UP, JOYSTICKID1) == true )  )
	{


		if (m_nInOptions == true && m_bBuildInterface == false && m_bRecruitInterface == false)
		{
			if (m_bBuildMode == false && m_bMoveandAttackMode == false)
			{

			}
			else if ( m_bMoveandAttackMode == true && m_nWidth != (800 - ( 5 * 32 ) ) )
			{
				--m_nCursor;
			}
			else if( m_bBuildMode == true )
			{
				if(m_nWidth = (800 - (5*32)))
					--m_nCursor;
			}

			// Stop at the top
			if( m_nCursor < 0 )		// PLAY
				m_nCursor = 1;
		}
		else if(m_bBuildInterface == true)
		{
			--m_nBuildCursor;
			if(m_nBuildCursor < 0)
				m_nBuildCursor = 5;
		}
		else if(m_bRecruitInterface == true)
		{
			--m_nBuildCursor;
			if(m_nBuildCursor > 3)
				m_nBuildCursor = 0;
		}

		else
		{
			Move( 1 );
		}
	}

	if (m_bBuildMode)    //Building phase
	{	
		BuildPhase();
	}


	if (m_nInOptions == false)
	{
		if( pDI->JoystickButtonPressed( 2, JOYSTICKID1 ) == true )
		{
			m_cCurrentTile->ChangeCurrent( m_cWorld->GetGrid()[0][0] );

			m_nInOptions = true;
			if (m_bIsMoving)
			{
				m_nCursor  = 0;
				m_nWidth = (800 - (9*32));			
			}
			else
			{
				m_nCursor  = 1;
				m_nWidth = (800 - (9*32));
			}

			return true;
		}
	}
	if (pDI->JoystickButtonPressed( 5, JOYSTICKID1 ) == true)
	{
		CGame::GetInstance()->PushState( CPause::GetInstance() );
		return true;
	}


	if(pDI->JoystickButtonPressed(3, JOYSTICKID1) == true)
	{
		cheatstring += "W";
		if(cheatstring.size() == 2 || cheatstring.size() == 3)
		{
			cheatstring.clear();

		}
	}
	if(pDI->JoystickButtonPressed(4, JOYSTICKID1) == true)
	{
		cheatstring += "I";
		if(cheatstring.size() == 1 || cheatstring.size() == 3)
		{
			cheatstring.clear();	
		}
	}
	if(pDI->JoystickButtonPressed(4, JOYSTICKID1) == true) 
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




	// Handle the Enter key
	HandleEnterKey( );

	}
	else if (m_bAMMode == true)
	{
		HandleInputForMoveAndAttack();
	}
	// Keep running
	return true;
}
void CArcadeGamePlay::Update ( float fElapsedTime ) 
{

	if (m_nTurnOrder == 0)
	{

	}
	else if (m_nTurnOrder == 1)
	{

	}


	if ( WinCheck() == true )
	{
		if (m_bPlayerOneWins == true)
		{
			CGame::GetInstance()->ChangeState( CWinGame::GetInstance() );
		}
		else if (m_bPlayerTwoWins == true)
		{
			CGame::GetInstance()->ChangeState( CLoseGame::GetInstance() );
		}
	}

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


	m_fTime += fElapsedTime;
}
void CArcadeGamePlay::Render ( void )			
{
	CSGD_TextureManager * pTM = CSGD_TextureManager::GetInstance();

	//pTM->Draw( m_nBackgroundID, 0, 0, 0.80f, 0.70f);

	m_cWorld->Render();

	//m_cPlayerOne->Render();

	//m_cPlayerTwo->Render();


	RenderBuildings();

	if(m_fCTimer >= 0.5f && m_fCTimer <= 1.0f)
	{
		m_psConstruction->Render();
	}

	if(m_fRTimer >= 0.5f && m_fRTimer <= 1.0f)
	{
		m_psRecruit->Render();
	}


	if(m_cCurrentTile->GetCurrent()->GetOwner() != 0)
	{
		RenderUnits();
	}



	CBitmapFont* pFont = CGame::GetInstance()->GetFont();

	//Render the Player Turn

	if (m_nTurnOrder == 0)
	{
		pFont->Draw( _T("Humans Turn"), 100, 0, 1.25f, D3DCOLOR_ARGB(255, 255, 0, 0) );

		PhaseDisplay();
	}
	else
	{
		pFont->Draw( _T("Undead Turn"), 100, 0, 1.25f, D3DCOLOR_ARGB(255, 0, 0, 255) );

		PhaseDisplay();
	}


	InterfaceRender();

	if(m_bBuildInterface && m_cCurrentTile->GetCurrent()->GetBuilding() == -1)
		BuildRender();

	if(m_bRecruitInterface)
		RecruitRender();





	//Resource Allocation at beginning of turn

	if(m_fTime <= 2.0f && m_nTurnNumber != 0)
	{
		TOSTRINGSTREAM f, w, s;

		f << food;
		w << wood;
		s << steel;

		RECT resource = {CGame::GetInstance()->GetScreenWidth()/2 -100, CGame::GetInstance()->GetScreenHeight()/2 -100,CGame::GetInstance()->GetScreenWidth()/2 +100,CGame::GetInstance()->GetScreenHeight()/2 +100};
		CSGD_Direct3D::GetInstance()->DrawRect(resource, D3DCOLOR_ARGB(200,0,0,0));

		pFont->Draw(_T("Resources this Turn"),CGame::GetInstance()->GetScreenWidth()/2 -100, CGame::GetInstance()->GetScreenHeight()/2 -100, 0.75f,D3DCOLOR_ARGB(255,255,255,255));

		pFont->Draw(_T("Food:"),CGame::GetInstance()->GetScreenWidth()/2 -100, CGame::GetInstance()->GetScreenHeight()/2 -50, 0.75f,D3DCOLOR_ARGB(255,255,255,255));
		pFont->Draw(f.str().c_str(),CGame::GetInstance()->GetScreenWidth()/2, CGame::GetInstance()->GetScreenHeight()/2 -50, 0.75f,D3DCOLOR_ARGB(255,255,255,255));

		pFont->Draw(_T("Wood:"),CGame::GetInstance()->GetScreenWidth()/2 -100, CGame::GetInstance()->GetScreenHeight()/2 , 0.75f,D3DCOLOR_ARGB(255,255,255,255));
		pFont->Draw(w.str().c_str(),CGame::GetInstance()->GetScreenWidth()/2, CGame::GetInstance()->GetScreenHeight()/2 , 0.75f,D3DCOLOR_ARGB(255,255,255,255));

		pFont->Draw(_T("Steel:"),CGame::GetInstance()->GetScreenWidth()/2 -100, CGame::GetInstance()->GetScreenHeight()/2 + 50, 0.75f,D3DCOLOR_ARGB(255,255,255,255));
		pFont->Draw(s.str().c_str(),CGame::GetInstance()->GetScreenWidth()/2, CGame::GetInstance()->GetScreenHeight()/2 + 50, 0.75f,D3DCOLOR_ARGB(255,255,255,255));


	}
	if (m_bAMMode == true)
	{

		RenderOptionsForMove();
	}

}
void CArcadeGamePlay::Enter ( void )		
{

	m_nBackgroundID			= -1;

	curX = 2;
	curY = 0;

	m_nBackgroundID =  CSGD_TextureManager::GetInstance()->LoadTexture( _T( "resources/graphics/WorldMap.png") );

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


	m_nResources = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("resources/sounds/allocation.wav"));

	m_nPlaceBuild = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("resources/sounds/placebuilding.wav"));

	m_nPlaceBuild = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("resources/sounds/placebuilding.wav"));
	m_nConquer =CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("resources/sounds/CountryTakeover.wav"));
	 


	m_cWorld = new CWorldGrid(0);

	m_cCurrentTile = new CWorldGrid(0);

	m_cPlayerOne = CPlayer::GetInstance();

	m_cPlayerTwo = CEnemy::GetInstance();

	m_nTurnOrder = 0;

	m_nWidth = m_cWorld->GetGrid()[2][0]->GetPosX();

	m_nHeight = m_cWorld->GetGrid()[2][0]->GetPosY();

	m_nInOptions = false;

	m_nCursor	 = 0;

	m_nBuildCursor = 0;

	m_bIsInWorld = true;

	m_bIsMoving = false;

	m_cCurrentTile->ChangeCurrent( m_cWorld->GetGrid()[2][0]);

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
	m_cCurrentTile->GetCurrent()->SetOwner( m_cWorld->GetGrid()[2][0]->GetOwner() );

	m_cCurrentTile->GetCurrent()->SetOccupied( m_cWorld->GetGrid()[2][0]->GetOccupied() );

	m_cCurrentTile->GetCurrent()->SetBuilding(-1);

	m_bPlayerOneWins = false;

	m_bPlayerTwoWins = false;

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
	m_cCurrentTile->GetCurrent()->SetCountry( m_cWorld->GetGrid()[2][0]->GetCountry());
	m_cCurrentTile->GetCurrent()->SetTerritory( m_cWorld->GetGrid()[2][0]->GetTerritory());

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

	m_bSelectTerritoryToMove = false;
	m_bAIEnabled = CMainMenu::GetInstance()->GetCampaign();


	m_bAMMode = false;

}
void CArcadeGamePlay::Exit ( void )	
{
	delete m_cWorld;
	delete m_cCurrentTile;

	delete m_psConstruction;
	delete m_psRecruit;
}
bool CArcadeGamePlay::Move ( int temp )
{

	switch (temp)
	{
	case 1:   //Move the cursor North and South
		{
			if ( m_cCurrentTile->GetCurrent()->GetPosY() == m_cWorld->GetGrid()[1][0]->GetPosY() && m_nWidth != m_cWorld->GetGrid()[0][0]->GetPosX())
			{
				if (m_cCurrentTile->GetCurrent()->GetPosX() == m_cWorld->GetGrid()[1][0]->GetPosX())
				{
					m_cCurrentTile->ChangeCurrent(  m_cWorld->GetGrid()[3][0] );
					m_nWidth = m_cWorld->GetGrid()[3][0]->GetPosX();
					m_nHeight = m_cWorld->GetGrid()[3][0]->GetPosY();
					m_cCurrentTile->GetCurrent()->SetOwner( m_cWorld->GetGrid()[3][0]->GetOwner() );
					m_cCurrentTile->GetCurrent()->SetOccupied( m_cWorld->GetGrid()[3][0]->GetOccupied() );
					m_cCurrentTile->GetCurrent()->SetBuilding(m_cWorld->GetGrid()[3][0]->GetBuilding());
					m_cCurrentTile->GetCurrent()->SetTerritory( m_cWorld->GetGrid()[3][0]->GetTerritory() );
					m_cCurrentTile->GetCurrent()->SetCountry( m_cWorld->GetGrid()[3][0]->GetCountry() );
					curX = 3;
				}
			}
			else if ( m_cCurrentTile->GetCurrent()->GetPosY() == m_cWorld->GetGrid()[0][0]->GetPosY() )
			{
				if (m_cCurrentTile->GetCurrent()->GetPosX() == m_cWorld->GetGrid()[0][0]->GetPosX())
				{
					m_cCurrentTile->ChangeCurrent(  m_cWorld->GetGrid()[2][0] );
					m_nWidth = m_cWorld->GetGrid()[2][0]->GetPosX();
					m_nHeight = m_cWorld->GetGrid()[2][0]->GetPosY();
					m_cCurrentTile->GetCurrent()->SetOwner( m_cWorld->GetGrid()[2][0]->GetOwner() );
					m_cCurrentTile->GetCurrent()->SetOccupied( m_cWorld->GetGrid()[2][0]->GetOccupied() );
					m_cCurrentTile->GetCurrent()->SetBuilding(m_cWorld->GetGrid()[2][0]->GetBuilding());
					m_cCurrentTile->GetCurrent()->SetTerritory( m_cWorld->GetGrid()[2][0]->GetTerritory() );
					m_cCurrentTile->GetCurrent()->SetCountry( m_cWorld->GetGrid()[2][0]->GetCountry() );
					curX = 2;

				}
			}
			else if ( m_cCurrentTile->GetCurrent()->GetPosY() == m_cWorld->GetGrid()[2][0]->GetPosY() && m_nWidth != m_cWorld->GetGrid()[3][0]->GetPosX())
			{
				if (m_cCurrentTile->GetCurrent()->GetPosX() == m_cWorld->GetGrid()[2][0]->GetPosX())
				{
					m_cCurrentTile->ChangeCurrent(  m_cWorld->GetGrid()[0][0]  );
					m_nWidth = m_cWorld->GetGrid()[0][0]->GetPosX();
					m_nHeight = m_cWorld->GetGrid()[0][0]->GetPosY();
					m_cCurrentTile->GetCurrent()->SetOwner( m_cWorld->GetGrid()[0][0]->GetOwner() );
					m_cCurrentTile->GetCurrent()->SetOccupied( m_cWorld->GetGrid()[0][0]->GetOccupied() );
					m_cCurrentTile->GetCurrent()->SetBuilding(m_cWorld->GetGrid()[0][0]->GetBuilding());
					m_cCurrentTile->GetCurrent()->SetTerritory( m_cWorld->GetGrid()[0][0]->GetTerritory() );
					m_cCurrentTile->GetCurrent()->SetCountry( m_cWorld->GetGrid()[0][0]->GetCountry() );
					curX = 0;

				}
			}
			else if ( m_cCurrentTile->GetCurrent()->GetPosY() == m_cWorld->GetGrid()[3][0]->GetPosY() )
			{
				if (m_cCurrentTile->GetCurrent()->GetPosX() == m_cWorld->GetGrid()[3][0]->GetPosX())
				{
					m_cCurrentTile->ChangeCurrent(  m_cWorld->GetGrid()[1][0] );
					m_nWidth = m_cWorld->GetGrid()[1][0]->GetPosX();
					m_nHeight = m_cWorld->GetGrid()[1][0]->GetPosY();
					m_cCurrentTile->GetCurrent()->SetOwner( m_cWorld->GetGrid()[1][0]->GetOwner() );
					m_cCurrentTile->GetCurrent()->SetOccupied( m_cWorld->GetGrid()[1][0]->GetOccupied() );
					m_cCurrentTile->GetCurrent()->SetBuilding(m_cWorld->GetGrid()[1][0]->GetBuilding());
					m_cCurrentTile->GetCurrent()->SetTerritory( m_cWorld->GetGrid()[1][0]->GetTerritory() );
					m_cCurrentTile->GetCurrent()->SetCountry( m_cWorld->GetGrid()[1][0]->GetCountry() );
					curX = 1;

				}
			}
		}
		break;
	case 2:   //Move the cursor East and West
		{
			if ( m_cCurrentTile->GetCurrent()->GetPosX() == m_cWorld->GetGrid()[1][0]->GetPosX() && m_nHeight != m_cWorld->GetGrid()[3][0]->GetPosY())
			{
				if (m_cCurrentTile->GetCurrent()->GetPosY() == m_cWorld->GetGrid()[1][0]->GetPosY())
				{
					m_cCurrentTile->ChangeCurrent(  m_cWorld->GetGrid()[0][0] );
					m_nWidth = m_cWorld->GetGrid()[0][0]->GetPosX();
					m_nHeight = m_cWorld->GetGrid()[0][0]->GetPosY();
					m_cCurrentTile->GetCurrent()->SetOwner( m_cWorld->GetGrid()[0][0]->GetOwner() );
					m_cCurrentTile->GetCurrent()->SetOccupied( m_cWorld->GetGrid()[0][0]->GetOccupied() );
					m_cCurrentTile->GetCurrent()->SetBuilding(m_cWorld->GetGrid()[0][0]->GetBuilding());
					m_cCurrentTile->GetCurrent()->SetTerritory( m_cWorld->GetGrid()[0][0]->GetTerritory() );
					m_cCurrentTile->GetCurrent()->SetCountry( m_cWorld->GetGrid()[0][0]->GetCountry() );
					curX = 0;

				}
			}
			else if ( m_cCurrentTile->GetCurrent()->GetPosX() == m_cWorld->GetGrid()[0][0]->GetPosX() && m_nHeight != m_cWorld->GetGrid()[2][0]->GetPosY())
			{
				if (m_cCurrentTile->GetCurrent()->GetPosY() == m_cWorld->GetGrid()[0][0]->GetPosY())
				{
					m_cCurrentTile->ChangeCurrent(  m_cWorld->GetGrid()[1][0] );
					m_nWidth = m_cWorld->GetGrid()[1][0]->GetPosX();
					m_nHeight = m_cWorld->GetGrid()[1][0]->GetPosY();
					m_cCurrentTile->GetCurrent()->SetOwner( m_cWorld->GetGrid()[1][0]->GetOwner() );
					m_cCurrentTile->GetCurrent()->SetOccupied( m_cWorld->GetGrid()[1][0]->GetOccupied() );
					m_cCurrentTile->GetCurrent()->SetBuilding(m_cWorld->GetGrid()[1][0]->GetBuilding());
					m_cCurrentTile->GetCurrent()->SetTerritory( m_cWorld->GetGrid()[1][0]->GetTerritory() );
					m_cCurrentTile->GetCurrent()->SetCountry( m_cWorld->GetGrid()[1][0]->GetCountry() );
					curX = 1;
				}
			}
			else if ( m_cCurrentTile->GetCurrent()->GetPosX() == m_cWorld->GetGrid()[2][0]->GetPosX() )
			{
				if (m_cCurrentTile->GetCurrent()->GetPosY() == m_cWorld->GetGrid()[2][0]->GetPosY())
				{
					m_cCurrentTile->ChangeCurrent(  m_cWorld->GetGrid()[3][0] );
					m_nWidth = m_cWorld->GetGrid()[3][0]->GetPosX();
					m_nHeight = m_cWorld->GetGrid()[3][0]->GetPosY();
					m_cCurrentTile->GetCurrent()->SetOwner( m_cWorld->GetGrid()[3][0]->GetOwner() );
					m_cCurrentTile->GetCurrent()->SetOccupied( m_cWorld->GetGrid()[3][0]->GetOccupied() );
					m_cCurrentTile->GetCurrent()->SetBuilding(m_cWorld->GetGrid()[3][0]->GetBuilding());
					m_cCurrentTile->GetCurrent()->SetTerritory( m_cWorld->GetGrid()[3][0]->GetTerritory() );
					m_cCurrentTile->GetCurrent()->SetCountry( m_cWorld->GetGrid()[3][0]->GetCountry() );
					curX = 3;
				}
			}
			else if ( m_cCurrentTile->GetCurrent()->GetPosX() == m_cWorld->GetGrid()[3][0]->GetPosX() )
			{
				if (m_cCurrentTile->GetCurrent()->GetPosY() == m_cWorld->GetGrid()[3][0]->GetPosY())
				{
					m_cCurrentTile->ChangeCurrent(  m_cWorld->GetGrid()[2][0] );
					m_nWidth = m_cWorld->GetGrid()[2][0]->GetPosX();
					m_nHeight = m_cWorld->GetGrid()[2][0]->GetPosY();
					m_cCurrentTile->GetCurrent()->SetOwner( m_cWorld->GetGrid()[2][0]->GetOwner() );
					m_cCurrentTile->GetCurrent()->SetOccupied( m_cWorld->GetGrid()[2][0]->GetOccupied() );
					m_cCurrentTile->GetCurrent()->SetBuilding(m_cWorld->GetGrid()[2][0]->GetBuilding());
					m_cCurrentTile->GetCurrent()->SetTerritory( m_cWorld->GetGrid()[2][0]->GetTerritory() );
					m_cCurrentTile->GetCurrent()->SetCountry( m_cWorld->GetGrid()[2][0]->GetCountry() );
					curX = 2;
				}
			}
		}
		break;

	default:
		break;
	}
	


	return true;

}
void CArcadeGamePlay::Build ( CBasePlayer* TempPlayer )
{
	m_bBuildInterface = true;

	if(m_cCurrentTile->GetCurrent()->GetBuilding() == BARRACKS || m_cCurrentTile->GetCurrent()->GetBuilding() == WARFACTORY)
	{
		m_bRecruitInterface = true;
		m_bBuildInterface = false;
	}
}
void CArcadeGamePlay::CheckIsBuilt ( int count )
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
void CArcadeGamePlay::BuildPhase ( void )
{
	if (m_nTurnOrder == 0)
	{
		CheckIsBuilt( 1 );
	}
	else if (m_nTurnOrder == 1)
	{
		CheckIsBuilt( 2 );
	}


}
void CArcadeGamePlay::PhaseDisplay ( void )
{

	CBitmapFont* pFont = CGame::GetInstance()->GetFont();
	int red;
	int blue;
	if (m_nTurnOrder == 0)
	{
		red = 255;
		blue = 0;
	}
	else if (m_nTurnOrder == 1)
	{
		red = 0;
		blue = 255;
	}
	if (m_bBuildMode)
	{
		pFont->Draw( _T("Build Phase"), 100, 30, 1.25f, D3DCOLOR_ARGB(255, red, 0, blue) );
		pFont->Draw( _T("End Build Phase"), (800 - (4*32)), 25, 0.75f, D3DCOLOR_ARGB(255, 255, 0, 0) );
	}
	else if (m_bMoveandAttackMode)
	{
		pFont->Draw( _T("Action Phase"), 100, 30, 1.25f, D3DCOLOR_ARGB(255, red, 0, blue) );
		pFont->Draw( _T("End Action Phase"), (800 - (4*32)), 25, 0.75f, D3DCOLOR_ARGB(255, 255, 0, 0) );
	}
	else if (m_bMoveandAttackMode == false)
	{
		pFont->Draw( _T("Press End Turn"), 100, 30, 1.25f, D3DCOLOR_ARGB(255, red, 0, blue) );
		pFont->Draw( _T("End Turn"), (800 - (4*32)) , 25, 0.75f, D3DCOLOR_ARGB(255, 255, 0, 0) );
	}

	if (m_nTurnCap == 10)
	{
		TOSTRINGSTREAM TurnCap;
		TurnCap << m_nTurnCap;

		pFont->Draw( TurnCap.str().c_str(),  225, 60, 1.25f, D3DCOLOR_ARGB( 255, 255, 0, 0 ) ); 

		TOSTRINGSTREAM CurrentTurn;
		CurrentTurn << m_nTurnNumber;

		pFont->Draw( CurrentTurn.str().c_str(),  175, 60, 1.25f, D3DCOLOR_ARGB( 255, 255, 0, 0 ) ); 

		pFont->Draw( _T("Turn: "), 100, 60, 1.25f, D3DCOLOR_ARGB(255, 255, 0, 0) );
		pFont->Draw( _T("/"), 200, 60, 1.25f, D3DCOLOR_ARGB(255, 255, 0, 0) );
	}

}
void CArcadeGamePlay::InterfaceRender ( void )
{

	CBitmapFont* pFont = CGame::GetInstance()->GetFont();

	//Render the interface
	if (m_bMoveandAttackMode == false && (m_cCurrentTile->GetCurrent()->GetBuilding() != CAPITOL && m_cCurrentTile->GetCurrent()->GetBuilding() != -1))
	{
		//pFont->Draw( _T("Move"), (800 - (8*32)), 0, 0.75f, D3DCOLOR_ARGB(m_nGreyedOut, 255, 0, 0) );
		//pFont->Draw( _T("Attack"), (800 - (8*32)), 25, 0.75f, D3DCOLOR_ARGB(m_nGreyedOut, 255, 0, 0) );
		pFont->Draw( _T("Sell"), (800 - (8*32)), 25, 0.75f, D3DCOLOR_ARGB(255, 255, 0, 0) );

	}
	else if( m_bMoveandAttackMode == true )
	{
		pFont->Draw( _T("Move"), (800 - (8*32)), 0, 0.75f, D3DCOLOR_ARGB(255, 255, 0, 0) );
		pFont->Draw( _T("Attack"), (800 - (8*32)), 25, 0.75f, D3DCOLOR_ARGB(255, 255, 0, 0) );
	}
	/*if (m_bBuildMode == false)
	{
	pFont->Draw( _T("Build"), (800 - (4 *32)), 0, 0.75f, D3DCOLOR_ARGB(m_nGreyedOut, 255, 0, 0) );
	}
	else*/ if( m_bBuildMode == true && m_cCurrentTile->GetCurrent()->GetBuilding() == -1 && (m_cCurrentTile->GetCurrent()->GetOwner() == 1 || m_cCurrentTile->GetCurrent()->GetOwner() == 2) )
	{
		pFont->Draw( _T("Build"), (800 - (4 *32)), 0, 0.75f, D3DCOLOR_ARGB(255, 255, 0, 0) );
	}
	else if( m_bBuildMode == true && (m_cCurrentTile->GetCurrent()->GetBuilding() != -1 && (m_cCurrentTile->GetCurrent()->GetBuilding() == BARRACKS || m_cCurrentTile->GetCurrent()->GetBuilding() == WARFACTORY)))
	{
		pFont->Draw( _T("Recruit"), (800 - (4 *32)), 0, 0.75f, D3DCOLOR_ARGB(255, 255, 0, 0) );
	}





	if (m_nInOptions == true && m_bBuildInterface == false && m_bRecruitInterface == false)
	{
		pFont->Draw( _T("->"), m_nWidth, 0 + (m_nCursor * 25), 1.0f, D3DCOLOR_ARGB(255, 255, 0, 0) );
	}
	else if(m_nInOptions == false)
	{
		pFont->Draw( _T("|||"), m_nWidth, m_nHeight - 25, 1.0f, D3DCOLOR_ARGB(255, 255, 0, 0) );

	}
	else if(m_bBuildInterface == true)
	{
		if(m_nBuildCursor != 5)
			pFont->Draw( _T("->"), 600, 125 + (m_nBuildCursor * 100), 1.0f, D3DCOLOR_ARGB(255, 255, 0, 0) );
		else if(m_nBuildCursor == 5)
		{
			pFont->Draw( _T("->"), 600, 577, 1.0f, D3DCOLOR_ARGB(255, 255, 0, 0) );
		}
	}
	else if(m_bRecruitInterface == true)
	{
		if(m_nBuildCursor != 3)
			pFont->Draw( _T("->"), 600, 125 + (m_nBuildCursor * 150), 1.0f, D3DCOLOR_ARGB(255, 255, 0, 0) );
		else if(m_nBuildCursor == 3)
		{
			pFont->Draw( _T("->"), 600, 577, 1.0f, D3DCOLOR_ARGB(255, 255, 0, 0) );
		}
	}



	RECT UnitsLists = { 200, 500, 600, 598 };  

	CSGD_Direct3D::GetInstance()->DrawHollowRect( UnitsLists, D3DCOLOR_ARGB( 255, 0, 0, 0 ) );
	//CSGD_Direct3D::GetInstance()->DrawRect(UnitsLists, D3DCOLOR_ARGB(155,0,0,0));

	pFont->Draw( _T("Food: "), (800 / 2 - ( 2 * 32 ) ), 0, 0.75f, D3DCOLOR_XRGB( 0, 255, 0 ) );
	pFont->Draw( _T("Wood: "), (800 / 2 - ( 2 * 32 ) ), 25, 0.75f, D3DCOLOR_XRGB( 0, 255, 0 ) );
	pFont->Draw( _T("Steel: "), (800 / 2 - ( 2 * 32 ) ), 50, 0.75f, D3DCOLOR_XRGB( 0, 255, 0 ) );

	if (m_nTurnOrder == 0)
	{
		TOSTRINGSTREAM FoodResource;
		FoodResource << m_cPlayerOne->GetInstance()->GetFood();

		pFont->Draw( FoodResource.str().c_str(),  (800 / 2 + (  32 ) ), 0, 0.75f, D3DCOLOR_ARGB( 255, 0, 0, 255 ) ); 

		TOSTRINGSTREAM WoodResource;
		WoodResource << m_cPlayerOne->GetInstance()->GetWood();

		pFont->Draw( WoodResource.str().c_str(),  (800 / 2 + (  32 ) ), 25, 0.75f, D3DCOLOR_ARGB( 255, 0, 0, 255 ) ); 

		TOSTRINGSTREAM SteelResource;
		SteelResource << m_cPlayerOne->GetInstance()->GetSteel();

		pFont->Draw( SteelResource.str().c_str(),  (800 / 2 + (  32 ) ), 50, 0.75f, D3DCOLOR_ARGB( 255, 0, 0, 255 ) ); 

	}
	else
	{
		TOSTRINGSTREAM FoodResource;
		FoodResource << m_cPlayerTwo->GetInstance()->GetFood();

		pFont->Draw( FoodResource.str().c_str(),  (800 / 2 + (  32 ) ), 0, 0.75f, D3DCOLOR_ARGB( 255, 0, 0, 255 ) ); 

		TOSTRINGSTREAM WoodResource;
		WoodResource << m_cPlayerTwo->GetInstance()->GetWood();

		pFont->Draw( WoodResource.str().c_str(),  (800 / 2 + (  32 ) ), 25, 0.75f, D3DCOLOR_ARGB( 255, 0, 0, 255 ) ); 

		TOSTRINGSTREAM SteelResource;
		SteelResource << m_cPlayerTwo->GetInstance()->GetSteel();

		pFont->Draw( SteelResource.str().c_str(),  (800 / 2 + (  32 ) ), 50, 0.75f, D3DCOLOR_ARGB( 255, 0, 0, 255 ) ); 
	}
}
bool CArcadeGamePlay::HandleEnterKey ( void )
{

	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();


	//Handling all options in the top menu
	if( pDI->JoystickButtonPressed( 1, JOYSTICKID1 ) == true && (m_nInOptions == true && m_bBuildInterface == false && m_bRecruitInterface == false))
	{

		if ( m_nWidth == 485 && m_nCursor == 0 )   // move
		{

			if (m_nTurnOrder == 0 && m_cCurrentTile->GetCurrent()->GetOwner() == 1)
			{

				m_nMovingFromX =  m_cCurrentTile->GetCurrent()->GetCountry();
				m_nMovingFromY = m_cCurrentTile->GetCurrent()->GetTerritory();
				m_nWidth = m_cWorld->GetGrid()[m_nMovingFromX][m_nMovingFromY]->GetPosX();
				m_nHeight = m_cWorld->GetGrid()[m_nMovingFromX][m_nMovingFromY]->GetPosY();
				m_nInOptions = false;
				m_bIsMoving = true;
				m_cCurrentTile->ChangeCurrent( m_cWorld->GetGrid()[m_nMovingFromX][m_nMovingFromY] );
				curX = m_nMovingFromX;
				m_bSelectTerritoryToMove = true;
				return true;
			}
			else if( m_nTurnOrder == 1 && m_cCurrentTile->GetCurrent()->GetOwner() == 2)
			{
				m_nMovingFromX =  m_cCurrentTile->GetCurrent()->GetCountry();
				m_nMovingFromY = m_cCurrentTile->GetCurrent()->GetTerritory();
				m_nWidth = m_cWorld->GetGrid()[m_nMovingFromX][m_nMovingFromY]->GetPosX();
				m_nHeight = m_cWorld->GetGrid()[m_nMovingFromX][m_nMovingFromY]->GetPosY();
				m_nInOptions = false;
				m_bIsMoving = true;
				m_cCurrentTile->ChangeCurrent( m_cWorld->GetGrid()[m_nMovingFromX][m_nMovingFromY]  );
				curX = m_nMovingFromX;
				m_bSelectTerritoryToMove = true;
				return true;

			}
		}
		else if (m_nWidth == 512 && m_nCursor == 1 && ( m_cCurrentTile->GetCurrent()->GetBuilding() != CAPITOL || m_cCurrentTile->GetCurrent()->GetBuilding() != -1 ))
		{
			SellBuilding();
		}
		else if ((m_nCursor == 0 && m_nWidth == 640) && m_cCurrentTile->GetCurrent()->GetBuilding() == -1 ) //Build
		{

			if (m_nTurnOrder == 0)
			{
				Build( m_cPlayerOne );
				return true;
			}
			else
			{
				Build( m_cPlayerTwo );
				return true;
			}

		}
		else if((m_nCursor == 0 && m_nWidth == 640) && (m_cCurrentTile->GetCurrent()->GetBuilding() == BARRACKS || m_cCurrentTile->GetCurrent()->GetBuilding() == WARFACTORY))
		{
			if (m_nTurnOrder == 0)
			{
				Build( m_cPlayerOne );
				return true;
			}
			else
			{
				Build( m_cPlayerTwo );
				return true;
			}
		}

		else if ( m_bBuildMode == false && m_bMoveandAttackMode == false && m_nWidth == 640 && m_nCursor == 1 ) //Handle End Turn
		{
			if (m_nTurnOrder == 0)
			{
				m_nTurnOrder = 1;
				m_bBuildMode = true;
				m_nInOptions = false;
				m_nWidth = m_cWorld->GetGrid()[1][0]->GetPosX();
				m_nHeight = m_cWorld->GetGrid()[1][0]->GetPosY();
				m_cCurrentTile->ChangeCurrent( m_cWorld->GetGrid()[1][0]);
				m_cCurrentTile->GetCurrent()->SetOwner( m_cWorld->GetGrid()[1][0]->GetOwner() );
				m_cCurrentTile->GetCurrent()->SetOccupied( m_cWorld->GetGrid()[1][0]->GetOccupied() );
				m_cCurrentTile->GetCurrent()->SetBuilding(m_cWorld->GetGrid()[1][0]->GetBuilding());
				m_cCurrentTile->GetCurrent()->SetCountry(m_cWorld->GetGrid()[1][0]->GetCountry() );
				m_cCurrentTile->GetCurrent()->SetTerritory(m_cWorld->GetGrid()[1][0]->GetTerritory() );
				curX = 1;
				AllocateResources();
				m_cPlayerOne->SetMoved ( false  ) ;

				return true;
			}
			else
			{
				m_nTurnOrder = 0;
				m_bBuildMode = true;
				m_nInOptions = false;
				m_nWidth = m_cWorld->GetGrid()[2][0]->GetPosX();
				m_nHeight = m_cWorld->GetGrid()[2][0]->GetPosY();
				m_cCurrentTile->ChangeCurrent( m_cWorld->GetGrid()[2][0]); 
				m_cCurrentTile->GetCurrent()->SetOwner( m_cWorld->GetGrid()[2][0]->GetOwner() );
				m_cCurrentTile->GetCurrent()->SetOccupied( m_cWorld->GetGrid()[2][0]->GetOccupied() );
				m_cCurrentTile->GetCurrent()->SetBuilding(m_cWorld->GetGrid()[2][0]->GetBuilding());
				m_cCurrentTile->GetCurrent()->SetCountry(m_cWorld->GetGrid()[2][0]->GetCountry() );
				m_cCurrentTile->GetCurrent()->SetTerritory(m_cWorld->GetGrid()[2][0]->GetTerritory() );
				m_cPlayerTwo->SetMoved ( false  ) ;
				m_nTurnNumber++;
				curX = 2;
				AllocateResources();
				return true;
			}
		}
		else if (m_bMoveandAttackMode == true && m_bBuildMode == false && m_nWidth == 640 && m_nCursor == 1 )  //Handle End attack Phase
		{
			m_bMoveandAttackMode = false;
			return true;
		}
		else if ( m_bBuildMode == true && m_nWidth != 512 && m_nCursor == 1 )  //Handle Skip Build phase
		{
			if (m_nTurnOrder == 0)
			{
				m_nWidth = m_cWorld->GetGrid()[2][0]->GetPosX();
				m_nHeight = m_cWorld->GetGrid()[2][0]->GetPosY();
				m_nInOptions = false;
				m_bBuildMode = false;
				m_cCurrentTile->ChangeCurrent( m_cWorld->GetGrid()[2][0] );
				m_bMoveandAttackMode = true;
				curX = 2;
				return true;
			}
			else
			{
				m_nWidth = m_cWorld->GetGrid()[1][0]->GetPosX();
				m_nHeight = m_cWorld->GetGrid()[1][0]->GetPosY();
				m_nInOptions = false;
				m_bBuildMode = false;
				m_cCurrentTile->ChangeCurrent( m_cWorld->GetGrid()[1][0] );
				m_bMoveandAttackMode = true;
				curX = 1;
				return true;
			}
		}


	}

	//Moving and atttacking on the world map
	if( pDI->JoystickButtonPressed( 1, JOYSTICKID1 ) == true  && (m_nInOptions == false && m_bBuildInterface == false && m_bRecruitInterface == false))
	{		




		if (m_cCurrentTile->GetCurrent()->GetOwner() == 1 && m_nTurnOrder == 0 && m_bBuildMode == true)
		{

			if (m_bBuildMode == true && m_cCurrentTile->GetCurrent()->GetBuilding() != CAPITOL && m_cCurrentTile->GetCurrent()->GetBuilding() == -1)
			{
				m_nWidth = (800 - (5 * 32 ) );
				m_nCursor = 0;
				m_nInOptions = true;
				return true;
			}
			else if (m_bBuildMode == true && (m_cCurrentTile->GetCurrent()->GetBuilding() == CAPITOL || m_cCurrentTile->GetCurrent()->GetBuilding() != BARRACKS || m_cCurrentTile->GetCurrent()->GetBuilding() != WARFACTORY ))
			{
				m_nWidth = (800 - (5 * 32 ) );
				m_nCursor = 1;
				m_nInOptions = true;
				return true;
			}


		}
		else if (m_cCurrentTile->GetCurrent()->GetOwner() == 2 && m_nTurnOrder == 1 && m_bBuildMode == true)
		{

			if (m_bBuildMode == true && m_cCurrentTile->GetCurrent()->GetBuilding() != CAPITOL && m_cCurrentTile->GetCurrent()->GetBuilding() == -1)
			{
				m_nWidth = (800 - (5 * 32 ) );
				m_nCursor = 0;
				m_nInOptions = true;
				return true;
			}
			else if (m_bBuildMode == true && (m_cCurrentTile->GetCurrent()->GetBuilding() == CAPITOL || m_cCurrentTile->GetCurrent()->GetBuilding() != BARRACKS || m_cCurrentTile->GetCurrent()->GetBuilding() != WARFACTORY ))
			{
				m_nWidth = (800 - (5 * 32 ) );
				m_nCursor = 1;
				m_nInOptions = true;
				return true;
			}


		}
		else if	( m_cCurrentTile->GetCurrent()->GetOwner() == 1 && m_cCurrentTile->GetCurrent()->GetOccupied() == true && m_bBuildMode == false && m_nTurnOrder == 0)  //move/attack
		{
			m_nInOptions = true;
			m_nWidth = (800 - ( 9 * 35 ) );
			m_nCursor = 0;
			return true;
		}
		else if	( m_cCurrentTile->GetCurrent()->GetOwner() == 2 && m_cCurrentTile->GetCurrent()->GetOccupied() == true && m_bBuildMode == false && m_nTurnOrder == 1 )  //move/attack
		{
			m_nInOptions = true;
			m_nWidth = (800 - ( 9 * 35 ) );
			m_nCursor = 0;
			return true;
		}
		else if (m_bSelectTerritoryToMove == true && m_nTurnOrder == 0)
		{
			 CheckMovement();			
		}

		else if (m_bSelectTerritoryToMove == true && m_nTurnOrder == 1)
		{
			 CheckMovement();			
		}


	}

	//Build interface
	if( pDI->JoystickButtonPressed( 1, JOYSTICKID1 ) == true  && (m_nInOptions == true && m_bBuildInterface == true && m_bRecruitInterface == false))
	{		
		int x = m_cCurrentTile->GetCurrent()->GetPosX();
		int y = m_cCurrentTile->GetCurrent()->GetPosY();


		int wood, steel;



		if(m_cCurrentTile->GetCurrent()->GetOwner() == 1)
		{
			wood = m_cPlayerOne->GetWood();
			steel = m_cPlayerOne->GetSteel();

			switch(m_nBuildCursor)
			{
			case 0:			//Farm
				if(wood >= 20)
				{
					m_cPlayerOne->AddBuilding(new Farm(1,x,y));
					m_cPlayerOne->SetWood(wood-20);

					m_cWorld->GetGrid()[curX][0]->SetBuilding(FARM);

					CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nPlaceBuild);

					m_fCTimer = 0.5f;
					m_psConstruction->GetEffect(0)->KillParticles();
					m_psConstruction->GetEffect(0)->SetEmitterPosX((float)m_cWorld->GetGrid()[curX][0]->GetPosX()-32);
					m_psConstruction->GetEffect(0)->SetEmitterPosY((float)m_cWorld->GetGrid()[curX][0]->GetPosY()+32);

				}
				break;
			case 1:			//Mine
				if(wood >= 15 && steel >= 10)
				{
					m_cPlayerOne->AddBuilding(new Mine(1,x,y));
					m_cPlayerOne->SetWood(wood-15);
					m_cPlayerOne->SetSteel(steel-10);
					m_cWorld->GetGrid()[curX][0]->SetBuilding(MINE);

					CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nPlaceBuild);

					m_fCTimer = 0.5f;
					m_psConstruction->GetEffect(0)->KillParticles();
					m_psConstruction->GetEffect(0)->SetEmitterPosX((float)m_cWorld->GetGrid()[curX][0]->GetPosX()-32);
					m_psConstruction->GetEffect(0)->SetEmitterPosY((float)m_cWorld->GetGrid()[curX][0]->GetPosY()+32);

				}
				break;
			case 2:			//Lumber Mill
				if(wood >= 20 && steel >= 5)
				{
					m_cPlayerOne->AddBuilding(new WoodMill(1,x,y));
					m_cPlayerOne->SetWood(wood-20);
					m_cPlayerOne->SetSteel(steel-5);
					m_cWorld->GetGrid()[curX][0]->SetBuilding(WOOD);
					CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nPlaceBuild);

					m_fCTimer = 0.5f;
					m_psConstruction->GetEffect(0)->KillParticles();
					m_psConstruction->GetEffect(0)->SetEmitterPosX((float)m_cWorld->GetGrid()[curX][0]->GetPosX()-32);
					m_psConstruction->GetEffect(0)->SetEmitterPosY((float)m_cWorld->GetGrid()[curX][0]->GetPosY()+32);


				}
				break;
			case 3:			//Barracks
				if(wood >= 30 && steel >= 10)
				{
					m_cPlayerOne->AddBuilding(new Barracks(1,x,y));
					m_cPlayerOne->SetWood(wood-30);
					m_cPlayerOne->SetSteel(steel-10);
					m_cWorld->GetGrid()[curX][0]->SetBuilding(BARRACKS);

					CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nPlaceBuild);

					m_fCTimer = 0.5f;
					m_psConstruction->GetEffect(0)->KillParticles();
					m_psConstruction->GetEffect(0)->SetEmitterPosX((float)m_cWorld->GetGrid()[curX][0]->GetPosX()-32);
					m_psConstruction->GetEffect(0)->SetEmitterPosY((float)m_cWorld->GetGrid()[curX][0]->GetPosY()+32);

				}
				break;
			case 4:			//War Factory
				if(wood >= 20 && steel >= 40)
				{
					m_cPlayerOne->AddBuilding(new WarFactory(1,x,y));
					m_cPlayerOne->SetWood(wood-20);
					m_cPlayerOne->SetSteel(steel-40);
					m_cWorld->GetGrid()[curX][0]->SetBuilding(WARFACTORY);

					CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nPlaceBuild);

					m_fCTimer = 0.5f;
					m_psConstruction->GetEffect(0)->KillParticles();
					m_psConstruction->GetEffect(0)->SetEmitterPosX((float)m_cWorld->GetGrid()[curX][0]->GetPosX()-32);
					m_psConstruction->GetEffect(0)->SetEmitterPosY((float)m_cWorld->GetGrid()[curX][0]->GetPosY()+32);

				}
				break;
			case 5:			//Exit
				m_bBuildInterface = false;
				m_nBuildCursor = 0;
				break;
			}
		}
		else if(m_cCurrentTile->GetCurrent()->GetOwner() == 2)
		{
			wood = m_cPlayerTwo->GetWood();
			steel = m_cPlayerTwo->GetSteel();

			switch(m_nBuildCursor)
			{
			case 0:			//Farm
				if(wood >= 20)
				{
					m_cPlayerTwo->AddBuilding(new Farm(2,x,y));
					m_cPlayerTwo->SetWood(wood-20);
					m_cWorld->GetGrid()[curX][0]->SetBuilding(FARM);
					CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nPlaceBuild);

					m_fCTimer = 0.5f;
					m_psConstruction->GetEffect(0)->KillParticles();
					m_psConstruction->GetEffect(0)->SetEmitterPosX((float)m_cWorld->GetGrid()[curX][0]->GetPosX()-32);
					m_psConstruction->GetEffect(0)->SetEmitterPosY((float)m_cWorld->GetGrid()[curX][0]->GetPosY()+32);

				}
				break;
			case 1:			//Mine
				if(wood >= 15 && steel >= 10)
				{
					m_cPlayerTwo->AddBuilding(new Mine(2,x,y));
					m_cPlayerTwo->SetWood(wood-15);
					m_cPlayerTwo->SetSteel(steel-10);
					m_cWorld->GetGrid()[curX][0]->SetBuilding(MINE);
					CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nPlaceBuild);

					m_fCTimer = 0.5f;
					m_psConstruction->GetEffect(0)->KillParticles();
					m_psConstruction->GetEffect(0)->SetEmitterPosX((float)m_cWorld->GetGrid()[curX][0]->GetPosX()-32);
					m_psConstruction->GetEffect(0)->SetEmitterPosY((float)m_cWorld->GetGrid()[curX][0]->GetPosY()+32);

				}

				break;
			case 2:			//Lumber Mill
				if(wood >= 20 && steel >= 5)
				{
					m_cPlayerTwo->AddBuilding(new WoodMill(2,x,y));
					m_cPlayerTwo->SetWood(wood-20);
					m_cPlayerTwo->SetSteel(steel-5);
					m_cWorld->GetGrid()[curX][0]->SetBuilding(FARM);
					CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nPlaceBuild);

					m_fCTimer = 0.5f;
					m_psConstruction->GetEffect(0)->KillParticles();
					m_psConstruction->GetEffect(0)->SetEmitterPosX((float)m_cWorld->GetGrid()[curX][0]->GetPosX()-32);
					m_psConstruction->GetEffect(0)->SetEmitterPosY((float)m_cWorld->GetGrid()[curX][0]->GetPosY()+32);

				}

				break;
			case 3:			//Barracks
				if(wood >= 30 && steel >= 10)
				{
					m_cPlayerTwo->AddBuilding(new Barracks(2,x,y));
					m_cPlayerTwo->SetWood(wood-30);
					m_cPlayerTwo->SetSteel(steel-10);
					m_cWorld->GetGrid()[curX][0]->SetBuilding(BARRACKS);
					CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nPlaceBuild);

					m_fCTimer = 0.5f;
					m_psConstruction->GetEffect(0)->KillParticles();
					m_psConstruction->GetEffect(0)->SetEmitterPosX((float)m_cWorld->GetGrid()[curX][0]->GetPosX()-32);
					m_psConstruction->GetEffect(0)->SetEmitterPosY((float)m_cWorld->GetGrid()[curX][0]->GetPosY()+32);

				}

				break;
			case 4:			//War Factory
				if(wood >= 20 && steel >= 40)
				{
					m_cPlayerTwo->AddBuilding(new WarFactory(2,x,y));
					m_cPlayerTwo->SetWood(wood-20);
					m_cPlayerTwo->SetSteel(steel-40);
					m_cWorld->GetGrid()[curX][0]->SetBuilding(WARFACTORY);
					CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nPlaceBuild);

					m_fCTimer = 0.5f;
					m_psConstruction->GetEffect(0)->KillParticles();
					m_psConstruction->GetEffect(0)->SetEmitterPosX((float)m_cWorld->GetGrid()[curX][0]->GetPosX()-32);
					m_psConstruction->GetEffect(0)->SetEmitterPosY((float)m_cWorld->GetGrid()[curX][0]->GetPosY()+32);

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


	//Recruit Interface
	if(  pDI->JoystickButtonPressed( 1, JOYSTICKID1 ) == true  && (m_nInOptions == true && m_bBuildInterface == false && m_bRecruitInterface == true))
	{
		int x = m_cCurrentTile->GetCurrent()->GetPosX();
		int y = m_cCurrentTile->GetCurrent()->GetPosY();


		int food, wood, steel;



		if(m_cCurrentTile->GetCurrent()->GetOwner() == 1)
		{
			food = m_cPlayerOne->GetFood();
			wood = m_cPlayerOne->GetWood();
			steel = m_cPlayerOne->GetSteel();

			if(m_cCurrentTile->GetCurrent()->GetBuilding() == BARRACKS)
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
			else if(m_cCurrentTile->GetCurrent()->GetBuilding()== WARFACTORY)
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
		else if(m_cCurrentTile->GetCurrent()->GetOwner() == 2)
		{
			food = m_cPlayerTwo->GetFood();
			wood = m_cPlayerTwo->GetWood();
			steel = m_cPlayerTwo->GetSteel();

			if(m_cCurrentTile->GetCurrent()->GetBuilding() == BARRACKS)
			{
				switch(m_nBuildCursor)
				{
				case 0:			//Foot Soldier
					if(food >= 10)
					{
						CBaseUnit* foot = new CBaseUnit();
						foot->CreateUnit(SKELETALWARRIOR,curX, curY);
						m_cPlayerTwo->AddUnit(foot);
						m_cPlayerTwo->SetFood(food -10);

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
			else if(m_cCurrentTile->GetCurrent()->GetBuilding()== WARFACTORY)
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

	return false;



}
bool CArcadeGamePlay::WinCheck ( void ) 
{
	for (unsigned int i = 0; i < m_cWorld->GetGrid().size(); i++)
	{
		for (unsigned int j = 0; j < m_cWorld->GetGrid()[i].size(); j++)
		{
			if (m_cWorld->GetGrid()[i][j]->GetOwner() == 1)
			{
				m_bPlayerOneWins = true;
			}
			else
			{
				m_bPlayerOneWins = false;
			}
		}
	}
	if (m_bPlayerOneWins == false)
	{
		for (unsigned int i = 0; i < m_cWorld->GetGrid().size(); i++)
		{
			for (unsigned int j = 0; j < m_cWorld->GetGrid()[i].size(); j++)
			{
				if (m_cWorld->GetGrid()[i][j]->GetOwner() == 2)
				{
					m_bPlayerTwoWins = true;
				}
				else
				{
					m_bPlayerTwoWins = false;
					return false;
				}

			}
		}
	}
	if ( m_nTurnNumber == m_nTurnCap )
	{
		int playeroneowned = 0;
		int playertwoowned = 0;
		for (unsigned int i = 0; i < m_cWorld->GetGrid().size(); i++)
		{
			for (unsigned int j = 0; j < m_cWorld->GetGrid()[i].size(); j++)
			{
				if (m_cWorld->GetGrid()[i][j]->GetOwner() == 2)
				{
					playertwoowned++;
				}				
			}
		}
		for (unsigned int i = 0; i < m_cWorld->GetGrid().size(); i++)
		{
			for (unsigned int j = 0; j < m_cWorld->GetGrid()[i].size(); j++)
			{
				if (m_cWorld->GetGrid()[i][j]->GetOwner() == 1)
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
	return true;
}
void CArcadeGamePlay::BuildRender(void)
{
	CBitmapFont* pFont = CGame::GetInstance()->GetFont();

	pFont->Draw(_T("Select A Building:"),635,45,1.0f,D3DCOLOR_ARGB(255,0,0,255));

	RECT main = {625,75,800,600};
	CSGD_Direct3D::GetInstance()->DrawRect(main,D3DCOLOR_ARGB(100,0,0,0));

	for(int i = 0; i < 5; i++)
	{
		RECT border = {625,75+(i*100),800,175 +(i*100)};
		CSGD_Direct3D::GetInstance()->DrawHollowRect(border,D3DCOLOR_ARGB(255,0,0,0),2);
	}

	RECT ex = {625,575,800,648};
	CSGD_Direct3D::GetInstance()->DrawHollowRect(ex,D3DCOLOR_ARGB(255,0,0,0),2);
	pFont->Draw(_T("Exit Menu"),635,577,0.75f,D3DCOLOR_ARGB(255,255,255,255));



	CSGD_TextureManager::GetInstance()->Draw(m_nFarm,625,75,1.0,1.0,0);
	pFont->Draw(_T("Farm"),695,75,0.75f,D3DCOLOR_ARGB(255,255,255,255));
	pFont->Draw(_T("Wood:  20"),695,100,0.75f,D3DCOLOR_ARGB(255,255,255,255));


	CSGD_TextureManager::GetInstance()->Draw(m_nMine,625,175,1.0,1.0,0);
	pFont->Draw(_T("Mine"),695,175,0.75f,D3DCOLOR_ARGB(255,255,255,255));
	pFont->Draw(_T("Wood:  15"),695,200,0.75f,D3DCOLOR_ARGB(255,255,255,255));
	pFont->Draw(_T("Steel:  10"),695,225,0.75f,D3DCOLOR_ARGB(255,255,255,255));



	CSGD_TextureManager::GetInstance()->Draw(m_nLumber,625,275,1.0,1.0,0);
	pFont->Draw(_T("Lumber Mill"),695,275,0.75f,D3DCOLOR_ARGB(255,255,255,255));
	pFont->Draw(_T("Wood:  20"),695,300,0.75f,D3DCOLOR_ARGB(255,255,255,255));
	pFont->Draw(_T("Steel:  5"),695,325,0.75f,D3DCOLOR_ARGB(255,255,255,255));


	CSGD_TextureManager::GetInstance()->Draw(m_nBarracks,625,375,1.0,1.0,0);
	pFont->Draw(_T("Barracks"),695,375,0.75f,D3DCOLOR_ARGB(255,255,255,255));
	pFont->Draw(_T("Wood:  30"),695,400,0.75f,D3DCOLOR_ARGB(255,255,255,255));
	pFont->Draw(_T("Steel:  10"),695,425,0.75f,D3DCOLOR_ARGB(255,255,255,255));


	CSGD_TextureManager::GetInstance()->Draw(m_nFactory,625,475,1.0,1.0,0);
	pFont->Draw(_T("War Factory"),695,475,0.75f,D3DCOLOR_ARGB(255,255,255,255));
	pFont->Draw(_T("Wood:  20"),695,500,0.75f,D3DCOLOR_ARGB(255,255,255,255));
	pFont->Draw(_T("Steel:  40"),695,525,0.75f,D3DCOLOR_ARGB(255,255,255,255));


}
void CArcadeGamePlay::RenderBuildings(void)
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
void CArcadeGamePlay::AllocateResources(void)
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

	if(m_nTurnNumber != 0)
		CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nResources);
}
void CArcadeGamePlay::RecruitRender(void)
{
	CBitmapFont* pFont = CGame::GetInstance()->GetFont();

	pFont->Draw(_T("Select A Unit:"),635,45,1.0f,D3DCOLOR_ARGB(255,0,0,255));

	RECT main = {625,75,800,600};
	CSGD_Direct3D::GetInstance()->DrawRect(main,D3DCOLOR_ARGB(100,0,0,0));




	if(m_cCurrentTile->GetCurrent()->GetBuilding() == BARRACKS)
	{

		for(int i = 0; i < 3; i++)
		{
			RECT border = {625,75+(i*150),800,225 +(i*150)};
			CSGD_Direct3D::GetInstance()->DrawHollowRect(border,D3DCOLOR_ARGB(255,0,0,0),2);
		}

		RECT ex = {625,575,800,648};
		CSGD_Direct3D::GetInstance()->DrawHollowRect(ex,D3DCOLOR_ARGB(255,0,0,0),2);
		pFont->Draw(_T("Exit Menu"),635,577,0.75f,D3DCOLOR_ARGB(255,255,255,255));



		if(m_nTurnOrder == 0)
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nFoot,625,75,1.0,1.0,0);
			pFont->Draw(_T("Footsoldier"),695,75,0.75f,D3DCOLOR_ARGB(255,255,255,255));
			pFont->Draw(_T("Food:  10"),695,100,0.75f,D3DCOLOR_ARGB(255,255,255,255));


			CSGD_TextureManager::GetInstance()->Draw(m_nPriest,625,225,1.0,1.0,0);
			pFont->Draw(_T("Priest"),695,225,0.75f,D3DCOLOR_ARGB(255,255,255,255));
			pFont->Draw(_T("Food:  25"),695,250,0.75f,D3DCOLOR_ARGB(255,255,255,255));



			CSGD_TextureManager::GetInstance()->Draw(m_nWizard,625,375,1.0,1.0,0);
			pFont->Draw(_T("Wizard"),695,375,0.75f,D3DCOLOR_ARGB(255,255,255,255));
			pFont->Draw(_T("Food:  50"),695,400,0.75f,D3DCOLOR_ARGB(255,255,255,255));

		}
		else if(m_nTurnOrder == 1)
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nSkeleton,625,75,1.0,1.0,0);
			pFont->Draw(_T("Skeletal Warrior"),695,75,0.75f,D3DCOLOR_ARGB(255,255,255,255));
			pFont->Draw(_T("Food:  10"),695,150,0.75f,D3DCOLOR_ARGB(255,255,255,255));


			CSGD_TextureManager::GetInstance()->Draw(m_nSPriest,625,225,1.0,1.0,0);
			pFont->Draw(_T("Shadow Priest"),695,225,0.75f,D3DCOLOR_ARGB(255,255,255,255));
			pFont->Draw(_T("Food:  25"),695,250,0.75f,D3DCOLOR_ARGB(255,255,255,255));



			CSGD_TextureManager::GetInstance()->Draw(m_nNecro,625,375,1.0,1.0,0);
			pFont->Draw(_T("Necromancer"),695,375,0.75f,D3DCOLOR_ARGB(255,255,255,255));
			pFont->Draw(_T("Food:  50"),695,400,0.75f,D3DCOLOR_ARGB(255,255,255,255));
		}
	}
	else if(m_cCurrentTile->GetCurrent()->GetBuilding() == WARFACTORY)
	{
		for(int i = 0; i < 2; i++)
		{
			RECT border = {625,75+(i*150),800,225 +(i*150)};
			CSGD_Direct3D::GetInstance()->DrawHollowRect(border,D3DCOLOR_ARGB(255,0,0,0),2);
		}

		RECT ex = {625,575,800,648};
		CSGD_Direct3D::GetInstance()->DrawHollowRect(ex,D3DCOLOR_ARGB(255,0,0,0),2);
		pFont->Draw(_T("Exit Menu"),635,577,0.75f,D3DCOLOR_ARGB(255,255,255,255));






		if(m_nTurnOrder == 0)
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nCavalry,625,75,1.0,1.0,0);
			pFont->Draw(_T("Cavalry"),695,75,0.75f,D3DCOLOR_ARGB(255,255,255,255));
			pFont->Draw(_T("Food:  50"),695,100,0.75f,D3DCOLOR_ARGB(255,255,255,255));
			pFont->Draw(_T("Steel:  10"),695,125,0.75f,D3DCOLOR_ARGB(255,255,255,255));



			CSGD_TextureManager::GetInstance()->Draw(m_nCatapult,625,225,1.0,1.0,0);
			pFont->Draw(_T("Catapult"),695,225,0.75f,D3DCOLOR_ARGB(255,255,255,255));
			pFont->Draw(_T("Wood:  100"),695,250,0.75f,D3DCOLOR_ARGB(255,255,255,255));
			pFont->Draw(_T("Steel:  100"),695,275,0.75f,D3DCOLOR_ARGB(255,255,255,255));


		}
		else if(m_nTurnOrder == 1)
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nDK,625,75,1.0,1.0,0);
			pFont->Draw(_T("Death Knight"),695,75,0.75f,D3DCOLOR_ARGB(255,255,255,255));
			pFont->Draw(_T("Food:  50"),695,150,0.75f,D3DCOLOR_ARGB(255,255,255,255));
			pFont->Draw(_T("Steel:  10"),695,175,0.75f,D3DCOLOR_ARGB(255,255,255,255));


			CSGD_TextureManager::GetInstance()->Draw(m_nWM,625,225,1.0,1.0,0);
			pFont->Draw(_T("War Machine"),695,225,0.75f,D3DCOLOR_ARGB(255,255,255,255));
			pFont->Draw(_T("Wood:  100"),695,250,0.75f,D3DCOLOR_ARGB(255,255,255,255));
			pFont->Draw(_T("Steel:  100"),695,275,0.75f,D3DCOLOR_ARGB(255,255,255,255));


		}


	}

}
bool CArcadeGamePlay::SellBuilding(void )
{
	int wood;
	int steel;

	vector<CBaseBuilding*> temp = m_cPlayerOne->GetBuildings();
	if (m_nTurnOrder == 0)
	{
		wood = m_cPlayerOne->GetWood();
		steel = m_cPlayerOne->GetSteel();

		for (unsigned int i = 0; i < m_cPlayerOne->GetBuildings().size(); i++)
		{
			if (m_cPlayerOne->GetBuildings()[i]->GetPosBuildX() == (int)m_cCurrentTile->GetCurrent()->GetPosX()  )
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
				m_cCurrentTile->GetCurrent()->SetBuilding( -1 );
				return true;
			}
		}
	}
	else if ( m_nTurnOrder == 1 )
	{
		wood = m_cPlayerTwo->GetWood();
		steel = m_cPlayerTwo->GetSteel();
		for (unsigned int i = 0; i < m_cPlayerTwo->GetBuildings().size(); i++)
		{
			if (m_cPlayerTwo->GetBuildings()[i]->GetPosBuildX() == (int)m_cCurrentTile->GetCurrent()->GetPosX()  )
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
				m_cCurrentTile->GetCurrent()->SetBuilding( -1 );
				return true;
			}
		}
	}


	return false;
}
void CArcadeGamePlay::RenderUnits(void)
{
	CBitmapFont* pFont = CGame::GetInstance()->GetFont();



	if(m_cCurrentTile->GetCurrent()->GetOwner() == 1)
	{

		soldierCount = 0;
		priestCount = 0;
		wizardCount = 0;
		cavalryCount = 0;
		catapultCount = 0;
		generalPresent = false;

		for(unsigned int i = 0; i < m_cPlayerOne->GetUnits().size();i++)
		{
			if(m_cPlayerOne->GetUnits()[i]->GetCountry() == curX)
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

		pFont->Draw(_T("Units in Territory: "),300,500,0.75f,D3DCOLOR_ARGB(255,255,255,255));

		if(generalPresent)
		{
			pFont->Draw(_T("General in Command"),275,515,0.75f,D3DCOLOR_ARGB(255,255,255,255));
		}

		pFont->Draw(_T("Foot Soldiers: "),200,540,0.65f,D3DCOLOR_ARGB(255,255,255,255));
		pFont->Draw(fs.str().c_str(),300,540,0.65f,D3DCOLOR_ARGB(255,255,255,255));

		pFont->Draw(_T("Priests: "),200,555,0.65f,D3DCOLOR_ARGB(255,255,255,255));
		pFont->Draw(p.str().c_str(),300,555,0.65f,D3DCOLOR_ARGB(255,255,255,255));

		pFont->Draw(_T("Wizards: "),200,570,0.65f,D3DCOLOR_ARGB(255,255,255,255));
		pFont->Draw(w.str().c_str(),300,570,0.65f,D3DCOLOR_ARGB(255,255,255,255));

		pFont->Draw(_T("Catapults: "),350,540,0.65f,D3DCOLOR_ARGB(255,255,255,255));
		pFont->Draw(cp.str().c_str(),450,540,0.65f,D3DCOLOR_ARGB(255,255,255,255));

		pFont->Draw(_T("Cavalry: "),350,555,0.65f,D3DCOLOR_ARGB(255,255,255,255));
		pFont->Draw(cv.str().c_str(),450,555,0.65f,D3DCOLOR_ARGB(255,255,255,255));










	}
	else if(m_cCurrentTile->GetCurrent()->GetOwner() == 2)
	{

		skeletonCount = 0;
		spriestCount = 0;
		necroCount = 0;
		dkCount = 0;
		wmCount = 0;
		lichPresent = false;

		for(unsigned int i = 0; i < m_cPlayerTwo->GetUnits().size();i++)
		{
			if(m_cPlayerTwo->GetUnits()[i]->GetCountry() == curX)
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
				case VOLATILEGHOUL:
				case FOOTSOLDIERS:
				case PRIEST:
				case WIZARD:
				case GENERAL:
				case CALVARY:
				case CATAPULT:

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

		pFont->Draw(_T("Units in Territory: "),300,500,0.75f,D3DCOLOR_ARGB(255,255,255,255));

		if(lichPresent)
		{
			pFont->Draw(_T("Lich in Command"),275,515,0.75f,D3DCOLOR_ARGB(255,255,255,255));
		}

		pFont->Draw(_T("Skeletal Warriors: "),200,540,0.65f,D3DCOLOR_ARGB(255,255,255,255));
		pFont->Draw(sw.str().c_str(),315,540,0.65f,D3DCOLOR_ARGB(255,255,255,255));

		pFont->Draw(_T("Shadow Priests: "),200,555,0.65f,D3DCOLOR_ARGB(255,255,255,255));
		pFont->Draw(sp.str().c_str(),300,555,0.65f,D3DCOLOR_ARGB(255,255,255,255));

		pFont->Draw(_T("Necromancers: "),200,570,0.65f,D3DCOLOR_ARGB(255,255,255,255));
		pFont->Draw(n.str().c_str(),300,570,0.65f,D3DCOLOR_ARGB(255,255,255,255));

		pFont->Draw(_T("War Machines: "),350,540,0.65f,D3DCOLOR_ARGB(255,255,255,255));
		pFont->Draw(wm.str().c_str(),450,540,0.65f,D3DCOLOR_ARGB(255,255,255,255));

		pFont->Draw(_T("Death Knights: "),350,555,0.65f,D3DCOLOR_ARGB(255,255,255,255));
		pFont->Draw(dk.str().c_str(),450,555,0.65f,D3DCOLOR_ARGB(255,255,255,255));






	}


}
bool CArcadeGamePlay::MoveUnitFromTerritory ( void )
{
	return true;
}
void CArcadeGamePlay::RenderOptionsForMove( void )
{
CBitmapFont* pFont = CGame::GetInstance()->GetFont();



RECT SelectUnits = { 50, 50, 750, 550 };

	
	CSGD_Direct3D::GetInstance()->DrawRect( SelectUnits, D3DCOLOR_XRGB( 0,0,0 ) );						  
	
	
	pFont->Draw( _T("->"), 100, 200 + (m_nMoveCursor * 50), 1.0f, D3DCOLOR_ARGB(255, 255, 0, 0) );
		

	if(m_cWorld->GetGrid()[m_nMovingFromX][m_nMovingFromY]->GetOwner() == 1)
	{

		soldierCount = 0;
		priestCount = 0;
		wizardCount = 0;
		cavalryCount = 0;
		catapultCount = 0;
		generalPresent = false;

		

		for(unsigned int i = 0; i < m_cPlayerOne->GetUnits().size();i++)
		{
			if(m_cPlayerOne->GetUnits()[i]->GetCountry() == m_nMovingFromX)
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
	else if(m_cWorld->GetGrid()[m_nMovingFromX][m_nMovingFromY]->GetOwner() == 2)
	{

		skeletonCount = 0;
		spriestCount = 0;
		necroCount = 0;
		dkCount = 0;
		wmCount = 0;
		lichPresent = false;

		

		for(unsigned int i = 0; i < m_cPlayerTwo->GetUnits().size();i++)
		{
			if(m_cPlayerTwo->GetUnits()[i]->GetCountry() == m_nMovingFromX)
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
bool CArcadeGamePlay::HandleInputForMoveAndAttack( void )
{


	CSGD_DirectInput*	pDI		=	CSGD_DirectInput::GetInstance();


	// Navigate the menu
	if( pDI->JoystickGetLStickDirPressed( DIR_DOWN, JOYSTICKID1 ) == true  )
	{
		++m_nMoveCursor;


	
		// Stop at the bottom
		if( m_nMoveCursor > 6 )		// EXIT
			m_nMoveCursor = 0;

		
			
	}
	else if(pDI->JoystickGetLStickDirPressed( DIR_UP, JOYSTICKID1 ) == true  )
	{
		--m_nMoveCursor;


		
		// Stop at the top
		if( m_nMoveCursor < 0 )		// EXIT
			m_nMoveCursor = 6;

		
	}

	if ( pDI->JoystickGetLStickDirPressed( DIR_RIGHT, JOYSTICKID1 ) == true   )
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
	if (  pDI->JoystickGetLStickDirPressed( DIR_LEFT, JOYSTICKID1 ) == true  )
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
	
	
	if( pDI->JoystickButtonPressed( 1, JOYSTICKID1 ) == true )
	{
		switch (m_nMoveCursor)
		{
		case 5:
			{
				if (m_nTurnOrder == 0)
				{
					if (m_cCurrentTile->GetCurrent()->GetOwner() == 2)
					{
						m_bAMMode = false;
						m_nMoveCursor = 0;
						m_bSelectTerritoryToMove = false;
						m_bIsInWorld = false;
						MoveUnitsBetweenTerritories();
						CGame::GetInstance()->PushState( CBattlePlay::GetInstance() );

					}
					else if (m_cCurrentTile->GetCurrent()->GetOwner() == 0)
					{
						m_cWorld->GetGrid()[m_cCurrentTile->GetCurrent()->GetCountry()][m_cCurrentTile->GetCurrent()->GetTerritory()]->SetOwner( 1 );
						m_bAMMode = false;
						m_nBuildCursor = false;
						m_bMoveandAttackMode =false;
						m_nWidth = (800 - ( 5 * 32 ) );
						m_nInOptions = true;
						m_nCursor = 1;
						m_nMoveCursor = 0;
						m_cWorld->GetGrid()[m_cCurrentTile->GetCurrent()->GetCountry()][m_cCurrentTile->GetCurrent()->GetTerritory()]->SetOccupied( true );
						m_bSelectTerritoryToMove = false;
						MoveUnitsBetweenTerritories();
						CSGD_XAudio2::GetInstance()->SFXPlaySound( m_nConquer );
						if (m_cCurrentTile->GetCurrent()->GetCountry() == 0)
						{
							m_nCountryOneOwner = 1;
						}
						else if (m_cCurrentTile->GetCurrent()->GetCountry() == 3)
						{
							m_nCountryOneOwner = 1;
						}
					}
				}
				else if (m_nTurnOrder == 1)
				{
					if (m_cCurrentTile->GetCurrent()->GetOwner() == 1)
					{
						m_bAMMode = false;
						m_nMoveCursor = 0;
						m_bSelectTerritoryToMove = false;
						m_bIsInWorld = false;
						MoveUnitsBetweenTerritories();
						CGame::GetInstance()->PushState( CBattlePlay::GetInstance() );
					}
					else if (m_cCurrentTile->GetCurrent()->GetOwner() == 0)
					{
						m_cWorld->GetGrid()[m_cCurrentTile->GetCurrent()->GetCountry()][m_cCurrentTile->GetCurrent()->GetTerritory()]->SetOwner( 2 );
						m_bAMMode = false;
						m_nBuildCursor = false;
						m_bMoveandAttackMode =false;
						m_nWidth = (800 - ( 5 * 32 ) );
						m_nInOptions = true;
						m_nCursor = 1;
						m_nMoveCursor = 0;
						m_cWorld->GetGrid()[m_cCurrentTile->GetCurrent()->GetCountry()][m_cCurrentTile->GetCurrent()->GetTerritory()]->SetOccupied( true );
						m_bSelectTerritoryToMove = false;
						MoveUnitsBetweenTerritories();
						CSGD_XAudio2::GetInstance()->SFXPlaySound( m_nConquer );
						if (m_cCurrentTile->GetCurrent()->GetCountry() == 0)
						{
							m_nCountryOneOwner = 2;
						}
						else if (m_cCurrentTile->GetCurrent()->GetCountry() == 3)
						{
							m_nCountryOneOwner = 2;
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
	
	
	

	

	


	
	
	
	


	
	
	



	return false;
}
void CArcadeGamePlay::CheckMovement ( void )
{
	if (m_nMovingFromX != m_cCurrentTile->GetCurrent()->GetCountry())
	{
		switch (m_nMovingFromX)
		{
		case 0:
			if (m_cCurrentTile->GetCurrent()->GetCountry() == 1 || m_cCurrentTile->GetCurrent()->GetCountry() == 2)
			{
				m_bAMMode = true;
			}
			break;
		case 1:
			if (m_cCurrentTile->GetCurrent()->GetCountry() == 0 || m_cCurrentTile->GetCurrent()->GetCountry() == 3)
			{
				m_bAMMode = true;
			}
			break;
		case 2:
			if (m_cCurrentTile->GetCurrent()->GetCountry() == 0 || m_cCurrentTile->GetCurrent()->GetCountry() == 3)
			{
				m_bAMMode = true;
			}
			break;
		case 3:
			if (m_cCurrentTile->GetCurrent()->GetCountry() == 1 || m_cCurrentTile->GetCurrent()->GetCountry() == 2)
			{
				m_bAMMode = true;
			}
			break;
		default:
			break;
		}
	}
}
void CArcadeGamePlay::MoveUnitsBetweenTerritories ( void )
{
	if (m_nTurnOrder == 0)
	{
		for(unsigned int i = 0; i < m_cPlayerOne->GetUnits().size();i++)
		{
			if(m_cPlayerOne->GetUnits()[i]->GetCountry() == m_nMovingFromX)
			{
				switch(m_cPlayerOne->GetUnits()[i]->GetUnitID())
				{
				case FOOTSOLDIERS:
					{
						if(m_MovesoldierCount > 0)
						{
							m_cPlayerOne->GetUnits()[i]->SetCountry(m_cCurrentTile->GetCurrent()->GetCountry());
							m_cPlayerOne->GetUnits()[i]->SetTerritory(m_cCurrentTile->GetCurrent()->GetTerritory());

							m_MovesoldierCount -= 1;
						}
					}
					break;
				case PRIEST:
					{
						if(m_MovepriestCount > 0)
						{
							m_cPlayerOne->GetUnits()[i]->SetCountry(m_cCurrentTile->GetCurrent()->GetCountry());
							m_cPlayerOne->GetUnits()[i]->SetTerritory(m_cCurrentTile->GetCurrent()->GetTerritory());

							m_MovepriestCount -= 1;
						}
					}
					break;
				case WIZARD:
					{
						if(m_MovewizardCount > 0)
						{
							m_cPlayerOne->GetUnits()[i]->SetCountry(m_cCurrentTile->GetCurrent()->GetCountry());
							m_cPlayerOne->GetUnits()[i]->SetTerritory(m_cCurrentTile->GetCurrent()->GetTerritory());

							m_MovewizardCount -= 1;
						}
					}
					break;
				case CALVARY:
					{
						if(m_MovecavalryCount > 0)
						{
							m_cPlayerOne->GetUnits()[i]->SetCountry(m_cCurrentTile->GetCurrent()->GetCountry());
							m_cPlayerOne->GetUnits()[i]->SetTerritory(m_cCurrentTile->GetCurrent()->GetTerritory());

							m_MovecavalryCount -= 1;
						}
					}
					break;
				case CATAPULT:
					{
						if(m_MovecatapultCount > 0)
						{
							m_cPlayerOne->GetUnits()[i]->SetCountry(m_cCurrentTile->GetCurrent()->GetCountry());
							m_cPlayerOne->GetUnits()[i]->SetTerritory(m_cCurrentTile->GetCurrent()->GetTerritory());

							m_MovecatapultCount -= 1;
						}
					}
					break;
				}
			}
		}
	}
	else if (m_nTurnOrder == 1)
	{
		for(unsigned int i = 0; i < m_cPlayerTwo->GetUnits().size();i++)
		{
			if(m_cPlayerTwo->GetUnits()[i]->GetCountry() == m_nMovingFromX)
			{
				switch(m_cPlayerTwo->GetUnits()[i]->GetUnitID())
				{
				case SKELETALWARRIOR:
					{
						if(m_MoveskeletonCount > 0)
						{
							m_cPlayerOne->GetUnits()[i]->SetCountry(m_cCurrentTile->GetCurrent()->GetCountry());
							m_cPlayerOne->GetUnits()[i]->SetTerritory(m_cCurrentTile->GetCurrent()->GetTerritory());

							m_MoveskeletonCount -= 1;
						}
					}
					break;
				case SHADOWPRIEST:
					{
						if(m_MovespriestCount > 0)
						{
							m_cPlayerOne->GetUnits()[i]->SetCountry(m_cCurrentTile->GetCurrent()->GetCountry());
							m_cPlayerOne->GetUnits()[i]->SetTerritory(m_cCurrentTile->GetCurrent()->GetTerritory());

							m_MovespriestCount -= 1;
						}
					}
					break;
				case NECROMANCER:
					{
						if(m_MovenecroCount > 0)
						{
							m_cPlayerOne->GetUnits()[i]->SetCountry(m_cCurrentTile->GetCurrent()->GetCountry());
							m_cPlayerOne->GetUnits()[i]->SetTerritory(m_cCurrentTile->GetCurrent()->GetTerritory());

							m_MovenecroCount -= 1;
						}
					}
					break;
				case DEATHKNIGHT:
					{
						if(m_MovedkCount > 0)
						{
							m_cPlayerOne->GetUnits()[i]->SetCountry(m_cCurrentTile->GetCurrent()->GetCountry());
							m_cPlayerOne->GetUnits()[i]->SetTerritory(m_cCurrentTile->GetCurrent()->GetTerritory());

							m_MovedkCount -= 1;
						}
					}
					break;
				case WARMACHINE:
					{
						if(m_MovewmCount > 0)
						{
							m_cPlayerOne->GetUnits()[i]->SetCountry(m_cCurrentTile->GetCurrent()->GetCountry());
							m_cPlayerOne->GetUnits()[i]->SetTerritory(m_cCurrentTile->GetCurrent()->GetTerritory());

							m_MovewmCount -= 1;
						}
					}
					break;

				}
			}
		}

	}
}
void CArcadeGamePlay::LoadBackFromBattle ( void )
{

}