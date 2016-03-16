/************************************************************
File:		Emitter.cpp
Author:		Roberto Crespo
Purpose:	CEmitter class Creates the Emitter with the particles in each of them.
************************************************************/

#include "Emitter.h"
#include <time.h>
#include <sstream>
CEmitter::CEmitter(void)
{

	m_nImageID							= 0;

	m_nWidth                           = 0;
	m_nHeight                          = 0;
	m_nSpreadValue						= 0;
	m_nAngularOffset					= 0; 
	m_nMaxParticle						= 0;

	m_nMinLife							= 0.0f;
	m_nMaxLife							= 0.0f;
	m_nGravityX						= 0.0f;
	m_nGravityY						= 0.0f;
	m_nCurrRotation					= 0.0f;
	m_nStartingRotation				= 0.0f;
	m_nEndingRotation					= 0.0f;
	m_nStartingAlpha					= 0.0f;
	m_nEndingAlpha						= 0.0f;
	m_nStartingColorR					= 0.0f;
	m_nEndingColorR					= 0.0f;
	m_nStartingColorG					= 0.0f;
	m_nEndingColorG					= 0.0f;
	m_nStartingColorB					= 0.0f;
	m_nEndingColorB					= 0.0f;
	m_nSpawnRate						= 0.0f;
	m_nStartingScaleX					= 0.0f;
	m_nEndingScaleX					= 0.0f;
	m_nStartingScaleY					= 0.0f;
	m_nEndingScaleY					= 0.0f;							
	m_fEmitterPosX						= 0.0f;
	m_fEmitterPosY						= 0.0f;
	m_fStartingAccelerationX			= 0.0f;
	m_fEndingAccelerationX				= 0.0f;
	m_fStartingAccelerationY			= 0.0f;
	m_fEndingAccelerationY				= 0.0f;
	m_fStartingVelocityX				= 0.0f;
	m_fEndingVelocityX					= 0.0f;
	m_fStartingVelocityY				= 0.0f;
	m_fEndingVelocityY					= 0.0f;
	m_fEmitterRangeX					= 0.0f;
	m_fEmitterRangeY					= 0.0f;
	m_fTimerBucket						= 0;
	m_bLooping							= false;

}


CEmitter::~CEmitter(void)
{
	for( unsigned int i = 0; i < m_vParticle.size(); i++)
	{
		delete m_vParticle[i];
		m_vParticle[i] = nullptr;
	}

}

CEmitter::CEmitter( const CEmitter& rhs )
{
	m_nImageID							= rhs.m_nImageID;
	m_nMinLife							= rhs.m_nMinLife;
	m_nMaxLife							= rhs.m_nMaxLife;
	m_nWidth                           = rhs.m_nWidth;
	m_nHeight                          = rhs.m_nHeight;
	m_nGravityX						= rhs.m_nGravityX;
	m_nGravityY						= rhs.m_nGravityY;
	m_nCurrRotation					= rhs.m_nCurrRotation;
	m_nStartingRotation				= rhs.m_nStartingRotation;
	m_nEndingRotation					= rhs.m_nEndingRotation;
	m_nSpreadValue						= rhs.m_nSpreadValue;
	m_nAngularOffset					= rhs.m_nAngularOffset;
	m_nStartingAlpha					= rhs.m_nStartingAlpha;
	m_nEndingAlpha						= rhs.m_nEndingAlpha;
	m_nStartingColorR					= rhs.m_nStartingColorR;
	m_nEndingColorR					= rhs.m_nEndingColorR;
	m_nStartingColorG					= rhs.m_nStartingColorG;
	m_nEndingColorG					= rhs.m_nEndingColorG;
	m_nStartingColorB					= rhs.m_nStartingColorB;
	m_nEndingColorB					= rhs.m_nEndingColorB;
	m_nSpawnRate						= rhs.m_nSpawnRate;
	m_nMaxParticle						= rhs.m_nMaxParticle;
	m_nStartingScaleX					= rhs.m_nStartingScaleX;
	m_nEndingScaleX					= rhs.m_nEndingScaleX;
	m_nStartingScaleY					= rhs.m_nStartingScaleY;
	m_nEndingScaleY					= rhs.m_nEndingScaleY;

	m_fEmitterPosX						= rhs.m_fEmitterPosX;
	m_fEmitterPosY						= rhs.m_fEmitterPosY;
	m_fStartingAccelerationX			= rhs.m_fStartingAccelerationX;
	m_fEndingAccelerationX				= rhs.m_fEndingAccelerationX;
	m_fStartingAccelerationY			= rhs.m_fStartingAccelerationY;
	m_fEndingAccelerationY				= rhs.m_fEndingAccelerationY;
	m_fStartingVelocityX				= rhs.m_fStartingVelocityX;
	m_fEndingVelocityX					= rhs.m_fEndingVelocityX;
	m_fStartingVelocityY				= rhs.m_fStartingVelocityY;
	m_fEndingVelocityY					= rhs.m_fEndingVelocityY;
	m_fEmitterRangeX					= rhs.m_fEmitterRangeX;
	m_fEmitterRangeY					= rhs.m_fEmitterRangeY;

	m_bLooping							= rhs.m_bLooping;

}

