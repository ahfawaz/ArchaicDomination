#include "BlizzardMessage.h"


CBlizzardMessage::CBlizzardMessage(CEntity* o) : CMessage(MSG_BLIZZARD)
{
	owner = o;
}


CBlizzardMessage::~CBlizzardMessage(void)
{
}
