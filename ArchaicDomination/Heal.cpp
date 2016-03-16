#include "Heal.h"
#include "SGD Wrappers\CSGD_TextureManager.h"
#include "SGD Wrappers\CSGD_Direct3D.h"
#include "SGD Wrappers\CSGD_XAudio2.h"
#include "SGD Wrappers\CSGD_MessageSystem.h"
#include "DestroyAbilityMessage.h"
#include "PowerBar.h"
#include "BattlePlay.h"
#include "Player.h"
#include <string>
#include "Particle System\ParticleSystem.h"

CHeal::CHeal(void)
{
	m_nHealSFXID = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("Resources/Sounds/AD_PrstHeal_Final.wav"));
	string name = "Heal";
	pHeal = new CParticleSystem;
	pHeal->LoadEffect(name,"Resources/XMLFiles/Heal.xml");
	pHeal->CreateEmitter("Heal");

	bar = new PBar;
}


CHeal::~CHeal(void)
{
	CSGD_XAudio2::GetInstance()->SFXUnloadSound(m_nHealSFXID);
	delete bar;
	delete pHeal;
}

///////////////////////////////////////////////
// IEntity Interface:
void		CHeal::Update				( float fElapsedTime )		
{
	if (GetIsCasting() == false)   //if powerbar is done
	{
		pHeal->Update(fElapsedTime);

		if(target->GetPosX() > GetPosX()) 
		{
			SetPosX(GetPosX()+1);


		}

		else if(target->GetPosX() < GetPosX()) 
		{   
			SetPosX(GetPosX()-1);


		}

		if(target->GetPosY() > GetPosY()) 
		{
			SetPosY(GetPosY()+1);



		}

		else if(target->GetPosY() < GetPosY()) 
		{

			SetPosY(GetPosY()-1);



		}
		pHeal->GetEffect(0)->SetEmitterPosX((float)GetPosX()+16);
		pHeal->GetEffect(0)->SetEmitterPosY((float)GetPosY()+16);










		if(GetPosX() == target->GetPosX() && GetPosY() == target->GetPosY())
		{



			int damage = (int)((bar->GetDamageScale() / 100.0f) * 20);

			target->SetCurrentHealth(target->GetCurrentHealth() + damage);
			if(target->GetCurrentHealth() > target->GetMaxHealth())
				target->SetCurrentHealth(target->GetMaxHealth());

			std::string text = "+" + to_string(damage);
			CFloatingText* newText = new CFloatingText(text, (float)target->GetPosX(), (float)target->GetPosY(), HEAL);
			CBattlePlay::GetInstance()->PushText(newText);

			dynamic_cast<CBaseUnit*>(GetOwner())->SetAbility1_Cooldown(2);
			dynamic_cast<CBaseUnit*>(GetOwner())->SetHasAttacked();
			dynamic_cast<CBaseUnit*>(GetOwner())->SetIsAttacking(false);
			CPlayer::GetInstance()->SetIsCastingAbility(false);
			if(dynamic_cast<CBaseUnit*>(GetOwner())->HasMoved() == false)CBattlePlay::GetInstance()->SetShowUnitOptions(true);
			else
				CBattlePlay::GetInstance()->SetCurrentUnit(nullptr);

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
			CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nHealSFXID,false);
			SetIsCasting(false);
			pHeal->GetEffect(0)->KillParticles();
		}
	}
}

void		CHeal::Render				( void )					
{
	if (GetIsCasting() == false)
	{
		pHeal->Render();
	}
	else
		bar->Render();
}

void		CHeal::PowerBar			( void )					
{
}