CEmitter& CEmitter::operator=( const CEmitter& rhs)
{
	//Check
	if( this != &rhs )
	{
		m_nImageID							= rhs.m_nImageID;
		m_nMinLife							= rhs.m_nMinLife;
		m_nMaxLife							= rhs.m_nMaxLife;
		m_nWidth                           = rhs.m_nWidth;
		m_nHeight                          = rhs.m_nHeight;
		m_nGravityX						= rhs.m_nGravityX;
		m_nGravityY						= rhs.m_nGravityY;
		m_nCurrRotation					= rhs.m_nCurrRotation;
		m_nStartingRotation				= rhs.m_nStartingRotation;
		m_nEndingRotation					= rhs.m_nEndingRotation;
		m_nSpreadValue						= rhs.m_nSpreadValue;
		m_nAngularOffset					= rhs.m_nAngularOffset;
		m_nStartingAlpha					= rhs.m_nStartingAlpha;
		m_nEndingAlpha						= rhs.m_nEndingAlpha;
		m_nStartingColorR					= rhs.m_nStartingColorR;
		m_nEndingColorR					= rhs.m_nEndingColorR;
		m_nStartingColorG					= rhs.m_nStartingColorG;
		m_nEndingColorG					= rhs.m_nEndingColorG;
		m_nStartingColorB					= rhs.m_nStartingColorB;
		m_nEndingColorB					= rhs.m_nEndingColorB;
		m_nSpawnRate						= rhs.m_nSpawnRate;
		m_nMaxParticle						= rhs.m_nMaxParticle;
		m_nStartingScaleX					= rhs.m_nStartingScaleX;
		m_nEndingScaleX					= rhs.m_nEndingScaleX;
		m_nStartingScaleY					= rhs.m_nStartingScaleY;
		m_nEndingScaleY					= rhs.m_nEndingScaleY;

		m_fEmitterPosX						= rhs.m_fEmitterPosX;
		m_fEmitterPosY						= rhs.m_fEmitterPosY;
		m_fStartingAccelerationX			= rhs.m_fStartingAccelerationX;
		m_fEndingAccelerationX				= rhs.m_fEndingAccelerationX;
		m_fStartingAccelerationY			= rhs.m_fStartingAccelerationY;
		m_fEndingAccelerationY				= rhs.m_fEndingAccelerationY;
		m_fStartingVelocityX				= rhs.m_fStartingVelocityX;
		m_fEndingVelocityX					= rhs.m_fEndingVelocityX;
		m_fStartingVelocityY				= rhs.m_fStartingVelocityY;
		m_fEndingVelocityY					= rhs.m_fEndingVelocityY;
		m_fEmitterRangeX					= rhs.m_fEmitterRangeX;
		m_fEmitterRangeY					= rhs.m_fEmitterRangeY;

		m_bLooping							= rhs.m_bLooping;
	}

	//Return
	return *this;
}

