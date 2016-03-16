#pragma once
#include "igamestate.h"
class CPause :
	public IGameState
{
public:
	

	////////////////////////////////////////////////////
	//Singleton
	static CPause* GetInstance( void );

	

	//Functions for every state
	virtual		bool	Input			( )						override;
	virtual		void	Update			( float fElapsedTime )	override;
	virtual		void	Render			( )						override;
	virtual		void	Enter			( )						override;
	virtual		void	Exit			( )						override;
	
	
private:

	////////////////////////////////////////////////////
	//Singleton
	CPause(void);
	virtual ~CPause(void){}
	
	CPause( const CPause& );				// copy constructor
	CPause& operator= ( const CPause& );	// assignment operator



	int					m_nCursor;
	int					m_nBackgroundID;
	float 				MusicVolume;
	
		//Button IDs
	int nonSelectedButtonID;
	int SelectedButtonID;
};

