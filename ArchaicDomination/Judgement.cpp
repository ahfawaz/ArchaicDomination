#include "Judgement.h"
#include "SGD Wrappers\CSGD_TextureManager.h"
#include "SGD Wrappers\CSGD_Direct3D.h"
#include "SGD Wrappers\CSGD_MessageSystem.h"
#include "SGD Wrappers\CSGD_XAudio2.h"
#include "Particle System\ParticleSystem.h"
#include "DestroyAbilityMessage.h"
#include "PowerBar.h"
#include "BattlePlay.h"
#include "Player.h"
#include "Enemy.h"
#include <string>
CJudgement::CJudgement(void)
{
	m_nJudgementSFXID = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("Resources/Sounds/Judgement.wav"));
	bar = new  PBar;
	//particleTimer = 0;

	for(unsigned int i = 0; i < CEnemy::GetInstance()->GetUnits().size(); i++)
	{

		CParticleSystem* pdebuff = new CParticleSystem;
		pdebuff->LoadEffect("Judgement", "Resources/XMLFiles/Judgement.xml");
		pdebuff->CreateEmitter("Judgement");
		pJudgement.push_back(pdebuff);
	}


	for(unsigned int i = 0; i < CEnemy::GetInstance()->GetUnits().size(); i++)
	{
		std::string text = "Judgement";
		CFloatingText* newText = new CFloatingText(text, (float)CEnemy::GetInstance()->GetUnits()[i]->GetPosX(), (float)CEnemy::GetInstance()->GetUnits()[i]->GetPosY(), DEBUFF);
		CBattlePlay::GetInstance()->PushText(newText);
	}
}


CJudgement::~CJudgement(void)
{
	CSGD_XAudio2::GetInstance()->SFXUnloadSound(m_nJudgementSFXID);
	delete bar;

	for(unsigned int i = 0; i < pJudgement.size(); i++)
		delete pJudgement[i];
}
///////////////////////////////////////////////
// IEntity Interface:
void		CJudgement::Update				( float fElapsedTime )		
{
	//particleTimer += fElapsedTime;
	if (GetIsCasting() == true )
	{
		if(CSGD_XAudio2::GetInstance()->SFXIsSoundPlaying(m_nJudgementSFXID) == false)
			CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nJudgementSFXID,true);

		for(unsigned int i = 0; i < CEnemy::GetInstance()->GetUnits().size(); i++)
		{
			pJudgement[i]->Update(fElapsedTime);

			pJudgement[i]->GetEffect(0)->SetEmitterPosX(CEnemy::GetInstance()->GetUnits()[i]->GetPosX() + 16.0f);
			pJudgement[i]->GetEffect(0)->SetEmitterPosY(CEnemy::GetInstance()->GetUnits()[i]->GetPosY() + 16.0f);
		}
	}
	if( GetIsCasting() == false /*&& particleTimer > 2.0f*/)
	{
		DealDamage();

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
	else 
	{
		bar->Upate(fElapsedTime);
		if(bar->GetActive() == false)
			SetIsCasting(false);



		SetDamageScale((int)(bar->GetDamageScale()/ 100.0f) * 20);


	}
}

void		CJudgement::Render				( void )					
{   
	if(bar->GetActive())
		bar->Render();

	//if (particleTimer < 2.0f)
	//{
	for(unsigned int i = 0; i < CEnemy::GetInstance()->GetUnits().size(); i++)
		pJudgement[i]->Render();
	//}

}

void		CJudgement::PowerBar			( void )					
{
}
void		CJudgement::DealDamage			( void )
{
	//particleTimer = 0;
	for(unsigned int i = 0; i < CEnemy::GetInstance()->GetUnits().size(); i++)
	{

		int damage = (int)((bar->GetDamageScale() / 100.0f) * 20);
		CEnemy::GetInstance()->GetUnits()[i]->TakeDamage(GetOwner(), damage);
	}
}