void CEmitter::Update(float  fElaspedTime)
{
	//int I = IMin + rand() % (IMax - IMin);



	m_fTimerBucket += fElaspedTime;
	//float n = GetSpawnRate();
	for(unsigned int i = 0; i < m_vParticle.size(); ++i)						
	{
		if(this->GetIsLoop() == true)						
		{
			if(m_fTimerBucket >= GetSpawnRate())
			{
				if(m_vParticle[i]->GetIsAlive() == false)
				{
					m_vParticle[i]->SetIsAlive(true);
					m_fTimerBucket = 0;
					continue;

				}
			}
		}

		//Life Adjustor
		//Check to see if the currentlife is maxed
		m_vParticle[i]->SetCurrLife(GetParticles()[i]->GetCurrLife() + fElaspedTime );
		if(m_vParticle[i]->GetCurrLife() >= m_vParticle[i]->GetMaxLife() )
		{

			m_vParticle[i]->SetCurrLife( m_vParticle[i]->GetMaxLife());
			m_vParticle[i]->SetIsAlive(false);

		}






		if(m_vParticle[i]->GetIsAlive() == true)
		{

			//Alter Velocity
			m_vParticle[i]->SetCurrVelocityX( m_vParticle[i]->GetCurrVelocityX() + m_vParticle[i]->GetCurrAccelerationX() * fElaspedTime);  
			m_vParticle[i]->SetCurrVelocityY( m_vParticle[i]->GetCurrVelocityY() + m_vParticle[i]->GetCurrAccelerationY() * fElaspedTime);

			////Alter Position
			m_vParticle[i]->SetParticlePosX( m_vParticle[i]->GetParticlePosX() + (m_vParticle[i]->GetCurrVelocityX() + this->GetGravityX())  *  fElaspedTime );
			m_vParticle[i]->SetParticlePosY( m_vParticle[i]->GetParticlePosY() + ( m_vParticle[i]->GetCurrVelocityY() - this->GetGravityY())  * fElaspedTime );	

			////Alter Rotation
			m_vParticle[i]->SetCurrRotation( ( GetEndingRotation() - GetStartingRotation() ) * ( m_vParticle[i]->GetCurrLife() / m_vParticle[i]->GetMaxLife() ) + GetStartingRotation() );

			////Alter Scale
			m_vParticle[i]->SetCurrScaleX( (GetEndingScaleX() - GetStartingScaleX() ) * (m_vParticle[i]->GetCurrLife() / m_vParticle[i]->GetMaxLife() ) + GetStartingScaleX() );
			m_vParticle[i]->SetCurrScaleY( (GetEndingScaleY() - GetStartingScaleY()) * (m_vParticle[i]->GetCurrLife() /  m_vParticle[i]->GetMaxLife() ) + GetStartingScaleY() );

			////Alter Color
			m_vParticle[i]->SetCurrColorA( ( GetEndingAlpha()  - GetStartingAlpha()  )  *(m_vParticle[i]->GetCurrLife() / m_vParticle[i]->GetMaxLife() ) + GetStartingAlpha() );
			m_vParticle[i]->SetCurrColorR( ( GetEndingColorR() - GetStartingColorR() ) * (m_vParticle[i]->GetCurrLife() / m_vParticle[i]->GetMaxLife() ) + GetStartingColorR() );
			m_vParticle[i]->SetCurrColorG( ( GetEndingColorG() - GetStartingColorG() ) * (m_vParticle[i]->GetCurrLife() / m_vParticle[i]->GetMaxLife() ) + GetStartingColorG() );
			m_vParticle[i]->SetCurrColorB( ( GetEndingColorB() - GetStartingColorB() ) * (m_vParticle[i]->GetCurrLife() / m_vParticle[i]->GetMaxLife() ) + GetStartingColorB() );

		}
		if(m_vParticle[i]->GetIsAlive() == false)
		{
			m_vParticle[i]->SetCurrLife(0);

			int min = (int)(GetMinLife()*100);
			int max = (int)(GetMaxLife()*100);

			float random = (min+rand()%(max - min))/100.0f;

			m_vParticle[i]->SetMaxLife(random );



			//Velocity
			/////////////////////////////////////////////////
			//Set  CurrVelX
			int velxmin = (int)(GetStartingVelocityX()*100);
			int velxmax = (int)(GetEndingVelocityX()*100);
			float randommvx = (velxmin + rand()%(velxmax - velxmin))/100.0f;
			m_vParticle[i]->SetCurrVelocityX(randommvx);
			//Set CurrVelY
			int velymin = (int)(GetStartingVelocityY()*100);
			int velymax = (int)(GetEndingVelocityY()*100);
			float randommvy = (velymin + rand()%(velymax - velymin))/100.0f;
			m_vParticle[i]->SetCurrVelocityY(randommvy);


			//Acceleration
			/////////////////////////////////////////////////
			//Set  CurrAccelerationX
			int accxmin = (int)(GetStartingAccelerationX()*100);
			int accxmax = (int)(GetEndingAccelerationX()*100);
			float randommax = (accxmin + rand()%(accxmax - accxmin))/100.0f;
			m_vParticle[i]->SetCurrAccelerationX(randommax);
			//Set CurrAccelerationY
			int accymin = (int)(GetStartingAccelerationY()*100);
			int accymax = (int)(GetEndingAccelerationY()*100);
			float randommay = (accymin + rand()%(accymax - accymin))/100.0f;
			m_vParticle[i]->SetCurrAccelerationY(randommay);

			//std::ostringstream output;

			//output << randommax << ", " << randommay << "\n";
			//OutputDebugStringA(output.str().c_str());

			//////////////////////////////////////////////////




			//Rotation
			//////////////////////////////////
			int rots = (int)(GetStartingRotation()*100);
			int rote = (int)(GetEndingRotation()*100);

			float randomr = (rots+rand()%(rote - rots))/100.0f;

			m_vParticle[i]->SetCurrRotation(randomr );
			///////////////////////////////////////////



			//Position
			/////////////////////////////////////////////////////////////
			int rangex = (int)(GetEmitterRangeX()*100);
			int rangey = (int)(GetEmitterRangeY()*100);
			//PosX
			if(rangex != 0)
			{
				float randposx = (rand()/ (float)RAND_MAX)*GetEmitterRangeX();
				m_vParticle[i]->SetParticlePosX(randposx + GetEmitterPosX());
			}
			else
				m_vParticle[i]->SetParticlePosX(GetEmitterPosX());

			//PosY
			if(rangey != 0)
			{
				float randposy = (rand()/ (float)RAND_MAX)*GetEmitterRangeY();
				m_vParticle[i]->SetParticlePosY(randposy + GetEmitterPosY());
			}
			else
				m_vParticle[i]->SetParticlePosY(GetEmitterPosY());

			/////////////////////////////////////////////////////////////////////



			////Alter Rotation
			m_vParticle[i]->SetCurrRotation( ( GetEndingRotation() - GetStartingRotation() ) * ( m_vParticle[i]->GetCurrLife() / m_vParticle[i]->GetMaxLife() ) + GetStartingRotation() );

			////Alter Scale
			m_vParticle[i]->SetCurrScaleX( (GetEndingScaleX() - GetStartingScaleX() ) * (m_vParticle[i]->GetCurrLife() / m_vParticle[i]->GetMaxLife() ) + GetStartingScaleX() );
			m_vParticle[i]->SetCurrScaleY( (GetEndingScaleY() - GetStartingScaleY()) * (m_vParticle[i]->GetCurrLife() /  m_vParticle[i]->GetMaxLife() ) + GetStartingScaleY() );

			////Alter Color
			m_vParticle[i]->SetCurrColorA( ( GetEndingAlpha()  - GetStartingAlpha()  )  *(m_vParticle[i]->GetCurrLife() /m_vParticle[i]->GetMaxLife() ) + GetStartingAlpha() );
			m_vParticle[i]->SetCurrColorR( ( GetEndingColorR() - GetStartingColorR() ) * (m_vParticle[i]->GetCurrLife() /m_vParticle[i]->GetMaxLife() ) + GetStartingColorR() );
			m_vParticle[i]->SetCurrColorG( ( GetEndingColorG() - GetStartingColorG() ) * (m_vParticle[i]->GetCurrLife() /m_vParticle[i]->GetMaxLife() ) + GetStartingColorG() );
			m_vParticle[i]->SetCurrColorB( ( GetEndingColorB() - GetStartingColorB() ) * (m_vParticle[i]->GetCurrLife() /m_vParticle[i]->GetMaxLife() ) + GetStartingColorB() );


		}

	}


}







