#include "EmpoweringCurse.h"
#include "SGD Wrappers\CSGD_TextureManager.h"
#include "DestroyAbilityMessage.h"
#include "SGD Wrappers\CSGD_MessageSystem.h"
#include "SGD Wrappers\CSGD_XAudio2.h"
#include "Particle System\ParticleSystem.h"
#include "Enemy.h"
#include "BattlePlay.h"
#include "BitmapFont.h"
#include "Game.h"
#include "Enemy.h"

CEmpoweringCurse::CEmpoweringCurse(void)
{
	duration = 0;
	ChatBubbleID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/ChatBubble.png"));
	for(unsigned int i = 0; i < CEnemy::GetInstance()->GetUnits().size(); i++)
	{
		CParticleSystem* power = new CParticleSystem;
		power->LoadEffect("Empower","Resources/XMLFiles/EmpoweringCurse.xml");
		power->CreateEmitter("Empower");
		power->GetEffect(0)->KillParticles();
		pEmpower.push_back(power);
	}

	empowerID = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("Resources/Sounds/empowering_curse.wav"));
	CSGD_XAudio2::GetInstance()->SFXPlaySound(empowerID);

	pFont = CGame::GetInstance()->GetFont();
	bChat = false;
	{
		int randNum = rand() % 100;
		if(randNum < 50)
		{

			bChat = true;

			phrase.push_back(_T("Kill the living!"));
			phrase.push_back(_T("MORE CHAOS!!!"));
			phrase.push_back(_T("Leave nothing!"));

			this->phraseToUse = rand() % 3;
		}

	}


}


CEmpoweringCurse::~CEmpoweringCurse(void)
{
	for(unsigned int i = 0; i < pEmpower.size(); i++)
		delete pEmpower[i];
	CSGD_TextureManager::GetInstance()->UnloadTexture(ChatBubbleID);
	CSGD_XAudio2::GetInstance()->SFXUnloadSound(empowerID);
}
///////////////////////////////////////////////
// IEntity Interface:
void		CEmpoweringCurse::Update				( float fElapsedTime )		
{
	duration += fElapsedTime;

	if(duration < 2)
	{
		for(unsigned int i = 0; i < pEmpower.size(); i++)
		{
			pEmpower[i]->Update(fElapsedTime);

			pEmpower[i]->GetEffect(0)->SetEmitterPosX(CEnemy::GetInstance()->GetUnits()[i]->GetPosX() + 32.0f);
			pEmpower[i]->GetEffect(0)->SetEmitterPosY(CEnemy::GetInstance()->GetUnits()[i]->GetPosY() + 32.0f);
		}
	}
	else
	{
		dynamic_cast<CBaseUnit*>(GetOwner())->SetAbility2_Cooldown(6);
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

void		CEmpoweringCurse::Render				( void )					
{
	for(unsigned int i = 0; i < pEmpower.size(); i++)
		pEmpower[i]->Render();

	if(bChat)
	{
		CSGD_TextureManager::GetInstance()->Draw(ChatBubbleID,(int)GetOwner()->GetPosX(), (int)GetOwner()->GetPosY() - 32, 1.7f, 0.6f);
		pFont->Draw(phrase[phraseToUse],(int)GetOwner()->GetPosX() + 12,(int)GetOwner()->GetPosY() - 22, 0.6f,D3DCOLOR_XRGB(0,0,0));
	}

}

void		CEmpoweringCurse::PowerBar			( void )					
{
}