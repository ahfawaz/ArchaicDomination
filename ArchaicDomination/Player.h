#pragma once


#include <vector>
#include "SGD Wrappers\IListener.h"
#include "BasePlayer.h"

class CBaseUnit;
class CBaseBuilding;
//class CParticleSystem;

class CPlayer : public	CBasePlayer, public IListener
{
public:

	////////////////////////////////
	//Singleton
	static CPlayer * GetInstance ( void ) ;
	std::vector< CBaseUnit* > GetUnits(void)  {return m_vUnits;} 
	std::vector< CBaseBuilding* > GetBuildings(void)  {return m_vBuildings;} 

	PlayerType GetType(void) {return type;}
	bool Input(void); 
	void Update(float fElapsedTime);
	void Render(void);

	void LoadUnitsOnBattleGrid(void);

	bool GetIsCastingAbility(void) const {return m_bIsCastingAbility;}
	void SetIsCastingAbility(bool casting) {m_bIsCastingAbility = casting;}

	///////////////////////////////////////////////
	// IListener Interface:
	virtual void HandleEvent( const CEvent* pEvent )	override;

	void AddBuilding(CBaseBuilding*);
	void DeleteBuilding( int index);

	bool GetMoved		( void )			{ return m_bHasMoved; }
	void SetMoved		( bool temp )		{ m_bHasMoved = temp; }
	bool GetAttacked	( void )			{ return m_bHasAttacked; }
	void SetAttacked	( bool temp )		{ m_bHasAttacked = temp; }
	int GetUnitsInBattle (void)				{ return unitsInBattle; }
	void AddUnit(CBaseUnit*);
	void ClearPlayerinfo();
	void Reset			( void );

private:

	CPlayer(void);
	~CPlayer(void);

	PlayerType type;

	typedef		std::vector< CBaseUnit* >			mBaseUnits;
	mBaseUnits										m_vUnits;

	typedef		std::vector< CBaseBuilding* >		mBaseBuildings;
	mBaseBuildings									m_vBuildings;

	bool			m_bIsCastingAbility;
	CAbility*		BattleCryActive;
	bool			m_bHasMoved;
	bool			m_bHasAttacked;
	//CParticleSystem* pBattleCry;


	int				unitsInBattle;
	int				currentX, currentY;


};

