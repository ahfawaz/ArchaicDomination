/**************************************************
|	File:		Entity.cpp
|	Author:		Joshua Ford(joshuacford@fullsail.edu
|	Course:		SGP 1310
|	Purpose:	CEntity class is used for a 
|				velocity-based game object. 
|				It is the parent class for all 
|				other objects.
**************************************************/


#include "Entity.h"

#include "SGD Wrappers/CSGD_TextureManager.h"

#include <cassert>


/*************************************************/
// Constructor
//	- set ALL data members to 'clean' values
CEntity::CEntity( void )
{
	m_nMaxHealth = m_nCurrentHealth;


}

/*************************************************/
// Destructor
CEntity::~CEntity( void )
{
}


void CEntity::Update( float fElapsedTime )
{
	
}

void CEntity::TakeDamage( CEntity* temp, int damage )
{

}

void CEntity::Render( void )
{

}

void CEntity::AddRef( void )
{
	assert( m_unRefCount != UINT_MAX && "CEntity::AddRef - reference count has exceeded max" );
	++m_unRefCount;
}

void CEntity::Release( void )
{
	--m_unRefCount;

	if( m_unRefCount == 0 )
		delete this;
}
