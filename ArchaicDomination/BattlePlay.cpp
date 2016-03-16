#include "BattlePlay.h"

#include <string>

#include "Game.h"
#include "FloatingText.h"

//Systems
#include "Animation System\AnimationSystem.h"
#include "Animation System\AnimTimeStamp.h"
#include "TileSystem\CTileSystem.h"
#include "Particle System\ParticleSystem.h"

//SGD Wrappers
#include "SGD Wrappers\CSGD_DirectInput.h"
#include "SGD Wrappers\CSGD_TextureManager.h"
#include "SGD Wrappers\CSGD_MessageSystem.h"
#include "SGD Wrappers\CSGD_Direct3D.h"
#include "SGD Wrappers\CSGD_XAudio2.h"
#include "SGD Wrappers\CSGD_EventSystem.h"

//Messages
#include "CreateAbilityMessage.h"
#include "CreateAbilityTwoMessage.h"
#include "DestroyAbilityMessage.h"
#include "CleaveMessage.h"
#include "HealingCircleMessage.h"
#include "BlizzardMessage.h"
#include "DestroyDarkCurseMessage.h"
#include "DestroyGhoulMessage.h"
#include "CatapultAttackingMessage.h"

//Game States
#include "LoseGame.h"
#include "WinGame.h"
#include "MainMenu.h"
#include "Pause.h"
#include "GamePlay.h"
#include "WorldPlay.h"
#include "ArcadeGamePlay.h"

//Game Objects
#include "Player.h"

//QTE
#include "BaseBar.h"
#include "PowerBar.h"

//Abilities
#include "Fireball.h"
#include "Blizzard.h"
#include "Charge.h"
#include "Heal.h"
#include "HealingCircle.h"
#include "FlameOn.h"
#include "Cleave.h"
#include "BattleCry.h"
#include "Judgement.h"
#include "ShadowBolt.h"
#include "DrainLife.h"
#include "TransferLife.h"
#include "PlagueOn.h"
#include "DarkCharge.h"
#include "SummonVolatileGhoul.h"
#include "EmpoweringCurse.h"
#include "DarkCurse.h"
#include "Explode.h"
#include "EmpoweringCurse.h"
#include "CatapultAttack.h"


////////////////////////////////////////////////////
//Singleton
CBattlePlay* CBattlePlay::GetInstance( void )
{

	//Global Instance
	static CBattlePlay s_Instance;
	return &s_Instance;

}

CBattlePlay::CBattlePlay(void)
{

	interfaceImageID = -1;
	wasdImageID = -1;
	arrowsImageID =-1;
	tabKeyImageID  = - 1;
	enterKeyImageID = -1;

	m_nCursor = 0;
	unitOptions = false;

}



//Functions for every state
bool	CBattlePlay::Input			( )					 
{


	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();

	if (pDI->JoystickButtonPressed( 6, 0 ) || pDI->JoystickButtonPressed( 6, 1 ) )
	{
		CGame::GetInstance()->PushState( CPause::GetInstance() );
		return true;
	}
	else if (pDI->KeyPressed( DIK_ESCAPE ) == true || pDI->JoystickButtonPressed( 9, JOYSTICKID1 ) == true)
	{
		CGame::GetInstance()->PushState( CPause::GetInstance() );
		return true;
	}

	if(currentPlayer->GetType() == ENEMY && enemy->IsAI() == true)
		return true;

	if (pDI->KeyPressed(DIK_P ) == true || pDI->JoystickButtonPressed( 0, JOYSTICKID1 ) == true || pDI->JoystickButtonPressed( 0, JOYSTICKID2 ) == true )
	{
		if(currentUnit != nullptr)
		{
			unitOptions = false;
			currentUnit = nullptr;
		}

		ChangeTurn();
	}

	if(unitOptions)
	{
		if(currentPlayer->GetType() == PLAYER)
		{
			if( pDI->KeyPressed( DIK_W )  || pDI->JoystickGetLStickDirPressed( DIR_UP, JOYSTICKID1  ))
			{
				if(m_nCursor > 0) --m_nCursor;
				CGame::GetInstance()->PlayMoveCursorSFX();
			}
			if(pDI->KeyPressed( DIK_S )  || pDI->JoystickGetLStickDirPressed( DIR_DOWN, JOYSTICKID1 ))
			{
				if(m_nCursor < 2) ++m_nCursor;
				CGame::GetInstance()->PlayMoveCursorSFX();
			}
			if(pDI->KeyPressed( DIK_TAB ) || pDI->JoystickButtonPressed( 1, JOYSTICKID1 ))
			{
				if(m_nCursor == 0 && !currentUnit->HasMoved())//movement phase
					unitOptions = false;
				if(m_nCursor == 0 && currentUnit->HasMoved())//Cannot Move
					CGame::GetInstance()->PlayInvalidInputSFX();

				else if(m_nCursor == 1 && !currentUnit->HasAttacked())//attack phase
				{
					currentUnit->SetIsAttacking(true);
					unitOptions = false;
				}
				else if(m_nCursor == 1 && currentUnit->HasAttacked())//Cannot Attack
					CGame::GetInstance()->PlayInvalidInputSFX();
				else if(m_nCursor == 2)
				{
					m_nCursor = 0;
					unitOptions = false;
					currentUnit = nullptr;
					//Cancel noise
					CGame::GetInstance()->PlayCancelSFX();
				}
				ResetCursor();
			}
		}
		else if(currentPlayer->GetType() == ENEMY && !enemy->IsAI())
		{
			if( pDI->KeyPressed( DIK_UP ) || pDI->JoystickGetLStickDirPressed( DIR_UP, JOYSTICKID2 )  )
			{
				if(m_nCursor > 0) --m_nCursor;
				CGame::GetInstance()->PlayMoveCursorSFX();
			}
			if(pDI->KeyPressed( DIK_DOWN ) || pDI->JoystickGetLStickDirPressed( DIR_DOWN, JOYSTICKID2 ))
			{
				if(m_nCursor < 2) ++m_nCursor;
				CGame::GetInstance()->PlayMoveCursorSFX();
			}
			if(pDI->KeyPressed( DIK_RETURN ) || pDI->JoystickButtonPressed( 1, JOYSTICKID2 ))
			{
				if(m_nCursor == 0 && !currentUnit->HasMoved())//movement phase
					unitOptions = false;
				if(m_nCursor == 0 && currentUnit->HasMoved())//Cannot Move
					CGame::GetInstance()->PlayInvalidInputSFX();

				else if(m_nCursor == 1 && !currentUnit->HasAttacked())//attack phase
				{
					currentUnit->SetIsAttacking(true);
					unitOptions = false;
				}
				else if(m_nCursor == 1 && currentUnit->HasAttacked())//Cannot Attack
					CGame::GetInstance()->PlayInvalidInputSFX();

				else if(m_nCursor == 2)
				{
					m_nCursor = 0;
					unitOptions = false;
					currentUnit = nullptr;
					//Cancel noise
					CGame::GetInstance()->PlayCancelSFX();
				}
			}
		}
	}
	else
	{
		bool valid;
		valid = currentPlayer->Input();

		if(valid == false)
			CGame::GetInstance()->PlayInvalidInputSFX();

	}


	return true;

}
void	CBattlePlay::Update			( float fElapsedTime ) 
{

	myParticleSystem->Update(fElapsedTime);
	//anime->Update(timeStamp0, fElapsedTime);
	//anime->Update(timeStamp1, fElapsedTime);
	//anime->Update(timeStamp2, fElapsedTime);

	if (player->GetUnitsInBattle() == 0)
	{
		resetTimer += fElapsedTime;
		if(resetTimer >3.0f)
			CGame::GetInstance()->PopState( );
	}	
	else if (enemy->GetUnitsInBattle() == 0)
	{
		resetTimer += fElapsedTime;
		if(resetTimer >3.0f)
			CGame::GetInstance()->PopState( );
	}
	CSGD_MessageSystem::GetInstance()->ProcessMessages();

	if(player->GetUnitsInBattle() != 0)
	player->Update(fElapsedTime);
	if(enemy->GetUnitsInBattle() != 0)
	{
	enemy->Update(fElapsedTime);
	if(currentPlayer->GetType() == ENEMY && enemy->IsAI())
		enemy->UpdateAI(fElapsedTime);
	}





	for(unsigned int i = 0; i < text.size(); i++)
	{
		text[i]->Update(fElapsedTime);
		if(text[i]->IsDead())
		{
			delete text[i];
			text[i] = nullptr;
			text.erase(text.begin()+i);

		}

	}

	//Updating Tile Cursor;
	hoverTimer += fElapsedTime;
	if(hoverTimer >= 0.06f)
	{
		if(hoverUp == true)
		{
			--hoverOffset;
			if(hoverOffset < -5)
				hoverUp = false;
		}
		else
		{
			++hoverOffset;
			if(hoverOffset > 5)
				hoverUp = true;
		}

		hoverTimer = 0;
	}


}
void	CBattlePlay::Render			( )			
{

	grid->Render();
	grid->Highlight();
	myParticleSystem->Render();
	//anime->Render(timeStamp0, 5,5);
	//anime->Render(timeStamp1, 10,10);
	//anime->Render(timeStamp2, 15,15);


	player->Render();
	enemy->Render();
	//if(casting)
	//	currAbility->Render();


	for(unsigned int i = 0; i < text.size(); i++)
	{
		text[i]->Render();
	}

	RenderInterface();

	if(unitOptions)
		ShowUnitOptions();

	RenderTileCursor();

	for(unsigned int i = 0; i < player->GetInstance()->GetUnits().size(); i++)
		if(player->GetInstance()->GetUnits()[i]->GetAbility() != nullptr)
			player->GetInstance()->GetUnits()[i]->GetAbility()->Render();

	for(unsigned int i = 0; i < enemy->GetInstance()->GetUnits().size(); i++)
		if(enemy->GetInstance()->GetUnits()[i]->GetAbility() != nullptr)
			enemy->GetInstance()->GetUnits()[i]->GetAbility()->Render();


	if(turnCount < 4 && showInstructions)
		ShowTutorial();

}

