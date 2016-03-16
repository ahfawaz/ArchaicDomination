/**************************************************
|	File:		Entity.h
|	Author:		Joshua Ford(joshuacford@fullsail.edu
|	Course:		SGP 1310
|	Purpose:	CEntity class is used for a 
|				velocity-based game object. 
|				It is the parent class for all 
|				other objects.
**************************************************/

#ifndef ENTITY_H
#define ENTITY_H


#include "IEntity.h"


/*************************************************/
// CEntity class
//	- concrete base class for game objects
class CEntity : public IEntity
{
public:
	///////////////////////////////////////////////
	// Destructor MUST be virtual to prevent 
	// memory leaks in dynamically-allocated children
	CEntity( void );
	~CEntity( void );	


	
	///////////////////////////////////////////////
	// IEntity Interface:
	virtual void		Update				( float fElapsedTime )		override;
	virtual void		Render				( void )					override;
	
	virtual void		TakeDamage			( CEntity* temp, int damage = 0 );

	virtual void		AddRef				( void )					final;
	virtual void		Release				( void )					final;

	
	void				SetMaxHealth			( int MaxHealth )		{ m_nMaxHealth = MaxHealth; }
	void				SetCurrentHealth		( int CurrentHealth)	{ m_nCurrentHealth = CurrentHealth; }


	int					GetMaxHealth			( void )				{ return m_nMaxHealth; }
	int					GetCurrentHealth		( void )				{ return m_nCurrentHealth; }

	void				SetSpriteID				( int TempSpriteID )	{ m_nSpriteID = TempSpriteID; }
	int					GetSpriteID				( void )				{ return m_nSpriteID; }

	void				SetPosX					( float PosX )			{ m_fPosX = PosX; }
	void				SetPosY					( float	PosY )			{ m_fPosY = PosY; }

	float				GetPosX					( void )				{ return m_fPosX; }
	float				GetPosY					( void )				{ return m_fPosY; }

		

private:
	///////////////////////////////////////////////
	// ID's
	int			m_nSpriteID;
	int			m_nSoundID;

	// position
	float		m_fPosX;
	float		m_fPosY;

	// reference count
	unsigned int	m_unRefCount;

	int			m_nMaxHealth;
	int			m_nCurrentHealth;

};

#endif //ENTITY_H

