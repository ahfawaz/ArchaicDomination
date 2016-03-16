#pragma once
#include "ability.h"
#include "SGD Wrappers\CSGD_Direct3D.h"
#include <vector>
#include <string>

using namespace std;

class CParticleSystem;
class CBitmapFont;

class CCleave :
	public CAbility
{
public:
	///////////////////////////////////////////////
	// IEntity Interface:
	virtual void		Update				( float fElapsedTime )		override;
	virtual void		Render				( void )					override;
	
	virtual void		PowerBar			( void )					override;

	CCleave();
	virtual				~CCleave			( void )					override;

private:

	int m_nCleaveSFXID;
	CParticleSystem* pCleave;
	float particleTimer;


	//Enemy AI bubble
	bool  bChat;
	int	ChatBubbleID;
	int phraseToUse;
	vector<TCHAR*> phrase;
	CBitmapFont* pFont;
};