void	CBattlePlay::Enter			( )		
{
	currentUnit = nullptr;
	vector<vector<CTile*>> temp;

	showInstructions = true;

	resetTimer = 0.0f;

	int randMap = rand() % 4;

	switch(randMap)
	{
	case 0:
		ts->Load("Resources/XMLFiles/map.xml",temp);
		break;
	case 1:
		ts->Load("Resources/XMLFiles/map1.xml",temp);
		break;
	case 2:
		ts->Load("Resources/XMLFiles/map2.xml",temp);
		break;
	case 3:
		ts->Load("Resources/XMLFiles/map3.xml",temp);
		break;
	}

	SetOffX(0);
	SetOffY(0);
	unitOptions = false;
	m_nCursor = 0;

	turnCount = 0;

	//Tile Cursor variables
	hoverTimer = 0;
	hoverOffset = 0;
	hoverUp = true;

	//	vector<vector<CTile*>> temp;
	//ts->Load("Resources/XMLFiles/map.xml",temp);

	enemy = CEnemy::GetInstance();
	player = CPlayer::GetInstance();
	enemy->LoadUnitsOnBattleGrid();
	player->LoadUnitsOnBattleGrid();

	currentPlayer = player;
	currentTarget = nullptr;


	grid->ChangeCurrent(0,0);

	string name = "snow";
	myParticleSystem = new CParticleSystem;
	myParticleSystem->LoadEffect(name,"Resources/XMLFiles/Snow.xml");
	myParticleSystem->CreateEmitter("snow");



	m_pMS = CSGD_MessageSystem::GetInstance();
	m_pMS->Initialize(&MessageProc);

	//Loading Interface Images
	interfaceImageID			= CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/BattleInterface.jpg"));
	turnImageID					= CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/Blank_HUD.png"));
	undeadIconID				= CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/Undead_Icon.png"));
	humanIconID					= CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/Human_Icon.png"));
	unitInfoID					= CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/Unit_info.png"));
	cursorImageID				= CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/Cursor_Battle.png"));
	commandImageID				= CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/Command_box.png"));

	controllerXButtonID			= CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/controller_x_button.png"));
	controllerAButtonID			= CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/controller_a_button.png"));
	controllerAbilityImageID	= CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/controller_ability_buttons.png"));
	arcadeImageID				= CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/ArcadeControlsIcons.png"));
	arrowsImageID				= CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/arrowKeys.png"));
	wasdImageID					= CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/wasd.png"));
	tabKeyImageID				= CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/tabKey.jpg"));
	enterKeyImageID				= CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/enterKey.png"));

	m_nHuman					= CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/humans.png"));
	m_nUndead					= CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/Undead_Icon.png"));
	m_nBlankBar 				= CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/blankbar.png"));

	//Loading Ability Icons
	HumanCleaveIconID			= CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/Soldier_cleave.png"));
	UndeadCleaveIconID			= CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/SkeletalWarrior_Cleave.png"));
	FireBallIconID				= CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/Wizard_Fireball.png"));
	BlizzardIconID				= CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/Wizard_Blizzard.png"));
	ShadowBoltIconID			= CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/Necromancer_ShadowBolt.png"));
	SummonGhoulIconID			= CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/Necromancer_SummonGhoul.png"));
	ChargeIconID				= CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/Cavalry_Charge.png"));
	DarkChargeIconID			= CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/DeathKnight_DarkCharge.png"));
	HealIconID					= CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/Priest_Heal.png"));
	HealingCircleIconID			= CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/Priest_HealingCircle.png"));
	DrainLifeIconID				= CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/ShadowPriest_DrainLife.png"));
	TransferLifeIconID			= CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/ShadowPriest_TransferLife.png"));
	ExplodeIconID				= CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/Ghoul_Explode.png"));
	FlameOnIconID				= CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/Catapult_FlameOn.png"));
	PlagueOnIconID				= CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/WarMachine_PlagueOn.png"));
	BattleCryIconID				= CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/General_BattleCry.png"));
	JudgementIconID				= CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/General_Judgement.png"));
	DarkCurseIconID				= CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/Lich_DarkCurse.png"));
	EmpoweringCurseIconID		= CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/Lich_EmpoweringCurse.png"));
	BasicAttackIconID			= CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/BasicAttack.png"));


	if (CMainMenu::GetInstance()->GetArcade() == true)
	{
		currentX = CArcadeGamePlay::GetInstance()->GetCurX();
	}
	else if (CMainMenu::GetInstance()->GetCampaign() == true)
	{
		currentX = CGamePlay::GetInstance()->GetCurX();
	}
	else if (CMainMenu::GetInstance()->GetQuickMatch() == true || CMainMenu::GetInstance()->GetVersus() == true )
	{
		currentX = CWorldPlay::GetInstance()->GetCurX();
	}
}

void CBattlePlay::Exit( )	
{
	if(CEnemy::GetInstance()->GetActiveCurse() != nullptr)
	{
		dynamic_cast<CBaseUnit*>(CEnemy::GetInstance()->GetActiveCurse()->GetOwner())->removeAbility();
		CEnemy::GetInstance()->ActivateDarkCurse(nullptr);
	}

	delete grid;
	grid = nullptr;
	
	for(unsigned int i = 0; i < player->GetUnits().size(); i++)
		player->GetUnits()[i]->ResetUnit();
	for(unsigned int i = 0; i < enemy->GetUnits().size(); i++)
		enemy->GetUnits()[i]->ResetUnit();

	delete myParticleSystem;
	if ( CMainMenu::GetInstance()->GetArcade() == true)
	{
		CArcadeGamePlay::GetInstance()->LoadBackFromBattle();
	}
	else if (CMainMenu::GetInstance()->GetCampaign())
	{
		CGamePlay::GetInstance()->LoadBackFromBattle();
	}
	else if (CMainMenu::GetInstance()->GetVersus() == true || CMainMenu::GetInstance()->GetQuickMatch() == true)
	{
		CWorldPlay::GetInstance()->LoadBackFromBattle();
	}

	//Unloading all Assets here
	CSGD_TextureManager::GetInstance()->UnloadTexture(HumanCleaveIconID);
	CSGD_TextureManager::GetInstance()->UnloadTexture(UndeadCleaveIconID);		
	CSGD_TextureManager::GetInstance()->UnloadTexture(FireBallIconID);			
	CSGD_TextureManager::GetInstance()->UnloadTexture(BlizzardIconID);			
	CSGD_TextureManager::GetInstance()->UnloadTexture(ShadowBoltIconID);		
	CSGD_TextureManager::GetInstance()->UnloadTexture(SummonGhoulIconID);	
	CSGD_TextureManager::GetInstance()->UnloadTexture(ChargeIconID);			
	CSGD_TextureManager::GetInstance()->UnloadTexture(DarkChargeIconID);		
	CSGD_TextureManager::GetInstance()->UnloadTexture(HealIconID);				
	CSGD_TextureManager::GetInstance()->UnloadTexture(HealingCircleIconID);		
	CSGD_TextureManager::GetInstance()->UnloadTexture(DrainLifeIconID);			
	CSGD_TextureManager::GetInstance()->UnloadTexture(TransferLifeIconID);		
	CSGD_TextureManager::GetInstance()->UnloadTexture(ExplodeIconID	);		
	CSGD_TextureManager::GetInstance()->UnloadTexture(FlameOnIconID);			
	CSGD_TextureManager::GetInstance()->UnloadTexture(PlagueOnIconID);			
	CSGD_TextureManager::GetInstance()->UnloadTexture(BattleCryIconID);			
	CSGD_TextureManager::GetInstance()->UnloadTexture(JudgementIconID);			
	CSGD_TextureManager::GetInstance()->UnloadTexture(DarkCurseIconID);			
	CSGD_TextureManager::GetInstance()->UnloadTexture(EmpoweringCurseIconID);	
	CSGD_TextureManager::GetInstance()->UnloadTexture(BasicAttackIconID);		
	CSGD_TextureManager::GetInstance()->UnloadTexture(interfaceImageID);	
	CSGD_TextureManager::GetInstance()->UnloadTexture(turnImageID);	
	CSGD_TextureManager::GetInstance()->UnloadTexture(undeadIconID);		
	CSGD_TextureManager::GetInstance()->UnloadTexture(humanIconID);			
	CSGD_TextureManager::GetInstance()->UnloadTexture(unitInfoID);			
	CSGD_TextureManager::GetInstance()->UnloadTexture(cursorImageID);			
	CSGD_TextureManager::GetInstance()->UnloadTexture(commandImageID);			
	CSGD_TextureManager::GetInstance()->UnloadTexture(arrowsImageID);	
	CSGD_TextureManager::GetInstance()->UnloadTexture(wasdImageID);			
	CSGD_TextureManager::GetInstance()->UnloadTexture(tabKeyImageID);			
	CSGD_TextureManager::GetInstance()->UnloadTexture(enterKeyImageID);	
	CSGD_TextureManager::GetInstance()->UnloadTexture(arcadeImageID);


}

