#include "DarkCharge.h"
#include "SGD Wrappers\CSGD_MessageSystem.h"
#include "SGD Wrappers\CSGD_XAudio2.h"
#include "SGD Wrappers\CSGD_TextureManager.h"
#include "DestroyAbilityMessage.h"
#include "Particle System\ParticleSystem.h"
#include "BattlePlay.h"
#include "AccuracyBar.h"

#include "BitmapFont.h"
#include "Game.h"
#include "Enemy.h"

#define KNIGHT_STEP 0.5f
#define PARTICLE_INTERVAL 0.25f


CDarkCharge::CDarkCharge(void)
{
	ChatBubbleID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/ChatBubble.png"));
	stepTime = 0;
	particleStep = 0;
	bar = new AccuracyBar;

	pDarkCharge = new CParticleSystem;
	pDarkCharge->LoadEffect("DarkCharge","Resources/XMLFiles/DarkCharge.xml");
	pDarkCharge->CreateEmitter("DarkCharge");

	SFXChargeID = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("Resources/Sounds/dark_charge.wav"));

	pFont = CGame::GetInstance()->GetFont();
	bChat = false;
	{
		int randNum = rand() % 100;
		if(randNum < 50)
		{

			bChat = true;

			phrase.push_back(_T("DEMACIA!!!!!"));
			phrase.push_back(_T("THIS...IS..."));
			phrase.push_back(_T("KA POW!!!"));

			this->phraseToUse = rand() % 3;
		}

	}
}


CDarkCharge::~CDarkCharge(void)
{
	delete bar;
	delete pDarkCharge;
	CSGD_TextureManager::GetInstance()->UnloadTexture(ChatBubbleID);
	CSGD_XAudio2::GetInstance()->SFXUnloadSound(SFXChargeID);
}
///////////////////////////////////////////////
// IEntity Interface:
void		CDarkCharge::Update				( float fElapsedTime )		
{
	if (GetIsCasting() == false) 
	{

		//Target's and Owner's position.
		int targetX = (int)target->GetPosX() / 32;
		int targetY = (int)target->GetPosY() / 32;
		int ownerX = (int)GetOwner()->GetPosX() / 32;
		int ownerY = (int)GetOwner()->GetPosY() / 32;

		//Owner's movement to Target
		if (stepTime == 0)
		{
			dynamic_cast<CBaseUnit*>(GetOwner())->Reset();

			if(target->GetPosX() > GetOwner()->GetPosX()) 
				dynamic_cast<CBaseUnit*>(GetOwner())->Move(CBattlePlay::GetInstance()->GetGrid()->GetTiles()[targetY][targetX - 1]);

			else if(target->GetPosX() < GetOwner()->GetPosX()) 
				dynamic_cast<CBaseUnit*>(GetOwner())->Move(CBattlePlay::GetInstance()->GetGrid()->GetTiles()[targetY][targetX + 1]);

			else if(target->GetPosY() > GetOwner()->GetPosY()) 
				dynamic_cast<CBaseUnit*>(GetOwner())->Move(CBattlePlay::GetInstance()->GetGrid()->GetTiles()[targetY - 1][targetX]);

			else if(target->GetPosY() < GetOwner()->GetPosY()) 
				dynamic_cast<CBaseUnit*>(GetOwner())->Move(CBattlePlay::GetInstance()->GetGrid()->GetTiles()[targetY + 1][targetX]);
			stepTime += 15;
		}

		pDarkCharge->Update(fElapsedTime);

		//Particle moving with Owner
		if( particleStep > PARTICLE_INTERVAL )
		{
			if(target->GetPosX() > GetOwner()->GetPosX()) 
				SetPosX(GetPosX() + 16);

			else if(target->GetPosX() < GetOwner()->GetPosX()) 
				SetPosX(GetPosX() - 16);

			else if(target->GetPosY() > GetOwner()->GetPosY()) 
				SetPosY(GetPosY() + 16);

			else if(target->GetPosY() < GetOwner()->GetPosY()) 
				SetPosY(GetPosY() - 16);

			particleStep = 0;
		}
		particleStep += fElapsedTime;

		pDarkCharge->GetEffect(0)->SetEmitterPosX((float)GetPosX());
		pDarkCharge->GetEffect(0)->SetEmitterPosY((float)GetPosY());

		if(targetX - 1 == ownerX || targetX + 1 == ownerX || targetY - 1 == ownerY || targetY + 1 == ownerY )
		{

			int damage = (int)((bar->GetDamageScale() / 100.0f) * 20);
			if(damage == 0)
			{
				std::string text = "Miss...";
				CFloatingText* newText = new CFloatingText(text, (float)GetOwner()->GetPosX(), (float)GetOwner()->GetPosY(), DAMAGE);
				CBattlePlay::GetInstance()->PushText(newText);
			}
			else
				target->TakeDamage(GetOwner(), damage);

			dynamic_cast<CBaseUnit*>(GetOwner())->SetAbility1_Cooldown(3);
			dynamic_cast<CBaseUnit*>(GetOwner())->SetHasAttacked();
			dynamic_cast<CBaseUnit*>(GetOwner())->SetIsAttacking(false);
			dynamic_cast<CBaseUnit*>(GetOwner())->SetHasMoved(hasMovedBefore);
			CEnemy::GetInstance()->SetIsCastingAbility(false);
			if(dynamic_cast<CBaseUnit*>(GetOwner())->HasMoved() == false)CBattlePlay::GetInstance()->SetShowUnitOptions(true);
			else
			{
				CBattlePlay::GetInstance()->SetShowUnitOptions(false);
				CBattlePlay::GetInstance()->SetCurrentUnit(nullptr);
			}

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
			CSGD_XAudio2::GetInstance()->SFXPlaySound(SFXChargeID,true);
			SetDamageScale(bar->GetDamageScale());

			SetPosX((int)GetOwner()->GetPosX() + 16);
			SetPosY((int)GetOwner()->GetPosY() + 16);

			particleStep = -0.25;
			pDarkCharge->GetEffect(0)->KillParticles();
		}
	}

}

void		CDarkCharge::Render				( void )					
{
	if(GetIsCasting())
	{
		bar->Render();
	}
	else
	{
		pDarkCharge->Render();
		if(bChat)
		{
			CSGD_TextureManager::GetInstance()->Draw(ChatBubbleID,(int)GetOwner()->GetPosX(), (int)GetOwner()->GetPosY() - 32, 2.0f, 0.6f);
			pFont->Draw(phrase[phraseToUse],(int)GetOwner()->GetPosX() + 16,(int)GetOwner()->GetPosY() - 22, 0.6f,D3DCOLOR_XRGB(0,0,0));
		}
	}


}

void		CDarkCharge::PowerBar			( void )					
{
}