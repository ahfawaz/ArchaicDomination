#pragma once
#include "ability.h"
#include "BaseUnit.h"
class CParticleSystem;
class CHealingCircle :
	public CAbility
{
public:
	///////////////////////////////////////////////
	// IEntity Interface:
	virtual void		Update				( float fElapsedTime )		override;
	virtual void		Render				( void )					override;
	
	virtual void		PowerBar			( void )					override;

	CHealingCircle();
	virtual				~CHealingCircle			( void )					override;

private:
	int m_nHealingCircleSFXID;
	CParticleSystem* pHealingCircle;
	float particleTimer;
};

