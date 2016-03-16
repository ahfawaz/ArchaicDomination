/************************************************************
	File:		Emitter.h
	Author:		Roberto Crespo
	Purpose:	CEmitter class Creates the Emitter with the particles in each of them.
************************************************************/

#pragma once
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "Particle.h"
#include <vector>
using namespace std;

class CEmitter
{
private:
	
	vector<CParticle*>m_vParticle;
	int m_nImageID;
	int m_nWidth;
	int m_nHeight;
	int m_nSpreadValue;
	int m_nAngularOffset;
    int m_nMaxParticle;

	float m_nMinLife;
	float m_nMaxLife;
	float m_nGravityX;
	float m_nGravityY;
	float m_nCurrRotation;
	float m_nStartingRotation;
	float m_nEndingRotation;
	float m_nStartingAlpha;
	float m_nEndingAlpha;
	float m_nStartingColorR;
	float m_nEndingColorR;
	float m_nStartingColorG;
	float m_nEndingColorG;
	float m_nStartingColorB;
	float m_nEndingColorB;
	float m_nSpawnRate;
	float m_nStartingScaleX;
	float m_nEndingScaleX;
	float m_nStartingScaleY;
	float m_nEndingScaleY;
    float m_fEmitterPosX;
	float m_fEmitterPosY;
	float m_fStartingAccelerationX;
	float m_fEndingAccelerationX;
	float m_fStartingAccelerationY;
	float m_fEndingAccelerationY;
	float m_fStartingVelocityX;
	float m_fEndingVelocityX;
	float m_fStartingVelocityY;
	float m_fEndingVelocityY;
	float m_fEmitterRangeX;
	float m_fEmitterRangeY;

	bool m_bLooping;
	
	float m_fTimerBucket;

public:

	CEmitter(void);
	~CEmitter(void);
	CEmitter& operator=( const CEmitter& rhs);
	CEmitter( const CEmitter& rhs );
	void Update(float  fElaspedTime);
	vector <CParticle*>& GetParticles() { return m_vParticle; }



	//Accessor
	int       GetImageID				( void )	const	 {  return m_nImageID					;}
	
	int       GetWidth                  ( void )    const    {  return m_nWidth                     ;}
	int       GetHeight                 ( void )    const    {  return m_nHeight                    ;}
	int       GetSpreadValue			( void )	const	 {  return m_nSpreadValue				;}
	int       GetAngularOffset			( void )	const	 {  return m_nAngularOffset				;}
	int       GetMaxParticle			( void )	const	 {  return m_nMaxParticle				;}

	float     GetMinLife				( void )	const	 {  return m_nMinLife					;}
	float     GetMaxLife				( void )	const	 {  return m_nMaxLife					;}
	float     GetGravityX				( void )	const	 {  return m_nGravityX					;}
	float     GetGravityY				( void )	const	 {  return m_nGravityY					;}
	float     GetCurrRotation			( void )	const	 {  return m_nCurrRotation				;}
	float     GetStartingRotation		( void )	const	 {  return m_nStartingRotation			;}
	float     GetEndingRotation			( void )	const	 {  return m_nEndingRotation			;}
	float     GetStartingAlpha			( void )	const	 {  return m_nStartingAlpha				;}
	float     GetEndingAlpha			( void )	const	 {  return m_nEndingAlpha				;}
	float     GetStartingColorR			( void )	const	 {  return m_nStartingColorR			;}
	float     GetEndingColorR			( void )	const	 {  return m_nEndingColorR				;}
	float     GetStartingColorG			( void )	const	 {  return m_nStartingColorG			;}
	float     GetEndingColorG			( void )	const	 {  return m_nEndingColorG				;}
	float     GetStartingColorB			( void )	const	 {  return m_nStartingColorB			;}
	float     GetEndingColorB			( void )	const	 {  return m_nEndingColorB				;}
	float     GetSpawnRate				( void )	const	 {  return m_nSpawnRate					;}
	float	  GetStartingScaleX			( void )	const	 {  return m_nStartingScaleX			;}
	float	  GetEndingScaleX			( void )	const	 {  return m_nEndingScaleX				;}
	float	  GetStartingScaleY			( void )	const	 {  return m_nStartingScaleY			;}
	float	  GetEndingScaleY			( void )	const	 {  return m_nEndingScaleY				;}
									  	   				    
