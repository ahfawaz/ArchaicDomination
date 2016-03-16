#pragma once

#include <vector>
#include "SGD Wrappers\IListener.h"

class CBaseUnit;
class CBaseBuilding;
class CAbility;
#include "BasePlayer.h"

class CEnemy : public CBasePlayer, public IListener
{
public:
	////////////////////////////////
	//Singleton
	static CEnemy * GetInstance ( void ) ;

	std::vector< CBaseUnit* > GetUnits(void) {return m_vUnits;}
	std::vector< CBaseBuilding* > GetBuildings(void)  {return m_vBuildings;} 

	PlayerType GetType(void) {return type;}
	bool Input(void); 
	void Update(float fElapsedTime);
	void Render(void);
	void AddUnit(CBaseUnit* unit);
	void LoadUnitsOnBattleGrid(void);
	
	bool GetIsCastingAbility(void) const {return m_bIsCastingAbility;}
	void SetIsCastingAbility(bool casting) {m_bIsCastingAbility = casting;}
	void UpdateAI(float fElapsedTime);

	void ActivateDarkCurse(CAbility* theCurse);
	CAbility* GetActiveCurse(void) { return darkCurseActive; }

	bool IsAI(void) {return m_bSinglePlayer;}
	void SetAI(bool b) {m_bSinglePlayer = b;}

	///////////////////////////////////////////////
	// IListener Interface:
	virtual void HandleEvent( const CEvent* pEvent )	override;

	void AddBuilding(CBaseBuilding*);
	void DeleteBuilding(int index);
	bool GetMoved		( void )			{ return m_bHasMoved; }
	void SetMoved		( bool temp )		{ m_bHasMoved = temp; }
	
	bool GetAttacked	( void )			{ return m_bHasAttacked; }
	void SetAttacked	( bool temp )		{ m_bHasAttacked = temp; }

	int GetUnitsInBattle (void)				{return unitsInBattle;}

	void	Reset	(void);
	void ClearEnemyinfo();
private:

	CEnemy(void);
	~CEnemy(void);
	PlayerType type;


	typedef		std::vector< CBaseUnit* >			mBaseUnits;
	mBaseUnits										m_vUnits;

	typedef		std::vector< CBaseBuilding* >		mBaseBuildings;
	mBaseBuildings									m_vBuildings;


	bool			m_bSinglePlayer;
	bool			m_bIsCastingAbility;
	float AIDelay;
	bool			m_bHasMoved;
	bool			m_bHasAttacked;
	CAbility*		darkCurseActive;


	int				unitsInBattle;
	int				currentX, currentY;
};





