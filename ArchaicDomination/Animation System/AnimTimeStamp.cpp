/*
==================================================
|	File:		AnimTimeStamp.cpp
|	Author:		Ahmad Fawaz (gamer3780@gmail.com)
|   Team:		Iron Clad
|	Course:		SGP (Structures of Game Production)
|	Purpose:	CAnimTimeStamp holds the time info 
|				of the current frame
==================================================
*/
#include "AnimTimeStamp.h"


CAnimTimeStamp::CAnimTimeStamp(std::string animationName)
{
	this->currAnimation = animationName;
	this->currFrame = 0;
	this->TimeOnFrame = 0;
}


CAnimTimeStamp::~CAnimTimeStamp(void)
{
}
