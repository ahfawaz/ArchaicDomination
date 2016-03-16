/*
==================================================
|	File:		Frames.cpp
|	Author:		Ahmad Fawaz (gamer3780@gmail.com)
|   Team:		Iron Clad
|	Course:		SGP (Structures of Game Production)
|	Purpose:	CFrames holds the info of the frame's
|				source rects to load the correct image.
==================================================
*/
#include "Frames.h"


CFrames::CFrames(void)
{
}


CFrames::~CFrames(void)
{
}

void CFrames::GetAnchorPoint( float &x, float &y ) const
{
	x = this->AnchorPointX;
	y = this->AnchorPointY;
}


void CFrames::SetAnchorPoint( float x, float y )
{
	AnchorPointX = x;
	AnchorPointY = y;
}

void CFrames::SetRenderRect( int left, int top, int right, int bottom )
{
	this->RenderRect.left		= left;
	this->RenderRect.top		= top;
	this->RenderRect.right		= right;
	this->RenderRect.bottom		= bottom;
}

void CFrames::SetRenderRect( RECT source )
{
	this->RenderRect = source;
}

void CFrames::SetActiveRect( int left, int top, int right, int bottom )
{
	this->ActiveRect.left		= left;
	this->ActiveRect.top		= top;
	this->ActiveRect.right		= right;
	this->ActiveRect.bottom		= bottom;
}

void CFrames::SetActiveRect( RECT source )
{
	this->ActiveRect = source;
}

void CFrames::SetCollisionRect( int left, int top, int right, int bottom )
{
	this->CollisionRect.left		= left;
	this->CollisionRect.top			= top;
	this->CollisionRect.right		= right;
	this->CollisionRect.bottom		= bottom;
}

void CFrames::SetCollisionRect( RECT source )
{
	this->CollisionRect = source;
}

void CFrames::SetEvent( std::string newEvent )
{
	Event = newEvent;
}

void CFrames::SetDuration( float duration )
{
	this->Duration = duration;
}
