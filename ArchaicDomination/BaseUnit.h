#pragma once
#include "entity.h"
#include "TileSystem\Tile.h"
#include <vector>
#include "Ability.h"
#include "SGD Wrappers\IListener.h"
#include "FloatingText.h"
#include <string>

class CAnimTimeStamp;

enum  UnitID { FOOTSOLDIERS, PRIEST, WIZARD, GENERAL, CALVARY, CATAPULT, SKELETALWARRIOR, SHADOWPRIEST,
	NECROMANCER, LICH, DEATHKNIGHT, VOLATILEGHOUL, WARMACHINE };

class CBaseUnit :
	public CEntity, public IListener
{
public:


	CBaseUnit( );
	~CBaseUnit(void);

	void		CreateUnit ( UnitID Type, int x, int y );
	///////////////////////////////////////////////
	// IEntity Interface:
	virtual void		Update				( float fElapsedTime )		override;
	virtual void		Render				( void )					override;

	virtual void		TakeDamage			( CEntity* attacker, int damage = 0 )			override;

	bool				Move				( CTile* moveTile );
	bool				Attack				( CBaseUnit* target );


	///////////////////////////////////////////////
	// IListener Interface:
	virtual void HandleEvent( const CEvent* pEvent )	override;

	const char * GetType(void);
	UnitID GetUnitID(void) const {return TypeOfUnit;}

	int GetDamage(void);

	void ResetUnit			( void );

	//Ability Accessors and Mutators
	CAbility* GetAbility(void) const {return ability;}
	void SetAbility(CAbility* a) {ability = a;}
	void removeAbility(void);

	std::string GetAbility1(void) const {return Ability1;}
	std::string GetAbility2(void) const {return Ability2;}

	int GetAbility1_Cooldown(void) const {return Ability1_Cooldown;}
	int GetAbility2_Cooldown(void) const {return Ability2_Cooldown;}
	void SetAbility1_Cooldown(int count) {Ability1_Cooldown = count;}
	void SetAbility2_Cooldown(int count) {Ability2_Cooldown = count;}
	void DecreaseAbilityCooldown(void);

	bool IsAttacking(void) {return attackMode;}
	bool IsInRange(CBaseUnit* target);

	int GetMovement(void) {return m_nMovement;}
	int GetAttackRange(void) {return m_nAttackRange;}
	int GetMinDamage(void) const {return m_nMinDamage;}
	int GetMaxDamage(void) const {return m_nMaxDamage;}

	bool HasMoved(void) {return hasMoved;}
	bool HasAttacked(void) {return hasAttacked;}
	void SetHasAttacked(void) {hasAttacked = true;}
	void SetHasMoved(bool hasIt) {hasMoved = hasIt;}
	void Reset(void);

	//Buff Accessors and Mutators
	bool GetGeneralBuff(void) {return GeneralBuff;}
	bool GetCatapultBuff(void) {return CatapultBuff;}

	void SetGeneralBuff(bool buff) {GeneralBuff = buff;}
	void SetCatapultBuff(bool buff) {CatapultBuff = buff;}

	void GetScale(float& x, float&y) const;
	float GetScaleX() const;
	float GetScaleY() const;

	void SetScale(float x, float y);
	void SetScaleX(float x);
	void SetScaleY(float y);

	void SetMovementRange(int m) {m_nMovement = m;}
	void SetAttackRange(int a) {m_nMovement = a;}

	void ResetRange(void);

	void SetIsAttacking(bool b) {attackMode = b;}

	bool GetMovingLeft(void) {return m_bMovingLeft;}
	bool GetIsMoving(void) {return m_bIsMoving;}

	CTile* GetTile(void) {return currentTile;}
	void SetTile(CTile* t) {currentTile = t;}
	


	int		GetCountry(void)	{return m_nCountry;}
	int		GetTerritory(void)	{return m_nTerritory;}

	void	SetCountry(int c)	{m_nCountry = c;}
	void	SetTerritory(int t)	{m_nTerritory = t;}

	//Animation TimeStamps
	CAnimTimeStamp* GetIdldWaitingTS(void) const {return m_pIdleWaiting;}
	CAnimTimeStamp* GetIdldDoneTS(void) const {return m_pIdleDone;}



private:

	//Sprite ImageID's
	/*int soldierSprite;
	int priestSprite;
	int wizardSprite;
	int calvarySprite;
	int catapultSprite;
	int generalSprite;*/

	//Stats
	int m_nAttackRange;

	int m_nMinDamage;

	int m_nMaxDamage;

	int m_nMovement;

	float m_fScaleX;

	float m_fScaleY;

	bool GeneralBuff;
	bool CatapultBuff;

	UnitID			TypeOfUnit;

	//Actions
	bool hasMoved;

	bool attackMode;

	bool hasAttacked;


	CFloatingText* text;

	//Ability Stuff
	std::string Ability1;
	std::string Ability2;
	CAbility* ability;
	int	Ability1_Cooldown;
	int	Ability2_Cooldown;

	CTile* currentTile;

	//OverWorld location

	int		m_nCountry;
	int		m_nTerritory;

	//AnimTimeStamps
	CAnimTimeStamp* m_pIdleWaiting;
	CAnimTimeStamp* m_pIdleDone;
	CAnimTimeStamp* m_pMoveLeft;
	CAnimTimeStamp* m_pMoveRight;

	//Movement
	bool m_bIsMoving;
	bool m_bMovingLeft;
	float m_fStepInterval;

};

