#pragma once
#include "BaseBar.h"


class AccuracyBar : public CBaseBar
{
public:
	AccuracyBar(void);
	~AccuracyBar(void);


	virtual void Upate(float fElapsedTime);

	virtual void Render(void);

};

