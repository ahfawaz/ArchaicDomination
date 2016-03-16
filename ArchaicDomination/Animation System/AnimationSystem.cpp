/*
==================================================
|	File:		AnimationSystem.cpp
|	Author:		Ahmad Fawaz (gamer3780@gmail.com)
|   Team:		Iron Clad
|	Course:		SGP (Structures of Game Production)
|	Purpose:	CAnimationSystem runs the animation
==================================================
*/

#include "AnimationSystem.h"
#include "AnimTimeStamp.h"
#include "Frames.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_EventSystem.h"

#include <cassert>

#define TEST_FILE "Resources/XMLFiles/XmlEditorTestSave.xml"
#define FILE_DEST_XML "../Resource/XML/"
#define FILE_DEST_SPRITE "../Resource/Graphics/"

CAnimationSystem::CAnimationSystem(void)
{
}


CAnimationSystem::~CAnimationSystem(void)
{
	for (int i = 0; i < (int)animeNames.size(); i++)
	{
		CSGD_TextureManager::GetInstance()->UnloadTexture(animation[animeNames[i]].GetImageID());
		animation[animeNames[i]].SetImageID(-1);

		animation[animeNames[i]].ClearFrames(); 
	}
}

bool CAnimationSystem::LoadedAnimation( const char* FileName)
{

	TiXmlDocument doc;

	//Checking to see if the file is valid.
	assert( doc.LoadFile(FileName) != false && "CAnimationSystem::LoadedAnimation could not load an XML file." && doc.ErrorDesc() );
	if(doc.LoadFile(FileName) == false)
	{
		doc.Clear();
		return false;		
	}

	TiXmlElement* pRoot = doc.RootElement();
	TiXmlElement* element = pRoot->FirstChildElement("Animation");
	while(element != nullptr)
	{
		CAnimation newAnimation;
		std::string animName = element->Attribute("AnimationName");
		animeNames.push_back(animName);

		std::string image = element->Attribute("ImageID");
		TCHAR path[200];
		mbstowcs_s(nullptr,path, 200, image.c_str(), _TRUNCATE);
		int id = CSGD_TextureManager::GetInstance()->LoadTexture(path);
		newAnimation.SetImageID(id);
		newAnimation.SetImageFile(image);

		int numOfFrames;
		element->Attribute("numFrames", &numOfFrames);
		newAnimation.SetNumOfFrames(numOfFrames);

		std::string textInfo = element->Attribute("isLooping");
		if(textInfo == "true")
			newAnimation.SetIsLooping(true);
		else
			newAnimation.SetIsLooping(false);

		TiXmlElement* pFrame = element->FirstChildElement("Frame");
		for (int i = 0; i < numOfFrames; i++, pFrame = pFrame->NextSiblingElement())
		{
			CFrames* frame = new CFrames;

			//Loading frame Anchorpoints, Duration and Event Name
			double Anchor, duration;
			pFrame->Attribute("AnchorPointX", &Anchor);
			frame->SetAnchorPointX((float)Anchor);

			pFrame->Attribute("AnchorPointY", &Anchor);
			frame->SetAnchorPointY((float)Anchor);

			pFrame->Attribute("Duration", &duration);
			frame->SetDuration((float)duration);

			textInfo.clear();
			textInfo = pFrame->Attribute("Event");
			frame->SetEvent(textInfo);

			//RECT source for rendering, active, collision
			int left, top, right, bottom;
			pFrame->Attribute("RenderRectLeft", &left);
			pFrame->Attribute("RenderRectTop", &top);
			pFrame->Attribute("RenderRectRight", &right);
			pFrame->Attribute("RenderRectBottom", &bottom);
			frame->SetRenderRect(left,top,right,bottom);

			pFrame->Attribute("ActiveRectLeft", &left);
			pFrame->Attribute("ActiveRectTop", &top);
			pFrame->Attribute("ActiveRectRight", &right);
			pFrame->Attribute("ActiveRectBottom", &bottom);
			frame->SetActiveRect(left,top,right,bottom);

			pFrame->Attribute("CollisionRectLeft", &left);
			pFrame->Attribute("CollisionRectTop", &top);
			pFrame->Attribute("CollisionRectRight", &right);
			pFrame->Attribute("CollisionRectBottom", &bottom);
			frame->SetCollisionRect(left,top,right,bottom);

			newAnimation.AddFrame(frame);
		}

		animation.insert(std::pair<std::string, CAnimation>(animName,  newAnimation));
		element = element->NextSiblingElement();
	}
	doc.Clear();
	return true;
}

void CAnimationSystem::Render(CAnimTimeStamp* timeStamp, int X, int Y)
{

	if(animation[timeStamp->GetCurrAnimation()].GetNumOfFrames() > timeStamp->GetCurrFrame())
	{
		float anchorX, anchorY;
		animation[timeStamp->GetCurrAnimation()].GetFrame(timeStamp->GetCurrFrame())->GetAnchorPoint(anchorX,anchorY);
		RECT renderSource = animation[timeStamp->GetCurrAnimation()].GetFrame(timeStamp->GetCurrFrame())->GetRenderRect();
		int posX = (X + 16) - (int)((anchorX - renderSource.left));
		int posY = (Y + 16) - (int)((anchorY - renderSource.top));
		CSGD_TextureManager::GetInstance()->Draw(animation[timeStamp->GetCurrAnimation()].GetImageID(),posX,posY,1.0,1.0,&animation[timeStamp->GetCurrAnimation()].GetFrame(timeStamp->GetCurrFrame())->GetRenderRect());
	}

}

void CAnimationSystem::Update(CAnimTimeStamp* timeStamp, float Dt)
{
	if(animation[timeStamp->GetCurrAnimation()].GetNumOfFrames() <= (timeStamp->GetCurrFrame()) && animation[timeStamp->GetCurrAnimation()].GetIsLooping() == false)
		return;

	if(timeStamp->GetTimeOnFrame() >= animation[timeStamp->GetCurrAnimation()].GetFrame(timeStamp->GetCurrFrame())->GetDuration())
	{
		timeStamp->SetCurrFrame(timeStamp->GetCurrFrame() + 1);
		if(animation[timeStamp->GetCurrAnimation()].GetNumOfFrames() <= timeStamp->GetCurrFrame() && animation[timeStamp->GetCurrAnimation()].GetIsLooping())
		{
			timeStamp->SetCurrFrame(0);
		}
		timeStamp->SetTimeOnFrame(0.0f);

		if(animation[timeStamp->GetCurrAnimation()].GetNumOfFrames() > (timeStamp->GetCurrFrame()) && animation[timeStamp->GetCurrAnimation()].GetFrame(timeStamp->GetCurrFrame())->GetEvent().empty() != true)
			PlayEvent(animation[timeStamp->GetCurrAnimation()].GetFrame(timeStamp->GetCurrFrame())->GetEvent());
	}

	timeStamp->SetTimeOnFrame(timeStamp->GetTimeOnFrame() + Dt);
}

void CAnimationSystem::PlayEvent(std::string eventName)
{
	if(eventName == "Step")
	{
	}
}

/*virtual*/ void CAnimationSystem::HandleEvent( const CEvent* pEvent )
{

}