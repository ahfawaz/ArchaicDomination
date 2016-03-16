#pragma once
#include "message.h"
#include "Entity.h"
class CHealingCircleMessage :
	  public CMessage
{
public:
	CHealingCircleMessage(CEntity* o);
	~CHealingCircleMessage(void);
	 CEntity* GetOwner(void) const { return owner; }

private:
	 CEntity* owner;
};

