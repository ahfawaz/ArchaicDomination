#pragma once
#include "iability.h"
#include "BaseBar.h"


class CAbility :
	public IAbility
{
public:
	
	///////////////////////////////////////////////
	// IEntity Interface:
	

	void SetSprite(int id) {m_nParticleID = id;}
	int GetSprite(void) {return m_nParticleID;}

	virtual void		Update				( float fElapsedTime );
	virtual void		Render				( void )					override;
	
	virtual void		PowerBar			( void )					override;

	CAbility();
	virtual				~CAbility			( void )					override;

	int		GetPosX(void)			{return m_nPosX;}
	int		GetPosY(void)			{return m_nPosY;}
	bool	GetIsCasting(void)		{return m_bIsCasting;}
	bool	GetIsScaling(void)		{return m_bIsScaling;}
	int		GetDamageScale(void)	{return m_nDamageScale;}
	float	GetPowerBarTimer(void)	{return m_fPowerBarTimer;}
	int		GetParticleID(void)		{return m_nParticleID; }


	void SetPosX(int x)						{m_nPosX = x;}
	void SetPosY(int y)						{m_nPosY = y;}
	void SetIsCasting(bool isCasting)		{m_bIsCasting = isCasting;}
	void SetIsScaling(bool isScaling)		{m_bIsScaling = isScaling;}
	void SetDamageScale(int damageScale)	{m_nDamageScale = damageScale;}
	void SetPowerBarTimer(float time)		{m_fPowerBarTimer = time;}
	void SetParticleID	( int ID )			{m_nParticleID = ID; }
	

	CEntity* GetOwner(void) {return m_pOwner;}
	void SetOwner(CEntity* o);

	void DamageScaleMinus() {m_nDamageScale -= 1;}
	void DamageScalePlus() {m_nDamageScale += 1;}

private:

	CEntity*				m_pOwner;
	int						m_nParticleID;
	int						m_nSoundID;
	int						m_nRange;
	int						m_nDamage;
	short					m_sCooldown;
	int						m_nHealing;
	int						m_nPosX;
	int						m_nPosY;
	int						m_nDamageScale;
	float					m_fPowerBarTimer;
	bool					m_bIsCasting;
	bool					m_bIsScaling;


protected:

	CBaseBar*				bar;
};

