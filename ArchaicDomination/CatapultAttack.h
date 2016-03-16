#pragma once
#include "ability.h"
#include "BaseUnit.h"

class CCatapultAttack :
	public CAbility
{
public:
	CCatapultAttack(void);
	virtual ~CCatapultAttack(void);

	///////////////////////////////////////////////
	// IEntity Interface:
	virtual void		Update				( float fElapsedTime )		override;
	virtual void		Render				( void )					override;

	virtual void		PowerBar			( void )					override;

	void SetTarget(CEntity* t) {target = t;}

private:
	CEntity* target;

	bool hit;

	float inputDelay;
};

