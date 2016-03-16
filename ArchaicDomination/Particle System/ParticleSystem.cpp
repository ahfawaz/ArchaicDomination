/************************************************************
	File:		ParticleSystem.cpp
	Author:		Roberto Crespo
	Purpose:	CParticleSystem class loads the Emitters and Renders them in the Game.
************************************************************/

#include "ParticleSystem.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"
#include "../SGD Wrappers/SGD_String.h"
#include "../TinyXML/tinyxml.h"
#include <cmath>
using namespace std;

#define TM CSGD_TextureManager::GetInstance()
#define D3 CSGD_Direct3D::GetInstance()

CParticleSystem::CParticleSystem(void)
{


}


CParticleSystem::~CParticleSystem(void)
{
	//TM->UnloadTexture(m_mLoaded["FireBall"].GetImageID());

	for ( unsigned int i = 0; i < m_vActive.size(); ++i )
	{
		//TM->UnloadTexture(m_vActive[i]->GetImageID());
		delete m_vActive[i];
		m_vActive[i] = nullptr;
	}

}
void CParticleSystem::Update (float fElaspedTime)
{
	
	for( unsigned int i = 0; i < m_vActive.size(); i++ )
	{
		m_vActive[i]->Update(fElaspedTime);

	}
	
}
void CParticleSystem::Render()
{

	for(unsigned int i = 0; i < m_vActive.size(); i++ )
	{
		
			for( unsigned int j =0; j <  m_vActive[i]->GetParticles().size(); ++j)
			{
				CParticle* p = m_vActive[i]->GetParticles()[j] ;
				if( p->GetIsAlive() ==  true)
				{
					RECT rect = {0,0,m_vActive[i]->GetWidth(),m_vActive[i]->GetHeight()};
					//Render the particles
				TM->Draw( 
					    m_vActive[i]->GetImageID(),
						(int)(p->GetParticlePosX() - m_vActive[i]->GetWidth()/2 * p->GetCurrScaleX()),
						(int)(p->GetParticlePosY() - m_vActive[i]->GetHeight()/2 * p->GetCurrScaleY()),
						p->GetCurrScaleX(),
						p->GetCurrScaleY(),
						&rect,
						(float)(m_vActive[i]->GetWidth()/2) /** m_vActive[i]->GetParticles()[j]->GetCurrScaleX()*/,
						(float)(m_vActive[i]->GetHeight()/2) /** m_vActive[i]->GetParticles()[j]->GetCurrScaleY()*/,
						m_vActive[i]->GetParticles()[j]->GetCurrRotation(),
						D3DCOLOR_ARGB(255/*(int)m_vActive[i]->GetParticles()[j]->GetCurrColorA()*/,(int)(p->GetCurrColorR()),(int)(p->GetCurrColorG()),(int)(p->GetCurrColorB()))
						);
				}
			}
	}
}
		


