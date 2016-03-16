#include "Cleave.h"

#include "SGD Wrappers\CSGD_TextureManager.h"
#include "SGD Wrappers\CSGD_MessageSystem.h"
#include "SGD Wrappers\CSGD_XAudio2.h"
#include "Particle System\ParticleSystem.h"
#include "CleaveMessage.h"
#include "DestroyAbilityMessage.h"
#include "PowerBar.h"
#include "BattlePlay.h"
#include "BitmapFont.h"
#include "Game.h"
#include "Enemy.h"
#include "Player.h"

CCleave::CCleave(void)
{
	ChatBubbleID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/ChatBubble.png"));
	m_nCleaveSFXID = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("Resources/Sounds/Cleave.wav"));
	string name = "Cleave";
	pCleave = new CParticleSystem;
	pCleave->LoadEffect(name,"Resources/XMLFiles/Cleave.xml");
	pCleave->CreateEmitter("Cleave");

	bar = new PBar();

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


CCleave::~CCleave(void)
{
	CSGD_XAudio2::GetInstance()->SFXUnloadSound(m_nCleaveSFXID);
	CSGD_TextureManager::GetInstance()->UnloadTexture(ChatBubbleID);
	delete bar;
	delete pCleave;
}
///////////////////////////////////////////////
// IEntity Interface:
void		CCleave::Update				( float fElapsedTime )		
{
	particleTimer += fElapsedTime;
	if(GetIsCasting() == false && particleTimer >= 2.0f)
	{
		CCleaveMessage* cleaveMSG = new CCleaveMessage(GetOwner());
		CSGD_MessageSystem::GetInstance()->QueueMessage(cleaveMSG);
		cleaveMSG = nullptr;

		dynamic_cast<CBaseUnit*>(GetOwner())->SetAbility1_Cooldown(2);
		dynamic_cast<CBaseUnit*>(GetOwner())->SetHasAttacked();
		dynamic_cast<CBaseUnit*>(GetOwner())->SetIsAttacking(false);
		CPlayer::GetInstance()->SetIsCastingAbility(false);
		CEnemy::GetInstance()->SetIsCastingAbility(false);
		if(dynamic_cast<CBaseUnit*>(GetOwner())->HasMoved() == false)CBattlePlay::GetInstance()->SetShowUnitOptions(true);
		else
			CBattlePlay::GetInstance()->SetCurrentUnit(nullptr);

		CDestroyAbilityMessage* msg = new CDestroyAbilityMessage(GetOwner());
		CSGD_MessageSystem::GetInstance()->QueueMessage(msg);
		msg = nullptr;
	}
	else if(GetIsCasting() == false)
	{
		pCleave->Update(fElapsedTime);
		pCleave->GetEffect(0)->SetEmitterPosX((float)GetPosX() + 16);
		pCleave->GetEffect(0)->SetEmitterPosY((float)GetPosY() + 16);
	}

	else if(GetIsCasting() == true)
	{
		bar->Upate(fElapsedTime);
		if(bar->GetActive() == false)
		{
			CSGD_XAudio2::GetInstance()->SFXPlaySound(m_nCleaveSFXID,false);
			SetIsCasting(false);
			pCleave->GetEffect(0)->KillParticles();
		}

		SetDamageScale(bar->GetDamageScale());
		pCleave->GetEffect(0)->SetEmitterPosX((float)GetOwner()->GetPosX());
		pCleave->GetEffect(0)->SetEmitterPosY((float)GetOwner()->GetPosY());

		particleTimer = 0;
	}
}

void		CCleave::Render				( void )					
{
	if (GetIsCasting() == false)
	{

		pCleave->Render();
		if(bChat)
		{
			CSGD_TextureManager::GetInstance()->Draw(ChatBubbleID,(int)GetOwner()->GetPosX(), (int)GetOwner()->GetPosY() - 32, 1.7f, 0.6f);
			pFont->Draw(phrase[phraseToUse],(int)GetOwner()->GetPosX() + 12,(int)GetOwner()->GetPosY() - 22, 0.6f,D3DCOLOR_XRGB(0,0,0));
		}

		//return;
	}
	else
		bar->Render();

}

void		CCleave::PowerBar			( void )					
{

}