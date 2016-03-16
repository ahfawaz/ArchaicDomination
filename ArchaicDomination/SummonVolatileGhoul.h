#pragma once
#include "ability.h"
#include "SGD Wrappers\CSGD_Direct3D.h"
#include <vector>
#include <string>

using namespace std;

class CParticleSystem;
class CBitmapFont;

class CSummonVolatileGhoul :
	public CAbility
{
public:
	///////////////////////////////////////////////
	// IEntity Interface:
	virtual void		Update				( float fElapsedTime )		override;
	virtual void		Render				( void )					override;

	virtual void		PowerBar			( void )					override;

	CSummonVolatileGhoul();
	virtual				~CSummonVolatileGhoul			( void )					override;

	bool SetSummonPosition(void);

private:
	CParticleSystem* pSummon;

	int SFXSummonID;
	int SFXRisenGhoulID;

	float risingTime;

	//Enemy AI bubble
	bool  bChat;
	int	ChatBubbleID;
	int phraseToUse;
	vector<TCHAR*> phrase;
	CBitmapFont* pFont;
};

