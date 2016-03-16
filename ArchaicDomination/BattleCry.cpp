#include "BattleCry.h"
#include "SGD Wrappers\CSGD_TextureManager.h"
#include "SGD Wrappers\CSGD_DirectInput.h"
#include "SGD Wrappers\CSGD_Direct3D.h"
#include "SGD Wrappers\CSGD_XAudio2.h"
#include "SGD Wrappers\CSGD_MessageSystem.h"
#include "Particle System\ParticleSystem.h"
#include "DestroyAbilityMessage.h"
#include "BattlePlay.h"
#include "Player.h"
#include <string>
#define PARTICLE_TIMER 1.8f
CBattleCry::CBattleCry(void)
{
	m_nBattleCrySFXID = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("Resources/Sounds/BattleCry.wav"));
	particleTimer = 0;
	for(unsigned int i = 0; i < CPlayer::GetInstance()->GetUnits().size(); i++)
	{
		CParticleSystem* pbuff = new CParticleSystem;
		pbuff->LoadEffect("BattleCry", "Resources/XMLFiles/BattleCry.xml");
		pbuff->CreateEmitter("BattleCry");
		pBattleCry.push_back(pbuff);
	}
}


CBattleCry::~CBattleCry(void)
{
	CSGD_XAudio2::GetInstance()->SFXUnloadSound(m_nBattleCrySFXID);
	for(unsigned int i = 0; i < pBattleCry.size(); i++)
		delete pBattleCry[i];
}
///////////////////////////////////////////////
// IEntity Interface:
void		CBattleCry::Update				( float fElapsedTime )		
{
	particleTimer += fElapsedTime;
	if(particleTimer < PARTICLE_TIMER)
	{

		for(unsigned int i = 0; i < CPlayer::GetInstance()->GetUnits().size(); i++)
		{
			pBattleCry[i]->Update(fElapsedTime);

			pBattleCry[i]->GetEffect(0)->SetEmitterPosX(CPlayer::GetInstance()->GetUnits()[i]->GetPosX() + 16.0f);
			pBattleCry[i]->GetEffect(0)->SetEmitterPosY(CPlayer::GetInstance()->GetUnits()[i]->GetPosY() + 16.0f);
		}

	}
	if ( particleTimer > PARTICLE_TIMER)
	{
		dynamic_cast<CBaseUnit*>(GetOwner())->SetAbility1_Cooldown(6);
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
	if(CSGD_XAudio2::GetInstance()->SFXIsSoundPlaying(m_nBattleCrySFXID) == false)
		CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nBattleCrySFXID,false);
	//pBattleCry[i]->GetEffect(0)->KillParticles();
}

void		CBattleCry::Render				( void )					
{

	if( particleTimer < PARTICLE_TIMER )
	{
		for(unsigned int i = 0; i < CPlayer::GetInstance()->GetUnits().size(); i++)
			pBattleCry[i]->Render();
	}
}

void		CBattleCry::PowerBar			( void )					
{
}