void CBattlePlay::ChangeTurn(void)
{

	vector<CBaseUnit*> temp;

	temp = currentPlayer->GetUnits();
	for(unsigned int i = 0; i < temp.size(); i++)
	{
		temp[i]->Reset();
	}

	if(currentPlayer->GetType() == PlayerType::ENEMY)
	{
		currentPlayer = player;
		temp = player->GetUnits();

		//Decreasing Human Ability Timers
		for(unsigned int i = 0; i < temp.size(); i++)
			temp[i]->DecreaseAbilityCooldown();

		if(CEnemy::GetInstance()->GetActiveCurse() != nullptr)
			CSGD_EventSystem::GetInstance()->SendEventNow("DARK_CURSE",nullptr,nullptr,nullptr);
	}
	else if(currentPlayer->GetType() == PlayerType::PLAYER)
	{
		currentPlayer = enemy; 
		temp = enemy->GetUnits();

		//Decreasing Undead Ability Timers
		for(unsigned int i = 0; i < temp.size(); i++)
			temp[i]->DecreaseAbilityCooldown();
	}

	for(int i = 0; i < (int)temp.size(); i++)
		temp[i]->Reset();
	grid->ChangeCurrent((int)temp[0]->GetPosX(), (int)temp[0]->GetPosY());



}

bool CBattlePlay::CheckForAttack(void)
{

	vector<CBaseUnit*> temp;

	if(currentPlayer->GetType() == PLAYER) temp = enemy->GetUnits();
	else if(currentPlayer->GetType() == ENEMY) temp = player->GetUnits();

	for(int i = 0; i < (int)temp.size(); i++)
	{
		if(grid->GetCurrent()->GetLocX() == temp[i]->GetPosX() &&
			grid->GetCurrent()->GetLocY() == temp[i]->GetPosY())
		{

			currentUnit->Attack(temp[i]);
			return true;
		}
	}

	if(currentPlayer->GetType() == ENEMY && enemy->IsAI())
		return currentUnit->Attack(nullptr);
	else
	{
		currentUnit->SetIsAttacking(false);
		unitOptions = true;
	}


	return false;

}


