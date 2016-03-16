#pragma once
#include "igamestate.h"
class CAcheivements :
	public IGameState
{
public:
	////////////////////////////////////////////////////
	//Singleton
	static CAcheivements* GetInstance( void );






	//Functions for every state
	virtual		bool	Input			( )						override;
	virtual		void	Update			( float fElapsedTime )	override;
	virtual		void	Render			( )						override;
	virtual		void	Enter			( )						override;
	virtual		void	Exit			( )						override;

private:

	////////////////////////////////////////////////////
	//Singleton
	CAcheivements(void){};
	virtual ~CAcheivements(void){};

	
	CAcheivements( const CAcheivements& ){};				// copy constructor
	CAcheivements& operator= ( const CAcheivements& ){};	// assignment operator


	int				m_nBackgroundImageID;


	//Cursor
	int m_nCursor;
};

