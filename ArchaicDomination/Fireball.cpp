#include "Fireball.h"
#include "SGD Wrappers\CSGD_TextureManager.h"
#include "SGD Wrappers\CSGD_Direct3D.h"
#include "SGD Wrappers\CSGD_MessageSystem.h"
#include "DestroyAbilityMessage.h"
#include "Particle System\ParticleSystem.h"
#include "Particle System\Emitter.h"
#include "SGD Wrappers\CSGD_XAudio2.h"
#include "BattlePlay.h"
#include "PowerBar.h"
#include "Player.h"
#include <string>

#define POWERBAR_INTERVAL 1.0f

CFireball::CFireball() 
{
	Timer = 0;
	m_nFireBallSFXID = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("Resources/Sounds/AD_WizFireBallATK_Final.wav"));
	m_nFireBallExplosionSFXID = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("Resources/Sounds/FireBallExplosion.wav"));
	string name = "FireBall";
	pFireBall = new CParticleSystem;
	pFireBall->LoadEffect(name,"Resources/XMLFiles/FireBall.xml");
	pFireBall->CreateEmitter("FireBall");



	//SetParticleID(CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/fireball.png")) );
	//SetSprite(GetParticleID());

	bar = new PBar();
}


CFireball::~CFireball(void)
{
	CSGD_XAudio2::GetInstance()->SFXUnloadSound(m_nFireBallSFXID);
	CSGD_XAudio2::GetInstance()->SFXUnloadSound(m_nFireBallExplosionSFXID);

	delete bar;
	delete pFireBall;

}
///////////////////////////////////////////////
// IEntity Interface:
void		CFireball::Update				( float fElapsedTime )		
{
	Timer += fElapsedTime;
	if (GetIsCasting() == false)   //if powerbar is done
	{

		pFireBall->Update(fElapsedTime);

		//bool hit = false;
		if(target->GetPosX() > GetPosX()) 
		{
			SetPosX(GetPosX()+2);
		}

		else if(target->GetPosX() < GetPosX()) 
		{   
			SetPosX(GetPosX()-2);
		}

		if(target->GetPosY() > GetPosY()) 
		{
			SetPosY(GetPosY()+2);
		}

		else if(target->GetPosY() < GetPosY()) 
		{
			SetPosY(GetPosY()-2);
		}
		pFireBall->GetEffect(0)->SetEmitterPosX((float)GetPosX()+16);
		pFireBall->GetEffect(0)->SetEmitterPosY((float)GetPosY()+16);

		if(GetPosX() == target->GetPosX() && GetPosY() == target->GetPosY())
		{
			//if(CSGD_XAudio2::GetInstance()->SFXIsSoundPlaying(m_nFireBallExplosionSFXID) == false)
			//	CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nFireBallExplosionSFXID,false);

			int damage = (int)((bar->GetDamageScale() / 100.0f) * 20);
			target->TakeDamage(GetOwner(), damage);

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
			CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nFireBallSFXID,false);
			SetIsCasting(false);
			pFireBall->GetEffect(0)->KillParticles();

		}
	}


}

void		CFireball::Render				( void )					
{
	if (GetIsCasting() == false)
	{

		pFireBall->Render();

	}
	else
		bar->Render();
	//PowerBar();
}

void		CFireball::PowerBar			( void )					
{
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();

	RECT rSource = {152,534,552,566};
	pD3D->DrawHollowRect(rSource,D3DCOLOR_ARGB(255,25,255,25),5);

	RECT rCursor = { rSource.left + GetDamageScale() * 4, rSource.top, rSource.left + GetDamageScale() * 4 + 5, rSource.bottom };
	pD3D->DrawRect(rCursor, D3DCOLOR_ARGB(255,0,0,255));

}