#pragma once
class CPlayer;
class CEnemy;
class CWorldGrid;
class CBasePlayer;
class CParticleSystem;

#include "IGameState.h"
#include "Animation System\AnimationSystem.h"
#include "Animation System\AnimTimeStamp.h"
class CArcadeGamePlay :
	public IGameState
{
public:


	////////////////////////////////////////////////////
	//Singleton
	static CArcadeGamePlay* GetInstance( void );


	bool				GetIsInWorld	( void )						{ return m_bIsInWorld; }
	void				SetIsInWorld	( bool TempWorld)				{ m_bIsInWorld = TempWorld; }



	//Functions for every state
	virtual		bool	Input			( void )						override;
	virtual		void	Update			( float fElapsedTime )			override;
	virtual		void	Render			( void )						override;
	virtual		void	Enter			( void )						override;
	virtual		void	Exit			( void )						override;

	bool				Move			( int temp );


	void				Build			( CBasePlayer* TempPlayer );
	void				CheckIsBuilt	( int count );
	void				BuildPhase		( void );
	void				PhaseDisplay	( void );
	void				InterfaceRender ( void );
	void				BuildRender		( void );
	void				RecruitRender	( void );
	void				RenderBuildings ( void );
	void				RenderUnits		( void );
	void				AllocateResources(void );
	bool				HandleEnterKey	( void );
	bool				WinCheck		( void );
	CWorldGrid*         GetWorld		( void )						{ return m_cWorld; }
	bool				SellBuilding	( void );
	void				LoadBackFromBattle ( void );
	bool				MoveUnitFromTerritory ( void );
	int					GetCurX			( void )						{ return curX; }
	int					GetCurY			( void )						{ return curY; }

	


	int					GetTurnOrder ( void )		{return m_nTurnOrder; }

	
	int					GetCountryOneOwner	( void )			{ return m_nCountryOneOwner; }
	int					GetCountryTwoOwner	( void )			{ return m_nCountryTwoOwner; }

	void				RenderOptionsForMove( void );
	bool				HandleInputForMoveAndAttack( void );
	void				CheckMovement ( void );
	void				MoveUnitsBetweenTerritories ( void );

	int					GetMovingFromX	( void )				{ return m_nMovingFromX; }
	int					GetMovingFromY	( void )				{ return m_nMovingFromY; }
	
	

private:

	////////////////////////////////////////////////////
	//Singleton
	CArcadeGamePlay(void){};
	virtual ~CArcadeGamePlay(void){};

	
	CArcadeGamePlay( const CArcadeGamePlay& ){};				// copy constructor
	CArcadeGamePlay& operator= ( const CArcadeGamePlay& ){};	// assignment operator

	//Turn based system for the world map
	int					m_nTurnOrder;
	CPlayer*			m_cPlayerOne;
	CEnemy*				m_cPlayerTwo;
	CWorldGrid*			m_cWorld;
	CWorldGrid*			m_cCurrentTile;
	int					m_nTurnNumber;
	int					m_nMovingFromX;
	int					m_nMovingFromY;



	//ID for background
	int					m_nBackgroundID;

	//IDs for buildings
	int					m_nFarm;
	int					m_nMine;
	int					m_nLumber;
	int					m_nBarracks;
	int					m_nFactory;


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

	//Bool for switching to battlemap for render purposes

	bool				m_bIsInWorld;


	//Width and height inside of the countries
	int					m_nHeight;
	int					m_nWidth;

	//cursor for moving up down left and right in interface
	int					m_nCursor;

	int					m_nBuildCursor;

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


	//Win Bools for determining who won
	bool				m_bPlayerOneWins;
	bool				m_bPlayerTwoWins;


	//Different Game Options
	int					m_nTurnCap;
	bool				m_bAIEnabled;



	//World Grid access
	int		curX, curY;



	//Unit Display Counters
	int			soldierCount;
	int			priestCount;
	int			wizardCount;
	int			cavalryCount;
	int			catapultCount;
	bool		generalPresent;

	int			skeletonCount;
	int			spriestCount;
	int			necroCount;
	int			dkCount;
	int			wmCount;
	bool		lichPresent;


	//Resource allocation
	int			wood, food, steel;
	float		m_fTime;

	std::string cheatstring;


	//Particle Emitters & stuff
	CParticleSystem* m_psConstruction;
	CParticleSystem* m_psRecruit;
	float			 m_fCTimer;
	float			 m_fRTimer;



	//Acheivement switches
	


	int			m_nCountryOneOwner;
	int			m_nCountryTwoOwner;




	//Handling what units to move and attack
	int			m_MovesoldierCount;
	int			m_MovepriestCount;
	int			m_MovewizardCount;
	int			m_MovecavalryCount;
	int			m_MovecatapultCount;
	int 		m_MovegeneralCount;
				
	int			m_MoveskeletonCount;
	int			m_MovespriestCount;
	int			m_MovenecroCount;
	int			m_MovedkCount;
	int			m_MovewmCount;
	int			m_MovelichCount;			
};


