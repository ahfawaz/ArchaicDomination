/*
==================================================
|	File:		Animation.h
|	Author:		Ahmad Fawaz (gamer3780@gmail.com)
|   Team:		Iron Clad
|	Course:		SGP (Structures of Game Production)
|	Purpose:	CAnimation holds the entire animation
==================================================
*/
#pragma once

#include <vector>
#include <string>

#include "../SGD Wrappers/CSGD_Direct3D.h"

class CFrames;

class CAnimation
{
public:
	CAnimation(void);
	~CAnimation(void);

	CAnimation( const CAnimation& cpy );
	CAnimation& operator=( const CAnimation& assign );

	void ClearFrames();

	void AddFrame(  CFrames* frame );
	void AddFrame(	float AnchorPointX,	float AnchorPointY,	RECT RenderRect, RECT ActiveRect, RECT CollisionRect,
					std::string Event, float Duration);
	void AddFrame(	float AnchorPointX,	float AnchorPointY,	
					int RenderRectLeft,    int RenderRectTop, 	 int RenderRectRight, 	 int RenderRectBottom, 
					int ActiveRectLeft,    int ActiveRectTop, 	 int ActiveRectRight, 	 int ActiveRectBottom, 
					int CollisionRectLeft, int CollisionRectTop, int CollisionRectRight, int CollisionRectBottom,
					std::string Event, float Duration);

	int GetImageID() const { return ImageID; }
	int GetNumOfFrames() const { return NumOfFrames; }
	std::string GetImageFile() const { return imageFile; }
	bool GetIsLooping() const { return IsLooping; }
	CFrames* GetFrame( int index );

	void SetImageID( int ID ) { ImageID = ID; }
	void SetNumOfFrames( int num ) { NumOfFrames = num; }	
	void SetImageFile( std::string image ) { imageFile = image; }
	void SetIsLooping( bool looping ) { IsLooping = looping; }
	void SetFrame( int index, CFrames* frame );

private:
	int ImageID;
	int NumOfFrames;
	std::string imageFile;
	bool IsLooping;
	std::vector<CFrames*> Frames;
};