void CBattlePlay::MessageProc(IMessage* pMsg)
{

	switch(pMsg->GetMessageID())
	{
	case MSG_CREATE_ABILITY:
		{
			CCreateAbilityMessage* pCreateMsg =
				dynamic_cast< CCreateAbilityMessage* >( pMsg );	

			CAbility* ability = GetInstance()->CreateAbility(pCreateMsg->GetOwner());


			CBattlePlay::GetInstance()->SetShowInstructions(false);
		}
		break;
	case MSG_CREATE_ABILITYTWO:
		{
			CCreateAbilityTwoMessage* pCreateMsg =
				dynamic_cast< CCreateAbilityTwoMessage* >( pMsg );

			CAbility* ability = GetInstance()->CreateAbilityTwo(pCreateMsg->GetOwner());
			CBattlePlay::GetInstance()->SetShowInstructions(false);

		}
		break;
	case MSG_CLEAVE:
		{
			CCleaveMessage* cleave = dynamic_cast< CCleaveMessage* >(pMsg);
			CBaseUnit* currUnit = dynamic_cast< CBaseUnit* >( cleave->GetOwner());

			int damage = (int)((currUnit->GetAbility()->GetDamageScale() / 100.0f) * 20);
			if(damage == 0)
				damage = -1;

			if(currUnit->GetUnitID() == SKELETALWARRIOR )
			{
				for(int i = 0; i < (int)GetInstance()->player->GetUnits().size(); i++)
				{
#pragma region Possible Targets
					//Possible Tile One
					if(GetInstance()->player->GetUnits()[i]->GetPosX() == currUnit->GetPosX() -32 && GetInstance()->player->GetUnits()[i]->GetPosY() == currUnit->GetPosY() -32)
					{
						GetInstance()->player->GetUnits()[i]->TakeDamage(currUnit, damage);
						continue;
					}

					//Possible Tile two
					if(GetInstance()->player->GetUnits()[i]->GetPosX() == currUnit->GetPosX()  && GetInstance()->player->GetUnits()[i]->GetPosY() == currUnit->GetPosY() -32)
					{
						GetInstance()->player->GetUnits()[i]->TakeDamage(currUnit, damage);
						continue;
					}

					//Possible Tile three
					if(GetInstance()->player->GetUnits()[i]->GetPosX() == currUnit->GetPosX() +32 && GetInstance()->player->GetUnits()[i]->GetPosY() == currUnit->GetPosY() -32)
					{
						GetInstance()->player->GetUnits()[i]->TakeDamage(currUnit, damage);
						continue;
					}

					//Possible Tile four
					if(GetInstance()->player->GetUnits()[i]->GetPosX() == currUnit->GetPosX() -32 && GetInstance()->player->GetUnits()[i]->GetPosY() == currUnit->GetPosY())
					{
						GetInstance()->player->GetUnits()[i]->TakeDamage(currUnit, damage);
						continue;
					}

					//Possible Tile five
					if(GetInstance()->player->GetUnits()[i]->GetPosX() == currUnit->GetPosX() +32 && GetInstance()->player->GetUnits()[i]->GetPosY() == currUnit->GetPosY())
					{
						GetInstance()->player->GetUnits()[i]->TakeDamage(currUnit, damage);
						continue;
					}

					//Possible Tile six
					if(GetInstance()->player->GetUnits()[i]->GetPosX() == currUnit->GetPosX() -32 && GetInstance()->player->GetUnits()[i]->GetPosY() == currUnit->GetPosY() +32)
					{
						GetInstance()->player->GetUnits()[i]->TakeDamage(currUnit, damage);
						continue;
					}

					//Possible Tile seven
					if(GetInstance()->player->GetUnits()[i]->GetPosX() == currUnit->GetPosX()  && GetInstance()->player->GetUnits()[i]->GetPosY() == currUnit->GetPosY() +32)
					{
						GetInstance()->player->GetUnits()[i]->TakeDamage(currUnit, damage);
						continue;
					}

					//Possible Tile eight
					if(GetInstance()->player->GetUnits()[i]->GetPosX() == currUnit->GetPosX() +32 && GetInstance()->player->GetUnits()[i]->GetPosY() == currUnit->GetPosY() +32)
					{
						GetInstance()->player->GetUnits()[i]->TakeDamage(currUnit, damage);
						continue;
					}  
#pragma endregion
				}
			}

			if(currUnit->GetUnitID() == FOOTSOLDIERS )
			{
				for(int i = 0; i < (int)GetInstance()->enemy->GetUnits().size(); i++)
				{
#pragma region Possible Targets
					//Possible Tile One
					if(GetInstance()->enemy->GetUnits()[i]->GetPosX() == currUnit->GetPosX() -32 && GetInstance()->enemy->GetUnits()[i]->GetPosY() == currUnit->GetPosY() -32)
					{
						GetInstance()->enemy->GetUnits()[i]->TakeDamage(currUnit, damage);
						continue;
					}

					//Possible Tile two
					if(GetInstance()->enemy->GetUnits()[i]->GetPosX() == currUnit->GetPosX()  && GetInstance()->enemy->GetUnits()[i]->GetPosY() == currUnit->GetPosY() -32)
					{
						GetInstance()->enemy->GetUnits()[i]->TakeDamage(currUnit, damage);
						continue;
					}

					//Possible Tile three
					if(GetInstance()->enemy->GetUnits()[i]->GetPosX() == currUnit->GetPosX() +32 && GetInstance()->enemy->GetUnits()[i]->GetPosY() == currUnit->GetPosY() -32)
					{
						GetInstance()->enemy->GetUnits()[i]->TakeDamage(currUnit, damage);
						continue;
					}

					//Possible Tile four
					if(GetInstance()->enemy->GetUnits()[i]->GetPosX() == currUnit->GetPosX() -32 && GetInstance()->enemy->GetUnits()[i]->GetPosY() == currUnit->GetPosY())
					{
						GetInstance()->enemy->GetUnits()[i]->TakeDamage(currUnit, damage);
						continue;
					}

					//Possible Tile five
					if(GetInstance()->enemy->GetUnits()[i]->GetPosX() == currUnit->GetPosX() +32 && GetInstance()->enemy->GetUnits()[i]->GetPosY() == currUnit->GetPosY())
					{
						GetInstance()->enemy->GetUnits()[i]->TakeDamage(currUnit, damage);
						continue;
					}

					//Possible Tile six
					if(GetInstance()->enemy->GetUnits()[i]->GetPosX() == currUnit->GetPosX() -32 && GetInstance()->enemy->GetUnits()[i]->GetPosY() == currUnit->GetPosY() +32)
					{
						GetInstance()->enemy->GetUnits()[i]->TakeDamage(currUnit, damage);
						continue;
					}

					//Possible Tile seven
					if(GetInstance()->enemy->GetUnits()[i]->GetPosX() == currUnit->GetPosX()  && GetInstance()->enemy->GetUnits()[i]->GetPosY() == currUnit->GetPosY() +32)
					{
						GetInstance()->enemy->GetUnits()[i]->TakeDamage(currUnit, damage);
						continue;
					}

					//Possible Tile eight
					if(GetInstance()->enemy->GetUnits()[i]->GetPosX() == currUnit->GetPosX() +32 && GetInstance()->enemy->GetUnits()[i]->GetPosY() == currUnit->GetPosY() +32)
					{
						GetInstance()->enemy->GetUnits()[i]->TakeDamage(currUnit, damage);
						continue;
					}  

#pragma endregion
				}
			}
			CBattlePlay::GetInstance()->SetShowInstructions(false);

		}
		break;
	case MSG_HEALINGCIRCLE:
		{
			CHealingCircleMessage* healingcircle = dynamic_cast< CHealingCircleMessage* >(pMsg);
			CBaseUnit* currUnit = dynamic_cast< CBaseUnit* >( healingcircle->GetOwner());

			int damage = (int)((currUnit->GetAbility()->GetDamageScale() / 100.0f) * 20);


			if(currUnit->GetUnitID() == PRIEST )
			{
				for(int i = 0; i < (int)GetInstance()->player->GetUnits().size(); i++)
				{
#pragma region Possible Targets
					//Possible Tile One
					if(GetInstance()->player->GetUnits()[i]->GetPosX() == currUnit->GetPosX() -32 && GetInstance()->player->GetUnits()[i]->GetPosY() == currUnit->GetPosY() -32)
					{

						string text = "+" + to_string(damage);
						CFloatingText* newText = new CFloatingText(text, (float)GetInstance()->player->GetUnits()[i]->GetPosX(), (float)GetInstance()->player->GetUnits()[i]->GetPosY(), HEAL);
						CBattlePlay::GetInstance()->PushText(newText);

						GetInstance()->player->GetUnits()[i]->SetCurrentHealth(GetInstance()->player->GetUnits()[i]->GetCurrentHealth() + damage);
						if(GetInstance()->player->GetUnits()[i]->GetCurrentHealth() > GetInstance()->player->GetUnits()[i]->GetMaxHealth())
							GetInstance()->player->GetUnits()[i]->SetCurrentHealth(GetInstance()->player->GetUnits()[i]->GetMaxHealth());
						continue;
					}

					//Possible Tile two
					if(GetInstance()->player->GetUnits()[i]->GetPosX() == currUnit->GetPosX()  && GetInstance()->player->GetUnits()[i]->GetPosY() == currUnit->GetPosY() -32)
					{
						string text = "+" + to_string(damage);
						CFloatingText* newText = new CFloatingText(text, (float)GetInstance()->player->GetUnits()[i]->GetPosX(), (float)GetInstance()->player->GetUnits()[i]->GetPosY(), HEAL);
						CBattlePlay::GetInstance()->PushText(newText);

						GetInstance()->player->GetUnits()[i]->SetCurrentHealth(GetInstance()->player->GetUnits()[i]->GetCurrentHealth() + damage);
						if(GetInstance()->player->GetUnits()[i]->GetCurrentHealth() > GetInstance()->player->GetUnits()[i]->GetMaxHealth())
							GetInstance()->player->GetUnits()[i]->SetCurrentHealth(GetInstance()->player->GetUnits()[i]->GetMaxHealth());
						continue;
					}

					//Possible Tile three
					if(GetInstance()->player->GetUnits()[i]->GetPosX() == currUnit->GetPosX() +32 && GetInstance()->player->GetUnits()[i]->GetPosY() == currUnit->GetPosY() -32)
					{
						string text = "+" + to_string(damage);
						CFloatingText* newText = new CFloatingText(text, (float)GetInstance()->player->GetUnits()[i]->GetPosX(), (float)GetInstance()->player->GetUnits()[i]->GetPosY(), HEAL);
						CBattlePlay::GetInstance()->PushText(newText);

						GetInstance()->player->GetUnits()[i]->SetCurrentHealth(GetInstance()->player->GetUnits()[i]->GetCurrentHealth() + damage);
						if(GetInstance()->player->GetUnits()[i]->GetCurrentHealth() > GetInstance()->player->GetUnits()[i]->GetMaxHealth())
							GetInstance()->player->GetUnits()[i]->SetCurrentHealth(GetInstance()->player->GetUnits()[i]->GetMaxHealth());
						continue;
					}

					//Possible Tile four
					if(GetInstance()->player->GetUnits()[i]->GetPosX() == currUnit->GetPosX() -32 && GetInstance()->player->GetUnits()[i]->GetPosY() == currUnit->GetPosY())
					{
						string text = "+" + to_string(damage);
						CFloatingText* newText = new CFloatingText(text, (float)GetInstance()->player->GetUnits()[i]->GetPosX(), (float)GetInstance()->player->GetUnits()[i]->GetPosY(), HEAL);
						CBattlePlay::GetInstance()->PushText(newText);

						GetInstance()->player->GetUnits()[i]->SetCurrentHealth(GetInstance()->player->GetUnits()[i]->GetCurrentHealth() + damage);
						if(GetInstance()->player->GetUnits()[i]->GetCurrentHealth() > GetInstance()->player->GetUnits()[i]->GetMaxHealth())
							GetInstance()->player->GetUnits()[i]->SetCurrentHealth(GetInstance()->player->GetUnits()[i]->GetMaxHealth());
						continue;
					}

					//Possible Tile five
					if(GetInstance()->player->GetUnits()[i]->GetPosX() == currUnit->GetPosX() +32 && GetInstance()->player->GetUnits()[i]->GetPosY() == currUnit->GetPosY())
					{
						string text = "+" + to_string(damage);
						CFloatingText* newText = new CFloatingText(text, (float)GetInstance()->player->GetUnits()[i]->GetPosX(), (float)GetInstance()->player->GetUnits()[i]->GetPosY(), HEAL);
						CBattlePlay::GetInstance()->PushText(newText);

						GetInstance()->player->GetUnits()[i]->SetCurrentHealth(GetInstance()->player->GetUnits()[i]->GetCurrentHealth() + damage);
						if(GetInstance()->player->GetUnits()[i]->GetCurrentHealth() > GetInstance()->player->GetUnits()[i]->GetMaxHealth())
							GetInstance()->player->GetUnits()[i]->SetCurrentHealth(GetInstance()->player->GetUnits()[i]->GetMaxHealth());
						continue;
					}

					//Possible Tile six
					if(GetInstance()->player->GetUnits()[i]->GetPosX() == currUnit->GetPosX() -32 && GetInstance()->player->GetUnits()[i]->GetPosY() == currUnit->GetPosY() +32)
					{
						string text = "+" + to_string(damage);
						CFloatingText* newText = new CFloatingText(text, (float)GetInstance()->player->GetUnits()[i]->GetPosX(), (float)GetInstance()->player->GetUnits()[i]->GetPosY(), HEAL);
						CBattlePlay::GetInstance()->PushText(newText);

						GetInstance()->player->GetUnits()[i]->SetCurrentHealth(GetInstance()->player->GetUnits()[i]->GetCurrentHealth() + damage);
						if(GetInstance()->player->GetUnits()[i]->GetCurrentHealth() > GetInstance()->player->GetUnits()[i]->GetMaxHealth())
							GetInstance()->player->GetUnits()[i]->SetCurrentHealth(GetInstance()->player->GetUnits()[i]->GetMaxHealth());
						continue;
					}

					//Possible Tile seven
					if(GetInstance()->player->GetUnits()[i]->GetPosX() == currUnit->GetPosX()  && GetInstance()->player->GetUnits()[i]->GetPosY() == currUnit->GetPosY() +32)
					{
						string text = "+" + to_string(damage);
						CFloatingText* newText = new CFloatingText(text, (float)GetInstance()->player->GetUnits()[i]->GetPosX(), (float)GetInstance()->player->GetUnits()[i]->GetPosY(), HEAL);
						CBattlePlay::GetInstance()->PushText(newText);

						GetInstance()->player->GetUnits()[i]->SetCurrentHealth(GetInstance()->player->GetUnits()[i]->GetCurrentHealth() + damage);
						if(GetInstance()->player->GetUnits()[i]->GetCurrentHealth() > GetInstance()->player->GetUnits()[i]->GetMaxHealth())
							GetInstance()->player->GetUnits()[i]->SetCurrentHealth(GetInstance()->player->GetUnits()[i]->GetMaxHealth());
						continue;
					}

					//Possible Tile eight
					if(GetInstance()->player->GetUnits()[i]->GetPosX() == currUnit->GetPosX() +32 && GetInstance()->player->GetUnits()[i]->GetPosY() == currUnit->GetPosY() +32)
					{
						string text = "+" + to_string(damage);
						CFloatingText* newText = new CFloatingText(text, (float)GetInstance()->player->GetUnits()[i]->GetPosX(), (float)GetInstance()->player->GetUnits()[i]->GetPosY(), HEAL);
						CBattlePlay::GetInstance()->PushText(newText);

						GetInstance()->player->GetUnits()[i]->SetCurrentHealth(GetInstance()->player->GetUnits()[i]->GetCurrentHealth() + damage);
						if(GetInstance()->player->GetUnits()[i]->GetCurrentHealth() > GetInstance()->player->GetUnits()[i]->GetMaxHealth())
							GetInstance()->player->GetUnits()[i]->SetCurrentHealth(GetInstance()->player->GetUnits()[i]->GetMaxHealth());
						continue;
					}  
#pragma endregion
				}
			}
			CBattlePlay::GetInstance()->SetShowInstructions(false);

		}
		break;
	case MSG_BLIZZARD:
		{
			CBlizzardMessage* blizzard= dynamic_cast< CBlizzardMessage* >(pMsg);
			CBaseUnit* currUnit = dynamic_cast< CBaseUnit* >( blizzard->GetOwner());

			int damage = (int)((currUnit->GetAbility()->GetDamageScale() / 100.0f) * 20);
			if(damage == 0)
				damage = -1;

			if(currUnit->GetUnitID() == WIZARD )
			{
				for(int i = 0; i < (int)GetInstance()->enemy->GetUnits().size(); i++)
				{
#pragma region Possible Targets
					//Possible Tile One
					if( GetInstance()->enemy->GetUnits()[i]->GetPosX() <= CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocX() + 64 &&
						GetInstance()->enemy->GetUnits()[i]->GetPosX() >= CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocX() - 64 &&
						GetInstance()->enemy->GetUnits()[i]->GetPosY() <= CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocY() + 64 &&
						GetInstance()->enemy->GetUnits()[i]->GetPosY() >= CBattlePlay::GetInstance()->GetGrid()->GetCurrent()->GetLocY() - 64 
						)
					{
						GetInstance()->enemy->GetUnits()[i]->TakeDamage(currUnit, damage);
					}
#pragma endregion
				}
			}
			CBattlePlay::GetInstance()->SetShowInstructions(false);

		}
		break;
	case MSG_DESTROY_ABILITY:
		{
			CDestroyAbilityMessage* destroyAbility = dynamic_cast< CDestroyAbilityMessage* >(pMsg);
			CBaseUnit* currUnit = dynamic_cast< CBaseUnit* >( destroyAbility->GetAbilityOwner());
			currUnit->removeAbility();
			CBattlePlay::GetInstance()->SetShowInstructions(true);

		}
		break;
	case MSG_DESTROY_DARKCURSE:
		{
			CDestroyDarkCurseMessage* destroyAbility = dynamic_cast< CDestroyDarkCurseMessage* >(pMsg);
			CBaseUnit* currUnit = dynamic_cast< CBaseUnit* >( destroyAbility->GetOwner());
			CEnemy::GetInstance()->ActivateDarkCurse(nullptr);
			currUnit->removeAbility();
			CBattlePlay::GetInstance()->SetShowInstructions(false);

		}
		break;
	case MSG_DESTROY_GHOUL:
		{
			CDestroyGhoulMessage* destroyAbility = dynamic_cast< CDestroyGhoulMessage* >(pMsg);
			CBaseUnit* currUnit = dynamic_cast< CBaseUnit* >( destroyAbility->GetOwner());
			CBattlePlay::GetInstance()->SetCurrentUnit(nullptr);
			currUnit->removeAbility();
			currUnit->SetCurrentHealth(0);
		}
		break;
	case MSG_CATAPULT_ATTACK:
		{
			CCatapultAttackingMessage* catapult = dynamic_cast< CCatapultAttackingMessage* >(pMsg);
			CBaseUnit* currUnit = dynamic_cast< CBaseUnit* >( catapult->GetOwner());
			CAbility* attack = new CCatapultAttack;
			attack->SetOwner(currUnit);
			attack->SetPosX((int)currUnit->GetPosX());
			attack->SetPosY((int)currUnit->GetPosY());
			attack->SetDamageScale(1);
			attack->SetIsCasting(true);
			attack->SetIsScaling(true);
			attack->SetPowerBarTimer(0.0f);
			dynamic_cast<CCatapultAttack*>(attack)->SetTarget(catapult->GetTarget());
			CSGD_DirectInput::GetInstance()->ClearInput();
			currUnit->SetAbility(attack);
			if(currUnit->GetUnitID() == CATAPULT)
				CPlayer::GetInstance()->SetIsCastingAbility(true);
			else if(currUnit->GetUnitID() == WARMACHINE)
				CEnemy::GetInstance()->SetIsCastingAbility(true);
		}
		break;
	}




}

