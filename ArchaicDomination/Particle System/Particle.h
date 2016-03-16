/************************************************************
	File:		Particle.h
	Author:		Roberto Crespo
	Purpose:	CParticle class Assigns the Current Status of Each Particle.
************************************************************/

#pragma once
#include "../SGD Wrappers/CSGD_Direct3D.h"

class CParticle
{
private:
	
	float  m_nCurrLife;
	float  m_nMaxLife;
	float  m_nCurrRotation;
	float  m_nCurrScaleX;
	float  m_nCurrScaleY;
	float  m_nCurrColorA;
	float  m_nCurrColorR;
	float  m_nCurrColorG;
	float  m_nCurrColorB;
	float m_fParticlePosX;
	float m_fParticlePosY;
	float m_fCurrAccelerationX;
	float m_fCurrAccelerationY;
	float m_fCurrVelocityX;
	float m_fCurrVelocityY;
	 
	bool  m_bAlive;



public:
	CParticle(void);
	~CParticle(void);
	CParticle& operator=( const CParticle& rhs);
	CParticle(const CParticle& rhs);

	//Accessor
	float	  GetCurrLife			( void )	const	{   return m_nCurrLife				;}
	float	  GetMaxLife			( void )	const	{   return m_nMaxLife				;}
	float	  GetCurrRotation		( void )	const	{   return m_nCurrRotation			;}
	float	  GetCurrScaleX			( void )	const	{   return m_nCurrScaleX			;}
	float	  GetCurrScaleY			( void )	const	{   return m_nCurrScaleY			;}
	float	  GetCurrColorA			( void )	const	{   return m_nCurrColorA			;}
	float	  GetCurrColorR			( void )	const	{   return m_nCurrColorR			;}
	float	  GetCurrColorG			( void )	const	{   return m_nCurrColorG			;}
	float	  GetCurrColorB			( void )	const	{   return m_nCurrColorB			;}
													    
	float GetParticlePosX		( void )	const	{   return m_fParticlePosX			;}
    float GetParticlePosY		( void )	const	{   return m_fParticlePosY			;}
    float GetCurrAccelerationX	( void )	const	{   return m_fCurrAccelerationX		;}
    float GetCurrAccelerationY	( void )	const	{   return m_fCurrAccelerationY		;}
    float GetCurrVelocityX		( void )	const	{   return m_fCurrVelocityX			;}
    float GetCurrVelocityY		( void )	const	{   return m_fCurrVelocityY			;}
													    
	bool  GetIsAlive			( void )	const	{   return m_bAlive					;}
	
	
	//Mutator
	void SetCurrLife			( float cl   )		{   m_nCurrLife				 = cl	;}
	void SetMaxLife				( float ml   )		{   m_nMaxLife				 = ml	;}
	void SetCurrRotation		( float cr   )		{   m_nCurrRotation			 = cr	;}
	void SetCurrScaleX			( float sx   )		{   m_nCurrScaleX			 = sx	;}
	void SetCurrScaleY			( float sy   )		{   m_nCurrScaleY			 = sy	;}
	void SetCurrColorA			( float ca   )		{   m_nCurrColorA			 = ca	;}
	void SetCurrColorR			( float cr   )		{   m_nCurrColorR			 = cr	;}
	void SetCurrColorG			( float cg   )		{   m_nCurrColorG			 = cg	;}
	void SetCurrColorB			( float cb   )		{   m_nCurrColorB			 = cb	;}

	void SetParticlePosX		( float px )		{   m_fParticlePosX			 = px	;}
    void SetParticlePosY		( float py )		{   m_fParticlePosY			 = py	;}
    void SetCurrAccelerationX	( float ax )		{   m_fCurrAccelerationX	 = ax	;}
    void SetCurrAccelerationY	( float ay )		{   m_fCurrAccelerationY	 = ay	;}
    void SetCurrVelocityX		( float vx )		{   m_fCurrVelocityX		 = vx	;}
    void SetCurrVelocityY		( float vy )		{   m_fCurrVelocityY		 = vy	;}
											   
	void SetIsAlive				( bool ia  )		{   m_bAlive				 = ia	;}




	
};
