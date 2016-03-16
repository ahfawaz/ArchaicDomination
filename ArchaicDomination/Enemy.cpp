#include "Enemy.h"
#include "SGD Wrappers\CSGD_DirectInput.h"
#include "BaseUnit.h"
#include "BattlePlay.h"
#include "SGD Wrappers\CSGD_TextureManager.h"
#include "GamePlay.h"
#include "SGD Wrappers\CSGD_MessageSystem.h"
#include "SGD Wrappers\CSGD_EventSystem.h"
#include "CreateAbilityMessage.h"
#include "CreateAbilityTwoMessage.h"
#include "Player.h"
#include "Ability.h"
#include "Game.h"
#include "DarkCurse.h"
#include "GamePlay.h"
#include "MainMenu.h"
#include "ArcadeGamePlay.h"
#include "WorldPlay.h"
#include "BaseBuilding.h"




////////////////////////////////
//Singleton
CEnemy * CEnemy::GetInstance ( void ) 
{
	//Global Instance
	static CEnemy s_Instance;
	return &s_Instance;

}


CEnemy::CEnemy(void)
{
	m_bSinglePlayer = true;
	AIDelay = 0.0f;
	unitsInBattle = 0;

	type = PlayerType::ENEMY;
	CBaseUnit* skeleton = new CBaseUnit();
	skeleton-> CreateUnit( SKELETALWARRIOR, 1, 0 );
	skeleton->SetPosX(6*32);
	skeleton->SetPosY(3*32);


	CBaseUnit* necro = new CBaseUnit();
	necro-> CreateUnit( NECROMANCER, 1, 0 );
	necro->SetPosX(12*32);
	necro->SetPosY(4*32);



	CBaseUnit* shadowPriest = new CBaseUnit();
	shadowPriest-> CreateUnit( SHADOWPRIEST, 1, 0 );
	shadowPriest->SetPosX(6*32);
	shadowPriest->SetPosY(5*32);

	CBaseUnit* warMachine = new CBaseUnit();
	warMachine-> CreateUnit( WARMACHINE, 1, 0 );
	warMachine->SetPosX(12*32);
	warMachine->SetPosY(7*32);

	CBaseUnit* deathKnight = new CBaseUnit();
	deathKnight-> CreateUnit( DEATHKNIGHT, 1, 0 );
	deathKnight->SetPosX(12*32);
	deathKnight->SetPosY(8*32);

	CBaseUnit* lich = new CBaseUnit();
	lich-> CreateUnit( LICH, 1, 0 );
	lich->SetPosX(12*32);
	lich->SetPosY(9*32);

	m_vUnits.push_back( skeleton );
	m_vUnits.push_back( necro );
	
	m_vUnits.push_back( shadowPriest );
	m_vUnits.push_back( warMachine );
	m_vUnits.push_back( deathKnight );
	m_vUnits.push_back( lich );


		//////////////////////////////////////////////////////////////
	//AoE Test units
	CBaseUnit* aoeUnit = new CBaseUnit();
	aoeUnit-> CreateUnit( SKELETALWARRIOR, 2, 0 );
	aoeUnit->SetPosX(5*32);
	aoeUnit->SetPosY(3*32);


	CBaseUnit* aoeUnit1 = new CBaseUnit();
	aoeUnit1-> CreateUnit( SKELETALWARRIOR, 2, 0 );
	aoeUnit1->SetPosX(7*32);
	aoeUnit1->SetPosY(3*32);



	CBaseUnit* aoeUnit2 = new CBaseUnit();
	aoeUnit2-> CreateUnit( SKELETALWARRIOR, 2, 0 );
	aoeUnit2->SetPosX(6*32);
	aoeUnit2->SetPosY(2*32);



	CBaseUnit* aoeUnit3 = new CBaseUnit();
	aoeUnit3-> CreateUnit( SKELETALWARRIOR, 2, 0 );
	aoeUnit3->SetPosX(6*32);
	aoeUnit3->SetPosY(4*32);



	CBaseUnit* aoeUnit4 = new CBaseUnit();
	aoeUnit4-> CreateUnit( SKELETALWARRIOR, 2, 0 );
	aoeUnit4->SetPosX(5*32);
	aoeUnit4->SetPosY(4*32);



	CBaseUnit* aoeUnit5 = new CBaseUnit();
	aoeUnit5-> CreateUnit( SKELETALWARRIOR, 2, 0 );
	aoeUnit5->SetPosX(7*32);
	aoeUnit5->SetPosY(2*32);



	m_vUnits.push_back( aoeUnit );
	m_vUnits.push_back( aoeUnit1);
	m_vUnits.push_back( aoeUnit2);
	m_vUnits.push_back( aoeUnit3);
	m_vUnits.push_back( aoeUnit4);
	m_vUnits.push_back( aoeUnit5);



	SetGeneralImageID( CSGD_TextureManager::GetInstance()->LoadTexture( _T( "resources/graphics/UndeadGeneral.png"), D3DCOLOR_XRGB(255,255,255) ) );
	SetPosX( 500 );
	SetPosY( 300 );

	SetSteel( 10 );
	SetWood ( 30 );
	SetFood ( 25 );

	CSGD_EventSystem::GetInstance()->RegisterClient("LICH_BUFF", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("DARK_CURSE", this);
	darkCurseActive = nullptr;
	m_bIsCastingAbility = false;




}

void CEnemy::ClearEnemyinfo()
{
	for (unsigned int i = 0; i < m_vUnits.size(); i++)
	{
		delete m_vUnits[i];
	}
	m_vUnits.clear();

	for (unsigned int i = 0; i < m_vBuildings.size(); i++)
	{
		delete m_vBuildings[i];
	}

	m_vBuildings.clear();

}
CEnemy::~CEnemy(void)
{
	for(unsigned int i = 0; i < m_vUnits.size(); i++)
	{
		delete m_vUnits[i];
	}

	m_vUnits.clear();

	for (unsigned int i = 0; i < m_vBuildings.size(); i++)
	{
		delete m_vBuildings[i];
	}

	m_vBuildings.clear();
}

void CEnemy::Update(float fElapsedTime)
{
	for(unsigned int i = 0; i < m_vUnits.size(); i++)
	{
		m_vUnits[i]->Update(fElapsedTime);
		if(m_vUnits[i]->GetCurrentHealth() <= 0)
		{
			m_vUnits[i]->GetTile()->SetOccupied(false);
			delete m_vUnits[i];
			m_vUnits[i] = nullptr;
			m_vUnits.erase(m_vUnits.begin()+i);
			unitsInBattle -= 1;
		}
	}

	int moveCount = 0;
	for(unsigned int i = 0; i < m_vUnits.size(); i++)
	{
		if(m_vUnits[i]->HasAttacked()&& m_vUnits[i]->HasMoved())++moveCount;
	}
	if(moveCount == unitsInBattle && unitsInBattle != NULL) CBattlePlay::GetInstance()->ChangeTurn(); 
}


bool CEnemy::Input(void)
{
	if(IsAI() == true)
		return true;

	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();

	if(pDI->KeyPressed(DIK_UP) == true || pDI->JoystickGetLStickDirPressed( DIR_UP, JOYSTICKID2) == true )
	{
		if(CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocY() > 0)
			CBattlePlay::GetInstance()->GetGrid()->ChangeCurrent(CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocX(), 
			CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocY()-32);
		if(CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocY() < CBattlePlay::GetInstance()->GetOffY())
		{
			CBattlePlay::GetInstance()->SetOffY(CBattlePlay::GetInstance()->GetOffY()-32);
		}
		CGame::GetInstance()->PlayMoveCursorSFX();
		return true;
	}
	if(pDI->KeyPressed(DIK_RIGHT) == true || pDI->JoystickGetLStickDirPressed( DIR_RIGHT, JOYSTICKID2) == true )
	{
		if(CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocX() < 992)
			CBattlePlay::GetInstance()->GetGrid()->ChangeCurrent(CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocX()+32, 
			CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocY());

		if(CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocX() >= CGame::GetInstance()->GetScreenWidth() + CBattlePlay::GetInstance()->GetOffX())
		{
			CBattlePlay::GetInstance()->SetOffX(CBattlePlay::GetInstance()->GetOffX()+32);
		}
		CGame::GetInstance()->PlayMoveCursorSFX();
		return true;
	}

	if(pDI->KeyPressed(DIK_DOWN) == true || pDI->JoystickGetLStickDirPressed( DIR_DOWN, JOYSTICKID2) == true )
	{
		if(CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocY() < 992)
			CBattlePlay::GetInstance()->GetGrid()->ChangeCurrent(CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocX(), 
			CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocY()+32);

		if(CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocY() >= CGame::GetInstance()->GetScreenHeight() + CBattlePlay::GetInstance()->GetOffY())
		{
			CBattlePlay::GetInstance()->SetOffY(CBattlePlay::GetInstance()->GetOffY()+32);

		}
		CGame::GetInstance()->PlayMoveCursorSFX();
		return true;
	}
	if(pDI->KeyPressed(DIK_LEFT) == true || pDI->JoystickGetLStickDirPressed( DIR_LEFT, JOYSTICKID2) == true )
	{

		if(CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocX() > 0)
			CBattlePlay::GetInstance()->GetGrid()->ChangeCurrent(CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocX()-32, 
			CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocY());

		if(CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocX() < CBattlePlay::GetInstance()->GetOffX())
		{
			CBattlePlay::GetInstance()->SetOffX(CBattlePlay::GetInstance()->GetOffX()-32);
		}

		CGame::GetInstance()->PlayMoveCursorSFX();
		return true;
	}
	if((pDI->KeyPressed(DIK_NUMPAD1) || pDI->JoystickButtonPressed( 2, JOYSTICKID2 ))== true && CBattlePlay::GetInstance()->GetCurrentUnit())//USING AN ABILITY
	{
		if(m_bIsCastingAbility == true || CBattlePlay::GetInstance()->GetCurrentUnit()->GetAbility1_Cooldown() > 0)
		{
			return false;
		}

		switch(CBattlePlay::GetInstance()->GetCurrentUnit()->GetUnitID())
		{
		case SKELETALWARRIOR:
			{
				CCreateAbilityMessage* msg = new CCreateAbilityMessage(CBattlePlay::GetInstance()->GetCurrentUnit());
				CSGD_MessageSystem::GetInstance()->QueueMessage(msg);
				msg = nullptr;
			}
			break;
		case NECROMANCER:
			{
				vector<CBaseUnit*> temp = CPlayer::GetInstance()->GetUnits();
				for(unsigned int i = 0; i < temp.size(); i++)
				{
					if(temp[i]->GetPosX() == CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocX() &&
						temp[i]->GetPosY() == CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocY())
					{
						if(CBattlePlay::GetInstance()->GetCurrentUnit()->IsInRange(temp[i]))
						{
							CBattlePlay::GetInstance()->SetCurrentTarget(temp[i]);


							CCreateAbilityMessage* msg = new CCreateAbilityMessage(CBattlePlay::GetInstance()->GetCurrentUnit());
							CSGD_MessageSystem::GetInstance()->QueueMessage(msg);
							msg = nullptr;
						}
					}
				}
			}
			break;
		case DEATHKNIGHT:
			{
				vector<CBaseUnit*> temp = CPlayer::GetInstance()->GetUnits();
				for(unsigned int i = 0; i < temp.size(); i++)
				{
					if(temp[i]->GetPosX() == CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocX() &&
						temp[i]->GetPosY() == CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocY())
					{
						int targetDistance = 0;
						targetDistance += int(_CMATH_::abs(temp[i]->GetPosX()-CBattlePlay::GetInstance()->GetCurrentUnit()->GetPosX()));
						targetDistance += int(_CMATH_::abs(temp[i]->GetPosY()-CBattlePlay::GetInstance()->GetCurrentUnit()->GetPosY()));

						bool inTheWay = true;

						if(temp[i]->GetPosX() > CBattlePlay::GetInstance()->GetCurrentUnit()->GetPosX() && temp[i]->GetPosY() == CBattlePlay::GetInstance()->GetCurrentUnit()->GetPosY())
						{
							for(int dist = 0; dist < ((targetDistance) / 32); dist++)
							{
								inTheWay = CBattlePlay::GetInstance()->GetGrid()->GetATile((int)CBattlePlay::GetInstance()->GetCurrentUnit()->GetPosX() / 32  + dist ,(int)CBattlePlay::GetInstance()->GetCurrentUnit()->GetPosY() / 32)->GetOccupied();
								if(inTheWay)
									break;
							}
						}

						else if(temp[i]->GetPosX() < CBattlePlay::GetInstance()->GetCurrentUnit()->GetPosX() && temp[i]->GetPosY() == CBattlePlay::GetInstance()->GetCurrentUnit()->GetPosY())
						{
							for(int dist = 0; dist < ((targetDistance) / 32); dist++)
							{
								inTheWay = CBattlePlay::GetInstance()->GetGrid()->GetATile((int)CBattlePlay::GetInstance()->GetCurrentUnit()->GetPosX() / 32  - dist, (int)CBattlePlay::GetInstance()->GetCurrentUnit()->GetPosY() / 32)->GetOccupied();
								if(inTheWay)
									break;
							}
						}

						else if(temp[i]->GetPosX() == CBattlePlay::GetInstance()->GetCurrentUnit()->GetPosX() && temp[i]->GetPosY() > CBattlePlay::GetInstance()->GetCurrentUnit()->GetPosY())
						{
							for(int dist = 0; dist < ((targetDistance) / 32); dist++)
							{
								inTheWay = CBattlePlay::GetInstance()->GetGrid()->GetATile((int)CBattlePlay::GetInstance()->GetCurrentUnit()->GetPosX() / 32, (int)CBattlePlay::GetInstance()->GetCurrentUnit()->GetPosY() / 32 + dist )->GetOccupied();
								if(inTheWay)
									break;
							}
						}

						else if(temp[i]->GetPosX() == CBattlePlay::GetInstance()->GetCurrentUnit()->GetPosX() && temp[i]->GetPosY() < CBattlePlay::GetInstance()->GetCurrentUnit()->GetPosY())
						{
							for(int dist = 0; dist < ((targetDistance) / 32); dist++)
							{
								inTheWay = CBattlePlay::GetInstance()->GetGrid()->GetATile((int)CBattlePlay::GetInstance()->GetCurrentUnit()->GetPosX() / 32, (int)CBattlePlay::GetInstance()->GetCurrentUnit()->GetPosY() / 32 - dist )->GetOccupied();
								if(inTheWay)
									break;
							}
						}



						if(CBattlePlay::GetInstance()->GetCurrentUnit()->GetMovement() >= (targetDistance / 32) && inTheWay == false) //Can hit target if inTheWay is false && within movement range
						{
							CBattlePlay::GetInstance()->SetCurrentTarget(temp[i]);

							CCreateAbilityMessage* msg = new CCreateAbilityMessage(CBattlePlay::GetInstance()->GetCurrentUnit());
							CSGD_MessageSystem::GetInstance()->QueueMessage(msg);
							msg = nullptr;
						}
					}
				}
			}
			break;
		case SHADOWPRIEST:
			{
				vector<CBaseUnit*> temp = CPlayer::GetInstance()->GetUnits();
				for(unsigned int i = 0; i < temp.size(); i++)
				{
					if(temp[i]->GetPosX() == CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocX() &&
						temp[i]->GetPosY() == CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocY())
					{
						if(CBattlePlay::GetInstance()->GetCurrentUnit()->IsInRange(temp[i]))
						{
							CBattlePlay::GetInstance()->SetCurrentTarget(temp[i]);


							CCreateAbilityMessage* msg = new CCreateAbilityMessage(CBattlePlay::GetInstance()->GetCurrentUnit());
							CSGD_MessageSystem::GetInstance()->QueueMessage(msg);
							msg = nullptr;
						}
					}
				}
			}
			break;
		case WARMACHINE:
			{
				CCreateAbilityMessage* msg = new CCreateAbilityMessage(CBattlePlay::GetInstance()->GetCurrentUnit());
				CSGD_MessageSystem::GetInstance()->QueueMessage(msg);
				msg = nullptr;
			}
			break;
		case LICH:
			{
				CCreateAbilityMessage* msg = new CCreateAbilityMessage(CBattlePlay::GetInstance()->GetCurrentUnit());
				CSGD_MessageSystem::GetInstance()->QueueMessage(msg);
				msg = nullptr;
			}
			break;
		case VOLATILEGHOUL:
			{
				CCreateAbilityMessage* msg = new CCreateAbilityMessage(CBattlePlay::GetInstance()->GetCurrentUnit());
				CSGD_MessageSystem::GetInstance()->QueueMessage(msg);
				msg = nullptr;
			}
			break;
		case FOOTSOLDIERS:
		case PRIEST:
		case WIZARD:
		case GENERAL:
		case CALVARY:
		case CATAPULT:
		default:
			break;
		}

		return true;
	}
	if((pDI->KeyPressed(DIK_NUMPAD2) || pDI->JoystickButtonPressed( 3, JOYSTICKID2 )) == true && CBattlePlay::GetInstance()->GetCurrentUnit())//USING AN ABILITY
	{
		if(m_bIsCastingAbility == true || CBattlePlay::GetInstance()->GetCurrentUnit()->GetAbility2_Cooldown() > 0)
		{
			return false;
		}

		switch(CBattlePlay::GetInstance()->GetCurrentUnit()->GetUnitID())
		{
		case SKELETALWARRIOR:
			break;
		case NECROMANCER:
			{
				CCreateAbilityTwoMessage* msg = new CCreateAbilityTwoMessage(CBattlePlay::GetInstance()->GetCurrentUnit());
				CSGD_MessageSystem::GetInstance()->QueueMessage(msg);
				msg = nullptr;
			}
			break;
		case DEATHKNIGHT:
			break;
		case SHADOWPRIEST:
			{
				for(unsigned int i = 0; i < m_vUnits.size(); i++)
				{
					if(m_vUnits[i]->GetPosX() == CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocX() &&
						m_vUnits[i]->GetPosY() == CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocY())
					{
						if(CBattlePlay::GetInstance()->GetCurrentUnit()->IsInRange(m_vUnits[i]))
						{
							CBattlePlay::GetInstance()->SetCurrentTarget(m_vUnits[i]);


							CCreateAbilityTwoMessage* msg = new CCreateAbilityTwoMessage(CBattlePlay::GetInstance()->GetCurrentUnit());
							CSGD_MessageSystem::GetInstance()->QueueMessage(msg);
							msg = nullptr;
						}
					}
				}
			}
			break;
		case WARMACHINE:
			break;
		case LICH:
			{
				CCreateAbilityTwoMessage* msg = new CCreateAbilityTwoMessage(CBattlePlay::GetInstance()->GetCurrentUnit());
				CSGD_MessageSystem::GetInstance()->QueueMessage(msg);
				msg = nullptr;
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


		return true;

	}
	if(pDI->KeyPressed(DIK_RETURN) == true || pDI->JoystickButtonPressed( 1, JOYSTICKID2 ) == true )
	{
		if (CMainMenu::GetInstance()->GetArcade() == true )
		{
			currentX = CArcadeGamePlay::GetInstance()->GetCurX();
		}
		else if (CMainMenu::GetInstance()->GetCampaign() == true)
		{
			currentX = CGamePlay::GetInstance()->GetCurX();
		}
		else if (CMainMenu::GetInstance()->GetVersus() == true || CMainMenu::GetInstance()->GetQuickMatch() == true)
		{
			currentX = CWorldPlay::GetInstance()->GetCurX();
		}

		if(m_bIsCastingAbility == true)
			return true;

		else
		{

			if(CBattlePlay::GetInstance()->GetCurrentUnit() == nullptr)
			{
				for(unsigned int i = 0; i < m_vUnits.size(); i++)
				{
					if(m_vUnits[i]->GetCountry() == currentX)
					{
						if(CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocX() == m_vUnits[i]->GetPosX() &&
							CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocY() == m_vUnits[i]->GetPosY()&& !(m_vUnits[i]->HasAttacked() && m_vUnits[i]->HasMoved()))
						{
							CBattlePlay::GetInstance()->SetCurrentUnit(m_vUnits[i]);
							CBattlePlay::GetInstance()->SetShowUnitOptions(true);
							break;
						}
					}
				}
				if(CBattlePlay::GetInstance()->GetCurrentUnit() == nullptr)
					return false;
				return true;
			}
			else if(CBattlePlay::GetInstance()->GetCurrentUnit() != nullptr && !CBattlePlay::GetInstance()->GetCurrentUnit()->HasMoved() && 
				!CBattlePlay::GetInstance()->GetCurrentUnit()->IsAttacking())
			{
				return CBattlePlay::GetInstance()->GetCurrentUnit()->Move(CBattlePlay::GetInstance()->GetGrid()->GetCurrent());
			}
			if(CBattlePlay::GetInstance()->GetCurrentUnit()->IsAttacking())//attack a unit
			{
				return CBattlePlay::GetInstance()->CheckForAttack();
			}

			return false;
		}
	}
	return true;
}


void CEnemy::Render(void)
{
	bool tempWorld;
	if (CMainMenu::GetInstance()->GetArcade() == true )
	{
		tempWorld = CArcadeGamePlay::GetInstance()->GetIsInWorld();
	}
	else if (CMainMenu::GetInstance()->GetCampaign() == true)
	{
		tempWorld =  CGamePlay::GetInstance()->GetIsInWorld();
	}
	else if (CMainMenu::GetInstance()->GetVersus() == true || CMainMenu::GetInstance()->GetQuickMatch() == true)
	{
		tempWorld = CWorldPlay::GetInstance()->GetIsInWorld();
	}

	if (CMainMenu::GetInstance()->GetArcade() == true )
	{
		currentX = CArcadeGamePlay::GetInstance()->GetCurX();
		currentY = CArcadeGamePlay::GetInstance()->GetCurY();

	}
	else if (CMainMenu::GetInstance()->GetCampaign() == true)
	{
		currentX = CGamePlay::GetInstance()->GetCurX();
		currentY = CGamePlay::GetInstance()->GetCurY();

	}
	else if (CMainMenu::GetInstance()->GetVersus() == true || CMainMenu::GetInstance()->GetQuickMatch() == true)
	{
		currentX = CWorldPlay::GetInstance()->GetCurX();
		currentY = CWorldPlay::GetInstance()->GetCurY();

	}


	if (tempWorld)
	{
		CBasePlayer::Render();
	}
	else
	{
		for(unsigned int i = 0; i < m_vUnits.size(); i++)
		{
			if(m_vUnits[i]->GetCountry() == currentX && m_vUnits[i]->GetTerritory() == currentY)
				m_vUnits[i]->Render();
		}
	}


}

void CEnemy::AddUnit(CBaseUnit* unit)
{
	m_vUnits.push_back(unit);
	if(unit->GetUnitID() == VOLATILEGHOUL)
		unitsInBattle++;
}


void CEnemy::LoadUnitsOnBattleGrid(void)
{
/*	if (CMainMenu::GetInstance()->GetArcade() == true )
	{
		currentX = CArcadeGamePlay::GetInstance()->GetCurX();
		currentY = CArcadeGamePlay::GetInstance()->GetCurY();

	}
	else */
	if (CMainMenu::GetInstance()->GetCampaign() == true)
	{
		currentX = CGamePlay::GetInstance()->GetCurX();
		currentY = CGamePlay::GetInstance()->GetCurY();

	}
	else if (CMainMenu::GetInstance()->GetVersus() == true || CMainMenu::GetInstance()->GetQuickMatch() == true)
	{
		currentX = CWorldPlay::GetInstance()->GetCurX();
		currentY = CWorldPlay::GetInstance()->GetCurY();

	}
	unitsInBattle = 0;
	vector<vector<CTile*>> tiles = CBattlePlay::GetInstance()->GetGrid()->GetTiles();
	for(unsigned int i = 0; i < m_vUnits.size(); i++)
	{
		if(m_vUnits[i]->GetCountry() == currentX && m_vUnits[i]->GetTerritory() == currentY)
		{
			m_vUnits[i]->SetTile(tiles[i][10]);
			m_vUnits[i]->GetTile()->SetOccupied(true);
			m_vUnits[i]->SetPosX(24*32.0f);
			m_vUnits[i]->SetPosY((i+5)*32.0f);
			unitsInBattle += 1;
		}
	}
}

void CEnemy::HandleEvent( const CEvent* pEvent )
{

	if( pEvent->GetEventID() == "LICH_BUFF" )
	{
		for(unsigned int i = 0; i < m_vUnits.size(); i++)
		{
			if(m_vUnits[i]->GetGeneralBuff() == false)
			{
				m_vUnits[i]->SetGeneralBuff(true);
				m_vUnits[i]->SetMaxHealth(m_vUnits[i]->GetMaxHealth() + 5);
				m_vUnits[i]->SetCurrentHealth(m_vUnits[i]->GetCurrentHealth() + 5);

				std::string text = "Empowering Curse";
				CFloatingText* newText = new CFloatingText(text, (float)m_vUnits[i]->GetPosX(), (float)m_vUnits[i]->GetPosY(), BUFF);
				CBattlePlay::GetInstance()->PushText(newText);
			}
		}
	}

	if( pEvent->GetEventID() == "DARK_CURSE" )
	{
		if(darkCurseActive != nullptr)
			dynamic_cast<CDarkCurse*>(darkCurseActive)->DealDamage();
	}

}

void CEnemy::ActivateDarkCurse(CAbility* theCurse)
{
	if(theCurse == nullptr)
		darkCurseActive = nullptr;

	if(darkCurseActive != nullptr)
		delete darkCurseActive;

	if(theCurse == darkCurseActive)
		return;

	if(darkCurseActive == nullptr)
		darkCurseActive = theCurse;
}




void CEnemy::AddBuilding(CBaseBuilding* b)
{
	m_vBuildings.push_back(b);
}


void CEnemy::DeleteBuilding(int index)
{
	
	for (auto it = m_vBuildings.begin(); it != m_vBuildings.end(); ++it) 
	{
		int deleteindex = std::distance(m_vBuildings.begin(), it);
		if (deleteindex == index)
		{
			delete m_vBuildings[deleteindex];
			m_vBuildings.erase( m_vBuildings.begin() + index );
			break;
		}
	}
	
}




void CEnemy::UpdateAI(float fElapsedTime)
{
	AIDelay += fElapsedTime;
	int attackOrAbility = rand() % 2;
	if (CMainMenu::GetInstance()->GetArcade() == true )
	{
		currentX = CArcadeGamePlay::GetInstance()->GetCurX();
	}
	else if (CMainMenu::GetInstance()->GetCampaign() == true)
	{
		currentX = CGamePlay::GetInstance()->GetCurX();
	}
	else if (CMainMenu::GetInstance()->GetVersus() == true || CMainMenu::GetInstance()->GetQuickMatch() == true)
	{
		currentX = CWorldPlay::GetInstance()->GetCurX();
	}

	if(AIDelay > 1.0f)
	{
		if(CBattlePlay::GetInstance()->GetCurrentUnit() == nullptr)//no current unit, so try to find one.
		{
			for(unsigned int i = 0; i <	m_vUnits.size(); i++)
			{
				if(m_vUnits[i]->GetCountry() == currentX)
				{
					if(!m_vUnits[i]->HasAttacked())
					{
						CBattlePlay::GetInstance()->SetCurrentUnit(m_vUnits[i]);
						CBattlePlay::GetInstance()->GetGrid()->ChangeCurrent((int)m_vUnits[i]->GetPosX(), (int)m_vUnits[i]->GetPosY());
						AIDelay = 0.0f;
						CBattlePlay::GetInstance()->SetShowUnitOptions(false);
						return;

					}
				}
			}
			CBattlePlay::GetInstance()->ChangeTurn();
		}
		else//move the unit, then attack
		{
			vector<CBaseUnit*> playerUnits = CPlayer::GetInstance()->GetUnits();
			int inRange = -1;



			CBaseUnit* unit = CBattlePlay::GetInstance()->GetCurrentUnit();	
			if(!unit->HasMoved() && inRange < 0)
			{
				CBaseUnit* target;
				for(unsigned int i = 0; i < playerUnits.size();i++)
				{
					if(playerUnits[i]->GetCountry() == CGamePlay::GetInstance()->GetCurX())
						target = playerUnits[i];
				}


				int distanceX = (int)(_CMATH_::abs(target->GetPosX() - CBattlePlay::GetInstance()->GetCurrentUnit()->GetPosX()));
				int distanceY = (int)(_CMATH_::abs(target->GetPosY() - CBattlePlay::GetInstance()->GetCurrentUnit()->GetPosY()));

				int horizontal = distanceY/32;
				int vertical = distanceX/32;


				int spaces = unit->GetMovement();

				if(spaces == 1)
				{
					if( horizontal >= vertical )
					{
						horizontal = 1;
						vertical = 0;
					}
					else if(vertical > horizontal)
					{
						horizontal = 0;
						vertical = 1;
					}
				}
				else
				{
					while(horizontal > spaces/2)
						horizontal--;

					while(vertical > spaces/2)
						vertical--;
				}



				if(target->GetPosX() < unit->GetPosX()) vertical *= -1;
				if(target->GetPosY() < unit->GetPosY()) horizontal *= -1;



				CBattlePlay::GetInstance()->GetGrid()->ChangeCurrent((int)(unit->GetPosX()+vertical*32), (int)(unit->GetPosY()+horizontal*32));
				unit->Move(CBattlePlay::GetInstance()->GetGrid()->GetCurrent());



				CBaseUnit* unit = CBattlePlay::GetInstance()->GetCurrentUnit();	
				if(!unit->HasMoved() && inRange < 0)
				{
					int spaces = unit->GetMovement();
					CBattlePlay::GetInstance()->GetGrid()->ChangeCurrent((int)unit->GetPosX()-(rand() % spaces+1)*32, (int)unit->GetPosY());
					unit->Move(CBattlePlay::GetInstance()->GetGrid()->GetCurrent());

					AIDelay = 0.0f;
					CBattlePlay::GetInstance()->SetShowUnitOptions(false);
					return;
				}		
				else if(!unit->HasAttacked())
				{	


					for(unsigned int i = 0; i < playerUnits.size(); i++)
					{
						if(playerUnits[i]->GetCountry() == CGamePlay::GetInstance()->GetCurX())
						{
							if(CBattlePlay::GetInstance()->GetCurrentUnit()->IsInRange(playerUnits[i]))
							{
								inRange = i;
								CBattlePlay::GetInstance()->SetCurrentTarget(playerUnits[i]);
							}
						}

					}


					if(attackOrAbility == 0)
					{
						if(inRange >= 0)
							unit->Attack(playerUnits[inRange]);	
						else
							unit->Attack(nullptr);

						AIDelay = 0.0f;
						CBattlePlay::GetInstance()->SetShowUnitOptions( false );
					}		
					else if (!unit->HasAttacked() )
					{
						if (inRange >=0)
						{
							unit->Attack( playerUnits[inRange] );
						}
						else
						{
							unit->Attack(nullptr );
						}
						CBattlePlay::GetInstance()->SetCurrentUnit( nullptr );
					}
					else if(attackOrAbility == 1 )
					{
						switch(unit->GetUnitID())
						{
						case SKELETALWARRIOR:
							{
								CCreateAbilityMessage* msg = new CCreateAbilityMessage(CBattlePlay::GetInstance()->GetCurrentUnit());
								CSGD_MessageSystem::GetInstance()->QueueMessage(msg);
								msg = nullptr;
							}
							break;
						case NECROMANCER:
							{
								vector<CBaseUnit*> temp = CPlayer::GetInstance()->GetUnits();
								for(unsigned int i = 0; i < temp.size(); i++)
								{
									if(temp[i]->GetPosX() == CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocX() &&
										temp[i]->GetPosY() == CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocY())
									{
										if(CBattlePlay::GetInstance()->GetCurrentUnit()->IsInRange(temp[i]))
										{
											CBattlePlay::GetInstance()->SetCurrentTarget(temp[i]);

											CCreateAbilityMessage* msg = new CCreateAbilityMessage(CBattlePlay::GetInstance()->GetCurrentUnit());
											CSGD_MessageSystem::GetInstance()->QueueMessage(msg);
											msg = nullptr;
										}
									}
								}
							}
							break;
						case DEATHKNIGHT:
							{
								vector<CBaseUnit*> temp = CPlayer::GetInstance()->GetUnits();
								for(unsigned int i = 0; i < temp.size(); i++)
								{
									if(temp[i]->GetPosX() == CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocX() &&
										temp[i]->GetPosY() == CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocY())
									{
										int targetDistance = 0;
										targetDistance += int(_CMATH_::abs(temp[i]->GetPosX()-CBattlePlay::GetInstance()->GetCurrentUnit()->GetPosX()));
										targetDistance += int(_CMATH_::abs(temp[i]->GetPosY()-CBattlePlay::GetInstance()->GetCurrentUnit()->GetPosY()));

										bool inTheWay = true;

										if(temp[i]->GetPosX() > CBattlePlay::GetInstance()->GetCurrentUnit()->GetPosX() && temp[i]->GetPosY() == CBattlePlay::GetInstance()->GetCurrentUnit()->GetPosY())
										{
											for(int dist = 0; dist < ((targetDistance) / 32); dist++)
											{
												inTheWay = CBattlePlay::GetInstance()->GetGrid()->GetATile((int)CBattlePlay::GetInstance()->GetCurrentUnit()->GetPosX() / 32  + dist ,(int)CBattlePlay::GetInstance()->GetCurrentUnit()->GetPosY() / 32)->GetOccupied();
												if(inTheWay)
													break;
											}
										}

										else if(temp[i]->GetPosX() < CBattlePlay::GetInstance()->GetCurrentUnit()->GetPosX() && temp[i]->GetPosY() == CBattlePlay::GetInstance()->GetCurrentUnit()->GetPosY())
										{
											for(int dist = 0; dist < ((targetDistance) / 32); dist++)
											{
												inTheWay = CBattlePlay::GetInstance()->GetGrid()->GetATile((int)CBattlePlay::GetInstance()->GetCurrentUnit()->GetPosX() / 32  - dist, (int)CBattlePlay::GetInstance()->GetCurrentUnit()->GetPosY() / 32)->GetOccupied();
												if(inTheWay)
													break;
											}
										}

										else if(temp[i]->GetPosX() == CBattlePlay::GetInstance()->GetCurrentUnit()->GetPosX() && temp[i]->GetPosY() > CBattlePlay::GetInstance()->GetCurrentUnit()->GetPosY())
										{
											for(int dist = 0; dist < ((targetDistance) / 32); dist++)
											{
												inTheWay = CBattlePlay::GetInstance()->GetGrid()->GetATile((int)CBattlePlay::GetInstance()->GetCurrentUnit()->GetPosX() / 32, (int)CBattlePlay::GetInstance()->GetCurrentUnit()->GetPosY() / 32 + dist )->GetOccupied();
												if(inTheWay)
													break;
											}
										}

										else if(temp[i]->GetPosX() == CBattlePlay::GetInstance()->GetCurrentUnit()->GetPosX() && temp[i]->GetPosY() < CBattlePlay::GetInstance()->GetCurrentUnit()->GetPosY())
										{
											for(int dist = 0; dist < ((targetDistance) / 32); dist++)
											{
												inTheWay = CBattlePlay::GetInstance()->GetGrid()->GetATile((int)CBattlePlay::GetInstance()->GetCurrentUnit()->GetPosX() / 32, (int)CBattlePlay::GetInstance()->GetCurrentUnit()->GetPosY() / 32 - dist )->GetOccupied();
												if(inTheWay)
													break;
											}
										}



										if(CBattlePlay::GetInstance()->GetCurrentUnit()->GetMovement() >= (targetDistance / 32) && inTheWay == false) //Can hit target if inTheWay is false && within movement range
										{
											CBattlePlay::GetInstance()->SetCurrentTarget(temp[i]);

											CCreateAbilityMessage* msg = new CCreateAbilityMessage(CBattlePlay::GetInstance()->GetCurrentUnit());
											CSGD_MessageSystem::GetInstance()->QueueMessage(msg);
											msg = nullptr;
										}
									}
								}
							}
							break;
						case SHADOWPRIEST:
							{
								vector<CBaseUnit*> temp = CPlayer::GetInstance()->GetUnits();
								for(unsigned int i = 0; i < temp.size(); i++)
								{
									if(temp[i]->GetPosX() == CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocX() &&
										temp[i]->GetPosY() == CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocY())
									{
										if(CBattlePlay::GetInstance()->GetCurrentUnit()->IsInRange(temp[i]))
										{
											CBattlePlay::GetInstance()->SetCurrentTarget(temp[i]);


											CCreateAbilityMessage* msg = new CCreateAbilityMessage(CBattlePlay::GetInstance()->GetCurrentUnit());
											CSGD_MessageSystem::GetInstance()->QueueMessage(msg);
											msg = nullptr;
										}
									}
								}
							}
							break;
						case WARMACHINE:
							{
								CCreateAbilityMessage* msg = new CCreateAbilityMessage(CBattlePlay::GetInstance()->GetCurrentUnit());
								CSGD_MessageSystem::GetInstance()->QueueMessage(msg);
								msg = nullptr;
							}
							break;
						case LICH:
							{
								CCreateAbilityMessage* msg = new CCreateAbilityMessage(CBattlePlay::GetInstance()->GetCurrentUnit());
								CSGD_MessageSystem::GetInstance()->QueueMessage(msg);
								msg = nullptr;
							}
							break;
						case VOLATILEGHOUL:
							{
								CCreateAbilityMessage* msg = new CCreateAbilityMessage(CBattlePlay::GetInstance()->GetCurrentUnit());
								CSGD_MessageSystem::GetInstance()->QueueMessage(msg);
								msg = nullptr;
							}
							break;
						case FOOTSOLDIERS:
						case PRIEST:
						case WIZARD:
						case GENERAL:
						case CALVARY:
						case CATAPULT:
						default:
							break;


						}
						unit->Attack(nullptr);
					}

				}

				CBattlePlay::GetInstance()->SetShowUnitOptions(false);

				AIDelay = 0.0f;
				CBattlePlay::GetInstance()->SetCurrentUnit(nullptr);
			}
		}
	}
}



void CEnemy::Reset()
{
	for(unsigned int i = 0; i < m_vUnits.size(); i++)
	{
		delete m_vUnits[i];
	}

	m_vUnits.clear();

	for (unsigned int i = 0; i < m_vBuildings.size(); i++)
	{
		delete m_vBuildings[i];
	}

	m_vBuildings.clear();


	//Reinitialize
	m_bSinglePlayer = true;
	AIDelay = 0.0f;
	unitsInBattle = 0;

	type = PlayerType::ENEMY;
	CBaseUnit* skeleton = new CBaseUnit();
	skeleton-> CreateUnit( SKELETALWARRIOR, 1, 0 );
	skeleton->SetPosX(6*32);
	skeleton->SetPosY(3*32);


	CBaseUnit* necro = new CBaseUnit();
	necro-> CreateUnit( NECROMANCER, 1, 0 );
	necro->SetPosX(12*32);
	necro->SetPosY(4*32);


	CBaseUnit* ghoul = new CBaseUnit();
	ghoul-> CreateUnit( VOLATILEGHOUL, 1, 0 );
	ghoul->SetPosX(12*32);
	ghoul->SetPosY(5*32);

	CBaseUnit* shadowPriest = new CBaseUnit();
	shadowPriest-> CreateUnit( SHADOWPRIEST, 1, 0 );
	shadowPriest->SetPosX(6*32);
	shadowPriest->SetPosY(5*32);

	CBaseUnit* warMachine = new CBaseUnit();
	warMachine-> CreateUnit( WARMACHINE, 1, 0 );
	warMachine->SetPosX(12*32);
	warMachine->SetPosY(7*32);

	CBaseUnit* deathKnight = new CBaseUnit();
	deathKnight-> CreateUnit( DEATHKNIGHT, 1, 0 );
	deathKnight->SetPosX(12*32);
	deathKnight->SetPosY(8*32);

	CBaseUnit* lich = new CBaseUnit();
	lich-> CreateUnit( LICH, 1, 0 );
	lich->SetPosX(12*32);
	lich->SetPosY(9*32);

	m_vUnits.push_back( skeleton );
	m_vUnits.push_back( necro );
	m_vUnits.push_back( ghoul );
	m_vUnits.push_back( shadowPriest );
	m_vUnits.push_back( warMachine );
	m_vUnits.push_back( deathKnight );
	m_vUnits.push_back( lich );


	SetGeneralImageID( CSGD_TextureManager::GetInstance()->LoadTexture( _T( "resources/graphics/UndeadGeneral.png"), D3DCOLOR_XRGB(255,255,255) ) );
	SetPosX( 500 );
	SetPosY( 300 );

	SetSteel( 100 );
	SetWood ( 300 );
	SetFood ( 100 );

	CSGD_EventSystem::GetInstance()->RegisterClient("LICH_BUFF", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("DARK_CURSE", this);
	darkCurseActive = nullptr;
	m_bIsCastingAbility = false;
}
