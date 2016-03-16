#include "DestroyAbilityMessage.h"


CDestroyAbilityMessage::CDestroyAbilityMessage(CEntity* abilityOwner) : CMessage(MSG_DESTROY_ABILITY)
{
	this->abilityOwner = abilityOwner;
}


CDestroyAbilityMessage::~CDestroyAbilityMessage(void)
{
}
