#pragma once
#include "ability.h"
#include "BaseUnit.h"
class CParticleSystem;
class CJudgement :
	public CAbility
{
public:
	///////////////////////////////////////////////
	// IEntity Interface:
	virtual void		Update				( float fElapsedTime )		override;
	virtual void		Render				( void )					override;
	
	virtual void		PowerBar			( void )					override;

	CJudgement();
	virtual				~CJudgement			( void )					override;

	void DealDamage();

private:
	int m_nJudgementSFXID;
	std::vector<CParticleSystem*> pJudgement;
	float particleTimer;
};

