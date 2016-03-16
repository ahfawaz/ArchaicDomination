#pragma once
#include "message.h"
#include "Entity.h"
class CDestroyAbilityMessage :
	public CMessage
{
public:
	CDestroyAbilityMessage(CEntity* abilityOwner);
	~CDestroyAbilityMessage(void);
	CEntity* GetAbilityOwner(void) {return abilityOwner;}
private:
	CEntity* abilityOwner;
};

