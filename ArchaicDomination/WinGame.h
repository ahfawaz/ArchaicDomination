#pragma once
#include "igamestate.h"
class CWinGame :
	public IGameState
{
public:
	////////////////////////////////////////////////////
	//Singleton
	static CWinGame* GetInstance( void );






	//Functions for every state
	virtual		bool	Input			( )						override;
	virtual		void	Update			( float fElapsedTime )	override;
	virtual		void	Render			( )						override;
	virtual		void	Enter			( )						override;
	virtual		void	Exit			( )						override;

private:

	////////////////////////////////////////////////////
	//Singleton
	CWinGame(void){};
	virtual ~CWinGame(void){};

	
	CWinGame( const CWinGame& ){};				// copy constructor
	CWinGame& operator= ( const CWinGame& ){};	// assignment operator


	int				m_nBackgroundImageID;


	float			m_fDeltaTime;

	//Cursor
	int m_nCursor;
};