CAbility* CBattlePlay::CreateAbility(CEntity* owner)
{
	if(dynamic_cast<CBaseUnit*>(owner)->GetAbility() != nullptr)
		return nullptr;

	CAbility* newAbility = nullptr;
	switch(dynamic_cast<CBaseUnit*>(owner)->GetUnitID())
	{
	case FOOTSOLDIERS:
		{
			newAbility = new CCleave();
			newAbility->SetOwner(owner);
			newAbility->SetPosX((int)owner->GetPosX());
			newAbility->SetPosY((int)owner->GetPosY());
			newAbility->SetDamageScale(1);
			newAbility->SetIsCasting(true);
			newAbility->SetIsScaling(true);
			newAbility->SetPowerBarTimer(0.0f);
			dynamic_cast<CBaseUnit*>(owner)->SetAbility(newAbility);
			CPlayer::GetInstance()->SetIsCastingAbility(true);

		}
		break;
	case PRIEST:  //Creation of all priest abilites
		{
			newAbility = new CHeal();
			newAbility->SetOwner(owner);
			newAbility->SetPosX((int)owner->GetPosX());
			newAbility->SetPosY((int)owner->GetPosY());
			newAbility->SetDamageScale(1);
			newAbility->SetIsCasting(true);
			newAbility->SetIsScaling(true);
			newAbility->SetPowerBarTimer(0.0f);
			dynamic_cast<CHeal*>(newAbility)->SetTarget(currentTarget);
			dynamic_cast<CBaseUnit*>(owner)->SetAbility(newAbility);
			CPlayer::GetInstance()->SetIsCastingAbility(true);



		}
		break;
	case WIZARD:
		{
			newAbility = new CFireball();
			newAbility->SetOwner(owner);
			newAbility->SetPosX((int)owner->GetPosX());
			newAbility->SetPosY((int)owner->GetPosY());
			newAbility->SetDamageScale(1);
			newAbility->SetIsCasting(true);
			newAbility->SetIsScaling(true);
			newAbility->SetPowerBarTimer(0.0f);
			dynamic_cast<CFireball*>(newAbility)->SetTarget(currentTarget);
			dynamic_cast<CBaseUnit*>(owner)->SetAbility(newAbility);
			CPlayer::GetInstance()->SetIsCastingAbility(true);
		}
		break;
	case GENERAL:
		{

			newAbility = new CBattleCry;
			newAbility->SetOwner(owner);
			newAbility->SetPosX((int)owner->GetPosX());
			newAbility->SetPosY((int)owner->GetPosY());
			newAbility->SetIsCasting(true);
			dynamic_cast<CBaseUnit*>(owner)->SetAbility(newAbility);
			//newAbility = nullptr;
			CSGD_EventSystem::GetInstance()->SendEventNow("GENERAL_BUFF", nullptr, nullptr, owner);
		}
		break;
	case CALVARY:
		{
			newAbility = new CCharge();
			newAbility->SetOwner(owner);
			newAbility->SetPosX((int)owner->GetPosX());
			newAbility->SetPosY((int)owner->GetPosY());
			newAbility->SetDamageScale(1);
			newAbility->SetIsCasting(true);
			newAbility->SetIsScaling(true);
			newAbility->SetPowerBarTimer(0.0f);
			dynamic_cast<CCharge*>(newAbility)->SetTarget(currentTarget);
			dynamic_cast<CCharge*>(newAbility)->SetHasMovedBefore( dynamic_cast<CBaseUnit*>(owner)->HasMoved() );
			dynamic_cast<CBaseUnit*>(owner)->SetAbility(newAbility);
			CPlayer::GetInstance()->SetIsCastingAbility(true);
		}
		break;
	case CATAPULT:
		{
			newAbility = new CFlameOn;
			newAbility->SetOwner(owner);
			newAbility->SetPosX((int)owner->GetPosX());
			newAbility->SetPosY((int)owner->GetPosY());
			newAbility->SetIsCasting(true);
			dynamic_cast<CBaseUnit*>(owner)->SetAbility(newAbility);
			newAbility = nullptr;
			dynamic_cast<CBaseUnit*>(owner)->SetCatapultBuff(true);

			std::string text = "FLAME ON!!";
			CFloatingText* newText = new CFloatingText(text, (float)owner->GetPosX(), (float)owner->GetPosY(), BUFF);
			CBattlePlay::GetInstance()->PushText(newText);
		}
		break;
	case SKELETALWARRIOR:
		{
			newAbility = new CCleave();
			newAbility->SetOwner(owner);
			newAbility->SetPosX((int)owner->GetPosX());
			newAbility->SetPosY((int)owner->GetPosY());
			newAbility->SetDamageScale(1);
			newAbility->SetIsCasting(true);
			newAbility->SetIsScaling(true);
			newAbility->SetPowerBarTimer(0.0f);
			dynamic_cast<CBaseUnit*>(owner)->SetAbility(newAbility);
			CEnemy::GetInstance()->SetIsCastingAbility(true);
		}
		break;
	case NECROMANCER:
		{
			newAbility = new CShadowBolt();
			newAbility->SetOwner(owner);
			newAbility->SetPosX((int)owner->GetPosX());
			newAbility->SetPosY((int)owner->GetPosY());
			newAbility->SetDamageScale(1);
			newAbility->SetIsCasting(true);
			newAbility->SetIsScaling(true);
			newAbility->SetPowerBarTimer(0.0f);
			dynamic_cast<CShadowBolt*>(newAbility)->SetTarget(currentTarget);
			dynamic_cast<CBaseUnit*>(owner)->SetAbility(newAbility);
			CEnemy::GetInstance()->SetIsCastingAbility(true);
		}
		break;
	case DEATHKNIGHT:
		{
			newAbility = new CDarkCharge();
			newAbility->SetOwner(owner);
			newAbility->SetPosX((int)owner->GetPosX());
			newAbility->SetPosY((int)owner->GetPosY());
			newAbility->SetDamageScale(1);
			newAbility->SetIsCasting(true);
			newAbility->SetIsScaling(true);
			newAbility->SetPowerBarTimer(0.0f);
			dynamic_cast<CDarkCharge*>(newAbility)->SetTarget(currentTarget);
			dynamic_cast<CDarkCharge*>(newAbility)->SetHasMovedBefore( dynamic_cast<CBaseUnit*>(owner)->HasMoved() );
			dynamic_cast<CBaseUnit*>(owner)->SetAbility(newAbility);
			CEnemy::GetInstance()->SetIsCastingAbility(true);
		}
		break;
	case SHADOWPRIEST:
		{
			newAbility = new CDrainLife();
			newAbility->SetOwner(owner);
			newAbility->SetPosX((int)owner->GetPosX());
			newAbility->SetPosY((int)owner->GetPosY());
			newAbility->SetDamageScale(1);
			newAbility->SetIsCasting(true);
			newAbility->SetIsScaling(true);
			newAbility->SetPowerBarTimer(0.0f);
			dynamic_cast<CDrainLife*>(newAbility)->SetTarget(currentTarget);
			dynamic_cast<CBaseUnit*>(owner)->SetAbility(newAbility);
			CEnemy::GetInstance()->SetIsCastingAbility(true);
		}
		break;
	case WARMACHINE:
		{
			newAbility = new CPlagueOn;
			newAbility->SetOwner(owner);
			newAbility->SetPosX((int)owner->GetPosX());
			newAbility->SetPosY((int)owner->GetPosY());
			dynamic_cast<CBaseUnit*>(owner)->SetAbility(newAbility);
			dynamic_cast<CBaseUnit*>(owner)->SetCatapultBuff(true);

			std::string text = "PLAGUE ON!!";
			CFloatingText* newText = new CFloatingText(text, (float)owner->GetPosX(), (float)owner->GetPosY(), BUFF);
			CBattlePlay::GetInstance()->PushText(newText);
		}
		break;
	case LICH:
		{
			newAbility = new CDarkCurse;
			newAbility->SetOwner(owner);
			newAbility->SetDamageScale(1);
			newAbility->SetIsCasting(true);
			newAbility->SetIsScaling(true);
			newAbility->SetPowerBarTimer(0.0f);
			dynamic_cast<CBaseUnit*>(owner)->SetAbility(newAbility);
			CEnemy::GetInstance()->ActivateDarkCurse(newAbility);
		}
		break;
	case VOLATILEGHOUL:
		{
			newAbility = new CExplode;
			newAbility->SetOwner(owner);
			newAbility->SetPosX((int)owner->GetPosX());
			newAbility->SetPosY((int)owner->GetPosY());
			newAbility->SetIsCasting(true);
			newAbility->SetIsScaling(true);
			newAbility->SetPowerBarTimer(0.0f);
			dynamic_cast<CBaseUnit*>(owner)->SetAbility(newAbility);
		}
		break;
	default:
		break;

	}
	return newAbility;

}

