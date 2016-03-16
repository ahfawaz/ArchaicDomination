/**************************************************
|	File:		Game.h
|	Author:		Joshua Ford(joshuacford@fullsail.edu
|	Course:		SGD 1308
|	Purpose:	CGame class initializes SGD wrappers
|				& handles the game state machine
**************************************************/

#ifndef GAME_H
#define GAME_H


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <vector>
#include <string>
using namespace std;
#include "BitmapFont.h"


/*************************************************/
// Forward Class Declarations
//	- declares that the class name exists
//	- used only for pointers & references
class CSGD_Direct3D;
class CSGD_DirectInput;
class CSGD_TextureManager;
class CSGD_XAudio2;
class CSGD_MessageSystem;
class CSGD_EventSystem;
class IMessage;

class CAnimationSystem;

class CEntity;
class CEntityManager;

class IGameState;



/*************************************************/
// CGame class
//	- SINGLETON:
//		- only ONE object is created
//		- prevents access to constructors
//		- object could be dynamically-allocated or global object
//		- class provides an access method for the object
//	- initializes & updates SGD Wrappers
//	- handles the game logic
class CGame
{
public:	
	///////////////////////////////////////////////
	// Singleton accessors:
	static CGame* GetInstance( void );
	static void DeleteInstance( void );


	///////////////////////////////////////////////
	// Setup, Run, Cleanup
	void Initialize( HWND hWnd, HINSTANCE hInstance,
		int nWidth, int nHeight,
		bool bIsWindowed );

	bool Main( void );

	void Terminate( void );

	static wstring SaveFolder();

	///////////////////////////////////////////////
	// Window Accessors:
	int GetScreenWidth ( void ) const	{	return m_nScreenWidth;	}
	int GetScreenHeight( void ) const	{	return m_nScreenHeight;	}

	///////////////////////////////////////////////
	// Font Accessor:
	CBitmapFont* GetFont( void ) const	{	return m_pFont;			}


	///////////////////////////////////////////////
	// Game State Machine stack:
	void ChangeState( IGameState* pState );

	void PushState	( IGameState* pState );
	void PopState	( void );
	void ClearStates( void );




	///////////////////////////////////////////////
	// Volume Option Control Accessor and Mutator:
	float	GetVolumeLevel			( void )	const		{ return m_nMasterMusicVolume; }
	void	SetVolumeLevel			( float TempVolume )	{ m_nMasterMusicVolume = TempVolume; }
	float	GetSFXVolumeLevel		( void )	const		{ return m_nMasterSFXVolume;	}
	void	SetSFXVolumeLevel		( float TempSFXVol )	{ m_nMasterSFXVolume = TempSFXVol; }

	int		GetScreenWidth			( void )				{ return m_nScreenWidth; }
	int		GetScreenHeight			( void )				{ return m_nScreenHeight; }

	void	SetScreenWidth			( int TempScreenWidth ) { m_nScreenWidth = TempScreenWidth; }
	void	SetScreenHeight			( int TempScreenHeight)	{ m_nScreenHeight = TempScreenHeight; }

	bool	GetIsWindowed			( void )				{ return m_bIsWindowed; }

	void	SetIsWindowed			( bool TempWindow )		{ m_bIsWindowed = TempWindow; }
	int		GetMusicID				( void )				{ return m_nbackgroundmusicID; }
	int		GetWorldMusicID			( void )				{ return m_nOverworldMusicID; }
	int		GetBattleMusicID		( void )				{ return m_nBattleMusicID; }

	void	GetIsWindowed			( bool TempWindow )		{ m_bIsWindowed = TempWindow; }

	int		GetSFXID				( void )    const		{ return m_nSFXMusicID; };

	//Play SFX
	void PlayMoveCursorSFX(void);
	void PlayCancelSFX(void);
	void PlayInvalidInputSFX(void);
	void PlayUndeadMoveSFX(void);
	void PlayHumanMoveSFX(void);

	//Accessor to Animation System
	CAnimationSystem* GetAnimeSystem(void) const {return animeSystem;}

	//Load Settings
	void LoadSettings(void);

private:	
	///////////////////////////////////////////////
	// Static pointer to the singleton object
	static CGame* s_pInstance;
	void CreateAppFiles();
	// Private constructor / destructor to prevent multiple objects
	CGame( void );						// default ctor
	~CGame( void );						// dtor
	CGame( const CGame& );				// copy ctor
	CGame& operator= ( const CGame& );	// assignment op


	///////////////////////////////////////////////
	// Helper Methods
	bool Input( void );
	void Update( void );
	void Render( void );



	///////////////////////////////////////////////
	// SGD Wrappers
	CSGD_Direct3D*			m_pD3D;
	CSGD_DirectInput*		m_pDI;
	CSGD_TextureManager*	m_pTM;
	CSGD_XAudio2*			m_pXA;

	CBitmapFont*			m_pFont;

	///////////////////////////////////////////////
	// Window Attributes
	int						m_nScreenWidth;
	int						m_nScreenHeight;
	bool					m_bIsWindowed;
	int						m_nFullScreenHeight;
	int						m_nFullScreenWidth;

	///////////////////////////////////////////////
	// Current Game State  and typedef for naming ease
	typedef		std::vector< IGameState* >			StateVector;
	StateVector										m_pCurrState;

	///////////////////////////////////////////////
	// Current time
	DWORD					m_dwCurrTime;


	///////////////////////////////////////////////
	// Game Background Music
	int						m_nbackgroundmusicID;
	float					m_nMasterMusicVolume;
	float					m_nMasterSFXVolume;
	int						m_nSFXMusicID;
	int						m_nOverworldMusicID;
	int						m_nBattleMusicID;

	////////////////////////////////////////////////
	// FPS
	int						m_nFPS;
	int						m_nFrames;
	float					m_fFPSTimer;

	//Sound Effect IDs
	int SFXCursorMoveID;
	int SFXCancelID;
	int SFXInvalidInputID;
	int SFXHumanMoveID;
	int SFXUndeadMoveID;

	//Animation system
	CAnimationSystem* animeSystem;

};

#endif //GAME_H
