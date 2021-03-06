#pragma once
#include "ability.h"
#include "BaseUnit.h"
#include "SGD Wrappers\CSGD_Direct3D.h"
#include <vector>
#include <string>

using namespace std;

class CParticleSystem;
class CBitmapFont;

class CDrainLife :
	public CAbility
{
public:
	///////////////////////////////////////////////
	// IEntity Interface:
	virtual void		Update				( float fElapsedTime )		override;
	virtual void		Render				( void )					override;

	virtual void		PowerBar			( void )					override;

	CDrainLife();
	virtual				~CDrainLife			( void )					override;

	void SetTarget(CBaseUnit* t) {target = t;}

private:
	CEntity* target;
	CParticleSystem* pDrainLife;

	int SFXDrainLifeID;

	float particleStep;

	//Enemy AI bubble
	bool  bChat;
	int	ChatBubbleID;
	int phraseToUse;
	vector<TCHAR*> phrase;
	CBitmapFont* pFont;
};

