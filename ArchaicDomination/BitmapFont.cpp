////////////////////////////////////////////////////
// File:	CBitmapFont.cpp
// Purpose:	Loads in the bitmap font to be used by 
//			Other classes throughout the project
//
// Author:	Joshua Ford (joshua.c.ford@gmail.com)
// Course:	SGP 1310
////////////////////////////////////////////////////


#include "BitmapFont.h"
#include "SGD Wrappers/CSGD_TextureManager.h"
#include <string>
#include <ostream>
#include <fstream>
#include <iostream>
#include <istream>
#include "Letters.h"



CBitmapFont::CBitmapFont(void)
{
	ImageID		= -1;
	
	NumRows		= 1;
	NumCols		= 1;
	CharWidth	= 1;
	CharHeight	= 1;
	
	FirstChar	= 0;
	
	m_bOnlyUpperCasse = false;
	
}


CBitmapFont::~CBitmapFont(void)
{
}


void CBitmapFont::Initialize	( void )
{
	// Load the font
	ImageID		= CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/Gamefont_0.tga"), D3DCOLOR_XRGB( 0, 0 , 0)  );
	
	LoadFiles( m_cCharacters );


	NumRows		= 6;
	NumCols		= 10;
	CharWidth	= 32;
	CharHeight	= 32;
	
	FirstChar	= _T(' ');	// space
	

	m_bOnlyUpperCasse = true;
}
void CBitmapFont::Terminate	( void )
{
	CSGD_TextureManager::GetInstance()->UnloadTexture( ImageID );
	ImageID = -1;

	for (unsigned int i = 0; i < m_cCharacters.size(); i++)
	{
		delete m_cCharacters[i];
	}
}
void CBitmapFont::Draw ( const TCHAR* OutputText, int X, int Y, float Scale , DWORD Color )
{
	
	


	// Store the starting x position (for newlines)
	int nColStart = X;
	

	// Iterate through the string
	for( int i = 0; OutputText[ i ]; i++ )
	{
		// Get the character
		TCHAR ch = OutputText[ i ];
		int count = 0;
		for (unsigned int i = 0; i < m_cCharacters.size(); i++)
		{
			if (ch == m_cCharacters[i]->GetCharID())
			{
				count = i;
				break;
			}
		}

		// Handle whitespace
		if( ch == _T(' ') )
		{
			// Move to the next character
			X += (int)(m_cCharacters[count]->GetCharWidthW() * Scale) ;
			continue;
		}
		else if( ch == _T('\n') )
		{
			// Move down a row & reset the x to the start
			Y += (int)(CharHeight * Scale);
			X = nColStart;
			continue;
		}
		else if( ch == _T('\t') )
		{
			// Calculate how many characters have been printed (on this line)
			int diff = (X - nColStart) / (int)(CharWidth * Scale);

			// Calculate the space to add for a 4-character alignment
			int tab = 4 - (diff % 4);

			// Move 
			X += tab * (int)(CharWidth * Scale);
			continue;
		}


		// Convert to uppercase?
//		if( m_bOnlyUpperCasse == true )
//			ch = TCTOUPPER( ch );

		



		// Calculate the source rect
		RECT rCell = CellAlgorithm( ch );

		int yoffset = ((m_cCharacters[count]->GetYOffset() * Scale) - (int)(m_cCharacters[count]->GetYOffset() * Scale) >0.5) ? (int)(m_cCharacters[count]->GetYOffset() * Scale)+1 : (int)(m_cCharacters[count]->GetYOffset() * Scale);

		// Draw the character
		CSGD_TextureManager::GetInstance()->Draw( ImageID, X , Y + yoffset, Scale, Scale, &rCell, 0.0f, 0.0f, 0.0f, Color );

		//(int)(CharWidth * Scale)
		// Move to the next character
		X += (int)(m_cCharacters[count]->GetXCharAdvance() * Scale  ) ;
	}
	
}


	RECT CBitmapFont::CellAlgorithm( int id ) const
{
	int count = 0;
	for (unsigned int i = 0; i < m_cCharacters.size(); i++)
	{
		if (id == m_cCharacters[i]->GetCharID())
		{
			count = i;
			break;
		}
	}

	RECT rSource = { };

	rSource.left	= m_cCharacters[count]->GetPosX();
	rSource.top		= m_cCharacters[count]->GetPosY();

	rSource.right	= rSource.left + m_cCharacters[count]->GetCharWidthW();
	rSource.bottom	= rSource.top  + m_cCharacters[count]->GetCharHeight();

	return rSource;
}

bool CBitmapFont::LoadFiles( std::vector<CLetters*> TempLetters )
{

	
	std::ifstream ifs;
	ifs.open( "Resources/Graphics/Gamefont.fnt" );
	std::istream& Stream();
	std::string Line;

	
	std::string Read, Key, Value;
	std::size_t i;
	
	while( !ifs.eof() )
	{
		std::stringstream LineStream;
		std::getline( ifs, Line );
		LineStream << Line;

		unsigned short temp;

		
		//read the line's type
		LineStream >> Read;
		if( Read == "common" )
		{
			//this holds common data
			while( !LineStream.eof() )
			{
				std::stringstream Converter;
				LineStream >> Read;
				i = Read.find( '=' );
				Key = Read.substr( 0, i );
				Value = Read.substr( i + 1 );

				//assign the correct value
				Converter << Value;
				if( Key == "lineHeight" )
				{
					Converter >> temp;
				}
				else if( Key == "base" )
				{
					Converter >> temp;
				}
				else if( Key == "scaleW" )
				{
					Converter >> temp;
					
				}
				else if( Key == "scaleH" )
				{
					Converter >> temp;
					
				}
				else if( Key == "pages" )
				{
					Converter >> temp;
					
				}
			}
			
		}
		else if( Read == "char" )
		{
			
			CLetters* TempLetter = new CLetters( );
			while( !LineStream.eof() )
			{
				std::stringstream Converter;
				LineStream >> Read;
				i = Read.find( '=' );
				Key = Read.substr( 0, i );
				Value = Read.substr( i + 1 );

				//assign the correct value
				Converter << Value;
				if( Key == "id" )
				{
					Converter >> temp;
					TempLetter->SetCharID( temp );
				}
				else if( Key == "x" )
				{
					Converter >> temp;
					TempLetter->SetPosX( temp );
				}
				else if( Key == "y" )
				{
					Converter >> temp;
					TempLetter->SetPosY( temp );
				}
				else if( Key == "width" )
				{
					Converter >> temp;
					TempLetter->SetCharWidth( temp );
				}
				else if( Key == "height" )
				{
					Converter >> temp;
					TempLetter->SetCharHeight( temp );
				}
				else if( Key == "xoffset" )
				{
					Converter >> temp;
				}
				else if( Key == "yoffset" )
				{
					Converter >> temp;
					TempLetter->SetYOffset( temp );
				}
				else if( Key == "xadvance" )
				{
					Converter >> temp;
					TempLetter->SetXCharAdvance( temp );
				}
				else if( Key == "page" )
				{
					Converter >> temp;
					TempLetter->SetCharPage( temp );
					
				}
				
			}
			m_cCharacters.push_back( TempLetter );
		}
		
		
		
		
		
	}
	//delete TempLetter;
	return true;
}
