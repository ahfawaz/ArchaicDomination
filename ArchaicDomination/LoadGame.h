////////////////////////////////////////////////////
// File:	CLoadGame.h
// Purpose:	Handles all the Loading information.
//			
//
// Author:	Roberto Crespo (Cresic95@fullsail.edu)
// Course:	SGP 1310
////////////////////////////////////////////////////
#pragma once
#include "igamestate.h"
#include <iostream>
using std::cout;
using std::endl;

#include <vector>
using std::vector;
//------------------------------------------------------------
// Player Info
struct tPlayerLoadInfo 
{
	//NameofPlayer
	char	szName[ 32 ];

	//GameModes
	enum  GameModeID {CAMPAIGN,QUICKMATCH,VERSUS};
	int gamemode;
	//Unit Counters
	//Human
	int			soldierCount;
	int			priestCount;
	int			wizardCount;
	int			cavalryCount;
	int			catapultCount;
	bool		generalPresent;
	//Undead
	int			skeletonCount;
	int			spriestCount;
	int			necroCount;
	int			dkCount;
	int			wmCount;
	bool		lichPresent;

	//Resources
	int			player1wood, player1food, player1steel;
	int			player2wood, player2food, player2steel;

	////Bool for switching to battlemap for render purposes
	//bool		m_bIsInWorld;
	int turncap;
    int aienabled;

	//Turn Based System for the World Map
	int			m_nTurnOrder;
	int			m_nTurnNumber;

	//Acheivement switches
	int			m_nCountryOneOwner;
	int			m_nCountryTwoOwner;

	//Handle Interface system
	/*bool		m_nInOptions;
	bool		m_bIsMoving;
	bool		m_bBuildMode;
	bool		m_bMoveandAttackMode;
	bool		m_bBuildInterface;
	bool		m_bRecruitInterface;
	bool		m_bAMMode;
	bool		m_bSelectTerritoryToMove;*/
};
//struct tPlayerLoadInfo2 
//{
//	//NameofPlayer
//	char	szName[ 32 ];
//
//	//GameModes
//	enum  GameModeID {CAMPAIGN,QUICKMATCH,VERSUS};
//	int gamemode;
//	//Unit Counters
//	//Human
//	int			soldierCount;
//	int			priestCount;
//	int			wizardCount;
//	int			cavalryCount;
//	int			catapultCount;
//	bool		generalPresent;
//	//Undead
//	int			skeletonCount;
//	int			spriestCount;
//	int			necroCount;
//	int			dkCount;
//	int			wmCount;
//	bool		lichPresent;
//
//	//Resources
//	int			player1wood, player1food, player1steel;
//	int			player2wood, player2food, player2steel;
//
//	////Bool for switching to battlemap for render purposes
//	//bool		m_bIsInWorld;
//	int turncap;
//    int aienabled;
//
//	//Turn Based System for the World Map
//	int			m_nTurnOrder;
//	int			m_nTurnNumber;
//
//	//Acheivement switches
//	int			m_nCountryOneOwner;
//	int			m_nCountryTwoOwner;
//	
//	//Handle Interface system
//	/*bool		m_nInOptions;
//	bool		m_bIsMoving;
//	bool		m_bBuildMode;
//	bool		m_bMoveandAttackMode;
//	bool		m_bBuildInterface;
//	bool		m_bRecruitInterface;
//	bool		m_bAMMode;
//	bool		m_bSelectTerritoryToMove;*/
//};
//struct tPlayerLoadInfo3 
//{
////NameofPlayer
//	char	szName[ 32 ];
//
//	//GameModes
//	enum  GameModeID {CAMPAIGN,QUICKMATCH,VERSUS};
//	int gamemode;
//	//Unit Counters
//	//Human
//	int			soldierCount;
//	int			priestCount;
//	int			wizardCount;
//	int			cavalryCount;
//	int			catapultCount;
//	bool		generalPresent;
//	//Undead
//	int			skeletonCount;
//	int			spriestCount;
//	int			necroCount;
//	int			dkCount;
//	int			wmCount;
//	bool		lichPresent;
//
//	//Resources
//	int			player1wood, player1food, player1steel;
//	int			player2wood, player2food, player2steel;
//
//	////Bool for switching to battlemap for render purposes
//	//bool		m_bIsInWorld;
//	int turncap;
//    int aienabled;
//
//	//Turn Based System for the World Map
//	int			m_nTurnOrder;
//	int			m_nTurnNumber;
//
//	//Acheivement switches
//	int			m_nCountryOneOwner;
//	int			m_nCountryTwoOwner;
//
//	//Handle Interface system
//	/*bool		m_nInOptions;
//	bool		m_bIsMoving;
//	bool		m_bBuildMode;
//	bool		m_bMoveandAttackMode;
//	bool		m_bBuildInterface;
//	bool		m_bRecruitInterface;
//	bool		m_bAMMode;
//	bool		m_bSelectTerritoryToMove;*/
//};

class CPlayer;
class CEnemy;
class CLoadGame :
	public IGameState
{
public:
	////////////////////////////////////////////////////
	//Singleton
	static CLoadGame* GetInstance( void );






	//Functions for every state
	virtual		bool	Input			( )						override;
	virtual		void	Update			( float fElapsedTime )	override;
	virtual		void	Render			( )						override;
	virtual		void	Enter			( )						override;
	virtual		void	Exit			( )						override;

	//bool	            LoadSelectedSlot		(int i ,const char* szFilename,  vector< tPlayerInfo >& vPlayers  );
	//just to show the names if the files have anything in it
	bool LoadSlotName		(const wchar_t* szFilename,  vector< tPlayerLoadInfo >& vPlayer1 );
	//bool LoadSlotName2		(const char* szFilename,  vector< tPlayerLoadInfo2 >& vPlayer2  );
	//bool LoadSlotName3		(const char* szFilename,  vector< tPlayerLoadInfo3 >& vPlayer3  );

	//Load the actual Game
	bool LoadSlotGame		(const wchar_t* szFilename,  vector< tPlayerLoadInfo >& vPlayer1 , int m_nCursor );
	//bool LoadSlotGame2		(const char* szFilename,  vector< tPlayerLoadInfo2 >& vPlayer2  );
	//bool LoadSlotGame3		(const char* szFilename,  vector< tPlayerLoadInfo3 >& vPlayer3  );

	void SaveDefault	( const wchar_t* szFilename);

private:

	////////////////////////////////////////////////////
	//Singleton
	CLoadGame(void){};
	virtual ~CLoadGame(void){};

	// Our players
	vector< tPlayerLoadInfo >vPlayer1; 
				   
	/*vector< tPlayerLoadInfo2> vPlayer2;
				   
	vector< tPlayerLoadInfo3> vPlayer3;
	*/
	CLoadGame( const CLoadGame& ){};				// copy constructor
	CLoadGame& operator= ( const CLoadGame& ){};	// assignment operator

	int						m_nBackgroundImageID;
	int                     m_nSlotImageID;
	int						m_nCursorID;
	//Cursor
	int m_nCursor;

	//Cursor Image ID Set
	int			cursorImageID;

	//Count
	int			m_nsoldierCount;
	int			m_npriestCount;
	int			m_nwizardCount;
	int			m_ncavalryCount;
	int			m_ncatapultCount;
	int			m_nskeletonCount;
	int			m_nspriestCount;
	int			m_nnecroCount;
	int			m_ndkCount;
	int			m_nwmCount;
	int         m_nslotCount;
	//std::vector<char> Namebuffer1;
	//std::vector<char> Namebuffer2;
	//std::vector<char> Namebuffer3;
};

