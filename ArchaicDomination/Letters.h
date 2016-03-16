#pragma once
class CLetters
{
public:
	CLetters(void);
	~CLetters(void);


	void		SetPosX				( unsigned short TempX )			{ PosX = TempX; }
	void		SetPosY				( unsigned short TempY )			{ PosY = TempY; }
	void		SetCharWidth		( unsigned short TempCharWidth )	{ CharWidth = TempCharWidth; }
	void		SetCharHeight		( unsigned short TempCharHeight )	{ CharHeight = TempCharHeight; }		
	void		SetYOffset			( unsigned short TempYoffset )		{ YCharOffset = TempYoffset; }	
	void		SetXCharAdvance		( unsigned short TempXcharadvance )	{ XCharAdvance = TempXcharadvance; }
	void		SetCharPage			( unsigned short TempCharPage )		{ CharPage = TempCharPage; }
	void		SetCharLineHeight	( unsigned short TempCharLineHeight){ CharLineHeight = TempCharLineHeight; }
	void		SetCharBase			( unsigned short TempCharBase )		{ CharBase = TempCharBase; }
	void		SetScaleWidth		( unsigned short TempScaleWidth )	{ ScaleWidth = TempScaleWidth; } 
	void		SetScaleHeight		( unsigned short TempScaleHeight )  { ScaleHeight = TempScaleHeight; }
	void		SetCharID			( unsigned short TempCharID )		{ CharID = TempCharID; }


	unsigned short		GetPosX				( void )		{ return PosX; }
	unsigned short		GetPosY				( void )		{ return PosY; }
	unsigned short		GetCharWidth		( void )		{ return CharWidth; }
	unsigned short		GetCharHeight		( void )		{ return CharHeight; }		
	unsigned short		GetYOffset			( void )		{ return YCharOffset; }	
	unsigned short		GetXCharAdvance		( void )		{ return XCharAdvance; }
	unsigned short		GetCharPage			( void )		{ return CharPage; }
	unsigned short		GetCharLineHeight	( void )		{ return CharLineHeight; }
	unsigned short		GetCharBase			( void )		{ return CharBase; }
	unsigned short		GetScaleWidth		( void )		{ return ScaleWidth; } 
	unsigned short		GetScaleHeight		( void )		{ return ScaleHeight; }
	unsigned short		GetCharID			( void )		{ return CharID; }




private:
	

	unsigned short CharID;
	unsigned short PosX, PosY;
	unsigned short CharWidth, CharHeight;
	unsigned short YCharOffset;
	unsigned short XCharAdvance;
	unsigned short CharPage;
	unsigned short CharLineHeight;
	unsigned short CharBase;
	unsigned short ScaleWidth, ScaleHeight;
	

};







