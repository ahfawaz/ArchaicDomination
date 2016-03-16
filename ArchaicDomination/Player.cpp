#include "Player.h"
#include "SGD Wrappers\CSGD_DirectInput.h"
#include "BaseUnit.h"
#include "BattlePlay.h"
#include "SGD Wrappers\CSGD_TextureManager.h"
#include "GamePlay.h"
#include "SGD Wrappers\CSGD_MessageSystem.h"
#include "SGD Wrappers\CSGD_EventSystem.h"
#include "CreateAbilityMessage.h"
#include "CreateAbilityTwoMessage.h"
#include "Enemy.h"
#include "Game.h"
#include "BattleCry.h"
#include "Judgement.h"
#include "Particle System\ParticleSystem.h"
#include "MainMenu.h"
#include "ArcadeGamePlay.h"
#include "WorldPlay.h"
#include "BaseBuilding.h"



////////////////////////////////
//Singleton
CPlayer * CPlayer::GetInstance ( void ) 
{
	//Global Instance
	static CPlayer s_Instance;
	return &s_Instance;

}

CPlayer::CPlayer(void)
{
	type = PlayerType::PLAYER;
	SetCounterForRemovingBuildings( 0 );

	unitsInBattle = 0;
	//HUMAN
	//////////////////////////////////////////////////////////////
	CBaseUnit* tempunit = new CBaseUnit();
	tempunit-> CreateUnit( FOOTSOLDIERS, 2, 0 );
	tempunit->SetPosX(0);
	tempunit->SetPosY(3*32);




	CBaseUnit* tempunit2 = new CBaseUnit();
	tempunit2-> CreateUnit( PRIEST, 2, 0 );
	tempunit2->SetPosX(0);
	tempunit2->SetPosY(4*32);



	CBaseUnit* tempunit3 = new CBaseUnit();
	tempunit3-> CreateUnit( WIZARD, 2, 0 );
	tempunit3->SetPosX(0);
	tempunit3->SetPosY(5*32);


	CBaseUnit* tempunit4 = new CBaseUnit();
	tempunit4-> CreateUnit( CALVARY, 2, 0 );
	tempunit4->SetPosX(0);
	tempunit4->SetPosY(6*32);


	CBaseUnit* tempunit5 = new CBaseUnit();
	tempunit5-> CreateUnit( CATAPULT, 2, 0 );
	tempunit5->SetPosX(0*32);
	tempunit5->SetPosY(7*32);


	CBaseUnit* tempunit6 = new CBaseUnit();
	tempunit6-> CreateUnit(GENERAL, 2, 0);
	tempunit6->SetPosX(0);
	tempunit6->SetPosY(8*32);

	//////////////////////////////////////////////////////////////
	//AoE Test units
	CBaseUnit* aoeUnit = new CBaseUnit();
	aoeUnit-> CreateUnit( FOOTSOLDIERS, 2, 0 );
	aoeUnit->SetPosX(5*32);
	aoeUnit->SetPosY(3*32);


	CBaseUnit* aoeUnit1 = new CBaseUnit();
	aoeUnit1-> CreateUnit( FOOTSOLDIERS, 2, 0 );
	aoeUnit1->SetPosX(7*32);
	aoeUnit1->SetPosY(3*32);



	CBaseUnit* aoeUnit2 = new CBaseUnit();
	aoeUnit2-> CreateUnit( FOOTSOLDIERS, 2, 0 );
	aoeUnit2->SetPosX(6*32);
	aoeUnit2->SetPosY(2*32);



	CBaseUnit* aoeUnit3 = new CBaseUnit();
	aoeUnit3-> CreateUnit( FOOTSOLDIERS, 2, 0 );
	aoeUnit3->SetPosX(6*32);
	aoeUnit3->SetPosY(4*32);



	CBaseUnit* aoeUnit4 = new CBaseUnit();
	aoeUnit4-> CreateUnit( FOOTSOLDIERS, 2, 0 );
	aoeUnit4->SetPosX(5*32);
	aoeUnit4->SetPosY(4*32);



	CBaseUnit* aoeUnit5 = new CBaseUnit();
	aoeUnit5-> CreateUnit( FOOTSOLDIERS, 2, 0 );
	aoeUnit5->SetPosX(7*32);
	aoeUnit5->SetPosY(2*32);



	m_vUnits.push_back( aoeUnit );
	m_vUnits.push_back( aoeUnit1);
	m_vUnits.push_back( aoeUnit2);
	m_vUnits.push_back( aoeUnit3);
	m_vUnits.push_back( aoeUnit4);
	m_vUnits.push_back( aoeUnit5);
	//Ending AoE Test units
	///////////////////////////////////////////////////////

	m_vUnits.push_back( tempunit );
	m_vUnits.push_back( tempunit2 );
	m_vUnits.push_back( tempunit3 );
	m_vUnits.push_back( tempunit4 );
	m_vUnits.push_back( tempunit5 );
	m_vUnits.push_back( tempunit6 );



	SetGeneralImageID( CSGD_TextureManager::GetInstance()->LoadTexture( _T( "resources/graphics/HumanGeneral.png"), D3DCOLOR_XRGB(255,255,255) ) );
	SetPosX( 180 );
	SetPosY( 300 );


	SetSteel( 10 );
	SetWood ( 30 );
	SetFood ( 25 );

	CSGD_EventSystem::GetInstance()->RegisterClient("GENERAL_BUFF", this);


	m_bIsCastingAbility = false;
}


