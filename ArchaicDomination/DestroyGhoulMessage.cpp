#include "DestroyGhoulMessage.h"


CDestroyGhoulMessage::CDestroyGhoulMessage(CEntity* o) : CMessage(MSG_DESTROY_GHOUL)
{
	owner = o;
}


CDestroyGhoulMessage::~CDestroyGhoulMessage(void)
{
}