CAbility* CBattlePlay::CreateAbilityTwo(CEntity* owner)
{
	if(dynamic_cast<CBaseUnit*>(owner)->GetAbility() != nullptr)
		return nullptr;

	CAbility* newAbility = nullptr;
	switch(dynamic_cast<CBaseUnit*>(owner)->GetUnitID())
	{
	case FOOTSOLDIERS:
		break;
	case PRIEST:
		{
			newAbility = new CHealingCircle();
			newAbility->SetOwner(owner);
			newAbility->SetPosX((int)owner->GetPosX());
			newAbility->SetPosY((int)owner->GetPosY());
			newAbility->SetDamageScale(1);
			newAbility->SetIsCasting(true);
			newAbility->SetIsScaling(true);
			newAbility->SetPowerBarTimer(0.0f);
			dynamic_cast<CBaseUnit*>(owner)->SetAbility(newAbility);
			CPlayer::GetInstance()->SetIsCastingAbility(true);

		}
		break;
	case WIZARD:
		{
			newAbility = new CBlizzard();
			newAbility->SetOwner(owner);
			newAbility->SetPosX(GetGrid()->GetCurrent()->GetLocX());
			newAbility->SetPosY(GetGrid()->GetCurrent()->GetLocY());
			newAbility->SetDamageScale(1);
			newAbility->SetIsCasting(true);
			newAbility->SetIsScaling(true);
			newAbility->SetPowerBarTimer(0.0f);
			dynamic_cast<CBaseUnit*>(owner)->SetAbility(newAbility);
			CPlayer::GetInstance()->SetIsCastingAbility(true);

		}
		break;
	case GENERAL:
		{
			newAbility = new CJudgement();
			newAbility->SetOwner(owner);
			newAbility->SetPosX((int)owner->GetPosX());
			newAbility->SetPosY((int)owner->GetPosY());
			newAbility->SetDamageScale(1);
			newAbility->SetIsCasting(true);
			newAbility->SetIsScaling(true);
			newAbility->SetPowerBarTimer(0.0f);
			dynamic_cast<CBaseUnit*>(owner)->SetAbility(newAbility);
			CPlayer::GetInstance()->SetIsCastingAbility(true);

		}
		break;
	case CALVARY:
	case CATAPULT:
	case SKELETALWARRIOR:
		break;
	case NECROMANCER:
		{
			newAbility = new CSummonVolatileGhoul;
			newAbility->SetOwner(owner);
			newAbility->SetPosX((int)owner->GetPosX());
			newAbility->SetPosY((int)owner->GetPosY());
			newAbility->SetDamageScale(1);
			newAbility->SetIsCasting(true);
			newAbility->SetIsScaling(true);
			newAbility->SetPowerBarTimer(0.0f);
			bool availSpot = dynamic_cast<CSummonVolatileGhoul*>(newAbility)->SetSummonPosition();
			CEnemy::GetInstance()->SetIsCastingAbility(true);

			if(availSpot == false)
			{
				delete newAbility;
				return nullptr;
			}
			dynamic_cast<CBaseUnit*>(owner)->SetAbility(newAbility);
		}
		break;
	case DEATHKNIGHT:
		break;
	case SHADOWPRIEST:
		{
			newAbility = new CTransferLife();
			newAbility->SetOwner(owner);
			newAbility->SetPosX((int)owner->GetPosX());
			newAbility->SetPosY((int)owner->GetPosY());
			newAbility->SetDamageScale(1);
			newAbility->SetIsCasting(true);
			newAbility->SetIsScaling(true);
			newAbility->SetPowerBarTimer(0.0f);
			dynamic_cast<CTransferLife*>(newAbility)->SetTarget(currentTarget);
			dynamic_cast<CBaseUnit*>(owner)->SetAbility(newAbility);
			CEnemy::GetInstance()->SetIsCastingAbility(true);
		}
		break;
	case WARMACHINE:
		break;
	case LICH:
		{
			newAbility = new CEmpoweringCurse;
			newAbility->SetOwner(owner);
			dynamic_cast<CBaseUnit*>(owner)->SetAbility(newAbility);
			CSGD_EventSystem::GetInstance()->SendEventNow("LICH_BUFF", nullptr, nullptr, owner);
		}
		break;
	case VOLATILEGHOUL:
	default:
		break;

	}

	return newAbility;

}

void CBattlePlay::RenderInterface(void)
{

	CSGD_TextureManager * pTM = CSGD_TextureManager::GetInstance();

	CBitmapFont* pFont = CGame::GetInstance()->GetFont();

	pTM->Draw(m_nBlankBar,540,30,1.0f,1.0f);


	if(currentPlayer->GetType() == PLAYER)
	{
		//pFont->Draw(_T("PLAYER 1's TURN"), 100, 40, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 255));
		pTM->Draw(m_nHuman, 585,45,1.0f,1.0f);
		pFont->Draw( _T("Human's Turn"), 620, 55, 0.75f, D3DCOLOR_ARGB(255, 255, 255, 255) );
	}
	else if(currentPlayer->GetType() == ENEMY)
	{
		//pFont->Draw(_T("PLAYER 2's TURN"), 400, 40, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 255));
		pTM->Draw(m_nUndead, 565,30,1.0f,1.0f);
		pFont->Draw( _T("Undead's Turn"), 620, 55, 0.75f, D3DCOLOR_ARGB(255, 0, 0, 0) );
	}


	//Printing the skip button
	if(CMainMenu::GetInstance()->GetArcade() == true)
	{
		RECT button1 = { 0, 32, 32, 64 };
		pFont->Draw(_T("Press"),  560, 96, 0.9f, D3DCOLOR_ARGB(255, 255, 255, 255) );
		pTM->Draw(arcadeImageID,605,96,1.0f,1.0f,&button1);
		pFont->Draw(_T("to End Turn"),  637, 96, 0.9f, D3DCOLOR_ARGB(255, 255, 255, 255) );

	}
	else if( CSGD_DirectInput::GetInstance()->JoystickIsUnplugged() == false )
	{
		pFont->Draw(_T("Press"),  560, 96, 0.9f, D3DCOLOR_ARGB(255, 255, 255, 255) );
		pTM->Draw(controllerXButtonID,605,96,1.0f,1.0f);
		pFont->Draw(_T("to End Turn"),  637, 96, 0.9f, D3DCOLOR_ARGB(255, 255, 255, 255) );

	}
	else
	{
		pFont->Draw(_T("Press 'P' to End Turn"), 560, 96, 0.85f, D3DCOLOR_ARGB( 255, 255, 255, 255 ));

	}
	


	if(currentUnit)
	{
		//pTM->Draw(interfaceImageID, 32, 480, 0.72f, 0.5f);

		ShowAbilities(currentUnit);
	}

	if(CEnemy::GetInstance()->GetIsCastingAbility() == false || CPlayer::GetInstance()->GetIsCastingAbility() == false)
		ShowStats();
}


