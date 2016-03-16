/*
==================================================
|	File:		Frames.h
|	Author:		Ahmad Fawaz (gamer3780@gmail.com)
|   Team:		Iron Clad
|	Course:		SGP (Structures of Game Production)
|	Purpose:	CFrames holds the info of the frame's
|				source rects to load the correct image.
==================================================
*/
#pragma once

#include <string>

#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"

class CFrames
{
public:
	CFrames(void);
	~CFrames(void);

	float GetAnchorPointX() const { return AnchorPointX; }
	float GetAnchorPointY() const { return AnchorPointY; }
	void GetAnchorPoint(float &x, float &y) const;
	RECT GetRenderRect() const { return RenderRect; }
	RECT GetActiveRect() const { return ActiveRect; }
	RECT GetCollisionRect() const { return CollisionRect; }
	std::string GetEvent() const { return Event; }
	float GetDuration() const { return Duration; }

	void SetAnchorPointX( float x) { AnchorPointX = x; }
	void SetAnchorPointY( float y) { AnchorPointY = y; }
	void SetAnchorPoint( float x, float y );
	void SetRenderRect( int left, int top, int right, int bottom );
	void SetRenderRect( RECT source );
	void SetActiveRect( int left, int top, int right, int bottom );
	void SetActiveRect( RECT source );
	void SetCollisionRect( int left, int top, int right, int bottom );
	void SetCollisionRect( RECT source );
	void SetEvent( std::string newEvent );
	void SetDuration( float duration );

private:
	float AnchorPointX;
	float AnchorPointY;
	RECT RenderRect;
	RECT ActiveRect;
	RECT CollisionRect;
	std::string Event;
	float Duration;
};

