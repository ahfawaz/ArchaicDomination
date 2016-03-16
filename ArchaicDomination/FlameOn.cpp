#include "FlameOn.h"
#include "SGD Wrappers\CSGD_XAudio2.h"
#include "SGD Wrappers\CSGD_MessageSystem.h"
#include "Particle System\ParticleSystem.h"
#include "DestroyAbilityMessage.h"
#include "BattlePlay.h"
#include "Player.h"
#include <string>

CFlameOn::CFlameOn(void)
{
	m_nFlameOnSFXID = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("Resources/Sounds/FlameOn.wav"));
	particleTimer = 0;
	string name = "FlameOn";
	pFlameOn = new CParticleSystem;
	pFlameOn->LoadEffect(name,"Resources/XMLFiles/FlameOn.xml");
	pFlameOn->CreateEmitter("FlameOn");
}


CFlameOn::~CFlameOn(void)
{
	CSGD_XAudio2::GetInstance()->SFXUnloadSound(m_nFlameOnSFXID);
	delete pFlameOn;
}
///////////////////////////////////////////////
// IEntity Interface:
void		CFlameOn::Update				( float fElapsedTime )		
{
	particleTimer += fElapsedTime;
	if(particleTimer > 2.0f)
	{
		dynamic_cast<CBaseUnit*>(GetOwner())->SetAbility1_Cooldown(5);
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
	else if(particleTimer < 2.0f)
	{
		pFlameOn->Update(fElapsedTime);
		pFlameOn->GetEffect(0)->SetEmitterPosX((float)GetPosX() + 16);
		pFlameOn->GetEffect(0)->SetEmitterPosY((float)GetPosY() + 16);
	}
	if(CSGD_XAudio2::GetInstance()->SFXIsSoundPlaying(m_nFlameOnSFXID) == false)
		CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nFlameOnSFXID,false);
	//pFlameOn->GetEffect(0)->KillParticles();
}

void		CFlameOn::Render				( void )					
{
	pFlameOn->Render();

}

void		CFlameOn::PowerBar			( void )					
{
}