#include "DarkCurse.h"
#include "SGD Wrappers\CSGD_TextureManager.h"
#include "SGD Wrappers\CSGD_MessageSystem.h"
#include "SGD Wrappers\CSGD_XAudio2.h"
#include "Particle System\ParticleSystem.h"
#include "DestroyDarkCurseMessage.h"
#include "BattlePlay.h"
#include "SpamBar.h"
#include "Player.h"

#include "BitmapFont.h"
#include "Game.h"
#include "Enemy.h"

#define PARTICLE_LIFE 2.0f

CDarkCurse::CDarkCurse(void)
{
	tickNum = 4;
	activeTick = 0;
	bar = new  SpamBar;

	for(unsigned int i = 0; i < CPlayer::GetInstance()->GetUnits().size(); i++)
	{
		std::string text = "Dark Curse";
		CFloatingText* newText = new CFloatingText(text, (float)CPlayer::GetInstance()->GetUnits()[i]->GetPosX(), (float)CPlayer::GetInstance()->GetUnits()[i]->GetPosY(), DEBUFF);
		CBattlePlay::GetInstance()->PushText(newText);
	}

	for(unsigned int i = 0; i < CPlayer::GetInstance()->GetUnits().size(); i++)
	{
		CParticleSystem* pCurse = new CParticleSystem;
		pCurse->LoadEffect("DarkCurse", "Resources/XMLFiles/DarkCurse.xml");
		pCurse->CreateEmitter("DarkCurse");
		pCurse->GetEffect(0)->KillParticles();
		pDarkCurse.push_back(pCurse);
	}

	SFXapplyCurseID = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("Resources/Sounds/dark_curse_apply.wav"));
	CSGD_XAudio2::GetInstance()->SFXPlaySound(SFXapplyCurseID);

	SFXCurseTickID = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("Resources/Sounds/dark_curse_tick.wav"));

	ChatBubbleID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/ChatBubble.png"));

	pFont = CGame::GetInstance()->GetFont();
	bChat = false;
	{
		int randNum = rand() % 100;
		if(randNum < 50)
		{

			bChat = true;

			phrase.push_back(_T("Feel my Wrath!"));
			phrase.push_back(_T("Embrace darkness!"));
			phrase.push_back(_T("Curse the living!"));

			this->phraseToUse = rand() % 3;
		}

	}
}


CDarkCurse::~CDarkCurse(void)
{
	delete bar;

	for(unsigned int i = 0; i < pDarkCurse.size(); i++)
		delete pDarkCurse[i];

	CSGD_XAudio2::GetInstance()->SFXUnloadSound(SFXapplyCurseID);
	CSGD_XAudio2::GetInstance()->SFXUnloadSound(SFXCurseTickID);
}
///////////////////////////////////////////////
// IEntity Interface:
void		CDarkCurse::Update				( float fElapsedTime )		
{
	activeTick += fElapsedTime;

	if(activeTick < PARTICLE_LIFE)
	{
		for(unsigned int i = 0; i < CPlayer::GetInstance()->GetUnits().size(); i++)
		{
			pDarkCurse[i]->Update(fElapsedTime);

			pDarkCurse[i]->GetEffect(0)->SetEmitterPosX(CPlayer::GetInstance()->GetUnits()[i]->GetPosX() + 16.0f);
			pDarkCurse[i]->GetEffect(0)->SetEmitterPosY(CPlayer::GetInstance()->GetUnits()[i]->GetPosY() + 16.0f);
		}
	}

	if (GetIsCasting() == false && activeTick > PARTICLE_LIFE)
	{
		if (tickNum == 4)
		{
			tickNum--;
			dynamic_cast<CBaseUnit*>(GetOwner())->SetAbility1_Cooldown(5);
			dynamic_cast<CBaseUnit*>(GetOwner())->SetHasAttacked();
			dynamic_cast<CBaseUnit*>(GetOwner())->SetIsAttacking(false);
			CEnemy::GetInstance()->SetIsCastingAbility(false);
			if(dynamic_cast<CBaseUnit*>(GetOwner())->HasMoved() == false)CBattlePlay::GetInstance()->SetShowUnitOptions(true);
			else
				CBattlePlay::GetInstance()->SetCurrentUnit(nullptr); 
		}

		if(tickNum <= 0)
		{
			CDestroyDarkCurseMessage* msg = new CDestroyDarkCurseMessage(GetOwner());
			CSGD_MessageSystem::GetInstance()->QueueMessage(msg);
			msg = nullptr;
		} 
	}
	else
	{
		bar->Upate(fElapsedTime);
		if(bar->GetActive() == false)
			SetIsCasting(false);

		SetDamageScale(bar->GetDamageScale() / 20);
	}

}

void		CDarkCurse::Render				( void )					
{
	if(bar->GetActive())
	{
		bar->Render();
		if(bChat)
		{
			CSGD_TextureManager::GetInstance()->Draw(ChatBubbleID,(int)GetOwner()->GetPosX(), (int)GetOwner()->GetPosY() - 32, 2.0f, 0.6f);
			pFont->Draw(phrase[phraseToUse],(int)GetOwner()->GetPosX() + 16,(int)GetOwner()->GetPosY() - 22, 0.6f,D3DCOLOR_XRGB(0,0,0));
		}
	}

	if( activeTick < PARTICLE_LIFE )
	{
		for(unsigned int i = 0; i < CPlayer::GetInstance()->GetUnits().size(); i++)
			pDarkCurse[i]->Render();
	}

}

void		CDarkCurse::PowerBar			( void )					
{
}

void		CDarkCurse::DealDamage			( void )
{
	tickNum--;
	activeTick = 0;
	for(unsigned int i = 0; i < CPlayer::GetInstance()->GetUnits().size(); i++)
	{
		CPlayer::GetInstance()->GetUnits()[i]->TakeDamage(GetOwner(), 2 + GetDamageScale());
		pDarkCurse[i]->GetEffect(0)->KillParticles();
	}

	CSGD_XAudio2::GetInstance()->SFXPlaySound(SFXCurseTickID);
}

