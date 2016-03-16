#include "CatapultAttack.h"
#include "SGD Wrappers\CSGD_MessageSystem.h"
#include "SGD Wrappers\CSGD_DirectInput.h"
#include "DestroyAbilityMessage.h"
#include "AccuracyBar.h"
#include "BattlePlay.h"
#include "Enemy.h"
#include "Player.h"


CCatapultAttack::CCatapultAttack(void)
{
	bar = new AccuracyBar;
	inputDelay = 0;
}


CCatapultAttack::~CCatapultAttack(void)
{
	delete bar;
}

void		CCatapultAttack::Update				( float fElapsedTime )		
{
	if(GetIsCasting() == false)
	{
		if(hit == true)
			target->TakeDamage(GetOwner());
		else
		{
			std::string text = "Miss...";
			CFloatingText* newText = new CFloatingText(text, (float)target->GetPosX(), (float)target->GetPosY(), DAMAGE);
			CBattlePlay::GetInstance()->PushText(newText);
		}

		dynamic_cast<CBaseUnit*>(GetOwner())->SetHasAttacked();
		dynamic_cast<CBaseUnit*>(GetOwner())->SetIsAttacking(false);
			CEnemy::GetInstance()->SetIsCastingAbility(false);
			CPlayer::GetInstance()->SetIsCastingAbility(false);
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
	else
	{
		if(inputDelay < 0.1f)
			CSGD_DirectInput::GetInstance()->ClearInput();

		bar->Upate(fElapsedTime);
		if(bar->GetActive() == false)
		{
			SetIsCasting(false);
			SetDamageScale(bar->GetDamageScale());

			int rand = std::rand() % 100;
			if(rand < GetDamageScale())
			{
				hit = true;
			}
			else 
				hit = false;

		}


	}

	inputDelay += fElapsedTime;

}

void		CCatapultAttack::Render				( void )				
{
	if(GetIsCasting() == true)
		bar->Render();
}


void		CCatapultAttack::PowerBar			( void )		
{

}
