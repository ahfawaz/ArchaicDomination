#include "DestroyDarkCurseMessage.h"


CDestroyDarkCurseMessage::CDestroyDarkCurseMessage(CEntity* o) : CMessage( MSG_DESTROY_DARKCURSE )
{
	owner = o;
}


CDestroyDarkCurseMessage::~CDestroyDarkCurseMessage(void)
{
}
