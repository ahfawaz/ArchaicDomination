#include "PlagueOn.h"
#include "SGD Wrappers\CSGD_XAudio2.h"
#include "SGD Wrappers\CSGD_MessageSystem.h"
#include "Particle System\ParticleSystem.h"
#include "SGD Wrappers\CSGD_TextureManager.h"
#include "DestroyAbilityMessage.h"
#include "BattlePlay.h"
#include "BitmapFont.h"
#include "Game.h"
#include "Enemy.h"

CPlagueOn::CPlagueOn(void)
{
	ChatBubbleID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/ChatBubble.png"));
	particleTimer = 0;
	pPlagueOn = new CParticleSystem;
	pPlagueOn->LoadEffect("PlagueOn", "Resources/XMLFiles/PlagueOn.xml");
	pPlagueOn->CreateEmitter("PlagueOn");

	SFXPlagueOnID = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("Resources/Sounds/plague_on.wav"));
	CSGD_XAudio2::GetInstance()->SFXPlaySound(SFXPlagueOnID);

	pFont = CGame::GetInstance()->GetFont();
	bChat = false;
	{
		int randNum = rand() % 100;
		if(randNum < 50)
		{

			bChat = true;

			phrase.push_back(_T("PLAGUE ON!!!"));
			phrase.push_back(_T("Careful with that!"));
			phrase.push_back(_T("Ready the machine!"));

			this->phraseToUse = rand() % 3;
		}

	}

	pPlagueOn->GetEffect(0)->KillParticles();

}


CPlagueOn::~CPlagueOn(void)
{
	delete pPlagueOn;
	CSGD_XAudio2::GetInstance()->SFXUnloadSound(SFXPlagueOnID);
	CSGD_TextureManager::GetInstance()->UnloadTexture(ChatBubbleID);
}
///////////////////////////////////////////////
// IEntity Interface:
void		CPlagueOn::Update				( float fElapsedTime )		
{
	particleTimer += fElapsedTime;
	if(particleTimer > 2.0f)
	{
		dynamic_cast<CBaseUnit*>(GetOwner())->SetAbility1_Cooldown(5);
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
	else 
	{
		pPlagueOn->Update(fElapsedTime);
		pPlagueOn->GetEffect(0)->SetEmitterPosX(GetOwner()->GetPosX() + 16);
		pPlagueOn->GetEffect(0)->SetEmitterPosY(GetOwner()->GetPosY() + 16);
	}

}

void		CPlagueOn::Render				( void )					
{
	pPlagueOn->Render();
	if(bChat)
	{
		CSGD_TextureManager::GetInstance()->Draw(ChatBubbleID,(int)GetOwner()->GetPosX(), (int)GetOwner()->GetPosY() - 32, 2.0f, 0.6f);
		pFont->Draw(phrase[phraseToUse],(int)GetOwner()->GetPosX() + 16,(int)GetOwner()->GetPosY() - 22, 0.6f,D3DCOLOR_XRGB(0,0,0));
	}
}

void		CPlagueOn::PowerBar			( void )					
{
}