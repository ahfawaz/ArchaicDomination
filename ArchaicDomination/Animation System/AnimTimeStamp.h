/*
==================================================
|	File:		AnimTimeStamp.h
|	Author:		Ahmad Fawaz (gamer3780@gmail.com)
|   Team:		Iron Clad
|	Course:		SGP (Structures of Game Production)
|	Purpose:	CAnimTimeStamp holds the time info 
|				of the current frame
==================================================
*/
#pragma once

#include <string>

class CAnimTimeStamp
{
public:
	CAnimTimeStamp(std::string animationName);
	~CAnimTimeStamp(void);

	int GetCurrFrame() const { return currFrame; }
	std::string GetCurrAnimation() const { return currAnimation; }
	float GetTimeOnFrame() const { return TimeOnFrame; }

	void SetCurrFrame( int frame ) { currFrame = frame; }
	void SetCurrAnimation( int animation ) { currAnimation = animation; }
	void SetTimeOnFrame( float time ) { TimeOnFrame = time; }

private:
	int currFrame;
	std::string currAnimation;
	float TimeOnFrame;
};

