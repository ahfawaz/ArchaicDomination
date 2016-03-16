#pragma once
#include "igamestate.h"
#include "TileSystem\BattleGrid.h"
#include "BaseUnit.h"
#include "Enemy.h"
#include "BasePlayer.h"
#include "FloatingText.h"



class CParticleSystem;
class CTileSystem;
class CPlayer;
class CAnimTimeStamp;
class CBasePlayer;
class IMessage;
class CSGD_MessageSystem;

class CBattlePlay :
	public IGameState
{
public:


	////////////////////////////////////////////////////
	//Singleton
	static CBattlePlay* GetInstance( void );

	void SetGrid(CBattleGrid* g) {grid = g;}
	CBattleGrid* GetGrid(void) {return grid;}

	void SetCurrentUnit(CBaseUnit* unit) {currentUnit = unit;}
	CBaseUnit* GetCurrentUnit(void) {return currentUnit;}

	void SetCurrentTarget(CBaseUnit* unit) {currentTarget = unit;}

	bool CheckForAttack(void);

	void ShowTutorial(void);

	void ShowStats(void);

	void ChangeTurn(void);
	//Functions for every state
	virtual		bool	Input			( )						override;
	virtual		void	Update			( float fElapsedTime )	override;
	virtual		void	Render			( )						override;
	virtual		void	Enter			( )						override;
	virtual		void	Exit			( )						override;

	CAbility* CreateAbility(CEntity* owner);
	CAbility* CreateAbilityTwo(CEntity* owner);

	int GetOffX()			{return offsetX;}
	int GetOffY()			{return offsetY;}

	void SetOffX(int x)		{offsetX = x;}
	void SetOffY(int y)		{offsetY = y;}

	void PushText(CFloatingText* newText);

	//Get Image for Arcade Input
	int GetArcadeImage(void) {return arcadeImageID;}
	int GetControllerAbilityImageID(void) {return controllerAbilityImageID;}
	int GetControllerAButtonID(void) {return controllerAButtonID;}

	//Render Interface functions
	void RenderInterface(void);
	void ShowUnitOptions(void);
	void ShowAbilities(CBaseUnit* unit) const;
	void RenderTileCursor(void);


	void SetShowUnitOptions(bool b) {unitOptions = b;}
	bool GetShowUnitOptions(void) {return unitOptions;}

	void SetShowInstructions(bool b) {showInstructions = b;}

private:

	////////////////////////////////////////////////////
	//Singleton
	CBattlePlay(void);
	virtual ~CBattlePlay(void){}

	CBattlePlay( const CBattlePlay& );				// copy constructor
	CBattlePlay& operator= ( const CBattlePlay& );	// assignment operator

	static void MessageProc(IMessage* pMsg);

	void ResetCursor(void) {m_nCursor = 0;}


	CTileSystem* ts;
	CBattleGrid* grid;
	CPlayer* player;
	CEnemy* enemy;

	CBasePlayer* currentPlayer;

	std::vector<CFloatingText*> text;

	CParticleSystem*   myParticleSystem;
	CBaseUnit* currentUnit;
	CBaseUnit* currentTarget;

	CSGD_MessageSystem* m_pMS;

	// Interface IDs
	int interfaceImageID;
	int turnImageID;
	int undeadIconID;
	int humanIconID;
	int unitInfoID;
	int commandImageID;
	int cursorImageID;
	int	m_nHuman;
	int m_nUndead;
	int m_nBlankBar;

	int controllerXButtonID;
	int controllerAButtonID;
	int controllerAbilityImageID;
	int arcadeImageID;
	int wasdImageID;
	int arrowsImageID;
	int tabKeyImageID;
	int enterKeyImageID;

	//Spell Icon ID's
	int BasicAttackIconID;
	int HumanCleaveIconID;
	int UndeadCleaveIconID;
	int FireBallIconID;
	int BlizzardIconID;
	int ShadowBoltIconID;
	int SummonGhoulIconID;
	int ChargeIconID;
	int DarkChargeIconID;
	int HealIconID;
	int HealingCircleIconID;
	int DrainLifeIconID;
	int TransferLifeIconID;
	int ExplodeIconID;
	int FlameOnIconID;
	int PlagueOnIconID;
	int BattleCryIconID;
	int JudgementIconID;
	int DarkCurseIconID;
	int EmpoweringCurseIconID;

	int							offsetX;
	int							offsetY;

	int turnCount;

	bool unitOptions;
	int m_nCursor;

	int			currentX;

	//Tile Cursor variables
	float hoverTimer;
	int hoverOffset;
	bool hoverUp;

	bool showInstructions;

	float resetTimer;

};

