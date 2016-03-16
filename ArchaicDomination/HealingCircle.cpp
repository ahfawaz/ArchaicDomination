#include "HealingCircle.h"
#include "SGD Wrappers\CSGD_TextureManager.h"
#include "SGD Wrappers\CSGD_DirectInput.h"
#include "SGD Wrappers\CSGD_Direct3D.h"
#include "SGD Wrappers\CSGD_XAudio2.h"
#include "SGD Wrappers\CSGD_MessageSystem.h"
#include "Particle System\ParticleSystem.h"
#include "HealingCircleMessage.h"
#include "DestroyAbilityMessage.h"
#include "SpamBar.h"
#include "BattlePlay.h"
#include "Player.h"
#include <string>

CHealingCircle::CHealingCircle(void)
{
	m_nHealingCircleSFXID = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("Resources/Sounds/HealingCircle.wav"));
	string name = "HealingCircle";
	pHealingCircle = new CParticleSystem;
	pHealingCircle->LoadEffect(name,"Resources/XMLFiles/HealingCircle.xml");
	pHealingCircle->CreateEmitter("HealingCircle");
	bar = new SpamBar();
}


CHealingCircle::~CHealingCircle(void)
{
	CSGD_XAudio2::GetInstance()->SFXUnloadSound(m_nHealingCircleSFXID);
	delete bar;
	delete pHealingCircle;
}
///////////////////////////////////////////////
// IEntity Interface:
void		CHealingCircle::Update				( float fElapsedTime )		
{
	particleTimer += fElapsedTime;
	if(GetIsCasting() == false && particleTimer >= 2.2f)
	{
		CHealingCircleMessage* healingcircleMSG = new CHealingCircleMessage(GetOwner());
		CSGD_MessageSystem::GetInstance()->QueueMessage(healingcircleMSG);
		healingcircleMSG = nullptr;

		dynamic_cast<CBaseUnit*>(GetOwner())->SetAbility2_Cooldown(5);
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
	else if(GetIsCasting() == false)
	{
		pHealingCircle->Update(fElapsedTime);
		pHealingCircle->GetEffect(0)->SetEmitterPosX((float)GetPosX() + 16);
		pHealingCircle->GetEffect(0)->SetEmitterPosY((float)GetPosY() + 16);
	}

	else if(GetIsCasting() == true)
	{
		bar->Upate(fElapsedTime);
		if(bar->GetActive() == false)
		{
			CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nHealingCircleSFXID,false);
			SetIsCasting(false);
			pHealingCircle->GetEffect(0)->KillParticles();
		}


		SetDamageScale(bar->GetDamageScale());
		pHealingCircle->GetEffect(0)->SetEmitterPosX((float)GetOwner()->GetPosX());
		pHealingCircle->GetEffect(0)->SetEmitterPosY((float)GetOwner()->GetPosY());

		particleTimer = 0;
	}

}

void		CHealingCircle::Render				( void )					
{
	if (GetIsCasting() == false)
	{
		pHealingCircle->Render();
		//return;
	}
	else
		bar->Render();
}

void		CHealingCircle::PowerBar			( void )					
{
}