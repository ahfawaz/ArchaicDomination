#pragma once
#include "ability.h"
#include "BaseUnit.h"
class CParticleSystem;
class CBattleCry :
	public CAbility
{
public:
	///////////////////////////////////////////////
	// IEntity Interface:
	virtual void		Update				( float fElapsedTime )		override;
	virtual void		Render				( void )					override;
	
	virtual void		PowerBar			( void )					override;

	CBattleCry();
	virtual				~CBattleCry			( void )					override;
private:
	//CParticleSystem* pBattleCry;
	int m_nBattleCrySFXID;
	float particleTimer;
	std::vector<CParticleSystem*> pBattleCry;
};

