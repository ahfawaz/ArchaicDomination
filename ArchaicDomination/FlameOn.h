#pragma once
#include "ability.h"
class CParticleSystem;
class CFlameOn :
	public CAbility
{
public:
	///////////////////////////////////////////////
	// IEntity Interface:
	virtual void		Update				( float fElapsedTime )		override;
	virtual void		Render				( void )					override;
	
	virtual void		PowerBar			( void )					override;

	CFlameOn();
	virtual				~CFlameOn			( void )					override;

private:
	int m_nFlameOnSFXID;
	CParticleSystem* pFlameOn;
	float particleTimer;
};

