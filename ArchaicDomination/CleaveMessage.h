#pragma once
#include "message.h"
#include "Entity.h"

class CCleaveMessage :
	public CMessage
{
public:
	CCleaveMessage(CEntity* o);
	~CCleaveMessage(void);
	CEntity* GetOwner(void) const { return owner; }
private:
	CEntity* owner;
};

