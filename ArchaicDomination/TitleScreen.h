////////////////////////////////////////////////////
// File:	CTitleScreen.h
// Purpose:	Handles all the title screen information
//			
//
// Author:	Joshua Ford (joshua.c.ford@gmail.com)
// Course:	SGP 1310
////////////////////////////////////////////////////

#pragma once
#include "igamestate.h"
class CTitleScreen :
	public IGameState
{
public:
	////////////////////////////////////////////////////
	//Singleton
	static CTitleScreen* GetInstance( void );






	//Functions for every state
	virtual		bool	Input			( )						override;
	virtual		void	Update			( float fElapsedTime )	override;
	virtual		void	Render			( )						override;
	virtual		void	Enter			( )						override;
	virtual		void	Exit			( )						override;

private:

	////////////////////////////////////////////////////
	//Singleton
	CTitleScreen(void){};
	virtual ~CTitleScreen(void){};

	
	CTitleScreen( const CTitleScreen& ){};				// copy constructor
	CTitleScreen& operator= ( const CTitleScreen& ){};	// assignment operator



	//Cursor
	int m_nCursor;


	int				m_nBackgroundID;
};

