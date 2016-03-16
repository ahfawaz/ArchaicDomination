#pragma once
#include "message.h"
#include "Entity.h"

class CCreateAbilityTwoMessage :
	public CMessage
{
public:
	CCreateAbilityTwoMessage(CEntity* owner);
	~CCreateAbilityTwoMessage(void);
	CEntity* GetOwner(void) {return owner;}

private:
	CEntity* owner;
};

