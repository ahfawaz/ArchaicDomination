#include "BaseUnit.h"
#include "SGD Wrappers\CSGD_TextureManager.h"
#include "SGD Wrappers\CSGD_EventSystem.h"
#include "SGD Wrappers\CSGD_MessageSystem.h"
#include "SGD Wrappers\SGD_Math.h"
#include "BattlePlay.h"
#include "SGD Wrappers\CSGD_Direct3D.h"
#include <cassert>
#include "TileSystem\Tile.h"
#include "FloatingText.h"
#include "GamePlay.h"
#include "CatapultAttackingMessage.h"
#include "CreateAbilityMessage.h"
#include "MainMenu.h"
#include "ArcadeGamePlay.h"
#include "WorldPlay.h"
#include "Animation System\AnimTimeStamp.h"
#include "Game.h"


CBaseUnit::CBaseUnit()
{
	//spirte ImageID's
	/*soldierSprite		=  -1;
	priestSprite		=  -1;
	wizardSprite		=  -1;
	calvarySprite		=  -1;
	catapultSprite		=  -1;
	generalSprite       =  -1;*/

	hasMoved = false;

	m_nAttackRange  = 0;

	m_nMinDamage	= 0;

	m_nMaxDamage	= 0;

	m_nMovement		= 0;

	m_fScaleX = 1.0f;

	m_fScaleY = 1.0f;

	attackMode = false;

	hasAttacked = false;

	ability = nullptr;

	GeneralBuff = false;

	CatapultBuff = false;

	CSGD_EventSystem::GetInstance()->RegisterClient("FIRE_BALL", this);
	CSGD_EventSystem::GetInstance()->RegisterClient("CLEAVE", this);

	Ability1 = "";
	Ability2 = "";
	Ability1_Cooldown = 0;
	Ability2_Cooldown = 0;

	m_pIdleDone = nullptr;
	m_pIdleWaiting = nullptr;
	m_pMoveLeft = nullptr;
	m_pMoveRight = nullptr;

	m_bIsMoving = false;
	m_bMovingLeft = true;
	m_fStepInterval = 0;
}


CBaseUnit::~CBaseUnit(void)
{
	delete m_pIdleDone;
	delete m_pIdleWaiting;
	delete m_pMoveLeft;
	delete m_pMoveRight;
}

void CBaseUnit::GetScale(float& x, float&y) const
{
	x = this->m_fScaleX;
	y = this->m_fScaleY;
}

float CBaseUnit::GetScaleX() const
{
	return this->m_fScaleX;	
}

float CBaseUnit::GetScaleY() const
{
	return this->m_fScaleY;
}

void CBaseUnit::SetScale(float x, float y)
{
	this->m_fScaleX = x;
	this->m_fScaleY = y;
}

void CBaseUnit::SetScaleX(float x)
{
	this->m_fScaleX = x;
}

void CBaseUnit::SetScaleY(float y)
{
	this->m_fScaleY = y;
}

//Deleting Ability Here.
void CBaseUnit::removeAbility(void)
{
	delete ability;
	ability = nullptr;
}



