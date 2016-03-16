#pragma once
#include "BaseBar.h"


class PBar : public CBaseBar
{
public:
	PBar(void);
	~PBar(void);

	virtual void Upate(float fElapsedTime);

	virtual void Render(void);
};

