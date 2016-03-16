#include "CreateAbilityTwoMessage.h"


CCreateAbilityTwoMessage::CCreateAbilityTwoMessage(CEntity* owner) : CMessage(MSG_CREATE_ABILITYTWO)
{
	this->owner = owner;
}


CCreateAbilityTwoMessage::~CCreateAbilityTwoMessage(void)
{
}
