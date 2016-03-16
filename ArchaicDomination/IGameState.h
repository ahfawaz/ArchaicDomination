////////////////////////////////////////////////////
// File:	IGameState.h
// Purpose:	Handles all menus of the game as well as
//			Game logic 
//
// Author:	Joshua Ford (joshua.c.ford@gmail.com)
// Course:	SGP 1310
////////////////////////////////////////////////////

#pragma once

class IGameState
{
public:

	//Abstract Base Class
	virtual ~IGameState(void) = 0
	{
	}


	//Functions for every state
	virtual		bool	Input			( )						= 0;
	virtual		void	Update			( float fElapsedTime )	= 0;
	virtual		void	Render			( )						= 0;
	virtual		void	Enter			( )						= 0;
	virtual		void	Exit			( )						= 0;
	
	
};