bool CParticleSystem::LoadEffect( string name,const char* filename )
{
	//Create EmitterInfo Storage
	EmitterInfo info;

	// Create a TinyXML document
	TiXmlDocument doc;

	// Attempt to load from the file
	if( doc.LoadFile( filename ) == false )
		return false;

	// Access the root element
	TiXmlElement* pRoot = doc.RootElement();	

	if( pRoot == nullptr )
		return false;

	// Find the Emiiter Element
	TiXmlElement* pEmitter = pRoot->FirstChildElement( "Emitter" );
	
	while( pEmitter != nullptr )
	{
		//Create Emitter
		CEmitter m_pEmitter; 

		const char* temp1 = pEmitter->Attribute( "ImageID" );

		if( temp1 != nullptr )
		{
			wchar_t* temp2 = new wchar_t[ strlen(temp1) + 1 ];
			
			//When the updated SGD wrapper is installed, get rid of the * 2 
			CSTR_TO_TSTR (temp2, ((strlen(temp1) + 1) * 2), temp1);
			
			 m_pEmitter.SetImageID(TM->LoadTexture( temp2 ) );
			delete [] temp2;
		}

		/////////////////////////////////////////////////////////bool
		//const char* tempb1 = pEmitter->Attribute( "Looping" );

		//if( tempb1 != nullptr )
		//{
		//	wchar_t* tempb2 = new wchar_t[ strlen(tempb1) + 1 ];

		//	//When the updated SGD wrapper is installed, get rid of the * 2 
		//	CSTR_TO_TSTR (tempb2, ((strlen(tempb1) + 1) * 2), tempb1);
		//	wchar_t* name = "true";
		//	if(tempb2 == name)
		//	{
		//m_pEmitter.SetIsLoop(true);

		//	}
		//else
		//	m_pEmitter.SetIsLoop(false);
		//	// m_pEmitter.SetImageID(TM->LoadTexture( tempb2 ) );
		//	delete [] tempb2;
		//}

		//std::string image = pEmitter->Attribute("ImageID");

		//Read Attributes
		pEmitter->Attribute("EmitterPosX",&info.EmitterPosX);
		pEmitter->Attribute("EmitterPosY",&info.EmitterPosY);
		pEmitter->Attribute("Width",&info.Width);
		pEmitter->Attribute("Height",&info.Height);
		pEmitter->Attribute("MinLife",&info.MinLife);
		pEmitter->Attribute("MaxLife",&info.MaxLife);
		pEmitter->Attribute("GravityX",&info.GravityX);
		pEmitter->Attribute("GravityY",&info.GravityY);
		pEmitter->Attribute("CurrRotation",&info.CurrRotation);
		pEmitter->Attribute("StartingRotation",&info.StartingRotation);
		pEmitter->Attribute("EndingRotation",&info.EndingRotation);
		pEmitter->Attribute("SpreadValue",&info.SpreadValue);
		pEmitter->Attribute("AngularOffset",&info.AngularOffset);
		pEmitter->Attribute("StartingAlpha",&info.StartingAlpha);
		pEmitter->Attribute("EndingAlpha",&info.EndingAlpha);
		pEmitter->Attribute("StartingColorR",&info.StartingColorR);
		pEmitter->Attribute("EndingColorR",&info.EndingColorR);
		pEmitter->Attribute("StartingColorG",&info.StartingColorG);
		pEmitter->Attribute("EndingColorG",&info.EndingColorG);
		pEmitter->Attribute("StartingColorB",&info.StartingColorB);
		pEmitter->Attribute("EndingColorB",&info.EndingColorB);
		//pEmitter->Attribute("Looping",&info.Looping);
		pEmitter->Attribute("SpawnRate",&info.SpawnRate);
		pEmitter->Attribute("MaxParticle",&info.MaxParticle);
		pEmitter->Attribute("StartingAccelerationX",&info.StartingAccelerationX);
		pEmitter->Attribute("EndingAccelerationX",&info.EndingAccelerationX);
		pEmitter->Attribute("StartingAccelerationY",&info.StartingAccelerationY);
		pEmitter->Attribute("EndingAccelerationY",&info.EndingAccelerationY);
		pEmitter->Attribute("StartingVelocityX",&info.StartingVelocityX);
		pEmitter->Attribute("EndingVelocityX",&info.EndingVelocityX);
		pEmitter->Attribute("StartingVelocityY",&info.StartingVelocityY);
		pEmitter->Attribute("EndingVelocityY",&info.EndingVelocityY);
		pEmitter->Attribute("StartingScaleX",&info.StartingScaleX);
		pEmitter->Attribute("EndingScaleX",&info.EndingScaleX);
		pEmitter->Attribute("StartingScaleY",&info.StartingScaleY);
		pEmitter->Attribute("EndingScaleY",&info.EndingScaleY);
		pEmitter->Attribute("EmitterRangeX", &info.EmitterRangeX);
		pEmitter->Attribute("EmitterRangeY", &info.EmitterRangeY);

		
		
		//store into vector
		m_pEmitter.SetMinLife				        ((float)info.MinLife);
		m_pEmitter.SetMaxLife					    ((float)info.MaxLife);
		m_pEmitter.SetWidth							(info.Width);
		m_pEmitter.SetHeight					    (info.Height);	
		m_pEmitter.SetGravityX					    ((float)info.GravityX);
		m_pEmitter.SetGravityY					    ((float)info.GravityY);
		m_pEmitter.SetCurrRotation					((float)info.CurrRotation);
		m_pEmitter.SetStartingRotation				((float)info.StartingRotation);
		m_pEmitter.SetEndingRotation				((float)info.EndingRotation);	
		m_pEmitter.SetSpreadValue					(info.SpreadValue);
		m_pEmitter.SetAngularOffset					(info.AngularOffset);
		m_pEmitter.SetStartingAlpha					((float)info.StartingAlpha);
		m_pEmitter.SetEndingAlpha					((float)info.EndingAlpha);
		m_pEmitter.SetStartingColorR				((float)info.StartingColorR);	
		m_pEmitter.SetEndingColorR					((float)info.EndingColorR);
		m_pEmitter.SetStartingColorG				((float)info.StartingColorG);	
		m_pEmitter.SetEndingColorG					((float)info.EndingColorG);
		m_pEmitter.SetStartingColorB				((float)info.StartingColorB);	
		m_pEmitter.SetEndingColorB					((float)info.EndingColorB);
		m_pEmitter.SetSpawnRate						((float)info.SpawnRate);
		m_pEmitter.SetMaxParticle					(info.MaxParticle);
		m_pEmitter.SetStartingScaleX				((float)info.StartingScaleX);	
		m_pEmitter.SetEndingScaleX					((float)info.EndingScaleX);
		m_pEmitter.SetStartingScaleY				((float)info.StartingScaleY);	
		m_pEmitter.SetEndingScaleY					((float)info.EndingScaleY);
		m_pEmitter.SetEmitterPosX					((float)info.EmitterPosX);	
		m_pEmitter.SetEmitterPosY					((float)info.EmitterPosY);
		m_pEmitter.SetStartingAccelerationX			((float)info.StartingAccelerationX);
		m_pEmitter.SetEndingAccelerationX			((float)info.EndingAccelerationX);
		m_pEmitter.SetStartingAccelerationY			((float)info.StartingAccelerationY);
		m_pEmitter.SetEndingAccelerationY			((float)info.EndingAccelerationY);
		m_pEmitter.SetStartingVelocityX				((float)info.StartingVelocityX);
		m_pEmitter.SetEndingVelocityX				((float)info.EndingVelocityX);
		m_pEmitter.SetStartingVelocityY				((float)info.StartingAccelerationY);
		m_pEmitter.SetEndingVelocityY				((float)info.EndingAccelerationY);
		m_pEmitter.SetEmitterRangeX					((float)info.EmitterRangeX);
		m_pEmitter.SetEmitterRangeY					((float)info.EmitterRangeY);

		m_pEmitter.SetIsLoop						(true/*info.Looping*/);	
				  

		

		//m_vActive.push_back(m_pEmitter);
		m_mLoaded.insert(pair<string,CEmitter>(name,m_pEmitter));
		//Move to the next node
		pEmitter = pEmitter->NextSiblingElement( "Emitter" );
	}

	// Did we load anything?
	if( m_mLoaded.size() > 0 )
		return true;

	return false;
}														  	

CEmitter* CParticleSystem::CreateEmitter(string name)
{

	CEmitter* ToAdd = new CEmitter();
	*ToAdd = m_mLoaded[name];
	m_vActive.push_back(ToAdd);
	ToAdd->SetupParticle();

	return ToAdd;
}