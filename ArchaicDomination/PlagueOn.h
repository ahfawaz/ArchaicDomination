#pragma once
#include "ability.h"
#include "SGD Wrappers\CSGD_Direct3D.h"
#include "BaseUnit.h"
#include <vector>
#include <string>

using namespace std;

class CParticleSystem;
class CBitmapFont;

class CPlagueOn :
	public CAbility
{
public:
	///////////////////////////////////////////////
	// IEntity Interface:
	virtual void		Update				( float fElapsedTime )		override;
	virtual void		Render				( void )					override;
	
	virtual void		PowerBar			( void )					override;

	CPlagueOn();
	virtual				~CPlagueOn			( void )					override;

private:
	int SFXPlagueOnID;

	CParticleSystem* pPlagueOn;
	float particleTimer;

	//Enemy AI bubble
	bool  bChat;
	int	ChatBubbleID;
	int phraseToUse;
	vector<TCHAR*> phrase;
	CBitmapFont* pFont;

};

