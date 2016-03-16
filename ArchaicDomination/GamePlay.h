////////////////////////////////////////////////////
// File:	CGamePlay.h
// Purpose:	Handles all game logic for the WorldMap
//			
//
// Author:	Joshua Ford (joshua.c.ford@gmail.com)
// Course:	SGP 1310
////////////////////////////////////////////////////


#pragma once

class CPlayer;
class CEnemy;
class CWorldGrid;
class CBasePlayer;
class CParticleSystem;
class CWorldTile;

#include "IGameState.h"
#include "Animation System\AnimationSystem.h"
#include "Animation System\AnimTimeStamp.h"

class CGamePlay : public IGameState
{
public:


	////////////////////////////////////////////////////
	//Singleton
	static CGamePlay* GetInstance							( void );





	//Functions for every state
	virtual		bool	Input								( void )						override;
	virtual		void	Update								( float fElapsedTime )			override;
	virtual		void	Render								( void )						override;
	virtual		void	Enter								( void )						override;
	virtual		void	Exit								( void )						override;

	//Handle Movement between the world nodes
	bool				Move								( CWorldTile * Tiletomove, int temp );
	void				CheckMovement						( void );


	//Input for moving
	bool				HandleInputForMoveAndAttack			( void );
	bool				MoveUnitsBetweenTerritories			( void );
	bool				HandleMovingOnTerritoryMenu			( void );
	bool				HandleMovingOnBuildInterface		( void );	
	bool				HandleMovingOnRecruitInterface		( void );
	bool				HandleMovingOnTerritories			( void );

	//Input for Enter
	bool				HandleEnterForMoveAndAttack			( void );
	bool				HandleEnterForClickingOnTerritories ( void );
	bool				HandleEnterOnTerritoryMenu			( void );
	bool				HandleEnterOnBuildInterface			( void );	
	bool				HandleEnterOnRecruitInterface		( void );

	//Renders
	void				InterfaceRender						( void );
	void				BuildRender							( void );
	void				RecruitRender						( void );
	void				RenderBuildings						( void );
	void				RenderUnits							( void );
	void				RenderResources						( void );
	void				PlayerResources						( void );
	void				TurnRender							( void );
	void				RenderOptionsForMove				( void );
	void				ShowTerritoryOptions				( void );
	void				RenderOwnership						( void );


	//Handle AI functionality
	void				UpdateAI							( void );

	//Tutorial mode
	void				ShowTutorial						( void );

	//Building and Resources
	void				AllocateResources					( void );
	bool				SellBuilding						( void );
	void				Build								( CBasePlayer* TempPlayer );
	void				CheckIsBuilt						( int count );

	//Handle the render and input for the sellback function
	void				RenderSellOrKeep					( void );
	bool				HandleAllInputForSellOrKeep			( void );
	void				SellOrKeepBuilding					( int temp );
	//Winchecks
	bool				WinCheck							( void );

	//CheatCheck
	bool				CheatCheck							( void );

	//Loading back into world play
	void				LoadBackFromBattle					( void );

	
	//Cursor
	void				RenderTileCursor					(void);



	//Mutators
	void				SetIsInWorld						( bool TempWorld)		{ m_bIsInWorld = TempWorld; }
	void				SetTurnOrder						( int TempOrder )		{ m_nTurnOrder = TempOrder; }
	void				SetTurnNumber						( int TempNumber )		{ m_nTurnNumber = TempNumber; }
	void				SetTurnCap							( int TempCap )			{ m_nTurnCap = TempCap; }
	void				SetAiEnabled						( bool tempEnabled )	{ m_bAIEnabled = tempEnabled; }
	void				SetCountryTwoOwner					( int TempOwner )		{ m_nCountryTwoOwner = TempOwner; }
	void				SetCountryOneOwner					( int TempOwner )		{ m_nCountryOneOwner = TempOwner; }
			
		

	//Accessors
	bool				GetIsInWorld						( void )	const		{ return m_bIsInWorld; }
	int					GetCurX								( void )	const		{ return curX; }
	int					GetCurY								( void )	const		{ return curY; }
	CWorldGrid*         GetWorld							( void )	const		{ return m_cWorld; }
	int					GetTurnOrder						( void )	const		{ return m_nTurnOrder; }
	int					GetCountryOneOwner					( void )	const		{ return m_nCountryOneOwner; }
	int					GetCountryTwoOwner					( void )	const		{ return m_nCountryTwoOwner; }
	int					GetMovingFromX						( void )	const		{ return m_nMovingFromX; }
	int					GetMovingFromY						( void )	const		{ return m_nMovingFromY; }
	bool				GetAiEnabled						( void )	const		{ return m_bAIEnabled;}
	int					GetTurnNumber						( void )	const		{ return m_nTurnNumber; }
	int					GetTurnCap							( void )	const		{ return m_nTurnCap; }

private:

