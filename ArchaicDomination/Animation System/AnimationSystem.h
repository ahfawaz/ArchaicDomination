/*
==================================================
|	File:		AnimationSystem.h
|	Author:		Ahmad Fawaz (gamer3780@gmail.com)
|   Team:		Iron Clad
|	Course:		SGP (Structures of Game Production)
|	Purpose:	CAnimationSystem runs the animation
==================================================
*/
#pragma once

#include "Animation.h"
#include "../SGD Wrappers/IListener.h"

//Forward Declared Classes
class CAnimTimeStamp;

#include <map>
#include <string>
#include "../TinyXML/tinyxml.h"

class CAnimationSystem : public IListener
{

	std::map<std::string, CAnimation> animation;
	std::vector<std::string> animeNames;

public:
	CAnimationSystem(void);
	~CAnimationSystem(void);

	///////////////////////////////////////////////
	// IListener Interface:
	virtual void HandleEvent( const CEvent* pEvent )	override;

	bool LoadedAnimation( const char* FileName);
	void Render(CAnimTimeStamp* timeStamp, int X, int Y);
	void Update(CAnimTimeStamp* timeStamp, float Dt);
	void PlayEvent(std::string eventName);
};

