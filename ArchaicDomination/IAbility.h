#pragma once

#include "Entity.h"


class IAbility
{
public:
	
	///////////////////////////////////////////////
	// IEntity Interface:
	virtual void		Update				( float fElapsedTime )		= 0;
	virtual void		Render				( void )					= 0;
	
	virtual void		PowerBar			( void )					= 0;

	virtual				~IAbility			( void )					= 0
	{
	}

protected:

	enum AbilityType
	{

	};


};

