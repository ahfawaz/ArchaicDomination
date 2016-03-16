#pragma once
#include "ability.h"
#include "BaseUnit.h"
class CParticleSystem;
class CHeal :
	public CAbility
{
public:
	///////////////////////////////////////////////
	// IEntity Interface:
	virtual void		Update				( float fElapsedTime )		override;
	virtual void		Render				( void )					override;
	
	virtual void		PowerBar			( void )					override;

	CHeal();
	virtual				~CHeal			( void )					override;
	void SetTarget(CBaseUnit* t) {target = t;}

private:
	int m_nHealSFXID;
	CParticleSystem* pHeal;
	CEntity* target;
};

