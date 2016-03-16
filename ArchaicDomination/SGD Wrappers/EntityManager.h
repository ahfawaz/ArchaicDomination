/**************************************************
|	File:		EntityManager.h
|	Author:		Doug Monroe (dmonroe@fullsail.com)
|	Course:		SGD
|	Purpose:	CEntityManager class stores 
|				& maintains all game entities.
**************************************************/

#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H


#include <vector>

class IEntity;


/*************************************************/
// CEntityManager class
//	- stores a vector of vectors of IEntity*
//	- updates & renders objects
class CEntityManager
{
public:
	///////////////////////////////////////////////
	CEntityManager ( void );
	~CEntityManager( void );
	
	///////////////////////////////////////////////
	// Entity Management
	void AddEntity			( IEntity* pEntity, unsigned int unType );
	void RemoveEntity		( IEntity* pEntity );
	void RemoveEntity		( IEntity* pEntity, unsigned int unType );
	void RemoveAll			( unsigned int unType );
	void RemoveAll			( void );
	
	///////////////////////////////////////////////
	// Entity Upkeep
	void UpdateAll			( float fElapsedTime );
	void RenderAll			( void );

private:
	///////////////////////////////////////////////
	// Use typedefs to clarify names
	typedef std::vector< IEntity* >		EntityVector;
	typedef EntityVector::iterator		EVIterator;
	typedef std::vector< EntityVector >	EntityTable;

	EntityTable		m_tEntities;	// table of IEntity*
	bool			m_bIterating;	// lock to protect the table
};

#endif //ENTITYMANAGER_H