void CBattlePlay::ShowStats(void)
{

	CSGD_TextureManager * pTM = CSGD_TextureManager::GetInstance();
	CBitmapFont* pFont = CGame::GetInstance()->GetFont();

	CBaseUnit* temp = nullptr;
	vector<CBaseUnit*> playerUnits = player->GetUnits();
	vector<CBaseUnit*> enemyUnits = enemy->GetUnits();

	for(unsigned int i = 0; i < playerUnits.size(); i++)
	{
		if(playerUnits[i]->GetCountry() == currentX)
		{
			if(grid->GetCurrent()->GetLocX() == playerUnits[i]->GetPosX() &&
				grid->GetCurrent()->GetLocY() == playerUnits[i]->GetPosY())
				temp = playerUnits[i];
		}
	}
	if(!temp)
	{
		for(unsigned int i = 0; i < enemyUnits.size(); i++)
		{
			if(enemyUnits[i]->GetCountry() == currentX)
			{
				if(grid->GetCurrent()->GetLocX() == enemyUnits[i]->GetPosX() &&
					grid->GetCurrent()->GetLocY() == enemyUnits[i]->GetPosY())
					temp = enemyUnits[i];
			}
		}
	}

	if(temp)
	{
		if(temp == currentUnit || (CEnemy::GetInstance()->GetIsCastingAbility() == true || CPlayer::GetInstance()->GetIsCastingAbility() == true))
			return;

		int posX = (int)temp->GetPosX() + 32;
		int posY = (int)temp->GetPosY() - 50;

		TOSTRINGSTREAM info;
		TCHAR type[64];


		if(posY < 30)
		{
			posY = 30;
		}
		else if(posY + 192 > 450)
		{
			posY = 260;
		}

		pTM->Draw(unitInfoID, posX, posY);

		mbstowcs_s(nullptr,type, 64, temp->GetType(), _TRUNCATE);
		pFont->Draw(type,posX+38, posY+10, 0.6f, D3DCOLOR_XRGB(255, 255, 255));


		info << temp->GetCurrentHealth() << " / "  << temp->GetMaxHealth();
		CSGD_Direct3D::GetInstance()->DrawText(info.str().c_str(), posX + 60, posY + 55, D3DCOLOR_XRGB(255, 255, 255));

		info.clear();
		info.str(L"");
		info << temp->GetMovement();
		CSGD_Direct3D::GetInstance()->DrawText(info.str().c_str(), posX + 85, posY + 95,D3DCOLOR_XRGB(255, 255, 255));

		info.clear();
		info.str(L"");
		info << temp->GetAttackRange();
		CSGD_Direct3D::GetInstance()->DrawText(info.str().c_str(), posX + 85, posY + 140,D3DCOLOR_XRGB(255, 255, 255));

		if(!currentUnit)
			ShowAbilities(temp);

		if(temp->GetUnitID() < 6)
			pTM->Draw(humanIconID, posX + 110, posY + 128);
		else
			pTM->Draw(undeadIconID, posX + 110, posY + 128);

	}


}

void CBattlePlay::PushText(CFloatingText* newText)
{
	text.push_back(newText);
}

void CBattlePlay::ShowUnitOptions(void)
{

	CBitmapFont* pFont = CGame::GetInstance()->GetFont();
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();
	int posX = (int)currentUnit->GetPosX() + 40;
	int posY = (int)currentUnit->GetPosY() - 50;

	if(posY - 50 < 30)
		posY = 30;
	else if(posY + 192 > 450)
	{
		posY = 260;
	}
	pTM->Draw(commandImageID,posX, posY, 0.75f, 0.75f);
	pFont->Draw(_T("Commands"), posX + 42, posY+5, 0.9f, D3DCOLOR_XRGB(255, 255, 255));

	if(!currentUnit->HasMoved())CSGD_Direct3D::GetInstance()->DrawText(_T("Move"), posX+62, posY+50, D3DCOLOR_XRGB(255, 255, 255));
	else CSGD_Direct3D::GetInstance()->DrawText(_T("Move"), posX+62, posY+50, D3DCOLOR_ARGB(255, 1, 1, 1 ));

	if(currentUnit->GetUnitID() == VOLATILEGHOUL)
	{
		CSGD_Direct3D::GetInstance()->DrawText(_T("Explode"), posX+62, posY+95, D3DCOLOR_ARGB(150, 150, 150, 150 ));
	}
	else
	{
		if(!currentUnit->HasAttacked())CSGD_Direct3D::GetInstance()->DrawText(_T("Attack"), posX+62, posY+95, D3DCOLOR_XRGB(255, 255, 255 ));
		else CSGD_Direct3D::GetInstance()->DrawText(_T("Attack"), posX+62, posY+95, D3DCOLOR_ARGB(255, 1, 1, 1 ));
	}
	CSGD_Direct3D::GetInstance()->DrawText(_T("Cancel"), posX+62, posY+140, D3DCOLOR_XRGB(255, 255, 255));

	//Render Cursor Position
	pTM->Draw(cursorImageID, posX + 20, (posY + 55) + (m_nCursor*45), 0.3f, 0.3f, nullptr, 35.0f / 2, 72.0f / 2, D3DX_PI / -2);
	pTM->Draw(cursorImageID, posX + 160, (posY + 45) + (m_nCursor*45), 0.3f, 0.3f, nullptr, 35.0f / 2, 72.0f / 2, D3DX_PI / 2);


}

