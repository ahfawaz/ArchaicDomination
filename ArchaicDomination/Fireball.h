#pragma once
#include "ability.h"
#include "BaseUnit.h"
#include "BaseBar.h"


class PowerBar;
class CParticleSystem;
class CFireball :
	public CAbility
{
public:
	///////////////////////////////////////////////
	// IEntity Interface:
	virtual void		Update				( float fElapsedTime )		override;
	virtual void		Render				( void )					override;
	
	virtual void		PowerBar			( void )					override;

	CFireball();
	virtual				~CFireball			( void )					override;

	void SetTarget(CBaseUnit* t) {target = t;}

private: 
	int Velx;
	int VelY;
	int		m_nFireBallSFXID;
	int     m_nFireBallExplosionSFXID;
	float Timer;
	CEntity* target;
	CParticleSystem* pFireBall;
	

};

