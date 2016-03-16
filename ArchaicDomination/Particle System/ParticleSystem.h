/************************************************************
	File:		ParticleSystem.h
	Author:		Roberto Crespo
	Purpose:	CParticleSystem class loads the Emitters and Renders them in the Game.
************************************************************/

#pragma once
#include "Emitter.h"
#include <string>
#include <map>
#include <vector>
using namespace std;

struct EmitterInfo
{
	//int ImageID;
	
	int Width;
	int Height;
	int SpreadValue;
	int AngularOffset;
	int MaxParticle;
	
	
	//float
	double MinLife;
	double MaxLife;
	double GravityX;
	double GravityY;
	double CurrRotation;
	double StartingRotation;
	double EndingRotation;
	double StartingAlpha;
	double EndingAlpha;
	double StartingColorR;
	double EndingColorR;
	double StartingColorG;
	double EndingColorG;
	double StartingColorB;
	double EndingColorB;
	double SpawnRate;
	double StartingScaleX;
	double EndingScaleX;
	double StartingScaleY;
	double EndingScaleY;
    double EmitterPosX;
	double EmitterPosY;
	double StartingAccelerationX;
	double EndingAccelerationX;
	double StartingAccelerationY;
	double EndingAccelerationY;
	double StartingVelocityX;
	double EndingVelocityX;
	double StartingVelocityY;
	double EndingVelocityY;
	double EmitterRangeX;				
	double EmitterRangeY;				
	//bool
	bool  Looping;


};



class CParticleSystem
{
private:
	vector<CEmitter*> m_vActive;
	map<string,CEmitter>m_mLoaded;
	
public:
	CParticleSystem(void);
	~CParticleSystem(void);
	void Render();
	void Update (float fElaspedTime);
	//Loads up the Emitters from an XML File and Setup the Particle List
	bool LoadEffect( string name ,const char* filename );

	//CEmitter GetEffect(string s){ return m_mLoaded[s];}
	CEmitter* CreateEmitter(string name);
	CEmitter* GetEffect(int i){ return m_vActive[i];}
};

