#pragma once
#include "message.h"
#include "BaseUnit.h"

class CDestroyGhoulMessage :
	public CMessage
{
public:
	CDestroyGhoulMessage(CEntity* o);
	~CDestroyGhoulMessage(void);
	CEntity* GetOwner(void) { return owner; }

private:
	CEntity* owner;
};