void CBattlePlay::ShowAbilities(CBaseUnit* unit) const
{
	CSGD_TextureManager * pTM = CSGD_TextureManager::GetInstance();
	CSGD_Direct3D * pD3D = CSGD_Direct3D::GetInstance();
	CBitmapFont* pFont = CGame::GetInstance()->GetFont();
	TOSTRINGSTREAM ability1;
	TOSTRINGSTREAM ability2;
	TOSTRINGSTREAM info;
	TCHAR type[64];

	pTM->Draw(interfaceImageID, 32, 450, 0.72f, 0.5f);

	//CURRENT UNIT WINDOW
	mbstowcs_s(nullptr,type, 64, unit->GetType(), _TRUNCATE);
	pFont->Draw(type, 632, 471, 0.7f, D3DCOLOR_XRGB(255, 255, 255));

	pFont->Draw(_T("HP: "), 610, 500, 0.7f, D3DCOLOR_ARGB(255, 255, 255, 255));
	info << unit->GetCurrentHealth() << " / "  << unit->GetMaxHealth();
	CSGD_Direct3D::GetInstance()->DrawText(info.str().c_str(), 655, 495, D3DCOLOR_XRGB(255, 255, 255));

	pFont->Draw(_T("Dmg: "), 610, 530, 0.7f, D3DCOLOR_ARGB(255, 255, 255, 255));





	info.clear();
	info.str(L"");
	info << unit->GetMinDamage() << " - " << unit->GetMaxDamage();
	CSGD_Direct3D::GetInstance()->DrawText(info.str().c_str(), 655, 530, D3DCOLOR_ARGB(255, 255, 255, 255));

	if(CEnemy::GetInstance()->GetIsCastingAbility() == true || CPlayer::GetInstance()->GetIsCastingAbility() == true)
		return;

	switch (unit->GetUnitID())
	{
	case FOOTSOLDIERS:
		{
			ability1 << unit->GetAbility1().c_str();
			ability2 << unit->GetAbility2().c_str();

			pTM->Draw(BasicAttackIconID, 75, 480);
			pFont->Draw(_T("Basic     Attack"), 75, 545, 0.6f,  D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			pTM->Draw(HumanCleaveIconID, 250, 480);
			pFont->Draw(ability1.str().c_str(), 250, 545, 0.6f,  D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		break;
	case SKELETALWARRIOR:
		{
			ability1 << unit->GetAbility1().c_str();
			ability2 << unit->GetAbility2().c_str();

			pTM->Draw(BasicAttackIconID, 75, 480);
			pFont->Draw(_T("Basic     Attack"), 75, 545, 0.6f,  D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			pTM->Draw(UndeadCleaveIconID, 250, 480);
			pFont->Draw(ability1.str().c_str(), 250, 545, 0.6f,  D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		break;
	case PRIEST:
		{
			ability1 << unit->GetAbility1().c_str();
			ability2 << unit->GetAbility2().c_str();

			pTM->Draw(BasicAttackIconID, 75, 480);
			pFont->Draw(_T("Basic     Attack"), 75, 545, 0.6f,  D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			pTM->Draw(HealIconID, 250, 480);
			pFont->Draw(ability1.str().c_str(), 250, 545, 0.6f,  D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			pTM->Draw(HealingCircleIconID, 418, 480);
			pFont->Draw(ability2.str().c_str(), 418, 545, 0.6f,  D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		break;
	case WIZARD:
		{
			ability1 << unit->GetAbility1().c_str();
			ability2 << unit->GetAbility2().c_str();

			pTM->Draw(BasicAttackIconID, 75, 480);
			pFont->Draw(_T("Basic     Attack"), 75, 545, 0.6f,  D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			pTM->Draw(FireBallIconID, 250, 480);
			pFont->Draw(ability1.str().c_str(), 250, 545, 0.6f,  D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			pTM->Draw(BlizzardIconID, 418, 480);
			pFont->Draw(ability2.str().c_str(), 418, 545, 0.6f,  D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		break;
	case GENERAL:
		{
			ability1 << unit->GetAbility1().c_str();
			ability2 << unit->GetAbility2().c_str();

			pTM->Draw(BasicAttackIconID, 75, 480);
			pFont->Draw(_T("Basic     Attack"), 75, 545, 0.6f,  D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			pTM->Draw(BattleCryIconID, 250, 480);
			pFont->Draw(ability1.str().c_str(), 250, 545, 0.6f,  D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			pTM->Draw(JudgementIconID, 418, 480);
			pFont->Draw(ability2.str().c_str(), 418, 545, 0.6f,  D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		break;
	case CALVARY:
		{
			ability1 << unit->GetAbility1().c_str();
			ability2 << unit->GetAbility2().c_str();

			pTM->Draw(BasicAttackIconID, 75, 480);
			pFont->Draw(_T("Basic     Attack"), 75, 545, 0.6f,  D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			pTM->Draw(ChargeIconID, 250, 480);
			pFont->Draw(ability1.str().c_str(), 250, 545, 0.6f,  D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		break;
	case CATAPULT:
		{
			ability1 << unit->GetAbility1().c_str();
			ability2 << unit->GetAbility2().c_str();

			pTM->Draw(BasicAttackIconID, 75, 480);
			pFont->Draw(_T("Basic     Attack"), 75, 545, 0.6f,  D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			pTM->Draw(FlameOnIconID, 250, 480);
			pFont->Draw(ability1.str().c_str(), 250, 545, 0.6f,  D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		break;
	case SHADOWPRIEST:
		{
			ability1 << unit->GetAbility1().c_str();
			ability2 << unit->GetAbility2().c_str();

			pTM->Draw(BasicAttackIconID, 75, 480);
			pFont->Draw(_T("Basic     Attack"), 75, 545, 0.6f,  D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			pTM->Draw(DrainLifeIconID, 250, 480);
			pFont->Draw(ability1.str().c_str(), 250, 545, 0.6f,  D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			pTM->Draw(TransferLifeIconID, 418, 480);
			pFont->Draw(ability2.str().c_str(), 418, 545, 0.6f,  D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		break;
	case NECROMANCER:
		{
			ability1 << unit->GetAbility1().c_str();
			ability2 << unit->GetAbility2().c_str();

			pTM->Draw(BasicAttackIconID, 75, 480);
			pFont->Draw(_T("Basic     Attack"), 75, 545, 0.6f,  D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			pTM->Draw(ShadowBoltIconID, 250, 480);
			pFont->Draw(ability1.str().c_str(), 250, 545, 0.6f,  D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			pTM->Draw(SummonGhoulIconID, 418, 480);
			pFont->Draw(ability2.str().c_str(), 418, 545, 0.6f,  D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		break;
	case LICH:
		{
			ability1 << unit->GetAbility1().c_str();
			ability2 << unit->GetAbility2().c_str();

			pTM->Draw(BasicAttackIconID, 75, 480);
			pFont->Draw(_T("Basic     Attack"), 75, 545, 0.6f,  D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			pTM->Draw(DarkCurseIconID, 250, 480);
			pFont->Draw(ability1.str().c_str(), 250, 545, 0.6f,  D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			pTM->Draw(EmpoweringCurseIconID, 418, 480);
			pFont->Draw(ability2.str().c_str(), 418, 545, 0.6f,  D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		break;
	case DEATHKNIGHT:
		{
			ability1 << unit->GetAbility1().c_str();
			ability2 << unit->GetAbility2().c_str();

			pTM->Draw(BasicAttackIconID, 75, 480);
			pFont->Draw(_T("Basic     Attack"), 75, 545, 0.6f,  D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			pTM->Draw(DarkChargeIconID, 250, 480);
			pFont->Draw(ability1.str().c_str(), 250, 545, 0.6f,  D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		break;
	case VOLATILEGHOUL:
		{
			ability1 << unit->GetAbility1().c_str();
			ability2 << unit->GetAbility2().c_str();

			pTM->Draw(ExplodeIconID, 75, 480);
			pFont->Draw(ability1.str().c_str(), 75, 545, 0.6f,  D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			return;
		}
	case WARMACHINE:
		{
			ability1 << unit->GetAbility1().c_str();
			ability2 << unit->GetAbility2().c_str();

			pTM->Draw(BasicAttackIconID, 75, 480);
			pFont->Draw(_T("Basic     Attack"), 75, 545, 0.6f,  D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			pTM->Draw(PlagueOnIconID, 250, 480);
			pFont->Draw(ability1.str().c_str(), 250, 545, 0.6f,  D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		break;
	default:
		break;
	}

	if(unit->GetAbility1_Cooldown() > 0)
	{
		RECT rCover = { 250, 480, 314, 544 };
		pD3D->DrawRect( rCover, D3DCOLOR_ARGB( 150, 0, 0, 0));

		stringstream ss;
		ss << unit->GetAbility1_Cooldown();
		TCHAR tBuffer[16];
		mbstowcs_s(nullptr,tBuffer,ss.str().c_str(),16);
		pFont->Draw(tBuffer, 265, 475, 2.5f, D3DCOLOR_ARGB( 255, 255, 0, 0 ));
	}

	if(unit->GetAbility2_Cooldown() > 0)
	{
		RECT rCover = { 418, 480, 514, 544 };
		pD3D->DrawRect( rCover, D3DCOLOR_ARGB( 150, 0, 0, 0));

		stringstream ss;
		ss << unit->GetAbility2_Cooldown();
		TCHAR tBuffer[16];
		mbstowcs_s(nullptr,tBuffer,ss.str().c_str(),16);
		pFont->Draw(tBuffer, 433, 475, 2.5f, D3DCOLOR_ARGB( 255, 255, 0, 0 ));
	}

	if(unit->GetUnitID() > 5 && CEnemy::GetInstance()->IsAI())
		return;


	if(CMainMenu::GetInstance()->GetArcade() == true)
	{
		RECT button1 = { 0, 96, 32, 128 };
		pTM->Draw(arcadeImageID,314,522,1.0f,1.0f,&button1);
		if(!unit->GetAbility2().empty())
		{
			RECT button2 = { 32, 32, 64, 64 };
			pTM->Draw(arcadeImageID,482,522,1.0f,1.0f,&button2);

		}

	}
	else if( CSGD_DirectInput::GetInstance()->JoystickIsUnplugged() == false )
	{
		RECT button1 = { 0, 0, 32, 32 };
		pTM->Draw(controllerAbilityImageID,314,522,0.75f,0.75f,&button1);
		if(!unit->GetAbility2().empty())
		{
			RECT button2 = { 32, 0, 64, 32 };
			pTM->Draw(controllerAbilityImageID,482,522,0.75f,0.75f,&button2);

		}
	}
	else
	{
		if(unit->GetUnitID() < 6)
		{
			pFont->Draw(_T("Key:\n          1"), 314, 512, 0.5f, D3DCOLOR_ARGB( 255, 255, 255, 255 ));

			if(!unit->GetAbility2().empty())
			{
				pFont->Draw(_T("Key:\n          2"), 482, 512, 0.5f, D3DCOLOR_ARGB( 255, 255, 255, 255 ));
			} 
		}
		else
		{
			pFont->Draw(_T("Key:\nNum    1"), 314, 512, 0.5f, D3DCOLOR_ARGB( 255, 255, 255, 255 ));

			if(!unit->GetAbility2().empty())
			{
				pFont->Draw(_T("Key:\nNum    2"), 482, 512, 0.5f, D3DCOLOR_ARGB( 255, 255, 255, 255 ));
			} 
		}
	}


}

void CBattlePlay::RenderTileCursor(void)
{
	CSGD_TextureManager * pTM = CSGD_TextureManager::GetInstance();

	if(currentPlayer->GetType() == PLAYER)
		pTM->Draw(cursorImageID, (int)grid->GetCurrent()->GetLocX() - offsetX + 6, (int)grid->GetCurrent()->GetLocY() - ( 44 + hoverOffset ) - offsetY, 0.3f, 0.3f, nullptr, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(50, 50, 255 ) );
	else
		pTM->Draw(cursorImageID, (int)grid->GetCurrent()->GetLocX() - offsetX + 6, (int)grid->GetCurrent()->GetLocY() - ( 44 + hoverOffset ) - offsetY, 0.3f, 0.3f, nullptr, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(255, 10, 10 ) );
}


void CBattlePlay::ShowTutorial(void)
{
	RECT box = { 200, 410, 600, 475};



	CSGD_Direct3D::GetInstance()->DrawRect(box, D3DCOLOR_ARGB(255,0,0, 0));

	TOSTRINGSTREAM output;

	if(!currentUnit)
		output << "select a unit to move/attack";
	else if(currentUnit && !currentUnit->HasAttacked() && !currentUnit->HasMoved() && unitOptions)
		output << "Select an Action to perform with this unit";
	else if(currentUnit && !currentUnit->HasAttacked() && currentUnit->HasMoved() && currentUnit->IsAttacking())
		output << "Select an enemy unit to attack";
	else if(currentUnit && !currentUnit->IsAttacking() && !currentUnit->HasMoved() && !unitOptions)
		output << "Select a tile to move to\n(The white grid shows your movement range)";

	CSGD_Direct3D::GetInstance()->DrawText(output.str().c_str(), 200, 420);


}
