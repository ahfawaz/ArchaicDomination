#pragma once

class CBaseBar
{
public:

	CBaseBar(void);
	virtual ~CBaseBar(void);

	virtual void Upate(float fElapsedTime);

	virtual void Render(void);

	int		GetDamageScale(void)	{return m_nDamageScale;}
	float	GetPowerBarTimer(void)	{return m_fPowerBarTimer;}
	bool	GetIsScaling(void)		{return m_bIsScaling;}
	bool	GetActive(void)			{return m_bActive;}


	void SetIsScaling(bool isScaling)		{m_bIsScaling = isScaling;}
	void SetDamageScale(int damageScale)	{m_nDamageScale = damageScale;}
	void SetPowerBarTimer(float time)		{m_fPowerBarTimer = time;}
	void SetActive(bool active)				{m_bActive = active;}


	void DamageScaleMinus()					{m_nDamageScale -= 1;}
	void DamageScalePlus()					{m_nDamageScale += 1;}

	void DamageScalePlus(int i)				{m_nDamageScale += i;}
	void DamageScaleMinus(int i)			{m_nDamageScale -= i;}


	bool GetCast()			{return m_bCast;}
	void SetCast(bool c)	{m_bCast = c;}



private:
	int						m_nDamageScale;
	bool					m_bIsScaling;
	float					m_fPowerBarTimer;
	bool					m_bActive;
	bool					m_bCast;

protected:
	int barImageID;
};
