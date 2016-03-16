#include "CatapultAttackingMessage.h"


CCatapultAttackingMessage::CCatapultAttackingMessage(CEntity* o, CEntity* t) : CMessage(MSG_CATAPULT_ATTACK)
{
	owner = o;
	target = t;
}


CCatapultAttackingMessage::~CCatapultAttackingMessage(void)
{
}
