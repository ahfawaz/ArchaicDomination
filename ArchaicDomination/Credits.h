////////////////////////////////////////////////////
// File:	CCredits.h
// Purpose:	Handles all the credits information
//			
//
// Author:	Joshua Ford (joshua.c.ford@gmail.com)
// Course:	SGP 1310
////////////////////////////////////////////////////

#pragma once
#include "igamestate.h"
class CCredits :
	public IGameState
{
public:
	////////////////////////////////////////////////////
	//Singleton
	static CCredits* GetInstance( void );






	//Functions for every state
	virtual		bool	Input			( )						override;
	virtual		void	Update			( float fElapsedTime )	override;
	virtual		void	Render			( )						override;
	virtual		void	Enter			( )						override;
	virtual		void	Exit			( )						override;

private:

	////////////////////////////////////////////////////
	//Singleton
	CCredits(void){};
	virtual ~CCredits(void){};

	
	CCredits( const CCredits& ){};				// copy constructor
	CCredits& operator= ( const CCredits& ){};	// assignment operator


	int				m_nBackgroundImageID;


	//Cursor
	int m_nCursor;
};

