#pragma once
#include "BaseBar.h"


class FillBar : public CBaseBar
{
public:
	FillBar(void);
	~FillBar(void);

	virtual void Upate(float fElapsedTime);

	virtual void Render(void);

private:

	//Image Description
	int				imagePressZID;
	int				imagePressXID;
	float			m_fswitchTime;
	bool			pressX;

	int				m_nMinSuccess;
	float			m_fBarTimer;
};

