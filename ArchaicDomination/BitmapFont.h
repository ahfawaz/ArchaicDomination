////////////////////////////////////////////////////
// File:	CBitmapFont.h
// Purpose:	Loads in the bitmap font to be used by 
//			Other classes throughout the project
//
// Author:	Joshua Ford (joshua.c.ford@gmail.com)
// Course:	SGP 1310
////////////////////////////////////////////////////

#pragma once
#include <Windows.h>
#include "SGD Wrappers\SGD_String.h"
#include <vector>

//class CAnimation;
class CLetters;

class CBitmapFont
{
public:
	CBitmapFont(void);
	~CBitmapFont(void);

	void		Initialize	( void );
	void		Terminate	( void );
	void		Draw		( const TCHAR* OutputText, int X, int Y, float Scale , DWORD Color );

	bool		LoadFiles	( std::vector<CLetters*> TempLetters );

private:
	
	int			ImageID;
	int			CharWidth;
	int			CharHeight;
	TCHAR		FirstChar;
	float		PositionX;
	float		PositionY;
	int NumRows;
	int NumCols;
	//CAnimation* m_cAnimation;

	RECT CellAlgorithm( int id ) const;

	std::vector<CLetters*>			m_cCharacters;


	bool m_bOnlyUpperCasse;

};

