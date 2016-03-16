#include "DrainLife.h"
#include "SGD Wrappers\CSGD_XAudio2.h"
#include "SGD Wrappers\CSGD_TextureManager.h"
#include "SGD Wrappers\CSGD_MessageSystem.h"
#include "DestroyAbilityMessage.h"
#include "PowerBar.h"
#include "BattlePlay.h"
#include "Particle System\ParticleSystem.h"

#include "BitmapFont.h"
#include "Game.h"
#include "Enemy.h"

#define PARTICLE_INTERVAL  0.1f

CDrainLife::CDrainLife(void)
{
	ChatBubbleID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/ChatBubble.png"));
	bar = new PBar;

	particleStep = 0;
	pDrainLife = new CParticleSystem;
	pDrainLife->LoadEffect("DrainLife", "Resources/XMLFiles/DrainLife.xml");
	pDrainLife->CreateEmitter("DrainLife");

	SFXDrainLifeID = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("Resources/Sounds/drain_life.wav"));

	pFont = CGame::GetInstance()->GetFont();
	bChat = false;
	{
		int randNum = rand() % 100;
		if(randNum < 50)
		{

			bChat = true;

			phrase.push_back(_T("Your life is mine..."));
			phrase.push_back(_T("hisssss....."));
			phrase.push_back(_T("*Demonic Sound*"));

			this->phraseToUse = rand() % 3;
		}

	}
}


CDrainLife::~CDrainLife(void)
{
	delete bar;
	delete pDrainLife;

	CSGD_XAudio2::GetInstance()->SFXUnloadSound(SFXDrainLifeID);
}
///////////////////////////////////////////////
// IEntity Interface:
void		CDrainLife::Update				( float fElapsedTime )		
{
	if (GetIsCasting() == false)   //if powerbar is done
	{
		//Intial Damage
		if (GetPosX() == target->GetPosX() && GetPosY() == target->GetPosY())
		{
			int damage = (int)((bar->GetDamageScale() / 100.0f) * 20);
			if(damage == 0) 
				damage = -1;

			target->TakeDamage(GetOwner(), damage); 
		}

		pDrainLife->Update(fElapsedTime);

		//Updating Particles Position
		if (particleStep >= PARTICLE_INTERVAL)
		{
			if(GetOwner()->GetPosX() > GetPosX()) 
			{
				SetPosX(GetPosX()+8);
			}

			else if( GetOwner()->GetPosX() < GetPosX()) 
			{   
				SetPosX(GetPosX()-8);
			}

			if(GetOwner()->GetPosY() > GetPosY()) 
			{
				SetPosY(GetPosY()+8);
			}

			else if(GetOwner()->GetPosY() < GetPosY()) 
			{
				SetPosY(GetPosY()-8);
			} 
			particleStep = 0;
		}
		particleStep += fElapsedTime;

		pDrainLife->GetEffect(0)->SetEmitterPosX((float)GetPosX()+16);
		pDrainLife->GetEffect(0)->SetEmitterPosY((float)GetPosY()+16);

		//Final Healing and destroying ability.
		if (GetPosX() == GetOwner()->GetPosX() && GetPosY() == GetOwner()->GetPosY())
		{
			int damage = (int)((bar->GetDamageScale() / 100.0f) * 20);

			GetOwner()->SetCurrentHealth(GetOwner()->GetCurrentHealth() + damage);

			std::string text = "+" + to_string(damage);
			CFloatingText* newText = new CFloatingText(text, (float)GetOwner()->GetPosX(), (float)GetOwner()->GetPosY(), HEAL);
			CBattlePlay::GetInstance()->PushText(newText);

			if(GetOwner()->GetCurrentHealth() > GetOwner()->GetMaxHealth())
				GetOwner()->SetCurrentHealth(GetOwner()->GetMaxHealth());

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
	else
	{
		bar->Upate(fElapsedTime);
		if(bar->GetActive() == false)
		{
			SetIsCasting(false);

			SetPosX((int)target->GetPosX());
			SetPosY((int)target->GetPosY());

			particleStep = PARTICLE_INTERVAL;

			CSGD_XAudio2::GetInstance()->SFXPlaySound(SFXDrainLifeID);
			pDrainLife->GetEffect(0)->KillParticles();
		}
	}

}

void		CDrainLife::Render				( void )					
{
	if(bar->GetActive())
		bar->Render();
	else
	{
		pDrainLife->Render();
		if(bChat)
		{
			CSGD_TextureManager::GetInstance()->Draw(ChatBubbleID,(int)GetOwner()->GetPosX(), (int)GetOwner()->GetPosY() - 32, 2.0f, 0.6f);
			pFont->Draw(phrase[phraseToUse],(int)GetOwner()->GetPosX() + 16,(int)GetOwner()->GetPosY() - 22, 0.6f,D3DCOLOR_XRGB(0,0,0));
		}
	}

}

void		CDrainLife::PowerBar			( void )					
{
}