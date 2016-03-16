#pragma once
#include "message.h"
#include "BaseUnit.h"

class CDestroyDarkCurseMessage :
	public CMessage
{
public:
	CDestroyDarkCurseMessage(CEntity* o);
	~CDestroyDarkCurseMessage(void);
	CEntity* GetOwner(void) {return owner;}

private:
	CEntity* owner;
};

