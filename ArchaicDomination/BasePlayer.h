#pragma once
#include "BaseUnit.h"
#include <vector>

enum PlayerType{PLAYER, ENEMY, BASE};


class CBasePlayer
{
public:
	CBasePlayer(void);
	virtual ~CBasePlayer(void);

	virtual std::vector<CBaseUnit*> GetUnits(void);
	virtual bool Input(void);
	void Render(void);
	void Update(void);
	virtual PlayerType GetType(void) {return PlayerType::BASE;}



	void				SetGeneralImageID		( int ImageID )		{ m_nGeneralImageID = ImageID; }
	void				SetPosX					( float X )			{ m_fPosX = X; }
	void				SetPosY					( float Y )			{ m_fPosY = Y; }

	int					GetGeneralImageID		( void )			{ return m_nGeneralImageID; }
	float				GetPosX					( void )			{ return m_fPosX; }
	float				GetPosY					( void )			{ return m_fPosY; }

	//Resource mutators
	void				SetSteel				( int Steel )		{ m_nSteel = Steel; }
	void				SetWood					( int Wood )		{ m_nWood = Wood; }
	void				SetFood					( int Food )		{ m_nFood = Food; }

	//Resource Accessors
	int					GetSteel				( void )			{ return m_nSteel; }
	int					GetWood					( void )			{ return m_nWood; }
	int					GetFood					( void )			{ return m_nFood; }


	int	GetCounterForRemovingBuildings() { return CounterForRemovingBuildings; }

	void  SetCounterForRemovingBuildings( int Counter )   { CounterForRemovingBuildings = Counter; }
private:
	
	int			CounterForRemovingBuildings;

	float				m_fPosX;
	float				m_fPosY;
	int					m_nGeneralImageID;



	//resource variables

	int					m_nSteel;
	int					m_nWood;
	int					m_nFood;
};