CPlayer::~CPlayer(void)
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

void CPlayer::Update(float fElapsedTime)
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
	for(unsigned int i = 0; i < m_vUnits.size(); i++)
	{
		if(m_vUnits[i]->GetCountry() == currentX)
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
	}

	int moveCount = 0;
	for(unsigned int i = 0; i < m_vUnits.size(); i++)
	{
		if(m_vUnits[i]->GetCountry() == currentX)
		{
			if(m_vUnits[i]->HasAttacked() && m_vUnits[i]->HasMoved())
				++moveCount;
		}
	}
	if(moveCount == unitsInBattle && unitsInBattle != NULL) 
		CBattlePlay::GetInstance()->ChangeTurn(); 
}

bool CPlayer::Input(void)
{
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();

	if(pDI->KeyPressed(DIK_W) == true || pDI->JoystickGetLStickDirPressed( DIR_UP, JOYSTICKID1) == true)
	{
		if(CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocY() > 0)
			CBattlePlay::GetInstance()->GetGrid()->ChangeCurrent(CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocX(), 
			CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocY()-32);

		if(CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocY() < CBattlePlay::GetInstance()->GetOffY())
		{
			CBattlePlay::GetInstance()->SetOffY(CBattlePlay::GetInstance()->GetOffY()-32);
		}
		//Play sound here
		CGame::GetInstance()->PlayMoveCursorSFX();
		return true;
	}
	if(pDI->KeyPressed(DIK_D) == true || pDI->JoystickGetLStickDirPressed( DIR_RIGHT, JOYSTICKID1) == true)
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

	if(pDI->KeyPressed(DIK_S) == true || pDI->JoystickGetLStickDirPressed( DIR_DOWN, JOYSTICKID1) == true)
	{
		if(CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocY() < 992)
		{
			CBattlePlay::GetInstance()->GetGrid()->ChangeCurrent(CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocX(), 
				CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocY()+32);

			if(CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocY() >= CGame::GetInstance()->GetScreenHeight() + CBattlePlay::GetInstance()->GetOffY())
			{
				CBattlePlay::GetInstance()->SetOffY(CBattlePlay::GetInstance()->GetOffY()+32);

			}
		}
		CGame::GetInstance()->PlayMoveCursorSFX();
		return true;
	}
	if(pDI->KeyPressed(DIK_A) == true || pDI->JoystickGetLStickDirPressed( DIR_LEFT, JOYSTICKID1) == true)
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
	if((pDI->KeyPressed(DIK_1) || pDI->JoystickButtonPressed( 2, JOYSTICKID1 )) == true && CBattlePlay::GetInstance()->GetCurrentUnit())//USING AN ABILITY
	{
		if(m_bIsCastingAbility == true || CBattlePlay::GetInstance()->GetCurrentUnit()->GetAbility1_Cooldown() > 0)
			return false;

		switch(CBattlePlay::GetInstance()->GetCurrentUnit()->GetUnitID())
		{
		case FOOTSOLDIERS:
			{

				CCreateAbilityMessage* msg = new CCreateAbilityMessage(CBattlePlay::GetInstance()->GetCurrentUnit());
				CSGD_MessageSystem::GetInstance()->QueueMessage(msg);
				msg = nullptr;

			}
			break;
		case PRIEST:
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
		case WIZARD:
			{
				vector<CBaseUnit*> temp = CEnemy::GetInstance()->GetUnits();
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
		case GENERAL:
			{
				CCreateAbilityMessage* msg = new CCreateAbilityMessage(CBattlePlay::GetInstance()->GetCurrentUnit());
				CSGD_MessageSystem::GetInstance()->QueueMessage(msg);
				msg = nullptr;
			}
			break;
		case CALVARY:
			{
				vector<CBaseUnit*> temp = CEnemy::GetInstance()->GetUnits();
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
		case CATAPULT:
			{
				CCreateAbilityMessage* msg = new CCreateAbilityMessage(CBattlePlay::GetInstance()->GetCurrentUnit());
				CSGD_MessageSystem::GetInstance()->QueueMessage(msg);
				msg = nullptr;
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

		return true;
	}
	if((pDI->KeyPressed(DIK_2) || pDI->JoystickButtonPressed( 3, JOYSTICKID1 )) == true && CBattlePlay::GetInstance()->GetCurrentUnit())//USING AN ABILITY2
	{
		if(m_bIsCastingAbility == true || CBattlePlay::GetInstance()->GetCurrentUnit()->GetAbility2_Cooldown() > 0)
			return false;

		switch(CBattlePlay::GetInstance()->GetCurrentUnit()->GetUnitID())
		{

		case FOOTSOLDIERS:
			break;
		case PRIEST:
			{
				CCreateAbilityTwoMessage* msg = new CCreateAbilityTwoMessage(CBattlePlay::GetInstance()->GetCurrentUnit());
				CSGD_MessageSystem::GetInstance()->QueueMessage(msg);
				msg = nullptr;
				break;
			}
			break;
		case WIZARD:
			{
				vector<CBaseUnit*> temp = CEnemy::GetInstance()->GetUnits();
				for(unsigned int i = 0; i < temp.size(); i++)
				{
					if(temp[i]->GetPosX() == CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocX() &&
						temp[i]->GetPosY() == CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocY())
					{
						if(CBattlePlay::GetInstance()->GetCurrentUnit()->IsInRange(temp[i]))
						{
							CCreateAbilityTwoMessage* msg = new CCreateAbilityTwoMessage(CBattlePlay::GetInstance()->GetCurrentUnit());
							CSGD_MessageSystem::GetInstance()->QueueMessage(msg);
							msg = nullptr;
							break;
						}
					}
				}

			}
			break;
		case GENERAL:
			{
				CCreateAbilityTwoMessage* msg = new CCreateAbilityTwoMessage(CBattlePlay::GetInstance()->GetCurrentUnit());
				CSGD_MessageSystem::GetInstance()->QueueMessage(msg);
				msg = nullptr;
			}
			break;
		case CALVARY:
		case CATAPULT:
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

		return true;
	}
	if(pDI->KeyPressed(DIK_TAB) == true || pDI->JoystickButtonPressed( 1, JOYSTICKID1 ) == true)
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
			if(CBattlePlay::GetInstance()->GetCurrentUnit() == nullptr)//selecting a unit to move
			{
				for(unsigned int i = 0; i < m_vUnits.size(); i++)
				{
					if(m_vUnits[i]->GetCountry() == currentX)
					{
						if(CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocX() == m_vUnits[i]->GetPosX() &&
							CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocY() == m_vUnits[i]->GetPosY() && !(m_vUnits[i]->HasAttacked() && m_vUnits[i]->HasMoved()))
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
				!CBattlePlay::GetInstance()->GetCurrentUnit()->IsAttacking())//moving the selected unit
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
void CPlayer::Render(void)
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

void CPlayer::LoadUnitsOnBattleGrid(void)
{

/*	if (CMainMenu::GetInstance()->GetCampaign() == true)
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
			m_vUnits[i]->SetTile(tiles[i][0]); 
			m_vUnits[i]->GetTile()->SetOccupied(true);
			m_vUnits[i]->SetPosX(0);
			m_vUnits[i]->SetPosY((i+5)*32.0f);
			unitsInBattle += 1;
		}
	}

}


void CPlayer::AddBuilding(CBaseBuilding* b)
{
	m_vBuildings.push_back(b);
}

void CPlayer::DeleteBuilding(int index)
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

void CPlayer::HandleEvent( const CEvent* pEvent )
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


	if( pEvent->GetEventID() == "GENERAL_BUFF" )
	{
		for(unsigned int i = 0; i < m_vUnits.size(); i++)
		{
			if(m_vUnits[i]->GetCountry() == currentX)
			{
				if(m_vUnits[i]->GetGeneralBuff() == false)
				{
					m_vUnits[i]->SetGeneralBuff(true);

					std::string text = "BattleCry";
					CFloatingText* newText = new CFloatingText(text, (float)m_vUnits[i]->GetPosX(), (float)m_vUnits[i]->GetPosY(), BUFF);
					CBattlePlay::GetInstance()->PushText(newText);
				}
			}
		}

	}

}




void CPlayer::AddUnit(CBaseUnit* u)
{
	m_vUnits.push_back(u);
}

void CPlayer::ClearPlayerinfo()
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

void CPlayer::Reset()
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





	//Reinitialilze
	type = PlayerType::PLAYER;
	SetCounterForRemovingBuildings( 0 );

	unitsInBattle = 0;
	//HUMAN
	//////////////////////////////////////////////////////////////
	CBaseUnit* tempunit = new CBaseUnit();
	tempunit-> CreateUnit( FOOTSOLDIERS, 2, 0 );
	tempunit->SetPosX(0);
	tempunit->SetPosY(3*32);




	CBaseUnit* tempunit2 = new CBaseUnit();
	tempunit2-> CreateUnit( PRIEST, 2, 0 );
	tempunit2->SetPosX(0);
	tempunit2->SetPosY(4*32);



	CBaseUnit* tempunit3 = new CBaseUnit();
	tempunit3-> CreateUnit( WIZARD, 2, 0 );
	tempunit3->SetPosX(0);
	tempunit3->SetPosY(5*32);


	CBaseUnit* tempunit4 = new CBaseUnit();
	tempunit4-> CreateUnit( CALVARY, 2, 0 );
	tempunit4->SetPosX(0);
	tempunit4->SetPosY(6*32);


	CBaseUnit* tempunit5 = new CBaseUnit();
	tempunit5-> CreateUnit( CATAPULT, 2, 0 );
	tempunit5->SetPosX(0*32);
	tempunit5->SetPosY(7*32);


	CBaseUnit* tempunit6 = new CBaseUnit();
	tempunit6-> CreateUnit(GENERAL, 0, 0);
	tempunit6->SetPosX(0);
	tempunit6->SetPosY(8*32);

	//////////////////////////////////////////////////////////////
	//AoE Test units
	CBaseUnit* aoeUnit = new CBaseUnit();
	aoeUnit-> CreateUnit( FOOTSOLDIERS, 2, 0 );
	aoeUnit->SetPosX(5*32);
	aoeUnit->SetPosY(3*32);


	CBaseUnit* aoeUnit1 = new CBaseUnit();
	aoeUnit1-> CreateUnit( FOOTSOLDIERS, 2, 0 );
	aoeUnit1->SetPosX(7*32);
	aoeUnit1->SetPosY(3*32);



	CBaseUnit* aoeUnit2 = new CBaseUnit();
	aoeUnit2-> CreateUnit( FOOTSOLDIERS, 2, 0 );
	aoeUnit2->SetPosX(6*32);
	aoeUnit2->SetPosY(2*32);



	CBaseUnit* aoeUnit3 = new CBaseUnit();
	aoeUnit3-> CreateUnit( FOOTSOLDIERS, 2, 0 );
	aoeUnit3->SetPosX(6*32);
	aoeUnit3->SetPosY(4*32);



	CBaseUnit* aoeUnit4 = new CBaseUnit();
	aoeUnit4-> CreateUnit( FOOTSOLDIERS, 2, 0 );
	aoeUnit4->SetPosX(5*32);
	aoeUnit4->SetPosY(4*32);



	CBaseUnit* aoeUnit5 = new CBaseUnit();
	aoeUnit5-> CreateUnit( FOOTSOLDIERS, 2, 0 );
	aoeUnit5->SetPosX(7*32);
	aoeUnit5->SetPosY(2*32);



	m_vUnits.push_back( aoeUnit );
	m_vUnits.push_back( aoeUnit1);
	m_vUnits.push_back( aoeUnit2);
	m_vUnits.push_back( aoeUnit3);
	m_vUnits.push_back( aoeUnit4);
	m_vUnits.push_back( aoeUnit5);
	//Ending AoE Test units
	///////////////////////////////////////////////////////

	m_vUnits.push_back( tempunit );
	m_vUnits.push_back( tempunit2 );
	m_vUnits.push_back( tempunit3 );
	m_vUnits.push_back( tempunit4 );
	m_vUnits.push_back( tempunit5 );
	m_vUnits.push_back( tempunit6 );



	SetGeneralImageID( CSGD_TextureManager::GetInstance()->LoadTexture( _T( "resources/graphics/HumanGeneral.png"), D3DCOLOR_XRGB(255,255,255) ) );
	SetPosX( 180 );
	SetPosY( 300 );


	SetSteel( 100 );
	SetWood ( 300 );
	SetFood ( 100 );

	CSGD_EventSystem::GetInstance()->RegisterClient("GENERAL_BUFF", this);


	m_bIsCastingAbility = false;

}
