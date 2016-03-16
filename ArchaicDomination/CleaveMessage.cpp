#include "CleaveMessage.h"


CCleaveMessage::CCleaveMessage(CEntity* o) : CMessage(MSG_CLEAVE)
{
	owner = o;
}


CCleaveMessage::~CCleaveMessage(void)
{
}
