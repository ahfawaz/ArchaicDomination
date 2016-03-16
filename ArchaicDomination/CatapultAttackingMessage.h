#pragma once
#include "message.h"
#include "BaseUnit.h"

class CCatapultAttackingMessage :
	public CMessage
{
public:
	CCatapultAttackingMessage(CEntity* o, CEntity* t);
	~CCatapultAttackingMessage(void);
	CEntity* GetOwner(void) {return owner;}
	CEntity* GetTarget(void) {return target;}

private:
	CEntity* owner;
	CEntity* target;
};

