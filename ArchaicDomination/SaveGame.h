////////////////////////////////////////////////////
// File:	CSaveGame.h
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
#include <string>
#include <vector>
using std::vector;

class CPlayer;
class CEnemy;
//------------------------------------------------------------
 //Player Info
struct tPlayerSaveInfo 
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



class CSaveGame :
	public IGameState
{
public:
	////////////////////////////////////////////////////
	//Singleton
	static CSaveGame* GetInstance( void );


	//Functions for every state
	virtual		bool	Input			( )						override;
	virtual		void	Update			( float fElapsedTime )	override;
	virtual		void	Render			( )						override;
	virtual		void	Enter			( )						override;
	virtual		void	Exit			( )						override;

	void SavePlayers	( const wchar_t* szFilename, 
					  vector< tPlayerSaveInfo >&	vPlayers ,int m_nCursor );
	
	bool LoadSlot		(const wchar_t* szFilename,  vector< tPlayerSaveInfo >& vPlayers );
	bool LoadSlotGame		(const wchar_t* szFilename,  vector< tPlayerSaveInfo >& vPlayers ,int m_nCursor );
	void SaveDefault	( const wchar_t* szFilename );

private:

	////////////////////////////////////////////////////
	//Singleton
	CSaveGame(void){};
	virtual ~CSaveGame(void){};

	// Our players
	vector< tPlayerSaveInfo >vPlayers; // vector of players
	
	
	
	//Cursor Image ID Set
	int			cursorImageID;


	CSaveGame( const CSaveGame& ){};				// copy constructor
	CSaveGame& operator= ( const CSaveGame& ){};	// assignment operator


	int						m_nBackgroundImageID;
	int                     m_nSlotImageID;
	int						m_nCursorID;
	int						m_nLetterCount;
	bool IsUserInput;
	int						m_nLetterCursorID;
	int						m_nLetterCursorDownID;
	/*CPlayer* m_pPlayer;
	CEnemy* m_pEnemy;*/
	//Cursor
	int m_nCursor;

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

	int m_nCap;
	std::vector<char> Namebuffer1;
	std::vector<char> Namebuffer2;
	std::vector<char> Namebuffer3;

	bool overwriteslot1;
	bool overwriteslot2;
	bool overwriteslot3;
};

