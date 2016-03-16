/*
==================================================
|	File:		Animation.cpp
|	Author:		Ahmad Fawaz (gamer3780@gmail.com)
|   Team:		Iron Clad
|	Course:		SGP (Structures of Game Production)
|	Purpose:	CAnimation holds the entire animation
==================================================
*/

#include "Animation.h"
#include "Frames.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"


CAnimation::CAnimation(void)
{
}


CAnimation::~CAnimation(void)
{

}

CAnimation::CAnimation( const CAnimation& cpy )
{
	*this = cpy;
}

CAnimation& CAnimation::operator=( const CAnimation& assign )
{
	if(this != &assign)
	{
		this->ImageID = assign.ImageID;
		this->NumOfFrames = assign.NumOfFrames;
		this->imageFile = assign.imageFile;
		this->IsLooping = assign.IsLooping;

		for (int i = 0; i < (int)assign.Frames.size(); i++)
		{
			this->AddFrame(assign.Frames[i]);
		}
	}
	return *this;
}

void CAnimation::ClearFrames()
{
	for (int i = 0; i < (int)Frames.size(); i++)
	{
		delete Frames[i];
		Frames[i] = nullptr;
	}
	Frames.clear();
}


CFrames* CAnimation::GetFrame( int index )
{
	if(Frames.empty())
	{
		return nullptr;
	}
	return Frames[index];
}

void CAnimation::SetFrame( int index, CFrames* frame )
{
	Frames[index] = frame;
}

void CAnimation::AddFrame( CFrames* frame )
{
	Frames.push_back(frame);
}

void CAnimation::AddFrame( float AnchorPointX,	float AnchorPointY,	RECT RenderRect, RECT ActiveRect, RECT CollisionRect,
						  std::string Event, float Duration)
{
	CFrames frame;
	frame.SetAnchorPoint(AnchorPointX, AnchorPointY);
	frame.SetRenderRect(RenderRect);
	frame.SetActiveRect(ActiveRect);
	frame.SetCollisionRect(CollisionRect);
	frame.SetEvent(Event);
	frame.SetDuration(Duration);
	Frames.push_back(&frame);
}

void CAnimation::AddFrame( float AnchorPointX,	float AnchorPointY,	
						  int RenderRectLeft,    int RenderRectTop, 	int RenderRectRight, 	int RenderRectBottom, 
						  int ActiveRectLeft,    int ActiveRectTop, 	int ActiveRectRight, 	int ActiveRectBottom, 
						  int CollisionRectLeft, int CollisionRectTop,	int CollisionRectRight, int CollisionRectBottom,
						  std::string Event, float Duration)
{
	CFrames frame;
	frame.SetAnchorPoint(AnchorPointX, AnchorPointY);
	frame.SetRenderRect( RenderRectLeft, RenderRectTop, RenderRectRight, RenderRectBottom );
	frame.SetActiveRect( ActiveRectLeft, ActiveRectTop, ActiveRectRight, ActiveRectBottom );
	frame.SetCollisionRect( CollisionRectLeft, CollisionRectTop, CollisionRectRight, CollisionRectBottom );
	frame.SetEvent(Event);
	frame.SetDuration(Duration);
	Frames.push_back(&frame);
}