void CBaseUnit::Update( float fElapsedTime )
{
	if(ability)
		ability->Update(fElapsedTime);

	if(hasMoved && hasAttacked)
	{
		if(m_pIdleDone != nullptr)
			CGame::GetInstance()->GetAnimeSystem()->Update(m_pIdleDone,fElapsedTime);
	}
	else if(m_bIsMoving == true)
	{
		m_fStepInterval += fElapsedTime;

		if(GetPosX() == currentTile->GetLocX() && GetPosY() == currentTile->GetLocY())
		{
			hasMoved = true;
			m_bIsMoving = false;
			if (CEnemy::GetInstance()->IsAI() && TypeOfUnit > 5)
			{
				return;
			}
			if(!hasAttacked && CBattlePlay::GetInstance()->GetCurrentUnit() != nullptr)CBattlePlay::GetInstance()->SetShowUnitOptions(true);
			else
				CBattlePlay::GetInstance()->SetCurrentUnit(nullptr);

			return;
		}

		if (m_fStepInterval > 0.25f)
		{
			if(GetPosX() < currentTile->GetLocX())
			{
				SetPosX(GetPosX() + 16);
				m_bMovingLeft = false;
				m_fStepInterval = 0;
			}
			if(GetPosX() > currentTile->GetLocX())
			{
				SetPosX(GetPosX() - 16);
				m_bMovingLeft = true;
				m_fStepInterval = 0;
			}
			if(GetPosY() < currentTile->GetLocY())
			{
				SetPosY(GetPosY() + 16);
				m_bMovingLeft = false;
				m_fStepInterval = 0;
			}
			if(GetPosY() > currentTile->GetLocY())
			{
				SetPosY(GetPosY() - 16);
				m_bMovingLeft = true;
				m_fStepInterval = 0;
			} 
		}

		if(m_bMovingLeft == true)
			CGame::GetInstance()->GetAnimeSystem()->Update(m_pMoveLeft,fElapsedTime);
		else
			CGame::GetInstance()->GetAnimeSystem()->Update(m_pMoveRight,fElapsedTime);

	}
	else 
	{
		if(m_pIdleWaiting != nullptr)
			CGame::GetInstance()->GetAnimeSystem()->Update(m_pIdleWaiting,fElapsedTime);
	}


}


bool CBaseUnit::Move(CTile* moveTile)
{
	//Consider revising
	if(moveTile->GetOccupied())
	{
		if(CEnemy::GetInstance()->IsAI() && TypeOfUnit > 5)
		{
			hasMoved = true;
			return true;
		}
		return false;
	}

	if(!hasMoved)
	{

		int spaceCount = 0;

		spaceCount += int(_CMATH_::abs(moveTile->GetLocX()-GetPosX()));
		spaceCount += int(_CMATH_::abs(moveTile->GetLocY()-GetPosY()));

		if(spaceCount/32 > this->GetMovement())
			return false;

		currentTile->SetOccupied(false);
		moveTile->SetOccupied(true);

		//SetPosX((float)moveTile->GetLocX());
		//SetPosY((float)moveTile->GetLocY());

		m_fStepInterval = 0;
		if(TypeOfUnit > 5 && CEnemy::GetInstance()->IsAI())
			hasMoved = true;



		if(moveTile->GetType() == TileType::TREE && currentTile->GetType() != TREE)
		{
			m_nMovement /= 2;
			m_nAttackRange /= 2;
		}
		else if(moveTile->GetType() == TileType::NONE)
		{
			ResetRange();
		}

		if(TypeOfUnit < 6)
		CGame::GetInstance()->PlayHumanMoveSFX();
		else
		CGame::GetInstance()->PlayUndeadMoveSFX();

		currentTile = moveTile;
		m_bIsMoving = true;
		return true;
	}
	
	return false;
}

bool CBaseUnit::Attack(CBaseUnit* target)
{
	if(target == nullptr)
	{
		hasAttacked = true;
		attackMode = false;
		return true;
	}
	else if(IsInRange(target))
	{
		if(TypeOfUnit == CATAPULT || (TypeOfUnit == WARMACHINE && !CEnemy::GetInstance()->IsAI()))
		{
			CCatapultAttackingMessage* msg = new CCatapultAttackingMessage(this, target);
			CSGD_MessageSystem::GetInstance()->QueueMessage(msg);
			msg = nullptr;
			return true;
		}
		else if(TypeOfUnit == VOLATILEGHOUL)
		{
			CCreateAbilityMessage* msg = new CCreateAbilityMessage(this);
			CSGD_MessageSystem::GetInstance()->QueueMessage(msg);
			msg = nullptr;
		}
		else
			target->TakeDamage(this);
	}
	else
		return false;

	hasAttacked = true;
	attackMode = false;

	if(!hasMoved)CBattlePlay::GetInstance()->SetShowUnitOptions(true);
	else
		CBattlePlay::GetInstance()->SetCurrentUnit(nullptr);

	return true;

}

