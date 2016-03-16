#include "Charge.h"
#include "SGD Wrappers\CSGD_MessageSystem.h"
#include "SGD Wrappers\CSGD_XAudio2.h"
#include "DestroyAbilityMessage.h"
#include "Particle System\ParticleSystem.h"
#include "BattlePlay.h"
#include "AccuracyBar.h"
#include "Player.h"

#define	KNIGHT_STEP 0.5f

CCharge::CCharge(void)
{
	m_nChargeSFXID = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("Resources/Sounds/Charge.wav"));
	stepTimer = 0;
	bar = new AccuracyBar();

	string name = "Charge";
	pCharge = new CParticleSystem;
	pCharge->LoadEffect(name,"Resources/XMLFiles/Charge.xml");
	pCharge->CreateEmitter("Charge");
}


CCharge::~CCharge(void)
{
	CSGD_XAudio2::GetInstance()->SFXUnloadSound(m_nChargeSFXID);
	delete bar;
	delete pCharge;
}
///////////////////////////////////////////////
// IEntity Interface:
void		CCharge::Update				( float fElapsedTime )		
{




	if (GetIsCasting() == false)   //if powerbar is done
	{
		//Target's and Owner's position.
		int targetX = (int)target->GetPosX() / 32;
		int targetY = (int)target->GetPosY() / 32;
		int ownerX = (int)GetOwner()->GetPosX() / 32;
		int ownerY = (int)GetOwner()->GetPosY() / 32;

		pCharge->Update(fElapsedTime);
					//Owner's movement to Target
		if (stepTimer == 0)
		{
			dynamic_cast<CBaseUnit*>(GetOwner())->Reset();

			if(target->GetPosX() > GetOwner()->GetPosX()) 
				dynamic_cast<CBaseUnit*>(GetOwner())->Move(CBattlePlay::GetInstance()->GetGrid()->GetTiles()[targetY][targetX - 1]);

			else if(target->GetPosX() < GetOwner()->GetPosX()) 
				dynamic_cast<CBaseUnit*>(GetOwner())->Move(CBattlePlay::GetInstance()->GetGrid()->GetTiles()[targetY][targetX + 1]);

			else if(target->GetPosY() > GetOwner()->GetPosY()) 
				dynamic_cast<CBaseUnit*>(GetOwner())->Move(CBattlePlay::GetInstance()->GetGrid()->GetTiles()[targetY - 1][targetX]);

			else if(target->GetPosY() < GetOwner()->GetPosY()) 
				dynamic_cast<CBaseUnit*>(GetOwner())->Move(CBattlePlay::GetInstance()->GetGrid()->GetTiles()[targetY + 1][targetX]);
			stepTimer += 15;
		}

			pCharge->GetEffect(0)->SetEmitterPosX((float)GetOwner()->GetPosX()+16);
			pCharge->GetEffect(0)->SetEmitterPosY((float)GetOwner()->GetPosY()+16);
		

		if(targetX - 1 == ownerX || targetX + 1 == ownerX || targetY - 1 == ownerY || targetY + 1 == ownerY )
		{
			int damage = (int)((bar->GetDamageScale() / 100.0f) * 20);
			if(damage == 0)
			{
				std::string text = "Miss...";
				CFloatingText* newText = new CFloatingText(text, (float)GetOwner()->GetPosX(), (float)GetOwner()->GetPosY(), DAMAGE);
				CBattlePlay::GetInstance()->PushText(newText);
			}
			else
				target->TakeDamage(GetOwner(), damage);

			dynamic_cast<CBaseUnit*>(GetOwner())->SetAbility1_Cooldown(3);
			dynamic_cast<CBaseUnit*>(GetOwner())->SetHasAttacked();
			dynamic_cast<CBaseUnit*>(GetOwner())->SetIsAttacking(false);
			CPlayer::GetInstance()->SetIsCastingAbility(false);
			dynamic_cast<CBaseUnit*>(GetOwner())->SetHasMoved(hasMovedBefore);
			if(dynamic_cast<CBaseUnit*>(GetOwner())->HasMoved() == false)
				CBattlePlay::GetInstance()->SetShowUnitOptions(true);
			else
			{
				CBattlePlay::GetInstance()->SetShowUnitOptions(false);
				CBattlePlay::GetInstance()->SetCurrentUnit(nullptr);
			}

			CDestroyAbilityMessage* msg = new CDestroyAbilityMessage(GetOwner());
			CSGD_MessageSystem::GetInstance()->QueueMessage(msg);
			msg = nullptr;
		}
	}
	else
	{
		bar->Upate(fElapsedTime);
		if(bar->GetActive() == false)
		{
			CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nChargeSFXID,false);
			SetIsCasting(false);
			pCharge->GetEffect(0)->KillParticles();
		}

		SetDamageScale(bar->GetDamageScale());
	}
}

void		CCharge::Render				( void )					
{

	if (GetIsCasting() == false)
	{
		pCharge->Render();
	}
	else
		bar->Render();
}

void		CCharge::PowerBar			( void )					
{
}