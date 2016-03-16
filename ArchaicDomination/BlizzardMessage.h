#pragma once
#include "message.h"
#include "Entity.h"
class CBlizzardMessage :
	public CMessage
{
public:
	CBlizzardMessage(CEntity* o);
	~CBlizzardMessage(void);
	CEntity* GetOwner(void) const { return owner; }

private:
	CEntity* owner;
};