	////////////////////////////////////////////////////
	//Singleton
	CGamePlay(void){};
	virtual ~CGamePlay(void){};

	
	CGamePlay( const CGamePlay& ){};				// copy constructor
	CGamePlay& operator= ( const CGamePlay& ){};	// assignment operator

	//Turn based system for the world map
	int					m_nTurnOrder;
	CPlayer*			m_cPlayerOne;
	CEnemy*				m_cPlayerTwo;
	CWorldGrid*			m_cWorld;	
	int					m_nTurnNumber;
	int					m_nMovingFromX;
	int					m_nMovingFromY;

	//CurrentPlayer for Turns
	CBasePlayer*		m_ncurrentPlayer;

	//Hold Time for the cursor updating
	float				m_fHoverTimer;
	int					m_nHoverOffset;
	bool				m_bHoverUp;
	


	//ID for background
	int					m_nBackgroundID;

	//ID for Cursor
	int					cursorImageID;
	int					commandImageID;
	

	//IDs for buildings
	int					m_nFarm;
	int					m_nMine;
	int					m_nLumber;
	int					m_nBarracks;
	int					m_nFactory;

	//IDs for Interface Assets
	int					m_nHuman;
	int					m_nUndead;
	int					m_nTurnBox;
	int					m_nResourceBox;
	int					m_nFoodIcon;
	int					m_nWoodIcon;
	int					m_nSteelIcon;
	int					m_nBlankBar;
	int					m_nBlankBox;
	int					m_nBlankPanel;
	int					m_nMoveBox;



	//IDs for units
	int					m_nFoot;
	int					m_nPriest;
	int					m_nWizard;
	int					m_nSkeleton;
	int					m_nSPriest;
	int					m_nNecro;
	int					m_nCavalry;
	int					m_nCatapult;
	int					m_nDK;
	int					m_nWM;

	//Sound IDs
	int					m_nResources;
	int					m_nPlaceBuild;
	int					m_nConquer;
	int					m_nTurnSound;


	//Bool for switching to battlemap for render purposes

	bool				m_bIsInWorld;


	//Width and height inside of the countries
	int					m_nHeight;
	int					m_nWidth;

	//cursor for moving up down left and right in interface
	int					m_nCursor;
	int					m_nTerritoryOptionsCursor;
	int					m_nBuildCursor;
	int					m_nMoveOrSellOptions;
	int					m_nMoveCursor;

	//Variable for greying out the buttons that are not in use
	int					m_nGreyedOut;


	//Handle Interface system
	bool				m_nInOptions;
	bool				m_bIsMoving;
	bool				m_bBuildMode;
	bool				m_bMoveandAttackMode;
	bool				m_bBuildInterface;
	bool				m_bRecruitInterface;
	bool				m_bAMMode;
	bool				m_bSelectTerritoryToMove;
	bool				m_bTerritoryMenuUp;
	bool				m_bChooseToSellBack;

	//Win ints for determining who won
	int					m_bPlayerOneWins;
	int					m_bPlayerTwoWins;


	//Different Game Options
	int					m_nTurnCap;
	bool				m_bAIEnabled;



	//World Grid access
	int					curX, curY;



	//Unit Display Counters
	int					soldierCount;
	int					priestCount;
	int					wizardCount;
	int					cavalryCount;
	int					catapultCount;
	bool				generalPresent;

	int					skeletonCount;
	int					spriestCount;
	int					necroCount;
	int					dkCount;
	int					wmCount;
	bool				lichPresent;


	//Resource allocation
	int					wood, food, steel;
	float				m_fTime;
		
	std::string			cheatstring;


	//Particle Emitters & stuff
	CParticleSystem* m_psConstruction;
	CParticleSystem* m_psRecruit;
	CParticleSystem* m_psTurn;
	float			 m_fCTimer;
	float			 m_fRTimer;
	float			 m_fTurnTimer; 



	//Acheivement switches
	int					m_nCountryOneOwner;
	int					m_nCountryTwoOwner;

	int m_nplayercountryCount;


	//Handling what units to move and attack
	int					m_MovesoldierCount;
	int					m_MovepriestCount;
	int					m_MovewizardCount;
	int					m_MovecavalryCount;
	int					m_MovecatapultCount;
	int 				m_MovegeneralCount;
						
	int					m_MoveskeletonCount;
	int					m_MovespriestCount;
	int					m_MovenecroCount;
	int					m_MovedkCount;
	int					m_MovewmCount;
	int					m_MovelichCount;			
};

