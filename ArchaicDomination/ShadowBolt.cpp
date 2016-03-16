#include "ShadowBolt.h"
#include "SGD Wrappers\CSGD_TextureManager.h"
#include "SGD Wrappers\CSGD_DirectInput.h"
#include "SGD Wrappers\CSGD_Direct3D.h"
#include "SGD Wrappers\CSGD_XAudio2.h"
#include "SGD Wrappers\CSGD_MessageSystem.h"
#include "Particle System\ParticleSystem.h"
#include "DestroyAbilityMessage.h"
#include "PowerBar.h"
#include "BattlePlay.h"
#include "BitmapFont.h"
#include "Game.h"
#include "Enemy.h"

CShadowBolt::CShadowBolt(void)
{
	//SetParticleID(CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/fireball.png")) );
	//SetSprite(GetParticleID());
	ChatBubbleID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/ChatBubble.png"));
	string name = "ShadowBolt";
	pShadowBolt = new CParticleSystem;
	pShadowBolt->LoadEffect(name,"Resources/XMLFiles/ShadowBolt.xml");
	pShadowBolt->CreateEmitter("ShadowBolt");
	bar = new PBar;

	shadowBoltCastID = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("Resources/Sounds/shadow_bolt.wav"));
	shadowBoltHitID = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("Resources/Sounds/shadow_bolt_impact.wav"));
	soundTime = -20;

	pFont = CGame::GetInstance()->GetFont();
	bChat = false;
	{
		int randNum = rand() % 100;
		if(randNum < 50)
		{

			bChat = true;

			phrase.push_back(_T("HADOUKEN"));
			phrase.push_back(_T("Taste FEAR!!"));
			phrase.push_back(_T("HAHAHAHAAHA!"));

			this->phraseToUse = rand() % 3;
		}

	}
}


CShadowBolt::~CShadowBolt(void)
{
	delete bar;
	delete pShadowBolt;

	CSGD_XAudio2::GetInstance()->SFXUnloadSound(shadowBoltCastID);
	CSGD_XAudio2::GetInstance()->SFXUnloadSound(shadowBoltHitID);
}
///////////////////////////////////////////////
// IEntity Interface:
void		CShadowBolt::Update				( float fElapsedTime )		
{
	if (GetIsCasting() == false)   //if powerbar is done
	{
		pShadowBolt->Update(fElapsedTime);
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
		pShadowBolt->GetEffect(0)->SetEmitterPosX((float)GetPosX()+16);
		pShadowBolt->GetEffect(0)->SetEmitterPosY((float)GetPosY()+16);

		soundTime += fElapsedTime;

		if(GetPosX() == target->GetPosX() && GetPosY() == target->GetPosY() && soundTime < 0)
		{
			int damage = (int)((bar->GetDamageScale() / 100.0f) * 20);
			if( damage == 0 )
				damage = -1;

			target->TakeDamage(GetOwner(), damage);

			if(CSGD_XAudio2::GetInstance()->SFXIsSoundPlaying(shadowBoltCastID))
				CSGD_XAudio2::GetInstance()->SFXStopSound(shadowBoltCastID);
			CSGD_XAudio2::GetInstance()->SFXPlaySound(shadowBoltHitID);

			soundTime = 0;
		}
	}
	else
	{
		bar->Upate(fElapsedTime);
		if(bar->GetActive() == false)
		{
			SetIsCasting(false);
			CSGD_XAudio2::GetInstance()->SFXPlaySound(shadowBoltCastID);
			pShadowBolt->GetEffect(0)->KillParticles();
		}
	}

	if(soundTime > 1.0f)
	{	

		dynamic_cast<CBaseUnit*>(GetOwner())->SetAbility1_Cooldown(2);
		dynamic_cast<CBaseUnit*>(GetOwner())->SetHasAttacked();
		dynamic_cast<CBaseUnit*>(GetOwner())->SetIsAttacking(false);
			CEnemy::GetInstance()->SetIsCastingAbility(false);
		if(dynamic_cast<CBaseUnit*>(GetOwner())->HasMoved() == false)CBattlePlay::GetInstance()->SetShowUnitOptions(true);
		else
			CBattlePlay::GetInstance()->SetCurrentUnit(nullptr);

		CDestroyAbilityMessage* msg = new CDestroyAbilityMessage(GetOwner());
		CSGD_MessageSystem::GetInstance()->QueueMessage(msg);
		msg = nullptr;
	}
}

void		CShadowBolt::Render				( void )					
{
	if (GetIsCasting() == false)
	{
		pShadowBolt->Render();
		if(bChat)
		{
			CSGD_TextureManager::GetInstance()->Draw(ChatBubbleID,(int)GetOwner()->GetPosX(), (int)GetOwner()->GetPosY() - 32, 1.7f, 0.6f);
			pFont->Draw(phrase[phraseToUse],(int)GetOwner()->GetPosX() + 12,(int)GetOwner()->GetPosY() - 22, 0.6f,D3DCOLOR_XRGB(0,0,0));
		}

	}
	else
		bar->Render();
}

void		CShadowBolt::PowerBar			( void )					
{
}