void CEmitter::SetupParticle( /*CEmitter em*/ )
{
	for(int i = 0; i < this->GetMaxParticle() ;++i)
	{	

		CParticle* temp = new CParticle();


		//Set m_bAlive;
		temp->SetIsAlive(false);

		//Set CurrLife
		///////////////////////////////////////////////////
		/*if(this->GetMinLife() >= this->GetMaxLife())
		{
		m_vParticle[i]->SetCurrLife(GetMaxLife());

		}
		else
		{*/
		int min = (int)(GetMinLife()*100);
		int max = (int)(GetMaxLife()*100);

		float random = (min+rand()%(max - min))/100.0f;

		temp->SetCurrLife(0);
		temp->SetMaxLife(random );


		//}
		////////////////////////////////////////////////


		//Roation
		////////////////////////////////////////////////////
		int rots = (int)(GetStartingRotation()*100);
		int rote = (int)(GetEndingRotation()*100);

		float randomr = (rots+rand()%(rote - rots))/100.0f;

		temp->SetCurrRotation(randomr );
		/////////////////////////////////////////////////////

		//Acceleration
		///////////////////////////////////////////////////
		//Set  CurrAccelerationX
		int accxmin = (int)(GetStartingAccelerationX()*100);
		int accxmax = (int)(GetEndingAccelerationX()*100);
		float randommax = (accxmin + rand()%(accxmax - accxmin))/100.0f;
		temp->SetCurrAccelerationX(randommax);
		//Set CurrAccelerationY
		int accymin = (int)(GetStartingAccelerationY()*100);
		int accymax = (int)(GetEndingAccelerationY()*100);
		float randommay = (accymin + rand()%(accymax - accymin))/100.0f;
		temp->SetCurrAccelerationY(randommay);

		//////////////////////////////////////////////////


		//Setting Position
		///////////////////////////////////////////////////

		int rangex = (int)(GetEmitterRangeX()*100);
		int rangey = (int)(GetEmitterRangeY()*100);
		//PosX
		if(rangex != 0)
		{
			float randposx = (rand()/ (float)RAND_MAX)*GetEmitterRangeX();
			temp->SetParticlePosX(randposx + GetEmitterPosX());
		}
		else
			temp->SetParticlePosX(GetEmitterPosX());

		//PosY
		if(rangey != 0)
		{
			float randposy = (rand()/ (float)RAND_MAX)*GetEmitterRangeY();
			temp->SetParticlePosY(randposy + GetEmitterPosY());
		}
		else
			temp->SetParticlePosY(GetEmitterPosY());

		//////////////////////////////////////////////////



		//Set Color
		temp->SetCurrColorA(GetStartingAlpha());
		temp->SetCurrColorR(GetStartingColorR());
		temp->SetCurrColorG(GetStartingColorG());
		temp->SetCurrColorB(GetStartingColorB());




		//Add the particle to the vector
		m_vParticle.push_back(temp);
	}

}

void CEmitter::KillParticles()
{
	for(unsigned int i = 0; i < this->m_vParticle.size(); i++)
		m_vParticle[i]->SetCurrLife(m_vParticle[i]->GetMaxLife() + 1);
}
