#pragma once
#include "igamestate.h"
class CLoseGame :
	public IGameState
{
public:
	////////////////////////////////////////////////////
	//Singleton
	static CLoseGame* GetInstance( void );






	//Functions for every state
	virtual		bool	Input			( )						override;
	virtual		void	Update			( float fElapsedTime )	override;
	virtual		void	Render			( )						override;
	virtual		void	Enter			( )						override;
	virtual		void	Exit			( )						override;

private:

	////////////////////////////////////////////////////
	//Singleton
	CLoseGame(void){};
	virtual ~CLoseGame(void){};

	
	CLoseGame( const CLoseGame& ){};				// copy constructor
	CLoseGame& operator= ( const CLoseGame& ){};	// assignment operator


	int				m_nBackgroundImageID;

	float			m_fDeltaTime;
	//Cursor
	int m_nCursor;
};

