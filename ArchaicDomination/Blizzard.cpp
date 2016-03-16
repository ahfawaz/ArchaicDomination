#include "Blizzard.h"
#include "SGD Wrappers\CSGD_TextureManager.h"
#include "SGD Wrappers\CSGD_Direct3D.h"
#include "SGD Wrappers\CSGD_MessageSystem.h"
#include "SGD Wrappers\CSGD_XAudio2.h"
#include "Particle System\ParticleSystem.h"
#include "BlizzardMessage.h"
#include "DestroyAbilityMessage.h"
#include "SpamBar.h"
#include "BattlePlay.h"
#include "Player.h"
#include <string>

CBlizzard::CBlizzard(void)
{
	m_nBlizzardSFXID = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("Resources/Sounds/Blizzard.wav"));
	string name = "Blizzard";
	pBlizzard = new CParticleSystem;
	pBlizzard->LoadEffect(name,"Resources/XMLFiles/Blizzard.xml");
	pBlizzard->CreateEmitter("Blizzard");
	bar = new SpamBar();
}


CBlizzard::~CBlizzard(void)
{
	CSGD_XAudio2::GetInstance()->SFXUnloadSound(m_nBlizzardSFXID);
	delete bar;
	delete pBlizzard;
}
///////////////////////////////////////////////
// IEntity Interface:
void		CBlizzard::Update				( float fElapsedTime )		
{
	particleTimer += fElapsedTime;
	if(GetIsCasting() == false && particleTimer >= 5.0f)
	{
		CBlizzardMessage* blizzardMSG = new CBlizzardMessage(GetOwner());
		CSGD_MessageSystem::GetInstance()->QueueMessage(blizzardMSG);
		blizzardMSG = nullptr;

		dynamic_cast<CBaseUnit*>(GetOwner())->SetAbility2_Cooldown(4);
		dynamic_cast<CBaseUnit*>(GetOwner())->SetHasAttacked();
		CPlayer::GetInstance()->SetIsCastingAbility(false);
		dynamic_cast<CBaseUnit*>(GetOwner())->SetIsAttacking(false);
		if(dynamic_cast<CBaseUnit*>(GetOwner())->HasMoved() == false)CBattlePlay::GetInstance()->SetShowUnitOptions(true);
		else
			CBattlePlay::GetInstance()->SetCurrentUnit(nullptr);

		CDestroyAbilityMessage* msg = new CDestroyAbilityMessage(GetOwner());
		CSGD_MessageSystem::GetInstance()->QueueMessage(msg);
		msg = nullptr;
	}

	else if(GetIsCasting() == false)
	{
		pBlizzard->Update(fElapsedTime);
		pBlizzard->GetEffect(0)->SetEmitterPosX((float)GetPosX() + 16);
		pBlizzard->GetEffect(0)->SetEmitterPosY((float)GetPosY() + 16);
	}

	else if(GetIsCasting() == true)
	{
		bar->Upate(fElapsedTime);
		if(bar->GetActive() == false)
		{
			CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nBlizzardSFXID,false);
			SetIsCasting(false);
			pBlizzard->GetEffect(0)->KillParticles();
		}

		SetDamageScale(bar->GetDamageScale());
		pBlizzard->GetEffect(0)->SetEmitterPosX((float)GetOwner()->GetPosX());
		pBlizzard->GetEffect(0)->SetEmitterPosY((float)GetOwner()->GetPosY());

		particleTimer = 0;
	}

}

void		CBlizzard::Render				( void )					
{
	if (GetIsCasting() == false)
		pBlizzard->Render();
	else
		bar->Render();
}

void		CBlizzard::PowerBar			( void )					
{
}