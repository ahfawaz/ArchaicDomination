#pragma once
#include "ability.h"
#include "BaseUnit.h"
class CParticleSystem;
class CCharge :
	public CAbility
{
public:
	///////////////////////////////////////////////
	// IEntity Interface:
	virtual void		Update				( float fElapsedTime )		override;
	virtual void		Render				( void )					override;

	virtual void		PowerBar			( void )					override;

	CCharge();
	virtual				~CCharge			( void )					override;

	void SetTarget(CBaseUnit* t) {target = t;}
	void SetHasMovedBefore(bool hasIt) { hasMovedBefore = hasIt; }

private: 
	int m_nChargeSFXID;
	float stepTimer;
	CParticleSystem* pCharge;
	CEntity* target;

	bool hasMovedBefore;
};

