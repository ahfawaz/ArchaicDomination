////////////////////////////////////////////////////
// File:	COptions.h
// Purpose:	Handles all the options screen information
//			
//
// Author:	Joshua Ford (joshua.c.ford@gmail.com)
// Course:	SGP 1310
////////////////////////////////////////////////////

#pragma once
#include "igamestate.h"
class COptions :
	public IGameState
{
public:
	////////////////////////////////////////////////////
	//Singleton
	static COptions* GetInstance( void );






	//Functions for every state
	virtual		bool	Input			( )						override;
	virtual		void	Update			( float fElapsedTime )	override;
	virtual		void	Render			( )						override;
	virtual		void	Enter			( )						override;
	virtual		void	Exit			( )						override;
	void				LoadOptions		( void );
	void				SaveOptions		( void )				const;




private:

	////////////////////////////////////////////////////
	//Singleton
	COptions(void){};
	virtual ~COptions(void){};


	COptions( const COptions& ){};				// copy constructor
	COptions& operator= ( const COptions& ){};	// assignment operator


	int						m_nBackgroundImageID;
	float					m_nMasterMusicVolume;
	float					m_nMasterSFXVolume;
	float					m_nTime;
	//Cursor
	int m_nCursor;


	bool					m_bIsWindowed;
	int						m_nScreenWidth;
	int						m_nScreenHeight;


	//Button IDs
	int nonSelectedButtonID;
	int SelectedButtonID;
};