bool CBaseUnit::IsInRange(CBaseUnit* target)
{

	int spaceCount = 0;

	spaceCount += int(_CMATH_::abs(target->GetPosX()-GetPosX()));
	spaceCount += int(_CMATH_::abs(target->GetPosY()-GetPosY()));

	if(spaceCount/32 > this->GetAttackRange())
		return false;
	else
		return true;


}


/*Parameters:
CEntity* attacke - Unit attacking "this"
int damage - damage being caused by an ability

if damage == NULL then the attacker's basic attack damage is used.
else it will only use the damage passed in for any abilities tha cause damage.
*/
void CBaseUnit::TakeDamage( CEntity* attacker, int damage )
{

	int dmg = dynamic_cast<CBaseUnit*>(attacker)->GetDamage();

	if(damage == NULL)
	{
		if(dynamic_cast<CBaseUnit*>(attacker)->GetGeneralBuff() && dynamic_cast<CBaseUnit*>(attacker)->GetCatapultBuff())
		{
			this->SetCurrentHealth(this->GetCurrentHealth() - (dmg + 10));
			dmg+=10;
		}
		else if(dynamic_cast<CBaseUnit*>(attacker)->GetCatapultBuff())
		{
			this->SetCurrentHealth(this->GetCurrentHealth() - (dmg + 5));
			dynamic_cast<CBaseUnit*>(attacker)->SetCatapultBuff(false);
			dmg += 5;
		}
		else if(dynamic_cast<CBaseUnit*>(attacker)->GetGeneralBuff())
		{
			this->SetCurrentHealth(this->GetCurrentHealth() - (dynamic_cast<CBaseUnit*>(attacker)->GetDamage() + 5));
			dmg += 5;
		}
		else
		{
			this->SetCurrentHealth(this->GetCurrentHealth() - (dmg));
		}
	}
	else if(damage == -1)
	{
		dmg = 0;
	}
	else 
	{
		this->SetCurrentHealth(this->GetCurrentHealth() - (damage));
		dmg = damage;
	}


	std::string text = "-" + to_string(dmg);
	CFloatingText* newText = new CFloatingText(text, GetPosX(), GetPosY(), DAMAGE);
	CBattlePlay::GetInstance()->PushText(newText);

	

}	

void CBaseUnit::Render( void )
{
	int offsetX = CBattlePlay::GetInstance()->GetOffX();
	int offsetY = CBattlePlay::GetInstance()->GetOffY();

	if(m_bIsMoving)
	{
		if(m_bMovingLeft == true)
			CGame::GetInstance()->GetAnimeSystem()->Render(m_pMoveLeft, (int)GetPosX()-offsetX, (int)GetPosY()-offsetY);
		else
			CGame::GetInstance()->GetAnimeSystem()->Render(m_pMoveRight, (int)GetPosX()-offsetX, (int)GetPosY()-offsetY);
	}

	else
	{

		if(hasMoved && hasAttacked)
		{
			if(m_pIdleDone != nullptr)
				CGame::GetInstance()->GetAnimeSystem()->Render(m_pIdleDone,(int)GetPosX()-offsetX, (int)GetPosY()-offsetY);
			else
				CSGD_TextureManager::GetInstance()->Draw(GetSpriteID(),(int)(GetPosX()- offsetX), (int)(GetPosY()-offsetY), 1.0f, 1.0f, nullptr, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(100,100,100,100));
		}
		else
		{
			if(m_pIdleWaiting != nullptr)
				CGame::GetInstance()->GetAnimeSystem()->Render(m_pIdleWaiting,(int)GetPosX()-offsetX, (int)GetPosY()-offsetY);
			else
				CSGD_TextureManager::GetInstance()->Draw(GetSpriteID(),(int)(GetPosX()- offsetX),(int)(GetPosY()-offsetY), 1.0f, 1.0f);
		} 
	}
	//RECT hpBar = {GetPosX(), GetPosY()-5, GetPosX()+(GetCurrentHealth()/10)*32, GetPosY()+5};
	//CSGD_Direct3D::GetInstance()->DrawRect(hpBar, D3DCOLOR_XRGB(0,255,0));
}

