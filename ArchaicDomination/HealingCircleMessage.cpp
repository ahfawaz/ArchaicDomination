#include "HealingCircleMessage.h"


CHealingCircleMessage::CHealingCircleMessage(CEntity* o) : CMessage(MSG_HEALINGCIRCLE)
{
	owner = o;
}


CHealingCircleMessage::~CHealingCircleMessage(void)
{
}
