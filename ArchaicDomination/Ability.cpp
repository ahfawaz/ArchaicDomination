#include "Ability.h"


CAbility::CAbility()
{
	m_pOwner = nullptr;

}


CAbility::~CAbility(void)
{

}

///////////////////////////////////////////////
// IEntity Interface:
void		CAbility::Update				( float fElapsedTime )
{

}


void		CAbility::Render				( void )					
{
}

void		CAbility::PowerBar			( void )					
{
}

void CAbility::SetOwner(CEntity* o)
{
	if(m_pOwner != nullptr)
	{
	m_pOwner->Release();
	}

	if(o != nullptr)
	{
	m_pOwner = o;
	m_pOwner->AddRef();
	}
}
