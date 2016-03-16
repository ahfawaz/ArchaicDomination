#include "Explode.h"
#include "SGD Wrappers\CSGD_TextureManager.h"
#include "SGD Wrappers\CSGD_MessageSystem.h"
#include "SGD Wrappers\CSGD_XAudio2.h"
#include "Particle System\ParticleSystem.h"
#include "DestroyGhoulMessage.h"
#include "Player.h"
#include "BattlePlay.h"

#include "BitmapFont.h"
#include "Game.h"
#include "Enemy.h"

CExplode::CExplode(void)
{
	explosionTime = 0;

	pExplode	 = new CParticleSystem;
	pExplode->LoadEffect("Explode", "Resources/XMLFiles/Explode.xml");
	pExplode->CreateEmitter("Explode");

	explosionSFX = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("Resources/Sounds/ghoul_exploding.wav"));
	CSGD_XAudio2::GetInstance()->SFXPlaySound(explosionSFX);

	ChatBubbleID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/ChatBubble.png"));
	pFont = CGame::GetInstance()->GetFont();
	bChat = false;
	{
		int randNum = rand() % 100;
		if(randNum < 50)
		{

			bChat = true;

			phrase.push_back(_T("eraghalejehf"));
			phrase.push_back(_T("ehhhh...."));
			phrase.push_back(_T("AAFGHABABGA!"));

			this->phraseToUse = rand() % 3;
		}

	}
	pExplode->GetEffect(0)->KillParticles();
}


CExplode::~CExplode(void)
{
	delete pExplode;

	CSGD_XAudio2::GetInstance()->SFXUnloadSound(explosionSFX);
}
///////////////////////////////////////////////
// IEntity Interface:
void		CExplode::Update				( float fElapsedTime )		
{
	if(explosionTime >= 0.1f)
	{
		pExplode->Update(fElapsedTime);

		pExplode->GetEffect(0)->SetEmitterPosX(GetOwner()->GetPosX()+16.0f);
		pExplode->GetEffect(0)->SetEmitterPosY(GetOwner()->GetPosY()+16.0f);
	}

	if(explosionTime >= 3)
	{
		//int distX = (int)GetOwner()->GetPosX() + 
		for(unsigned int i = 0; i < CPlayer::GetInstance()->GetUnits().size(); i++)
		{
			if( CPlayer::GetInstance()->GetUnits()[i]->GetPosX() <= (int)GetOwner()->GetPosX() + 64 &&
				CPlayer::GetInstance()->GetUnits()[i]->GetPosX() >= (int)GetOwner()->GetPosX() - 64 &&
				CPlayer::GetInstance()->GetUnits()[i]->GetPosY() <= (int)GetOwner()->GetPosY() + 64 &&
				CPlayer::GetInstance()->GetUnits()[i]->GetPosY() >= (int)GetOwner()->GetPosY() - 64 )
				CPlayer::GetInstance()->GetUnits()[i]->TakeDamage(GetOwner());
		}


		CDestroyGhoulMessage* msg = new CDestroyGhoulMessage(GetOwner());
		CSGD_MessageSystem::GetInstance()->QueueMessage(msg);
		msg = nullptr;
	}
	explosionTime += fElapsedTime;
}

void		CExplode::Render				( void )					
{

	if(explosionTime >= 0.1f)
		pExplode->Render();
	if(bChat)
	{
		CSGD_TextureManager::GetInstance()->Draw(ChatBubbleID,(int)GetOwner()->GetPosX(), (int)GetOwner()->GetPosY() - 32, 2.0f, 0.6f);
		pFont->Draw(phrase[phraseToUse],(int)GetOwner()->GetPosX() + 16,(int)GetOwner()->GetPosY() - 22, 0.6f,D3DCOLOR_XRGB(0,0,0));
	}

}

void		CExplode::PowerBar			( void )					
{
}