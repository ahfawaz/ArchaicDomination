#pragma once
class IEntity
{
public:

	///////////////////////////////////////////////
	// IEntity Interface:
	virtual void		Update				( float fElapsedTime )		= 0;
	virtual void		Render				( void )					= 0;
	
	

	virtual void		AddRef				( void ) = 0;
	virtual void		Release				( void ) = 0;


	virtual				~IEntity			( void ) = 0
	{

	}
};

