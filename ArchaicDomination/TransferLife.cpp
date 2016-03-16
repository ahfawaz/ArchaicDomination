#include "TransferLife.h"
#include "SGD Wrappers\CSGD_TextureManager.h"
#include "SGD Wrappers\CSGD_MessageSystem.h"
#include "SGD Wrappers\CSGD_XAudio2.h"
#include "DestroyAbilityMessage.h"
#include "PowerBar.h"
#include "BattlePlay.h"
#include "Particle System\ParticleSystem.h"

#include "BitmapFont.h"
#include "Game.h"
#include "Enemy.h"


#define PARTICLE_INTERVAL  0.1f

CTransferLife::CTransferLife(void)
{
	ChatBubbleID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/ChatBubble.png"));
	bar = new PBar;

	particleStep = 0;
	pTransferLife = new CParticleSystem;
	pTransferLife->LoadEffect("TransferLife", "Resources/XMLFiles/TransferLife.xml");
	pTransferLife->CreateEmitter("TransferLife");

	SFXTransferID = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("Resources/Sounds/AD_ShdwPrstHeal_Final.wav"));

	pFont = CGame::GetInstance()->GetFont();
	bChat = false;
	int randNum = rand() % 100;
	if(randNum < 50)
	{

		bChat = true;

		phrase.push_back(_T("My life for the Lich"));
		phrase.push_back(_T("Be more useful!"));
		phrase.push_back(_T("......"));

		this->phraseToUse = rand() % 3;
	}

}


CTransferLife::~CTransferLife(void)
{
	delete bar;
	delete pTransferLife;

	CSGD_XAudio2::GetInstance()->SFXUnloadSound(SFXTransferID);
}
///////////////////////////////////////////////
// IEntity Interface:
void		CTransferLife::Update				( float fElapsedTime )		
{
	if (GetIsCasting() == false)   //if powerbar is done
	{
		//Intial Damage
		if (GetPosX() == GetOwner()->GetPosX() && GetPosY() == GetOwner()->GetPosY())
		{
			int damage = (int)((bar->GetDamageScale() / 100.0f) * 20);
			if(damage <= 1)
				damage = -1;
			if(GetOwner()->GetCurrentHealth() <= damage)
				damage = GetOwner()->GetCurrentHealth() - 1;

			GetOwner()->TakeDamage(GetOwner(),damage); 
		}

		pTransferLife->Update(fElapsedTime);

		//Updating Particles Position
		if (particleStep >= PARTICLE_INTERVAL)
		{
			if(target->GetPosX() > GetPosX()) 
			{
				SetPosX(GetPosX()+8);
			}

			else if( target->GetPosX() < GetPosX()) 
			{   
				SetPosX(GetPosX()-8);
			}

			if(target->GetPosY() > GetPosY()) 
			{
				SetPosY(GetPosY()+8);
			}

			else if(target->GetPosY() < GetPosY()) 
			{
				SetPosY(GetPosY()-8);
			} 
			particleStep = 0;
		}
		particleStep += fElapsedTime;

		pTransferLife->GetEffect(0)->SetEmitterPosX((float)GetPosX()+16);
		pTransferLife->GetEffect(0)->SetEmitterPosY((float)GetPosY()+16);

		//Final Healing and destroying ability.
		if (GetPosX() == target->GetPosX() && GetPosY() == target->GetPosY())
		{
			int damage = (int)((bar->GetDamageScale() / 100.0f) * 20);

			target->SetCurrentHealth(target->GetCurrentHealth() + damage);
			if(target->GetCurrentHealth() > target->GetMaxHealth())
				target->SetCurrentHealth(target->GetMaxHealth());


			std::string text = "+" + to_string(damage);
			CFloatingText* newText = new CFloatingText(text, (float)target->GetPosX(), (float)target->GetPosY(), HEAL);
			CBattlePlay::GetInstance()->PushText(newText);

			dynamic_cast<CBaseUnit*>(GetOwner())->SetAbility2_Cooldown(1);
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

			SetPosX((int)GetOwner()->GetPosX());
			SetPosY((int)GetOwner()->GetPosY());

			particleStep = PARTICLE_INTERVAL;
			CSGD_XAudio2::GetInstance()->SFXPlaySound(SFXTransferID);

			pTransferLife->GetEffect(0)->KillParticles();
		}
	}
}

void		CTransferLife::Render				( void )					
{
	if(bar->GetActive())
		bar->Render();
	else
	{
		pTransferLife->Render();
		if(bChat)
		{
			CSGD_TextureManager::GetInstance()->Draw(ChatBubbleID,(int)GetOwner()->GetPosX(), (int)GetOwner()->GetPosY() - 32, 2.0f, 0.6f);
			pFont->Draw(phrase[phraseToUse],(int)GetOwner()->GetPosX() + 16,(int)GetOwner()->GetPosY() - 22, 0.6f,D3DCOLOR_XRGB(0,0,0));
		}
	}
}

void		CTransferLife::PowerBar			( void )					
{
}