void CBaseUnit::CreateUnit ( UnitID Type, int x, int y )
{
	switch (Type)
	{
	case FOOTSOLDIERS:
		{

			SetMaxHealth( 45 );
			m_nAttackRange		= 2;
			m_nMovement			= 4;
			m_nMinDamage		= 10;
			m_nMaxDamage		= 20;
			Ability1 = "Cleave";
			m_pIdleWaiting = new CAnimTimeStamp("Soldier_Idle_Waiting");
			m_pIdleDone = new CAnimTimeStamp("Soldier_Idle_Done");
			m_pMoveLeft = new CAnimTimeStamp("Soldier_Left");
			m_pMoveRight = new CAnimTimeStamp("Soldier_Right");
			SetSpriteID(CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/swordsman.png")));
		}
		break;
	case PRIEST:
		{
			SetMaxHealth( 25 );
			m_nAttackRange		= 3;
			m_nMovement			= 3;
			m_nMinDamage		= 5;
			m_nMaxDamage		= 10;
			Ability1 = "Heal";
			Ability2 = "Healing Circle";
			m_pIdleWaiting = new CAnimTimeStamp("Priest_Idle_Waiting");
			m_pIdleDone = new CAnimTimeStamp("Priest_Idle_Done");
			m_pMoveLeft = new CAnimTimeStamp("Priest_Left");
			m_pMoveRight = new CAnimTimeStamp("Priest_Right");
			SetSpriteID(CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/priest.png")));

		}
		break;
	case WIZARD:
		{
			SetMaxHealth( 30 );
			m_nAttackRange		= 3;
			m_nMovement			= 4;
			m_nMinDamage		= 15;
			m_nMaxDamage		= 25;
			Ability1 = "Fireball";
			Ability2 = "Blizzard";
			m_pIdleWaiting = new CAnimTimeStamp("Wizard_Idle_Waiting");
			m_pIdleDone = new CAnimTimeStamp("Wizard_Idle_Done");
			m_pMoveLeft = new CAnimTimeStamp("Wizard_Left");
			m_pMoveRight = new CAnimTimeStamp("Wizard_Right");
			SetSpriteID( CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/wizard.png")) );
		}
		break;
	case GENERAL:
		{
			SetMaxHealth( 55 );
			m_nAttackRange		= 2;
			m_nMovement			= 4;
			m_nMinDamage		= 15;
			m_nMaxDamage		= 25;
			Ability1 = "Battle Cry";
			Ability2 = "Judgement";
			m_pIdleWaiting = new CAnimTimeStamp("General_Idle_Waiting");
			m_pIdleDone = new CAnimTimeStamp("General_Idle_Done");
			m_pMoveLeft = new CAnimTimeStamp("General_Left");
			m_pMoveRight = new CAnimTimeStamp("General_Right");
			SetSpriteID( CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/General.png") ) );
		}
		break;
	case CALVARY:
		{
			SetMaxHealth( 55 );
			m_nAttackRange		= 1;
			m_nMovement			= 5;
			m_nMinDamage		= 15;
			m_nMaxDamage		= 25;
			Ability1 = "Charge";
			m_pIdleWaiting = new CAnimTimeStamp("Calvary_Idle_Waiting");
			m_pIdleDone = new CAnimTimeStamp("Calvary_Idle_Done");
			m_pMoveLeft = new CAnimTimeStamp("Calvary_Left");
			m_pMoveRight = new CAnimTimeStamp("Calvary_Right");
			SetSpriteID( CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/calvary.png")) );

		}
		break;
	case CATAPULT:
		{
			SetMaxHealth( 50 );
			m_nAttackRange		= 5;
			m_nMovement			= 2;
			m_nMinDamage		= 15;
			m_nMaxDamage		= 25;
			Ability1 = "FLAME ON!";
			m_pIdleWaiting = new CAnimTimeStamp("Catapult_Idle_Waiting");
			m_pIdleDone = new CAnimTimeStamp("Catapult_Idle_Done");
			m_pMoveLeft = new CAnimTimeStamp("Catapult_Left");
			m_pMoveRight = new CAnimTimeStamp("Catapult_Right");
			SetSpriteID(CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/catapult.png")));
		}
		break;
	case SKELETALWARRIOR:
		{
			SetMaxHealth( 20 );
			m_nAttackRange		= 1;
			m_nMovement			= 4;
			m_nMinDamage		= 10;
			m_nMaxDamage		= 20;
			Ability1 = "Cleave";
			m_pIdleWaiting = new CAnimTimeStamp("Skeleton_Idle_Waiting");
			m_pIdleDone = new CAnimTimeStamp("Skeleton_Idle_Done");
			m_pMoveLeft = new CAnimTimeStamp("Skeleton_Left");
			m_pMoveRight = new CAnimTimeStamp("Skeleton_Right");
			SetSpriteID( CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/skeletalwarrior.png")) );
		}
		break;
	case SHADOWPRIEST:
		{
			SetMaxHealth( 30 );
			m_nAttackRange		= 3;
			m_nMovement			= 3;
			m_nMinDamage		= 5;
			m_nMaxDamage		= 10;
			Ability1 = "Drain Life";
			Ability2 = "Transfer Life";
			m_pIdleWaiting = new CAnimTimeStamp("ShadowPriest_Idle_Waiting");
			m_pIdleDone = new CAnimTimeStamp("ShadowPriest_Idle_Done");
			m_pMoveLeft = new CAnimTimeStamp("ShadowPriest_Left");
			m_pMoveRight = new CAnimTimeStamp("ShadowPriest_Right");
			SetSpriteID(CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/shadow_priest.png")));
		}
		break;
	case NECROMANCER:
		{
			SetMaxHealth( 25 );
			m_nAttackRange		= 3;
			m_nMovement			= 3;
			m_nMinDamage		= 15;
			m_nMaxDamage		= 25;
			Ability2 = "Summon Ghoul";
			Ability1 = "Shadowbolt";
			m_pIdleWaiting = new CAnimTimeStamp("Necromancer_Idle_Waiting");
			m_pIdleDone = new CAnimTimeStamp("Necromancer_Idle_Done");
			m_pMoveLeft = new CAnimTimeStamp("Necromancer_Left");
			m_pMoveRight = new CAnimTimeStamp("Necromancer_Right");
			SetSpriteID(CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/necromancer.png")));
		}
		break;
	case LICH:
		{
			SetMaxHealth( 50 );
			m_nAttackRange		= 4;
			m_nMovement			= 2;
			m_nMinDamage		= 15;
			m_nMaxDamage		= 25;
			Ability1 = "Dark Curse";
			Ability2 = "Empowering Curse";
			m_pIdleWaiting = new CAnimTimeStamp("Lich_Idle_Waiting");
			m_pIdleDone = new CAnimTimeStamp("Lich_Idle_Done");
			m_pMoveLeft = new CAnimTimeStamp("Lich_Left");
			m_pMoveRight = new CAnimTimeStamp("Lich_Right");
			SetSpriteID(CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/Lich.png")));
		}
		break;
	case DEATHKNIGHT:
		{
			SetMaxHealth( 55 );
			m_nAttackRange		= 1;
			m_nMovement			= 5;
			m_nMinDamage		= 15;
			m_nMaxDamage		= 25;
			Ability1 = "Dark Charge";
			m_pIdleWaiting = new CAnimTimeStamp("DeathKnight_Idle_Waiting");
			m_pIdleDone = new CAnimTimeStamp("DeathKnight_Idle_Done");
			m_pMoveLeft = new CAnimTimeStamp("DeathKnight_Left");
			m_pMoveRight = new CAnimTimeStamp("DeathKnight_Right");
			SetSpriteID( CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/death_knight.png")));

		}
		break;
	case VOLATILEGHOUL:
		{
			SetMaxHealth( 15 );
			m_nAttackRange		= 1;
			m_nMovement			= 4;
			m_nMinDamage		= 15;
			m_nMaxDamage		= 25;
			Ability1 = "Explode";
			m_pIdleWaiting = new CAnimTimeStamp("Ghoul_Idle_Waiting");
			m_pIdleDone = new CAnimTimeStamp("Ghoul_Idle_Done");
			m_pMoveLeft = new CAnimTimeStamp("Ghoul_Left");
			m_pMoveRight = new CAnimTimeStamp("Ghoul_Right");
			SetSpriteID(CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/ghoul.png")));
		}
		break;
	case WARMACHINE:
		{
			SetMaxHealth( 50 );
			m_nAttackRange		= 5;
			m_nMovement			= 2;
			m_nMinDamage		= 15;
			m_nMaxDamage		= 25;
			Ability1 = "PLAGUE ON!";
			m_pIdleWaiting = new CAnimTimeStamp("WarMachine_Idle_Waiting");
			m_pIdleDone = new CAnimTimeStamp("WarMachine_Idle_Done");
			m_pMoveLeft = new CAnimTimeStamp("WarMachine_Left");
			m_pMoveRight = new CAnimTimeStamp("WarMachine_Right");
			SetSpriteID(CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/war_machine.png")));
		}
		break;
	default:
		break;
	}

	this->TypeOfUnit = Type;

	this->SetCurrentHealth(this->GetMaxHealth());
	//this->SetPosX( 0 );
	//this->SetPosY( 0 );

	//if (CMainMenu::GetInstance()->GetArcade() == true )
	//{
	//	SetCountry(CArcadeGamePlay::GetInstance()->GetCurX());
	//	SetTerritory(CArcadeGamePlay::GetInstance()->GetCurY());
	//}
	//else if (CMainMenu::GetInstance()->GetCampaign() == true)
	//{
	//	SetCountry(CGamePlay::GetInstance()->GetCurX());
	//	SetTerritory(CGamePlay::GetInstance()->GetCurY());
	//}
	//else if (CMainMenu::GetInstance()->GetVersus() == true || CMainMenu::GetInstance()->GetQuickMatch() == true)
	//{
	//	SetCountry(CWorldPlay::GetInstance()->GetCurX());
	//	SetTerritory(CWorldPlay::GetInstance()->GetCurY());
	//}

	SetCountry(x);
	SetTerritory(y);
}

int CBaseUnit::GetDamage(void)
{
	return rand() % (m_nMaxDamage - m_nMinDamage) + m_nMinDamage;
}

void CBaseUnit::Reset(void)
{
	hasMoved = false;
	attackMode = false;
	hasAttacked = false;
}

/*virtual*/ void CBaseUnit::HandleEvent( const CEvent* pEvent )
{
	if( pEvent->GetEventID() == "CLEAVE" )
	{

	}

	if( pEvent->GetEventID() == "FIRE_BALL" && this == pEvent->GetDestination() )
	{

	}

}

const char * CBaseUnit::GetType(void)
{
	switch(TypeOfUnit)
	{
	case FOOTSOLDIERS:
		{
			return "Soldier";
		}
		break;
	case PRIEST:
		{
			return "Priest";
		}
		break;
	case WIZARD:
		{
			return "Wizard";
		}
		break;
	case GENERAL:
		{
			return "General";
		}
		break;
	case CALVARY:
		{
			return "Calvary";
		}
		break;
	case CATAPULT:
		{
			return "Catapult";
		}
		break;
	case SKELETALWARRIOR:
		{
			return "Skeleton";
		}
		break;
	case SHADOWPRIEST:
		{
			return "Shadow Priest";
		}
		break;
	case NECROMANCER:
		{
			return "Necromancer";
		}
		break;
	case LICH:
		{
			return "Lich";
		}
		break;
	case DEATHKNIGHT:
		{
			return "Deathknight";
		}
		break;
	case VOLATILEGHOUL:
		{
			return "Volatile Ghoul";
		}
		break;
	case WARMACHINE:
		{
			return "War Machine";
		}
		break;
	default:
		{
			return "NULL";
		}
		break;
	}

}

void CBaseUnit::ResetRange(void)
{
	switch (TypeOfUnit)
	{
	case FOOTSOLDIERS:
		{

			m_nAttackRange		= 2;
			m_nMovement			= 4;


		}
		break;
	case PRIEST:
		{

			m_nAttackRange		= 3;
			m_nMovement			= 3;


		}
		break;
	case WIZARD:
		{

			m_nAttackRange		= 3;
			m_nMovement			= 4;


		}
		break;
	case GENERAL:
		{

			m_nAttackRange		= 2;
			m_nMovement			= 4;


		}
		break;
	case CALVARY:
		{

			m_nAttackRange		= 1;
			m_nMovement			= 5;


		}
		break;
	case CATAPULT:
		{

			m_nAttackRange		= 4;
			m_nMovement			= 2;

		}
		break;
	case SKELETALWARRIOR:
		{

			m_nAttackRange		= 1;
			m_nMovement			= 4;

		}
		break;
	case SHADOWPRIEST:	
		{

			m_nAttackRange		= 3;
			m_nMovement			= 3;

		}
		break;
	case NECROMANCER:
		{

			m_nAttackRange		= 3;
			m_nMovement			= 3;

		}
		break;
	case LICH:
		{

			m_nAttackRange		= 4;
			m_nMovement			= 2;

		}
		break;
	case DEATHKNIGHT:
		{

			m_nAttackRange		= 1;
			m_nMovement			= 5;

		}
		break;
	case VOLATILEGHOUL:
		{

			m_nAttackRange		= 1;
			m_nMovement			= 4;

		}
		break;
	case WARMACHINE:
		{
			m_nAttackRange		= 4;
			m_nMovement			= 2;
		}
		break;
	default:
		break;
	}
}

void CBaseUnit::DecreaseAbilityCooldown(void)
{
	if(Ability1_Cooldown > 0)
		--Ability1_Cooldown;
	if(Ability2_Cooldown > 0)
		--Ability2_Cooldown;
}

void				CBaseUnit::ResetUnit			( void )
{
	switch (TypeOfUnit)
	{
	case FOOTSOLDIERS:
		SetMaxHealth(45);
		break;
	case PRIEST:
		SetMaxHealth(25);
		break;
	case WIZARD:
		SetMaxHealth(30);
		break;
	case GENERAL:
		SetMaxHealth(55);
		break;
	case CALVARY:
		SetMaxHealth(55);
		break;
	case CATAPULT:
		SetMaxHealth(50);
		break;
	case SKELETALWARRIOR:
		SetMaxHealth(20);
		break;
	case SHADOWPRIEST:
		SetMaxHealth(30);
		break;
	case NECROMANCER:
		SetMaxHealth(25);
		break;
	case LICH:
		SetMaxHealth(50);
		break;
	case DEATHKNIGHT:
		SetMaxHealth(55);
		break;
	case VOLATILEGHOUL:
		SetMaxHealth(15);
		break;
	case WARMACHINE:
		SetMaxHealth(50);
		break;
	default:
		break;
	}

	SetCurrentHealth(GetMaxHealth());
	m_bIsMoving = false;
	m_bMovingLeft = false;
	GeneralBuff = false;
	CatapultBuff = false;

	Ability1_Cooldown = 0;
	Ability2_Cooldown = 0;
	m_fStepInterval = 0;
	Reset();
	ResetRange();
}