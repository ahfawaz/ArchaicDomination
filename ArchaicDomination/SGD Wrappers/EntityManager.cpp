/**************************************************
|	File:		EntityManager.cpp
|	Author:		Doug Monroe (dmonroe@fullsail.com)
|	Course:		SGD
|	Purpose:	CEntityManager class stores 
|				& maintains all game entities.
**************************************************/


#include "EntityManager.h"

#include "../IEntity.h"

#include <cassert>


/*************************************************/
// Constructor
CEntityManager::CEntityManager( void )
{
	m_bIterating = false;
}

/*************************************************/
// Destructor
CEntityManager::~CEntityManager( void )
{
	// Validate the iteration state
	assert( m_bIterating == false && "~CEntityManager - cannot be destroyed while iterating!" );
}


/*************************************************/
// AddEntity
//	- add the entity into the table in the specified bucket
//	  & store a reference to the entity
void CEntityManager::AddEntity( IEntity* pEntity, unsigned int unType )
{
	// Validate the iteration state
	assert( m_bIterating == false && "CEntityManager::AddEntity - cannot add while iterating" );

	// Validate the parameters
	assert( pEntity != nullptr && "CEntityManager::AddEntity - pointer cannot be null" );

	// Expand the vector?
	if( unType >= m_tEntities.size() )
		m_tEntities.resize( unType + 1 );


	// Add the entity to the end of the bucket
	m_tEntities[ unType ].push_back( pEntity );
	pEntity->AddRef();
}

/*************************************************/
// RemoveEntity
//	- remove & release the entity from any bucket
void CEntityManager::RemoveEntity( IEntity* pEntity )
{
	// Validate the iteration state
	assert( m_bIterating == false && "CEntityManager::RemoveEntity - cannot remove while iterating" );

	// Validate the parameters
	assert( pEntity != nullptr && "CEntityManager::RemoveEntity - pointer cannot be null" );

	// Try to find the entity
	for( int type = (int)m_tEntities.size()-1; type >= 0; type-- )
	{
		EntityVector& vec = m_tEntities[ type ];
		for( int i = (int)vec.size() - 1; i >= 0; i-- )
		{
			if( vec[ i ] == pEntity )
			{
				// Remove the entity
				pEntity->Release();
				vec.erase( vec.begin() + i );
				return;
			}
		}
	}
}

/*************************************************/
// RemoveEntity
//	- remove & release an entity from a specific bucket
void CEntityManager::RemoveEntity( IEntity* pEntity, unsigned int unType )
{
	// Validate the iteration state
	assert( m_bIterating == false && "CEntityManager::RemoveEntity - cannot remove while iterating" );

	// Validate the parameters
	assert( pEntity != nullptr && "CEntityManager::RemoveEntity - pointer cannot be null" );
	assert( unType < m_tEntities.size() && "CEntityManager::RemoveEntity - invalid type" );

	// Try to find the entity
	EntityVector& vec = m_tEntities[ unType ];
	for( int i = (int)vec.size() - 1; i >= 0; i-- )
	{
		if( vec[ i ] == pEntity )
		{
			// Remove the entity
			pEntity->Release();
			vec.erase( vec.begin() + i );
			break;
		}
	}
}

/*************************************************/
// RemoveAll
//	- remove & release all the entities from a specific bucket
void CEntityManager::RemoveAll( unsigned int unType )
{
	// Validate the iteration state
	assert( m_bIterating == false && "CEntityManager::RemoveAll - cannot remove while iterating" );

	// Validate the parameter
	assert( unType < m_tEntities.size() && "CEntityManager::RemoveAll - invalid type" );

	// Release each entity in the bucket
	EntityVector& vec = m_tEntities[ unType ];
	for( int i = (int)vec.size() - 1; i >= 0; i-- )
	{
		vec[ i ]->Release();
	}

	// Collapse the vector
	m_tEntities[ unType ].clear();
}

/*************************************************/
// RemoveAll
//	- remove all entities from the table
void CEntityManager::RemoveAll( void )
{
	// Validate the iteration state
	assert( m_bIterating == false && "CEntityManager::RemoveAll - cannot remove while iterating" );


	// Lock the iterator
	m_bIterating = true;
	{
		// Release every entity
		for( int type = (int)m_tEntities.size()-1; type >= 0; type-- )
		{
			EntityVector& vec = m_tEntities[ type ];
			for( int i = (int)vec.size() - 1; i >= 0; i-- )
			{
				vec[ i ]->Release();
			}
		}

		// Collapse the table
		m_tEntities.clear();
	}
	// Unlock the iterator
	m_bIterating = false;
}

/*************************************************/
// UpdateAll
//	- update each entity
void CEntityManager::UpdateAll( float fElapsedTime )
{
	// Validate the iteration state
	assert( m_bIterating == false && "CEntityManager::UpdateAll - cannot update while iterating" );


	// Lock the iterator
	m_bIterating = true;
	{
		// Loop through the buckets
		for( int type = (int)m_tEntities.size()-1; type >= 0; type-- )
		{
			// Loop through the entities
			EntityVector& vec = m_tEntities[ type ];
			for( int i = (int)vec.size() - 1; i >= 0; i-- )
			{
				vec[ i ]->Update( fElapsedTime );
			}
		}
	}
	// Unlock the iterator
	m_bIterating = false;
}

/*************************************************/
// RenderAll
//	- render each entity
void CEntityManager::RenderAll( void )
{
	// Validate the iteration state
	assert( m_bIterating == false && "CEntityManager::RenderAll - cannot render while iterating" );


	// Lock the iterator
	m_bIterating = true;
	{
		// Loop through the buckets
		for( int type = (int)m_tEntities.size()-1; type >= 0; type-- )
		{
			// Loop through the entities
			EntityVector& vec = m_tEntities[ type ];
			for( int i = (int)vec.size() - 1; i >= 0; i-- )
			{
				vec[ i ]->Render();
			}
		}
	}
	// Unlock the iterator
	m_bIterating = false;
}


