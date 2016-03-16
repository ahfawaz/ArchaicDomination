#include "CreateAbilityMessage.h"



CCreateAbilityMessage::CCreateAbilityMessage(CEntity* o) : CMessage(MSG_CREATE_ABILITY)
{
	owner = o;
}


CCreateAbilityMessage::~CCreateAbilityMessage(void)
{
	
}
