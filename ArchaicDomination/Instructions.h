#pragma once
#include "IGameState.h"
class CInstructions : public IGameState
{
public:

	////////////////////////////////////////////////////
	//Singleton
	static CInstructions* GetInstance( void );

	

	//Functions for every state
	virtual		bool	Input			( )						override;
	virtual		void	Update			( float fElapsedTime )	override;
	virtual		void	Render			( )						override;
	virtual		void	Enter			( )						override;
	virtual		void	Exit			( )						override;


	//Page Renders
	void	Render0						( );
	void	Render1						( );
	void	Render2						( );
	void	Render3						( );

	
	
private:

	////////////////////////////////////////////////////
	//Singleton
	CInstructions(void);
	virtual ~CInstructions(void){}
	
	CInstructions( const CInstructions& );				// copy constructor
	CInstructions& operator= ( const CInstructions& );	// assignment operator



	int					m_nCursor;
	int					m_nBackgroundID;
	int					m_nCPos;

	
	int					m_nArcade;
	int					m_nController;

	int					m_nTurn;
	int					m_nUnits;
	int					m_nResources;
	int					m_nRecruit;
	int					m_nCommands;
	int					m_nBuildings;

};