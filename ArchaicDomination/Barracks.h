#pragma once
#include "basebuilding.h"
#include <vector>
using namespace std;

class CBaseUnit;

class Barracks :
	public CBaseBuilding
{
public:
	Barracks(int o, int x, int y);
	~Barracks(void);

	vector< vector<CBaseUnit*> > GetUnits(void)  {return m_vUnits;} 


private:
	typedef		vector< vector<CBaseUnit*> >			mBaseUnits;
	mBaseUnits										m_vUnits;
};

