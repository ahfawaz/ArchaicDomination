#pragma once
#include "ability.h"
#include "BaseUnit.h"
class CParticleSystem;
class CBlizzard :
	public CAbility
{
public:
	///////////////////////////////////////////////
	// IEntity Interface:
	virtual void		Update				( float fElapsedTime )		override;
	virtual void		Render				( void )					override;
	
	virtual void		PowerBar			( void )					override;

	CBlizzard();
	virtual				~CBlizzard			( void )					override;

private:
	int m_nBlizzardSFXID;
	CParticleSystem* pBlizzard;
	float particleTimer;
};

