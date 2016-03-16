#pragma once
#include "Message.h"
#include "Entity.h"


class CCreateAbilityMessage :
	public CMessage
{
public:
	CCreateAbilityMessage(CEntity* o);
	virtual ~CCreateAbilityMessage(void);
	CEntity* GetOwner(void) {return owner;}

private:
	CEntity* owner;

};

