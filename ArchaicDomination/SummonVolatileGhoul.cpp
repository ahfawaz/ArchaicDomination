#include "SummonVolatileGhoul.h"
#include "SGD Wrappers\CSGD_XAudio2.h"
#include "SGD Wrappers\CSGD_TextureManager.h"
#include "SGD Wrappers\CSGD_MessageSystem.h"
#include "GamePlay.h"
#include "DestroyAbilityMessage.h"
#include "Particle System\ParticleSystem.h"
#include "BaseUnit.h"
#include "BattlePlay.h"
#include "BitmapFont.h"
#include "Game.h"
#include "Enemy.h"
#include "FillBar.h"
#include "WorldPlay.h"
#include "ArcadeGamePlay.h"
#include "MainMenu.h"




CSummonVolatileGhoul::CSummonVolatileGhoul(void)
{
	ChatBubbleID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/ChatBubble.png"));
	bar = new FillBar;

	pSummon = new CParticleSystem;
	pSummon->LoadEffect("SummonVolatileGhoul", "Resources/XMLFiles/SummonVolatileGhoul.xml");
	pSummon->CreateEmitter("SummonVolatileGhoul");

	SFXSummonID = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("Resources/Sounds/ghoul_summon.wav"));
	CSGD_XAudio2::GetInstance()->SFXPlaySound(SFXSummonID, true);

	SFXRisenGhoulID = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("Resources/Sounds/ghoul_rising.wav"));
	risingTime = -20;

	pFont = CGame::GetInstance()->GetFont();
	bChat = false;
	if(CEnemy::GetInstance()->IsAI())
	{
		int randNum = rand() % 100;
		if(randNum < 50)
		{

			bChat = true;

			phrase.push_back(_T("RISE MINION!"));
			phrase.push_back(_T("Must focus."));
			phrase.push_back(_T("RISE!!!"));

			this->phraseToUse = rand() % 3;
		}

	}
}

CSummonVolatileGhoul::~CSummonVolatileGhoul(void)
{
	delete bar;
	delete pSummon;

	CSGD_XAudio2::GetInstance()->SFXUnloadSound(SFXRisenGhoulID);
	CSGD_XAudio2::GetInstance()->SFXUnloadSound(SFXSummonID);
}

///////////////////////////////////////////////
// IEntity Interface:
void		CSummonVolatileGhoul::Update				( float fElapsedTime )		
{

	if (GetIsCasting() == false)
	{
		if (risingTime < 0)
		{
			CBaseUnit* ghoul = new CBaseUnit();
			int ghoulSprite = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/ghoul.png"));
			if ( CMainMenu::GetInstance()->GetArcade() == true )
			{
				ghoul-> CreateUnit( VOLATILEGHOUL, CArcadeGamePlay::GetInstance()->GetCurX(), CArcadeGamePlay::GetInstance()->GetCurY());
			}
			else if (CMainMenu::GetInstance()->GetCampaign())
			{
				ghoul-> CreateUnit( VOLATILEGHOUL, CGamePlay::GetInstance()->GetCurX(), CGamePlay::GetInstance()->GetCurY());
			}
			else if (CMainMenu::GetInstance()->GetVersus() == true || CMainMenu::GetInstance()->GetQuickMatch() == true)
			{
				ghoul-> CreateUnit( VOLATILEGHOUL, CWorldPlay::GetInstance()->GetCurX(), CWorldPlay::GetInstance()->GetCurY());
			}
			ghoul->SetSpriteID(ghoulSprite);
			ghoul->SetPosX((float)GetPosX());
			ghoul->SetPosY((float)GetPosY());
			ghoul->SetTile(CBattlePlay::GetInstance()->GetGrid()->GetATile(GetPosY() / 32,GetPosX() / 32));
			CBattlePlay::GetInstance()->GetGrid()->GetATile(GetPosY() / 32,GetPosX() / 32)->SetOccupied(true);
			CEnemy::GetInstance()->AddUnit(ghoul);

			CSGD_XAudio2::GetInstance()->SFXPlaySound(SFXRisenGhoulID); 
			risingTime = 0;
		}
		else if(risingTime >= 1.2f)
		{
			dynamic_cast<CBaseUnit*>(GetOwner())->SetAbility2_Cooldown(4);
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

		risingTime += fElapsedTime;
	}
	else 
	{
		bar->Upate(fElapsedTime);
		pSummon->Update(fElapsedTime);
		pSummon->GetEffect(0)->SetEmitterPosX((float)GetPosX() + 16.0f);
		pSummon->GetEffect(0)->SetEmitterPosY((float)GetPosY() + 16.0f);

		if(bar->GetActive() == false)
			SetIsCasting(false);
	}

}

void		CSummonVolatileGhoul::Render				( void )					
{
	if(GetIsCasting())
	{
		bar->Render();
		pSummon->Render();
		if(bChat)
		{
			CSGD_TextureManager::GetInstance()->Draw(ChatBubbleID,(int)GetOwner()->GetPosX(), (int)GetOwner()->GetPosY() - 32, 1.7f, 0.6f);
			pFont->Draw(phrase[phraseToUse],(int)GetOwner()->GetPosX() + 12,(int)GetOwner()->GetPosY() - 22, 0.6f,D3DCOLOR_XRGB(0,0,0));
		}
	}
}

void		CSummonVolatileGhoul::PowerBar			( void )					
{
}

bool		CSummonVolatileGhoul::SetSummonPosition ( void )
{
	int posX = (int)GetOwner()->GetPosX() / 32;
	int posY = (int)GetOwner()->GetPosY() / 32;
	pSummon->GetEffect(0)->KillParticles();

	if(CBattlePlay::GetInstance()->GetGrid()->GetATile(posY,posX - 1)->GetOccupied() == false)
	{
		SetPosX((posX - 1) * 32);
		SetPosY(posY * 32);

		return true;
	}
	else if(CBattlePlay::GetInstance()->GetGrid()->GetATile(posY -1 ,posX)->GetOccupied() == false)
	{
		SetPosX(posX * 32);
		SetPosY((posY - 1) * 32);

		return true;
	}
	else if(CBattlePlay::GetInstance()->GetGrid()->GetATile(posY + 1,posX)->GetOccupied() == false)
	{
		SetPosX(posX * 32);
		SetPosY((posY + 1) * 32);

		return true;
	}
	else if(CBattlePlay::GetInstance()->GetGrid()->GetATile(posY,posX + 1)->GetOccupied() == false)
	{
		SetPosX((posX + 1) * 32);
		SetPosY(posY * 32);

		return true;
	}

	return false;
}

