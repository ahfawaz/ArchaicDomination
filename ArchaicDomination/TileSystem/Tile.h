#pragma once
#pragma once

enum TileType		{TREE, MOUNTAIN, WATER, NONE};

class CTile
{

private:

	bool				m_bIsOccupied;
	int					m_nPosX;
	int					m_nPosY;
	const char*			m_cImageID;
	const char*			m_cObj;
	int					m_nWidth;
	int					m_nHeight;
	int					m_nLocX;
	int					m_nLocY;
	


public:
	CTile(void);


	CTile(int X, int Y, int W, int H, const char* ID, const char* O,int lX, int lY, int T);
	virtual ~CTile(void);
	

	
	bool GetOccupied()			{return m_bIsOccupied;}
	int GetHeight()				{return m_nHeight;}
	int GetWidth()				{return m_nWidth;}
	int GetPosX()				{return m_nPosX;}
	int GetPosY()				{return m_nPosY;}
	const char* GetImageID()	{return m_cImageID;}
	const char* GetObj()		{return m_cObj;}
	int GetLocX()				{return m_nLocX;}
	int GetLocY()				{return m_nLocY;}
	int GetType(void)			{return m_eType;}

	void SetOccupied(bool o)	{m_bIsOccupied = o;}
	void SetHeight(int h)		{m_nHeight = h;}
	void SetWidth(int w)		{m_nWidth = w;}
	void SetPosX(int x)			{m_nPosX = x;}
	void SetPosY(int y)			{m_nPosY = y;}	
	void SetLocX(int x)			{m_nLocX = x;}
	void SetLocY(int y)			{m_nLocY = y;}


private:
	TileType		m_eType;

};