	float GetEmitterPosX			( void )	const	 {  return m_fEmitterPosX				;}
	float GetEmitterPosY			( void )	const	 {  return m_fEmitterPosY				;}
	float GetStartingAccelerationX	( void )	const	 {  return m_fStartingAccelerationX		;}
	float GetEndingAccelerationX	( void )	const	 {  return m_fEndingAccelerationX		;}
	float GetStartingAccelerationY	( void )	const	 {  return m_fStartingAccelerationY		;}
	float GetEndingAccelerationY	( void )	const	 {  return m_fEndingAccelerationY		;}
	float GetStartingVelocityX		( void )	const	 {  return m_fStartingVelocityX			;}
	float GetEndingVelocityX		( void )	const	 {  return m_fEndingVelocityX			;}
	float GetStartingVelocityY		( void )	const	 {  return m_fStartingVelocityY			;}
	float GetEndingVelocityY		( void )	const	 {  return m_fEndingVelocityY			;}
	float GetEmitterRangeX          ( void )	const    {  return m_fEmitterRangeX				;}										    
	float GetEmitterRangeY          ( void )	const    {  return m_fEmitterRangeY				;}	

	bool  GetIsLoop					( void )	const	 {  return m_bLooping					;}




	//Mutator
	//void SetMaxParticle( int mp ) { m_nMaxParticle = mp; }
	void SetImageID					( int id    )		 {  m_nImageID					= id	;}
	void SetWidth					( int w     )        {  m_nWidth                    = w     ;}
	void SetHeight					( int h     )        {  m_nHeight                   = h     ;}
	void SetSpreadValue				( int sv    )		 {  m_nSpreadValue				= sv	;}
	void SetAngularOffset			( int ao    )		 {  m_nAngularOffset			= ao	;}
	void SetMaxParticle				( int mp    )		 {  m_nMaxParticle				= mp	;}

	void SetMinLife					( float minl)		 {  m_nMinLife					= minl	;}
	void SetMaxLife					( float maxl)		 {  m_nMaxLife					= maxl	;}
	void SetGravityX				( float gx  )		 {  m_nGravityX					= gx	;}
	void SetGravityY				( float gy  )		 {  m_nGravityY					= gy	;}
	void SetCurrRotation			( float cr  )		 {  m_nCurrRotation				= cr	;}
	void SetStartingRotation		( float sr  )		 {  m_nStartingRotation			= sr	;}
	void SetEndingRotation			( float er  )		 {  m_nEndingRotation			= er	;}
	void SetStartingAlpha			( float sa  )		 {  m_nStartingAlpha			= sa	;}
	void SetEndingAlpha				( float ea  )		 {  m_nEndingAlpha				= ea	;}
	void SetStartingColorR			( float scr )		 {  m_nStartingColorR			= scr	;}
	void SetEndingColorR			( float ecr )		 {  m_nEndingColorR				= ecr	;}
	void SetStartingColorG			( float scg )		 {  m_nStartingColorG			= scg	;}
	void SetEndingColorG			( float ecg )		 {  m_nEndingColorG				= ecg	;}
	void SetStartingColorB			( float scb )		 {  m_nStartingColorB			= scb	;}
	void SetEndingColorB			( float ecb )		 {  m_nEndingColorB				= ecb	;}
	void SetSpawnRate				( float spr )		 {  m_nSpawnRate				= spr	;}
	void SetStartingScaleX			( float ssx )		 {  m_nStartingScaleX			= ssx	;}
	void SetEndingScaleX			( float esx )		 {  m_nEndingScaleX				= esx	;}
	void SetStartingScaleY			( float ssy )		 {  m_nStartingScaleY			= ssy	;}
	void SetEndingScaleY			( float esy )		 {  m_nEndingScaleY				= esy	;}					  	   
	void SetEmitterPosX				( float px  )		 {  m_fEmitterPosX				= px	;}
	void SetEmitterPosY				( float py  )		 {  m_fEmitterPosY				= py	;}
	void SetStartingAccelerationX	( float sax )		 {  m_fStartingAccelerationX	= sax	;}
	void SetEndingAccelerationX		( float eax )		 {  m_fEndingAccelerationX		= eax	;}
	void SetStartingAccelerationY	( float say )		 {  m_fStartingAccelerationY	= say	;}
	void SetEndingAccelerationY		( float eay )		 {  m_fEndingAccelerationY		= eay	;}
	void SetStartingVelocityX		( float svx )		 {  m_fStartingVelocityX		= svx	;}
	void SetEndingVelocityX			( float evx )		 {  m_fEndingVelocityX			= evx	;}
	void SetStartingVelocityY		( float svy )		 {  m_fStartingVelocityY		= svy	;}
	void SetEndingVelocityY			( float evy )		 {  m_fEndingVelocityY			= evy	;}
    void SetEmitterRangeX           ( float erx     )	 {   m_fEmitterRangeX			= erx	;}										    
	void SetEmitterRangeY           ( float ery     )	 {   m_fEmitterRangeY			= ery	;}	

	void SetIsLoop					( bool l    )		 {  m_bLooping					= l		;}

	
	void SetParticle( CParticle* id ) { m_vParticle.push_back(id); }

	//Set Particles
	void SetupParticle( /*CEmitter em*/ );

	void KillParticles();
};
