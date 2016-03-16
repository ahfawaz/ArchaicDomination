#pragma once
#include "BaseBar.h"


class SpamBar : public CBaseBar
{
public:
	SpamBar(void);
	~SpamBar(void);

	virtual void Upate(float fElapsedTime);

	virtual void Render(void);


private:

	//Image Description
	int				imagePressZID;
	int				imagePressXID;
	float			m_fswitchTime;
	bool			pressX;

	float			m_fTimeLimit;
};

