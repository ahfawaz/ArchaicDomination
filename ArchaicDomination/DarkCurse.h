#pragma once
#include "ability.h"
#include "SGD Wrappers\CSGD_Direct3D.h"
#include <vector>
#include <string>

using namespace std;

class CParticleSystem;
class CBitmapFont;

class CDarkCurse : public CAbility
{
public:
	///////////////////////////////////////////////
	// IEntity Interface:
	virtual void		Update				( float fElapsedTime )		override;
	virtual void		Render				( void )					override;
	
	virtual void		PowerBar			( void )					override;

	CDarkCurse();
	virtual				~CDarkCurse			( void )					override;

	void DealDamage();

private:
	int tickNum;
	float activeTick;
	std::vector<CParticleSystem*> pDarkCurse;

	int SFXapplyCurseID;
	int SFXCurseTickID;

	//Enemy AI bubble
	bool  bChat;
	int	ChatBubbleID;
	int phraseToUse;
	vector<TCHAR*> phrase;
	CBitmapFont* pFont;
};

