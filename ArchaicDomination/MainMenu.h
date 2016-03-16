////////////////////////////////////////////////////
// File:	CMainMenu.h
// Purpose:	Handles all input for the main menu
//			screen
//
// Author:	Joshua Ford (joshua.c.ford@gmail.com)
// Course:	SGP 1310
////////////////////////////////////////////////////


#pragma once
#include "igamestate.h"
class CMainMenu :
	public IGameState
{
public: 
	////////////////////////////////////////////////////
	//Singleton
	static CMainMenu* GetInstance( void );






	//Functions for every state
	virtual		bool	Input			( )						override;
	virtual		void	Update			( float fElapsedTime )	override;
	virtual		void	Render			( )						override;
	virtual		void	Enter			( )						override;
	virtual		void	Exit			( )						override;
	bool				GetCampaign		( void )				{ return m_bCampaign; }
	bool				GetQuickMatch	( void )				{ return m_bQuickMatch; }
	bool				GetInGame		( void )				{ return m_bInGame; }
	bool				GetArcade		( void )				{ return m_bArcadeMode; }
	bool				GetVersus		( void )				{ return m_bVersus;}
	void				SetInGame		(bool ig)				{ m_bInGame = ig;}
	void				SetArcade		(bool ar)				{m_bArcadeMode = ar;}
	void				SetCampaign		(bool ca)				{m_bCampaign = ca;}
private:

	////////////////////////////////////////////////////
	//Singleton
	CMainMenu(void){};
	virtual ~CMainMenu(void){};


	CMainMenu( const CMainMenu& ){};				// copy constructor
	CMainMenu& operator= ( const CMainMenu& ){};	// assignment operator



	//Cursor
	int m_nCursor;


	//BackgroundID
	int				m_nBackgroundID;

	//Button IDs
	int nonSelectedButtonID;
	int SelectedButtonID;

	//Which play mode are we loading in
	bool			m_bCampaign;
	bool			m_bQuickMatch;
	bool			m_bArcadeMode;
	bool			m_bInGame;
	bool			m_bVersus;

	//ID for Cursor
	int					cursorImageID;


};

