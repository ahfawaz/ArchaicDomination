/************************************************************
	File:		Particle.cpp
	Author:		Roberto Crespo
	Purpose:	CParticle class Assigns the Current Status of Each Particle.
************************************************************/

#include "Particle.h"


CParticle::CParticle(void)
{
	m_nCurrLife				= 0;
	m_nMaxLife				= 0;
	m_nCurrRotation			= 0;
	m_nCurrScaleX			= 0;
	m_nCurrScaleY			= 0;
	m_nCurrColorA           = 0;
	m_nCurrColorR           = 0;
	m_nCurrColorG           = 0;
	m_nCurrColorB           = 0;
	m_fParticlePosX			= 0.0f;
	m_fParticlePosY			= 0.0f;
	m_fCurrAccelerationX	= 0.0f;
	m_fCurrAccelerationY	= 0.0f;
	m_fCurrVelocityX		= 0.0f;
	m_fCurrVelocityY		= 0.0f;
	
	 m_bAlive				= false;
}


CParticle::~CParticle(void)
{
}

CParticle::CParticle(const CParticle& rhs)
{
	m_nCurrLife				      = rhs.m_nCurrLife;
	m_nMaxLife					  = rhs.m_nMaxLife;
	m_nCurrRotation				  = rhs.m_nCurrRotation;
	m_nCurrScaleX				  = rhs.m_nCurrScaleX;
	m_nCurrScaleY				  = rhs.m_nCurrScaleY;
	m_nCurrColorA				  = rhs.m_nCurrColorA;
	m_nCurrColorR				  = rhs.m_nCurrColorR;
	m_nCurrColorG				  = rhs.m_nCurrColorG;
	m_nCurrColorB				  = rhs.m_nCurrColorB;

    m_fParticlePosX			      = rhs.m_fParticlePosX;
    m_fParticlePosY			      = rhs.m_fParticlePosY;
    m_fCurrAccelerationX		  = rhs.m_fCurrAccelerationX;
    m_fCurrAccelerationY		  = rhs.m_fCurrAccelerationY;
    m_fCurrVelocityX		      = rhs.m_fCurrVelocityX;
    m_fCurrVelocityY		      = rhs.m_fCurrVelocityX;
   							  
    m_bAlive					  =	rhs.m_bAlive;
}

CParticle& CParticle::operator=( const CParticle& rhs)
{
	//Check
	if( this != &rhs )
	{
	m_nCurrLife				      = rhs.m_nCurrLife;
	m_nMaxLife					  = rhs.m_nMaxLife;
	m_nCurrRotation				  = rhs.m_nCurrRotation;
	m_nCurrScaleX				  = rhs.m_nCurrScaleX;
	m_nCurrScaleY				  = rhs.m_nCurrScaleY;
	m_nCurrColorA				  = rhs.m_nCurrColorA;
	m_nCurrColorR				  = rhs.m_nCurrColorR;
	m_nCurrColorG				  = rhs.m_nCurrColorG;
	m_nCurrColorB				  = rhs.m_nCurrColorB;

    m_fParticlePosX			      = rhs.m_fParticlePosX;
    m_fParticlePosY			      = rhs.m_fParticlePosY;
    m_fCurrAccelerationX		  = rhs.m_fCurrAccelerationX;
    m_fCurrAccelerationY		  = rhs.m_fCurrAccelerationY;
    m_fCurrVelocityX		      = rhs.m_fCurrVelocityX;
    m_fCurrVelocityY		      = rhs.m_fCurrVelocityX;
   							  
    m_bAlive					  =	rhs.m_bAlive;
	}

	//Return
	return *this;
}
