////////////////////////////////////////////////////
// File:	CSaveGame.cpp
// Purpose:	Handles all the Loading information.
//			
//
// Author:	Roberto Crespo (Cresic95@fullsail.edu)
// Course:	SGP 1310
////////////////////////////////////////////////////
#include "SaveGame.h"
#include "SGD Wrappers\CSGD_XAudio2.h"
#include "SGD Wrappers\CSGD_TextureManager.h"
#include "SGD Wrappers\CSGD_DirectInput.h"
#include "SGD Wrappers\CSGD_Direct3D.h"
#include "SGD Wrappers\SGD_String.h"
#include "BitmapFont.h"
#include <fstream>
#include <iostream>
#include "Game.h"
#include "GamePlay.h"
#include "Enemy.h"
#include "Player.h"
#include "BaseBuilding.h"
#include "MainMenu.h"
#include "TinyXML\tinyxml.h"
#include "LoadGame.h"
#include <Shlobj.h>
#include <string>
#include <ostream>
#include "TileSystem\WorldGrid.h"

#define Black				D3DCOLOR_ARGB( 0, 0, 0, 0 )
#define White				D3DCOLOR_ARGB( 255, 255, 255, 255 )
#define White_Highlight		D3DCOLOR_ARGB( 50, 255, 255, 255 )
#define Red					D3DCOLOR_ARGB( 255, 255, 0,	0	)
#define Blue				D3DCOLOR_ARGB( 255, 0,	0,	255 )
#define Blue2				D3DCOLOR_ARGB( 255, 0,	51, 102 )
#define Blue3				D3DCOLOR_ARGB( 255, 18,	52,	86	)
#define Blue_Highlight		D3DCOLOR_ARGB( 100, 0,	51, 102 )
#define Player CPlayer::GetInstance()
#define Enemy CEnemy::GetInstance()
#define GamePlay CGamePlay::GetInstance()
#define PosY 170.0f;
#define PosX 400.0f;
#include "SGD Wrappers\CSGD_DirectInput.h"
#include "Game.h"
#include "MainMenu.h"



////////////////////////////////////////////////////
//Singleton
CSaveGame* CSaveGame::GetInstance( void )
{
	//Global Instance
	static CSaveGame s_Instance;
	return &s_Instance;
}






//Functions for every state
bool	CSaveGame::Input			( )					 
{
	CSGD_DirectInput*	pDI		=	CSGD_DirectInput::GetInstance();
	CSGD_Direct3D*		p3D		=	CSGD_Direct3D::GetInstance();
	if(IsUserInput == false)
	{
		if(   (pDI->KeyPressed( DIK_S )  || pDI->JoystickGetLStickDirPressed( DIR_DOWN, JOYSTICKID1 ) )  || ( pDI->KeyPressed( DIK_DOWN ) || pDI->JoystickGetLStickDirPressed( DIR_DOWN, JOYSTICKID2 )  ) )
		{
			++m_nCursor;


			// Stop at the bottom
			if( m_nCursor > 3 )		// EXIT
				m_nCursor = 0;
			CGame::GetInstance()->PlayMoveCursorSFX();
		}
		else if(  (pDI->KeyPressed( DIK_W )  || pDI->JoystickGetLStickDirPressed( DIR_UP, JOYSTICKID1 ) )  || ( pDI->KeyPressed( DIK_UP ) || pDI->JoystickGetLStickDirPressed( DIR_UP, JOYSTICKID2 )  ) )
		{
			--m_nCursor;


			// Stop at the top
			if( m_nCursor < 0 )		// PLAY
				m_nCursor = 3;
			CGame::GetInstance()->PlayMoveCursorSFX();
		}
	}
	else 
	{
		if(   (pDI->KeyPressed( DIK_S )  || pDI->JoystickGetLStickDirPressed( DIR_DOWN, JOYSTICKID1 ) )  || ( pDI->KeyPressed( DIK_DOWN ) || pDI->JoystickGetLStickDirPressed( DIR_DOWN, JOYSTICKID2 )  ) )
		{
			++m_nLetterCount;
			if( m_nLetterCount > 26 )		// PLAY
				m_nLetterCount = 0;
			CGame::GetInstance()->PlayMoveCursorSFX();

			if( ( pDI->KeyPressed( DIK_TAB ) || pDI->JoystickButtonPressed( 1, JOYSTICKID1 ) ) || (pDI->KeyPressed( DIK_RETURN ) || pDI->JoystickButtonPressed( 1, JOYSTICKID2 )  ) )

				// Stop at the bottom

					CGame::GetInstance()->PlayMoveCursorSFX();
		}
		else if(  (pDI->KeyPressed( DIK_W )  || pDI->JoystickGetLStickDirPressed( DIR_UP, JOYSTICKID1 ) )  || ( pDI->KeyPressed( DIK_UP ) || pDI->JoystickGetLStickDirPressed( DIR_UP, JOYSTICKID2 )  ) )
		{

			--m_nLetterCount;


			// Stop at the top
			if( m_nLetterCount < 0 )		// PLAY
				m_nLetterCount = 26;
			CGame::GetInstance()->PlayMoveCursorSFX();


		}







	}
	if( ( pDI->KeyPressed( DIK_TAB ) || pDI->JoystickButtonPressed( 1, JOYSTICKID1 ) ) || (pDI->KeyPressed( DIK_RETURN ) || pDI->JoystickButtonPressed( 1, JOYSTICKID2 )  ) )
	{
		// What selection?
		switch( m_nCursor )
		{

		case 0:
			{
				IsUserInput = true;
				overwriteslot1 = true;


				//tPlayerSaveInfo1 player1 = {"jason"};
				//vPlayer1[0] = player1;
				//SavePlayer1("Slot1.xml", vPlayer1 );


			}
			break;

		case 1:
			{

				IsUserInput = true;
				overwriteslot2 = true;
				//tPlayerSaveInfo2 player2 = { "hawk"};
				//vPlayer2[0] = player2;
				//SavePlayer2("Slot2.xml", vPlayer2 );
			}
			break;

		case 2:
			{
				IsUserInput = true;
				overwriteslot3 = true;
				// Default players
				//tPlayerSaveInfo3 player3 = { "boss"};

				//vPlayer3[0] = player3;
				//SavePlayer3("Slot3.xml", vPlayer3 );
			}
			break;
		case 3:
			{
				CGame::GetInstance()->PlayCancelSFX();
				CGame::GetInstance()->PopState();
				return true;
			}
			break;

		}

#pragma region AddingLetters

		switch( m_nLetterCount )
		{
		case 0:
			{
				if(m_nCursor == 0 && Namebuffer1.size() < 10  )
					Namebuffer1.push_back('A');
				else if(m_nCursor == 1 && Namebuffer2.size() < 10)
					Namebuffer2.push_back('A');
				else if(m_nCursor == 2 && Namebuffer3.size() < 10)
					Namebuffer3.push_back('A');

				m_nCap++;
			}
			break;
		case 1:
			{
				if(m_nCursor == 0 && Namebuffer1.size() < 10 )
					Namebuffer1.push_back('B');
				else if(m_nCursor == 1 && Namebuffer2.size() < 10 )	   
					Namebuffer2.push_back('B');
				else if(m_nCursor == 2 && Namebuffer3.size() < 10 )					   
					Namebuffer3.push_back('B');

				m_nCap++;
			}
			break;
		case 2:
			{
				if(m_nCursor == 0 && Namebuffer1.size() < 10  )
					Namebuffer1.push_back('C');
				else if(m_nCursor == 1 && Namebuffer2.size() < 10  )
					Namebuffer2.push_back('C');
				else if(m_nCursor == 2 && Namebuffer3.size() < 10  )
					Namebuffer3.push_back('C');

				m_nCap++;
			}
			break;
		case 3:
			{
				if(m_nCursor == 0 && Namebuffer1.size() < 10  )
					Namebuffer1.push_back('D');
				else if(m_nCursor == 1 && Namebuffer2.size() < 10  )
					Namebuffer2.push_back('D');
				else if(m_nCursor == 2 && Namebuffer3.size() < 10  )
					Namebuffer3.push_back('D');

				m_nCap++;
			}
			break;
		case 4:
			{
				if(m_nCursor == 0 && Namebuffer1.size() < 10  )
					Namebuffer1.push_back('E');
				else if(m_nCursor == 1 && Namebuffer2.size() < 10  )
					Namebuffer2.push_back('E');
				else if(m_nCursor == 2 && Namebuffer3.size() < 10  )
					Namebuffer3.push_back('E');

				m_nCap++;
			}
			break;
		case 5:
			{
				if(m_nCursor == 0 && Namebuffer1.size() < 10 )
					Namebuffer1.push_back('F');
				else if(m_nCursor == 1 && Namebuffer2.size() < 10 )
					Namebuffer2.push_back('F');
				else if(m_nCursor == 2 && Namebuffer3.size() < 10 )
					Namebuffer3.push_back('F');

				m_nCap++;
			}
			break;
		case 6:
			{
				if(m_nCursor == 0 && Namebuffer1.size() < 10 )
					Namebuffer1.push_back('G');
				else if(m_nCursor == 1 && Namebuffer2.size() < 10 )
					Namebuffer2.push_back('G');
				else if(m_nCursor == 2 && Namebuffer3.size() < 10 )
					Namebuffer3.push_back('G');

				m_nCap++;
			}
			break;
		case 7:
			{
				if(m_nCursor == 0 && Namebuffer1.size() < 10 )
					Namebuffer1.push_back('H');
				else if(m_nCursor == 1 && Namebuffer2.size() < 10 )
					Namebuffer2.push_back('H');
				else if(m_nCursor == 2 && Namebuffer3.size() < 10 )
					Namebuffer3.push_back('H');

				m_nCap++;
			}
			break;
		case 8:
			{
				if(m_nCursor == 0 && Namebuffer1.size() < 10 )
					Namebuffer1.push_back('I');
				else if(m_nCursor == 1 && Namebuffer2.size() < 10 )
					Namebuffer2.push_back('I');
				else if(m_nCursor == 2 && Namebuffer3.size() < 10 )
					Namebuffer3.push_back('I');

				m_nCap++;
			}
			break;
		case 9:
			{
				if(m_nCursor == 0 && Namebuffer1.size() < 10 )
					Namebuffer1.push_back('J');
				else if(m_nCursor == 1 && Namebuffer2.size() < 10 )
					Namebuffer2.push_back('J');
				else if(m_nCursor == 2 && Namebuffer3.size() < 10 )
					Namebuffer3.push_back('J');

				m_nCap++;
			}
			break;
		case 10:
			{
				if(m_nCursor == 0 && Namebuffer1.size() < 10 )
					Namebuffer1.push_back('K');
				else if(m_nCursor == 1 && Namebuffer2.size() < 10 )
					Namebuffer2.push_back('K');
				else if(m_nCursor == 2 && Namebuffer3.size() < 10 )
					Namebuffer3.push_back('K');

				m_nCap++;
			}
			break;
		case 11:
			{
				if(m_nCursor == 0 && Namebuffer1.size() < 10 )
					Namebuffer1.push_back('L');
				else if(m_nCursor == 1 && Namebuffer2.size() < 10 )
					Namebuffer2.push_back('L');
				else if(m_nCursor == 2 && Namebuffer3.size() < 10 )
					Namebuffer3.push_back('L');

				m_nCap++;
			}
			break;
		case 12:
			{
				if(m_nCursor == 0 && Namebuffer1.size() < 10 )
					Namebuffer1.push_back('M');
				else if(m_nCursor == 1 && Namebuffer2.size() < 10 )
					Namebuffer2.push_back('M');
				else if(m_nCursor == 2 && Namebuffer3.size() < 10 )
					Namebuffer3.push_back('M');

				m_nCap++;
			}
			break;
		case 13:
			{
				if(m_nCursor == 0 && Namebuffer1.size() < 10 )
					Namebuffer1.push_back('N');
				else if(m_nCursor == 1 && Namebuffer2.size() < 10 )
					Namebuffer2.push_back('N');
				else if(m_nCursor == 2 && Namebuffer3.size() < 10 )
					Namebuffer3.push_back('N');

				m_nCap++;
			}
			break;
		case 14:
			{
				if(m_nCursor == 0 && Namebuffer1.size() < 10 )
					Namebuffer1.push_back('O');
				else if(m_nCursor == 1 && Namebuffer2.size() < 10 )
					Namebuffer2.push_back('O');
				else if(m_nCursor == 2 && Namebuffer3.size() < 10 )
					Namebuffer3.push_back('O');

				m_nCap++;
			}
			break;
		case 15:
			{
				if(m_nCursor == 0 && Namebuffer1.size() < 10 )
					Namebuffer1.push_back('P');
				else if(m_nCursor == 1 && Namebuffer2.size() < 10 )
					Namebuffer2.push_back('P');
				else if(m_nCursor == 2 && Namebuffer3.size() < 10 )
					Namebuffer3.push_back('P');

				m_nCap++;
			}
			break;
		case 16:
			{
				if(m_nCursor == 0 && Namebuffer1.size() < 10 )
					Namebuffer1.push_back('Q');
				else if(m_nCursor == 1 && Namebuffer2.size() < 10 )
					Namebuffer2.push_back('Q');
				else if(m_nCursor == 2 && Namebuffer3.size() < 10 )
					Namebuffer3.push_back('Q');

				m_nCap++;
			}
			break;
		case 17:
			{
				if(m_nCursor == 0 && Namebuffer1.size() < 10 )
					Namebuffer1.push_back('R');
				else if(m_nCursor == 1 && Namebuffer2.size() < 10 )
					Namebuffer2.push_back('R');
				else if(m_nCursor == 2 && Namebuffer3.size() < 10 )
					Namebuffer3.push_back('R');

				m_nCap++;
			}
			break;
		case 18:
			{
				if(m_nCursor == 0 && Namebuffer1.size() < 10 )
					Namebuffer1.push_back('S');
				else if(m_nCursor == 1 && Namebuffer2.size() < 10 )
					Namebuffer2.push_back('S');
				else if(m_nCursor == 2 && Namebuffer3.size() < 10 )
					Namebuffer3.push_back('S');

				m_nCap++;
			}
			break;
		case 19:
			{
				if(m_nCursor == 0 && Namebuffer1.size() < 10 )
					Namebuffer1.push_back('T');
				else if(m_nCursor == 1 && Namebuffer2.size() < 10 )
					Namebuffer2.push_back('T');
				else if(m_nCursor == 2 && Namebuffer3.size() < 10 )
					Namebuffer3.push_back('T');

				m_nCap++;
			}
			break;
		case 20:
			{
				if(m_nCursor == 0 && Namebuffer1.size() < 10 )
					Namebuffer1.push_back('U');
				else if(m_nCursor == 1 && Namebuffer2.size() < 10 )
					Namebuffer2.push_back('U');
				else if(m_nCursor == 2 && Namebuffer3.size() < 10 )
					Namebuffer3.push_back('U');

				m_nCap++;
			}
			break;
		case 21:
			{
				if(m_nCursor == 0 && Namebuffer1.size() < 10 )
					Namebuffer1.push_back('V');
				else if(m_nCursor == 1 && Namebuffer2.size() < 10 )
					Namebuffer2.push_back('V');
				else if(m_nCursor == 2 && Namebuffer3.size() < 10 )
					Namebuffer3.push_back('V');

				m_nCap++;
			}
			break;
		case 22:
			{
				if(m_nCursor == 0 && Namebuffer1.size() < 10 )
					Namebuffer1.push_back('W');
				else if(m_nCursor == 1 && Namebuffer2.size() < 10 )
					Namebuffer2.push_back('W');
				else if(m_nCursor == 2 && Namebuffer3.size() < 10 )
					Namebuffer3.push_back('W');

				m_nCap++;
			}
			break;
		case 23:
			{
				if(m_nCursor == 0 && Namebuffer1.size() < 10 )
					Namebuffer1.push_back('X');
				else if(m_nCursor == 1 && Namebuffer2.size() < 10 )
					Namebuffer2.push_back('X');
				else if(m_nCursor == 2 && Namebuffer3.size() < 10 )
					Namebuffer3.push_back('X');

				m_nCap++;
			}
			break;
		case 24:
			{
				if(m_nCursor == 0 && Namebuffer1.size() < 10 )
					Namebuffer1.push_back('Y');
				else if(m_nCursor == 1 && Namebuffer2.size() < 10 )
					Namebuffer2.push_back('Y');
				else if(m_nCursor == 2 && Namebuffer3.size() < 10 )
					Namebuffer3.push_back('Y');

				m_nCap++;
			}
			break;
		case 25:
			{


				if(m_nCursor == 0 && Namebuffer1.size() < 10 )
					Namebuffer1.push_back('Z');
				else if(m_nCursor == 1 && Namebuffer2.size() < 10 )
					Namebuffer2.push_back('Z');
				else if(m_nCursor == 2 && Namebuffer3.size() < 10 )
					Namebuffer3.push_back('Z');

				m_nCap++;
			}
			break;
		case 26:
			{
				IsUserInput = false;

				if(m_nCursor == 0 )
				{
					tPlayerSaveInfo player;
					for(unsigned int i = 0;  i < Namebuffer1.size();++i)
					{
						player.szName[i] = Namebuffer1[i];

					}
					player.szName[Namebuffer1.size()] = '\0';
					vPlayers[0] = player;
					TCHAR * wszPath = NULL;

					HRESULT hr = SHGetKnownFolderPath( FOLDERID_LocalAppData, 0, 0, &wszPath);

					if(SUCCEEDED(hr))
					{
						std::wstring name = wszPath;
						name += L"\\ArchaicDomination\\Slots.xml";


						SavePlayers(name.c_str(), vPlayers, m_nCursor );
					}
				}
				else if(m_nCursor == 1  )
				{
					tPlayerSaveInfo player;
					for(unsigned int i = 0;  i < Namebuffer2.size();++i)
					{
						player.szName[i] = Namebuffer2[i];

					}
					player.szName[Namebuffer2.size()] = '\0';
					vPlayers[1] = player;
					TCHAR * wszPath = NULL;

					HRESULT hr = SHGetKnownFolderPath( FOLDERID_LocalAppData, 0, 0, &wszPath);

					if(SUCCEEDED(hr))
					{
						std::wstring name = wszPath;
						name += L"\\ArchaicDomination\\Slots.xml";


						SavePlayers(name.c_str(), vPlayers, m_nCursor );
					}
				}
				else if(m_nCursor == 2  )
				{
					tPlayerSaveInfo player;
					for(unsigned int i = 0;  i < Namebuffer3.size();++i)
					{
						player.szName[i] = Namebuffer3[i];

					}
					player.szName[Namebuffer3.size()] = '\0';
					vPlayers[2] = player;
					TCHAR * wszPath = NULL;

					HRESULT hr = SHGetKnownFolderPath( FOLDERID_LocalAppData, 0, 0, &wszPath);

					if(SUCCEEDED(hr))
					{
						std::wstring name = wszPath;
						name += L"\\ArchaicDomination\\Slots.xml";


						SavePlayers(name.c_str(), vPlayers, m_nCursor );
					}
				}
				m_nLetterCount = -1;
			}
			break;

		}
#pragma endregion

	}
	///Escape to exit
	if (pDI->JoystickButtonPressed( 3, 0 ) || pDI->JoystickButtonPressed( 3, 1 ) )
	{
		m_nCursor = 3;
		CGame::GetInstance()->PlayCancelSFX();
		return true;
	}
	if((pDI->KeyPressed( DIK_ESCAPE ) == true || pDI->JoystickButtonPressed( 2, JOYSTICKID1 ) == true) && IsUserInput == false)
	{

		m_nCursor = 3;
		CGame::GetInstance()->PlayCancelSFX();
		return true;
	}
	if ((pDI->KeyPressed( DIK_ESCAPE ) == true || pDI->JoystickButtonPressed( 2, JOYSTICKID1 ) == true) && IsUserInput == true)
	{

		//Namebuffer1.clear();	
		// Namebuffer2.clear();	
		//Namebuffer3.clear();
		m_nLetterCount = 26;
		CGame::GetInstance()->PlayCancelSFX();
		//IsUserInput = false;
		//m_nCursor = 3;
		//CGame::GetInstance()->PlayCancelSFX();
		return true;
	}


	// Keep running
	return true;


}
void	CSaveGame::Update			( float fElapsedTime ) 
{
}
void	CSaveGame::Render			( )			
{

	CSGD_Direct3D * m_D3D = CSGD_Direct3D::GetInstance();
	CSGD_TextureManager * pTM = CSGD_TextureManager::GetInstance();
	CBitmapFont* pFont = CGame::GetInstance()->GetFont();

	m_D3D->Clear(Black);
	pTM->Draw( m_nBackgroundImageID, 0, 0);


#pragma region saveslots
	for ( int i = 0; i < 3; i++)
	{
		RECT rSlots;

		rSlots.left		= 40;
		rSlots.top		= 140 + ( i * 120 );
		rSlots.right	= 760;
		rSlots.bottom	= 240 + ( i * 120 );

		//m_D3D->DrawRect( rSlots, Blue_Highlight );
		//m_D3D->DrawHollowRect( rSlots, White );


		pTM->Draw( m_nSlotImageID, 40, 140);

		pTM->Draw( m_nSlotImageID, 40, 260);

		pTM->Draw( m_nSlotImageID, 40, 380);

		pFont->Draw( _T("Save" ),340, 10,2.0f, Blue);




		//TOSTRINGSTREAM sSlot;
		//sSlot << "Slot " << i+1;
		//pFont->Draw(sSlot.str().c_str(), 42, 142 + (i * 120),1.0f, Red );


#pragma region Name of slots
		if(overwriteslot1 == false)
		{
			TOSTRINGSTREAM sname1;

			sname1 << vPlayers[0].szName;

			pFont->Draw(sname1.str().c_str(), 300+(i*16), 170 + (0),1.0f, White );

		}
		else
		{
			for(unsigned int i = 0; i < Namebuffer1.size(); i++)
			{

				TOSTRINGSTREAM sname1;

				sname1 << Namebuffer1[i];

				pFont->Draw(sname1.str().c_str(), 300+(i*16), 170 + (0),1.0f, White );
			}
		}
		if(overwriteslot2 == false)
		{
			TOSTRINGSTREAM sname2;

			sname2 << vPlayers[1].szName;

			pFont->Draw(sname2.str().c_str(), 300+(i*16), 170 + (120),1.0f, White );

		}
		else
		{
			for(unsigned int i = 0; i < Namebuffer2.size(); i++)
			{

				TOSTRINGSTREAM sname2;

				sname2 << Namebuffer2[i];

				pFont->Draw(sname2.str().c_str(), 300+(i*16), 170 + (120),1.0f, White );
			}
		}
		if(overwriteslot3 == false)
		{
			TOSTRINGSTREAM sname3;

			sname3 << vPlayers[2].szName;

			pFont->Draw(sname3.str().c_str(), 300+(i*16), 170 + (240),1.0f, White );

		}
		else
		{
			for(unsigned int i = 0; i < Namebuffer3.size(); i++)
			{

				TOSTRINGSTREAM sname3;

				sname3 << Namebuffer3[i];

				pFont->Draw(sname3.str().c_str(), 300+(i*16), 170 + (240),1.0f, White );
			}
		}
#pragma endregion

		pFont->Draw( _T("Exit"), 720, 550, 1.0f,White );

	}

#pragma endregion

#pragma region Cursor

	RECT rSelection;

	rSelection.left		= 40;
	rSelection.top		= 140 +	( m_nCursor * 120 );
	rSelection.right	= 760;
	rSelection.bottom	= 240 + ( m_nCursor * 120 );

	if (m_nCursor < 3 )
		m_D3D->DrawRect( rSelection, White_Highlight );
	else
	{
		pTM->Draw( m_nCursorID, 550, 545);
		//pFont->Draw( _T("->"),685, 545, 1.0f, Red );	
	}

#pragma endregion

#pragma region LetterInput
	if(IsUserInput == true)
	{



		switch( m_nLetterCount )
		{
		case -1:
			{
				if(m_nCursor == 0)
				{   pTM->Draw( m_nLetterCursorDownID, 570, 135);
				pTM->Draw( m_nLetterCursorID, 570, 190);
				pFont->Draw(_T("Name?"), 600, 170 + (0),1.0f, White );
				}
				else if(m_nCursor == 1)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135+(120));
					pTM->Draw( m_nLetterCursorID, 570, 190+ (120));
					pFont->Draw(_T("Name?"), 600, 170 + (120),1.0f, White );
				}
				else if(m_nCursor == 2)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135+(240));
					pTM->Draw( m_nLetterCursorID, 570, 190+ (240));
					pFont->Draw(_T("Name?"), 600, 170 + (240),1.0f, White );
				}
			}
			break;
		case 0:
			{
				if(m_nCursor == 0)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135);
					pTM->Draw( m_nLetterCursorID, 570, 190);
					pFont->Draw(_T("A"), 600, 170 + (0),1.0f, White );
				}
				else if(m_nCursor == 1)
				{   pTM->Draw( m_nLetterCursorDownID, 570, 135+ (120));
				pTM->Draw( m_nLetterCursorID, 570, 190+ (120));
				pFont->Draw(_T("A"), 600, 170 + (120),1.0f, White );
				}

				else if(m_nCursor == 2)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135+ (240));
					pTM->Draw( m_nLetterCursorID, 570, 190+ (240));
					pFont->Draw(_T("A"), 600, 170 + (240),1.0f, White );
				}

			}
			break;
		case 1:
			{
				if(m_nCursor == 0)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135);
					pTM->Draw( m_nLetterCursorID, 570, 190);
					pFont->Draw(_T("B"), 600, 170 + (0),1.0f, White );
				}
				else if(m_nCursor == 1)
				{   pTM->Draw( m_nLetterCursorDownID, 570, 135+ (120));
				pTM->Draw( m_nLetterCursorID, 570, 190+ (120));
				pFont->Draw(_T("B"), 600, 170 + (120),1.0f, White );
				}

				else if(m_nCursor == 2)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135+ (240));
					pTM->Draw( m_nLetterCursorID, 570, 190+ (240));
					pFont->Draw(_T("B"), 600, 170 + (240),1.0f, White );
				}


			}
			break;
		case 2:
			{
				if(m_nCursor == 0)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135);
					pTM->Draw( m_nLetterCursorID, 570, 190);
					pFont->Draw(_T("C"), 600, 170 + (0),1.0f, White );
				}
				else if(m_nCursor == 1)
				{   pTM->Draw( m_nLetterCursorDownID, 570, 135+ (120));
				pTM->Draw( m_nLetterCursorID, 570, 190+ (120));
				pFont->Draw(_T("C"), 600, 170 + (120),1.0f, White );
				}

				else if(m_nCursor == 2)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135+ (240));
					pTM->Draw( m_nLetterCursorID, 570, 190+ (240));
					pFont->Draw(_T("C"), 600, 170 + (240),1.0f, White );
				}


			}
			break;
		case 3:
			{
				if(m_nCursor == 0)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135);
					pTM->Draw( m_nLetterCursorID, 570, 190);
					pFont->Draw(_T("D"), 600, 170 + (0),1.0f, White );
				}
				else if(m_nCursor == 1)
				{   pTM->Draw( m_nLetterCursorDownID, 570, 135+ (120));
				pTM->Draw( m_nLetterCursorID, 570, 190+ (120));
				pFont->Draw(_T("D"), 600, 170 + (120),1.0f, White );
				}

				else if(m_nCursor == 2)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135+ (240));
					pTM->Draw( m_nLetterCursorID, 570, 190+ (240));
					pFont->Draw(_T("D"), 600, 170 + (240),1.0f, White );
				}


			}
			break;
		case 4:
			{
				if(m_nCursor == 0)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135);
					pTM->Draw( m_nLetterCursorID, 570, 190);
					pFont->Draw(_T("E"), 600, 170 + (0),1.0f, White );
				}
				else if(m_nCursor == 1)
				{   pTM->Draw( m_nLetterCursorDownID, 570, 135+ (120));
				pTM->Draw( m_nLetterCursorID, 570, 190+ (120));
				pFont->Draw(_T("E"), 600, 170 + (120),1.0f, White );
				}

				else if(m_nCursor == 2)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135+ (240));
					pTM->Draw( m_nLetterCursorID, 570, 190+ (240));
					pFont->Draw(_T("E"), 600, 170 + (240),1.0f, White );
				}


			}
			break;
		case 5:
			{
				if(m_nCursor == 0)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135);
					pTM->Draw( m_nLetterCursorID, 570, 190);
					pFont->Draw(_T("F"), 600, 170 + (0),1.0f, White );
				}
				else if(m_nCursor == 1)
				{   pTM->Draw( m_nLetterCursorDownID, 570, 135+ (120));
				pTM->Draw( m_nLetterCursorID, 570, 190+ (120));
				pFont->Draw(_T("F"), 600, 170 + (120),1.0f, White );
				}

				else if(m_nCursor == 2)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135+ (240));
					pTM->Draw( m_nLetterCursorID, 570, 190+ (240));
					pFont->Draw(_T("F"), 600, 170 + (240),1.0f, White );
				}


			}
			break;
		case 6:
			{
				if(m_nCursor == 0)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135);
					pTM->Draw( m_nLetterCursorID, 570, 190);
					pFont->Draw(_T("G"), 600, 170 + (0),1.0f, White );
				}
				else if(m_nCursor == 1)
				{   pTM->Draw( m_nLetterCursorDownID, 570, 135+ (120));
				pTM->Draw( m_nLetterCursorID, 570, 190+ (120));
				pFont->Draw(_T("G"), 600, 170 + (120),1.0f, White );
				}

				else if(m_nCursor == 2)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135+ (240));
					pTM->Draw( m_nLetterCursorID, 570, 190+ (240));
					pFont->Draw(_T("G"), 600, 170 + (240),1.0f, White );
				}

			}
			break;
		case 7:
			{
				if(m_nCursor == 0)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135);
					pTM->Draw( m_nLetterCursorID, 570, 190);
					pFont->Draw(_T("H"), 600, 170 + (0),1.0f, White );
				}
				else if(m_nCursor == 1)
				{   pTM->Draw( m_nLetterCursorDownID, 570, 135+ (120));
				pTM->Draw( m_nLetterCursorID, 570, 190+ (120));
				pFont->Draw(_T("H"), 600, 170 + (120),1.0f, White );
				}

				else if(m_nCursor == 2)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135+ (240));
					pTM->Draw( m_nLetterCursorID, 570, 190+ (240));
					pFont->Draw(_T("H"), 600, 170 + (240),1.0f, White );
				}

			}
			break;
		case 8:
			{
				if(m_nCursor == 0)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135);
					pTM->Draw( m_nLetterCursorID, 570, 190);
					pFont->Draw(_T("I"), 600, 170 + (0),1.0f, White );
				}
				else if(m_nCursor == 1)
				{   pTM->Draw( m_nLetterCursorDownID, 570, 135+ (120));
				pTM->Draw( m_nLetterCursorID, 570, 190+ (120));
				pFont->Draw(_T("I"), 600, 170 + (120),1.0f, White );
				}

				else if(m_nCursor == 2)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135+ (240));
					pTM->Draw( m_nLetterCursorID, 570, 190+ (240));
					pFont->Draw(_T("I"), 600, 170 + (240),1.0f, White );
				}

			}
			break;
		case 9:
			{
				if(m_nCursor == 0)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135);
					pTM->Draw( m_nLetterCursorID, 570, 190);
					pFont->Draw(_T("J"), 600, 170 + (0),1.0f, White );
				}
				else if(m_nCursor == 1)
				{   pTM->Draw( m_nLetterCursorDownID, 570, 135+ (120));
				pTM->Draw( m_nLetterCursorID, 570, 190+ (120));
				pFont->Draw(_T("J"), 600, 170 + (120),1.0f, White );
				}

				else if(m_nCursor == 2)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135+ (240));
					pTM->Draw( m_nLetterCursorID, 570, 190+ (240));
					pFont->Draw(_T("J"), 600, 170 + (240),1.0f, White );
				}

			}
			break;
		case 10:
			{
				if(m_nCursor == 0)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135);
					pTM->Draw( m_nLetterCursorID, 570, 190);
					pFont->Draw(_T("K"), 600, 170 + (0),1.0f, White );
				}
				else if(m_nCursor == 1)
				{   pTM->Draw( m_nLetterCursorDownID, 570, 135+ (120));
				pTM->Draw( m_nLetterCursorID, 570, 190+ (120));
				pFont->Draw(_T("K"), 600, 170 + (120),1.0f, White );
				}

				else if(m_nCursor == 2)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135+ (240));
					pTM->Draw( m_nLetterCursorID, 570, 190+ (240));
					pFont->Draw(_T("K"), 600, 170 + (240),1.0f, White );
				}

			}
			break;
		case 11:
			{
				if(m_nCursor == 0)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135);
					pTM->Draw( m_nLetterCursorID, 570, 190);
					pFont->Draw(_T("L"), 600, 170 + (0),1.0f, White );
				}
				else if(m_nCursor == 1)
				{   pTM->Draw( m_nLetterCursorDownID, 570, 135+ (120));
				pTM->Draw( m_nLetterCursorID, 570, 190+ (120));
				pFont->Draw(_T("L"), 600, 170 + (120),1.0f, White );
				}

				else if(m_nCursor == 2)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135+ (240));
					pTM->Draw( m_nLetterCursorID, 570, 190+ (240));
					pFont->Draw(_T("L"), 600, 170 + (240),1.0f, White );
				}

			}
			break;
		case 12:
			{
				if(m_nCursor == 0)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135);
					pTM->Draw( m_nLetterCursorID, 570, 190);
					pFont->Draw(_T("M"), 600, 170 + (0),1.0f, White );
				}
				else if(m_nCursor == 1)
				{   pTM->Draw( m_nLetterCursorDownID, 570, 135+ (120));
				pTM->Draw( m_nLetterCursorID, 570, 190+ (120));
				pFont->Draw(_T("M"), 600, 170 + (120),1.0f, White );
				}

				else if(m_nCursor == 2)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135+ (240));
					pTM->Draw( m_nLetterCursorID, 570, 190+ (240));
					pFont->Draw(_T("M"), 600, 170 + (240),1.0f, White );
				}

			}
			break;
		case 13:
			{
				if(m_nCursor == 0)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135);
					pTM->Draw( m_nLetterCursorID, 570, 190);
					pFont->Draw(_T("N"), 600, 170 + (0),1.0f, White );
				}
				else if(m_nCursor == 1)
				{   pTM->Draw( m_nLetterCursorDownID, 570, 135+ (120));
				pTM->Draw( m_nLetterCursorID, 570, 190+ (120));
				pFont->Draw(_T("N"), 600, 170 + (120),1.0f, White );
				}

				else if(m_nCursor == 2)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135+ (240));
					pTM->Draw( m_nLetterCursorID, 570, 190+ (240));
					pFont->Draw(_T("N"), 600, 170 + (240),1.0f, White );
				}

			}
			break;
		case 14:
			{
				if(m_nCursor == 0)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135);
					pTM->Draw( m_nLetterCursorID, 570, 190);
					pFont->Draw(_T("O"), 600, 170 + (0),1.0f, White );
				}
				else if(m_nCursor == 1)
				{   pTM->Draw( m_nLetterCursorDownID, 570, 135+ (120));
				pTM->Draw( m_nLetterCursorID, 570, 190+ (120));
				pFont->Draw(_T("O"), 600, 170 + (120),1.0f, White );
				}

				else if(m_nCursor == 2)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135+ (240));
					pTM->Draw( m_nLetterCursorID, 570, 190+ (240));
					pFont->Draw(_T("O"), 600, 170 + (240),1.0f, White );
				}

			}
			break;
		case 15:
			{
				if(m_nCursor == 0)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135);
					pTM->Draw( m_nLetterCursorID, 570, 190);
					pFont->Draw(_T("P"), 600, 170 + (0),1.0f, White );
				}
				else if(m_nCursor == 1)
				{   pTM->Draw( m_nLetterCursorDownID, 570, 135+ (120));
				pTM->Draw( m_nLetterCursorID, 570, 190+ (120));
				pFont->Draw(_T("P"), 600, 170 + (120),1.0f, White );
				}

				else if(m_nCursor == 2)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135+ (240));
					pTM->Draw( m_nLetterCursorID, 570, 190+ (240));
					pFont->Draw(_T("P"), 600, 170 + (240),1.0f, White );
				}

			}
			break;
		case 16:
			{
				if(m_nCursor == 0)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135);
					pTM->Draw( m_nLetterCursorID, 570, 190);
					pFont->Draw(_T("Q"), 600, 170 + (0),1.0f, White );
				}
				else if(m_nCursor == 1)
				{   pTM->Draw( m_nLetterCursorDownID, 570, 135+ (120));
				pTM->Draw( m_nLetterCursorID, 570, 190+ (120));
				pFont->Draw(_T("Q"), 600, 170 + (120),1.0f, White );
				}

				else if(m_nCursor == 2)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135+ (240));
					pTM->Draw( m_nLetterCursorID, 570, 190+ (240));
					pFont->Draw(_T("Q"), 600, 170 + (240),1.0f, White );
				}

			}
			break;
		case 17:
			{
				if(m_nCursor == 0)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135);
					pTM->Draw( m_nLetterCursorID, 570, 190);
					pFont->Draw(_T("R"), 600, 170 + (0),1.0f, White );
				}
				else if(m_nCursor == 1)
				{   pTM->Draw( m_nLetterCursorDownID, 570, 135+ (120));
				pTM->Draw( m_nLetterCursorID, 570, 190+ (120));
				pFont->Draw(_T("R"), 600, 170 + (120),1.0f, White );
				}

				else if(m_nCursor == 2)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135+ (240));
					pTM->Draw( m_nLetterCursorID, 570, 190+ (240));
					pFont->Draw(_T("R"), 600, 170 + (240),1.0f, White );
				}

			}
			break;
		case 18:
			{
				if(m_nCursor == 0)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135);
					pTM->Draw( m_nLetterCursorID, 570, 190);
					pFont->Draw(_T("S"), 600, 170 + (0),1.0f, White );
				}
				else if(m_nCursor == 1)
				{   pTM->Draw( m_nLetterCursorDownID, 570, 135+ (120));
				pTM->Draw( m_nLetterCursorID, 570, 190+ (120));
				pFont->Draw(_T("S"), 600, 170 + (120),1.0f, White );
				}

				else if(m_nCursor == 2)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135+ (240));
					pTM->Draw( m_nLetterCursorID, 570, 190+ (240));
					pFont->Draw(_T("S"), 600, 170 + (240),1.0f, White );
				}

			}
			break;
		case 19:
			{
				if(m_nCursor == 0)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135);
					pTM->Draw( m_nLetterCursorID, 570, 190);
					pFont->Draw(_T("T"), 600, 170 + (0),1.0f, White );
				}
				else if(m_nCursor == 1)
				{   pTM->Draw( m_nLetterCursorDownID, 570, 135+ (120));
				pTM->Draw( m_nLetterCursorID, 570, 190+ (120));
				pFont->Draw(_T("T"), 600, 170 + (120),1.0f, White );
				}

				else if(m_nCursor == 2)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135+ (240));
					pTM->Draw( m_nLetterCursorID, 570, 190+ (240));
					pFont->Draw(_T("T"), 600, 170 + (240),1.0f, White );
				}

			}
			break;
		case 20:
			{
				if(m_nCursor == 0)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135);
					pTM->Draw( m_nLetterCursorID, 570, 190);
					pFont->Draw(_T("U"), 600, 170 + (0),1.0f, White );
				}
				else if(m_nCursor == 1)
				{   pTM->Draw( m_nLetterCursorDownID, 570, 135+ (120));
				pTM->Draw( m_nLetterCursorID, 570, 190+ (120));
				pFont->Draw(_T("U"), 600, 170 + (120),1.0f, White );
				}

				else if(m_nCursor == 2)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135+ (240));
					pTM->Draw( m_nLetterCursorID, 570, 190+ (240));
					pFont->Draw(_T("U"), 600, 170 + (240),1.0f, White );
				}

			}
			break;
		case 21:
			{
				if(m_nCursor == 0)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135);
					pTM->Draw( m_nLetterCursorID, 570, 190);
					pFont->Draw(_T("V"), 600, 170 + (0),1.0f, White );
				}
				else if(m_nCursor == 1)
				{   pTM->Draw( m_nLetterCursorDownID, 570, 135+ (120));
				pTM->Draw( m_nLetterCursorID, 570, 190+ (120));
				pFont->Draw(_T("V"), 600, 170 + (120),1.0f, White );
				}

				else if(m_nCursor == 2)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135+ (240));
					pTM->Draw( m_nLetterCursorID, 570, 190+ (240));
					pFont->Draw(_T("V"), 600, 170 + (240),1.0f, White );
				}

			}
			break;
		case 22:
			{
				if(m_nCursor == 0)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135);
					pTM->Draw( m_nLetterCursorID, 570, 190);
					pFont->Draw(_T("W"), 600, 170 + (0),1.0f, White );
				}
				else if(m_nCursor == 1)
				{   pTM->Draw( m_nLetterCursorDownID, 570, 135+ (120));
				pTM->Draw( m_nLetterCursorID, 570, 190+ (120));
				pFont->Draw(_T("W"), 600, 170 + (120),1.0f, White );
				}

				else if(m_nCursor == 2)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135+ (240));
					pTM->Draw( m_nLetterCursorID, 570, 190+ (240));
					pFont->Draw(_T("W"), 600, 170 + (240),1.0f, White );
				}

			}
			break;
		case 23:
			{
				if(m_nCursor == 0)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135);
					pTM->Draw( m_nLetterCursorID, 570, 190);
					pFont->Draw(_T("X"), 600, 170 + (0),1.0f, White );
				}
				else if(m_nCursor == 1)
				{   pTM->Draw( m_nLetterCursorDownID, 570, 135+ (120));
				pTM->Draw( m_nLetterCursorID, 570, 190+ (120));
				pFont->Draw(_T("X"), 600, 170 + (120),1.0f, White );
				}

				else if(m_nCursor == 2)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135+ (240));
					pTM->Draw( m_nLetterCursorID, 570, 190+ (240));
					pFont->Draw(_T("X"), 600, 170 + (240),1.0f, White );
				}

			}
			break;
		case 24:
			{
				if(m_nCursor == 0)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135);
					pTM->Draw( m_nLetterCursorID, 570, 190);
					pFont->Draw(_T("Y"), 600, 170 + (0),1.0f, White );
				}
				else if(m_nCursor == 1)
				{   pTM->Draw( m_nLetterCursorDownID, 570, 135+ (120));
				pTM->Draw( m_nLetterCursorID, 570, 190+ (120));
				pFont->Draw(_T("Y"), 600, 170 + (120),1.0f, White );
				}

				else if(m_nCursor == 2)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135+ (240));
					pTM->Draw( m_nLetterCursorID, 570, 190+ (240));
					pFont->Draw(_T("Y"), 600, 170 + (240),1.0f, White );
				}

			}
			break;
		case 25:
			{
				if(m_nCursor == 0)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135);
					pTM->Draw( m_nLetterCursorID, 570, 190);
					pFont->Draw(_T("Z"), 600, 170 + (0),1.0f, White );
				}
				else if(m_nCursor == 1)
				{   pTM->Draw( m_nLetterCursorDownID, 570, 135+ (120));
				pTM->Draw( m_nLetterCursorID, 570, 190+ (120));
				pFont->Draw(_T("Z"), 600, 170 + (120),1.0f, White );
				}

				else if(m_nCursor == 2)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135+ (240));
					pTM->Draw( m_nLetterCursorID, 570, 190+ (240));
					pFont->Draw(_T("Z"), 600, 170 + (240),1.0f, White );
				}


			}
			break;
		case 26:
			{

				if(m_nCursor == 0)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135);
					pTM->Draw( m_nLetterCursorID, 570, 190);
					pFont->Draw(_T("End"), 600, 170 + (0),1.0f, White );
				}
				else if(m_nCursor == 1)
				{   pTM->Draw( m_nLetterCursorDownID, 570, 135+ (120));
				pTM->Draw( m_nLetterCursorID, 570, 190+ (120));
				pFont->Draw(_T("End"), 600, 170 + (120),1.0f, White );
				}

				else if(m_nCursor == 2)
				{
					pTM->Draw( m_nLetterCursorDownID, 570, 135+ (240));
					pTM->Draw( m_nLetterCursorID, 570, 190+ (240));
					pFont->Draw(_T("End"), 600, 170 + (240),1.0f, White );
				}

			}
			break;

		}








	}

#pragma endregion

}
void	CSaveGame::Enter			( )		
{
	m_nLetterCursorDownID = CSGD_TextureManager::GetInstance()->LoadTexture( _T( "resources/graphics/LetterCursorDown.png") );
	m_nLetterCursorID = CSGD_TextureManager::GetInstance()->LoadTexture( _T( "resources/graphics/LetterCursor.png") );
	m_nBackgroundImageID =  CSGD_TextureManager::GetInstance()->LoadTexture( _T( "resources/graphics/SaveLoadBackground.png") );
	m_nSlotImageID = CSGD_TextureManager::GetInstance()->LoadTexture( _T( "resources/graphics/Slot.png") );
	m_nCursorID = CSGD_TextureManager::GetInstance()->LoadTexture( _T( "resources/graphics/Cursor_MainMenu.png") );
	//Cursor Image ID Set
	cursorImageID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/Cursor_Battle.png"));
	m_nLetterCount = -1;
	m_nCursor = 0;
	IsUserInput = false;
	m_nsoldierCount			 = 0;
	m_npriestCount			 = 0;
	m_nwizardCount			 = 0;
	m_ncavalryCount			 = 0;
	m_ncatapultCount		 = 0;
	m_nskeletonCount		 = 0;
	m_nspriestCount			 = 0;
	m_nnecroCount			 = 0;
	m_ndkCount				 = 0;
	m_nwmCount				 = 0;
	m_nCap   = 0;
	overwriteslot1 = false;
	overwriteslot2 = false;
	overwriteslot3 = false;
	/*Namebuffer1 = "";
	Namebuffer2 = "";
	Namebuffer3 = "";*/
	TCHAR * wszPath = NULL;

	HRESULT hr = SHGetKnownFolderPath( FOLDERID_LocalAppData, 0, 0, &wszPath);

	if(SUCCEEDED(hr))
	{
		std::wstring name = wszPath;
		name += L"\\ArchaicDomination\\Slots.xml";
		DWORD path = GetFileAttributes(name.c_str());

		if(path == INVALID_FILE_ATTRIBUTES)
		{
			SaveDefault(name.c_str());
			LoadSlot(name.c_str(), vPlayers);

		}
		else
		{
			LoadSlot(name.c_str(), vPlayers);
		}
	}
	//LoadSlot("Slots.xml", vPlayers);

	IsUserInput = false;
}
void	CSaveGame::Exit			( )	
{
}

void CSaveGame::SavePlayers	( const wchar_t* szFilename, 
							  vector< tPlayerSaveInfo >&	vPlayers, int m_nCursor )
{	
	   /* TCHAR * wszPath = NULL;

				HRESULT hr = SHGetKnownFolderPath( FOLDERID_LocalAppData, 0, 0, &wszPath);

				if(SUCCEEDED(hr))
				{
					std::wstring name = wszPath;
					name += L"\\ArchaicDomination\\Slots.xml";

					LoadSlotGame(name.c_str(),vPlayers, m_nCursor);

				}*/
	

#pragma region saveslot1
	// Create a TinyXML Document
	TiXmlDocument doc;


	// Create the TinyXML Declaration (version)
	TiXmlDeclaration* pDecl = new TiXmlDeclaration( "1.0", "utf-8", "" );

	// Link the declaration to the document
	doc.LinkEndChild( pDecl );



	// Create a root node ("players_list")
	TiXmlElement* pRoot = new TiXmlElement( "World" );

	// Link the root to the document
	doc.LinkEndChild( pRoot);

	// Loop through the vector of players
	for( unsigned int i = 0; i < vPlayers.size(); i++ )
	{
		
		TiXmlElement* pName = new TiXmlElement( "Name" );
	  
		
		TiXmlText* pText = new TiXmlText( vPlayers[ i ].szName );
		pName->LinkEndChild( pText );
		pRoot->LinkEndChild( pName );
		

#pragma region PlayerOneInfo
		// Create a node for the player info ("player_info")
		TiXmlElement* pPlayer = new TiXmlElement("playerone_info" );


		// Create a Text node for the player's name (stored between tags)
		//pPlayer->SetAttribute("Name",vPlayers[i].szName);

		//vPlayers[i].szName = Namebuffer1;



		pPlayer->SetAttribute( "player1wood",  Player->GetWood());
		pPlayer->SetAttribute( "player1food",  Player->GetFood() );
		pPlayer->SetAttribute( "player1steel", Player->GetSteel() );
		// Link the text to the player node
		//pPlayer->LinkEndChild( pText );
		// Link the player node to the root node
		pName->LinkEndChild( pPlayer );

#pragma endregion

#pragma region PlayerTwoInfo

		TiXmlElement* pPlayerTwo = new TiXmlElement("playertwo_info" );
		pPlayerTwo->SetAttribute( "player2wood",  Enemy->GetWood());
		pPlayerTwo->SetAttribute( "player2food",  Enemy->GetFood() );
		pPlayerTwo->SetAttribute( "player2steel", Enemy->GetSteel() );
		// Link the player node to the root node
		pName->LinkEndChild( pPlayerTwo );


#pragma endregion

#pragma region WorldInfo

		TiXmlElement* pWorldInfo = new TiXmlElement("world_info" );
		pWorldInfo->SetAttribute( "GameMode",        vPlayers[i].CAMPAIGN );
		pWorldInfo->SetAttribute( "TurnOrder",       GamePlay->GetTurnOrder());
		pWorldInfo->SetAttribute( "TurnNumber",      GamePlay->GetTurnNumber() );
		pWorldInfo->SetAttribute( "TurnCap",         GamePlay->GetTurnCap() );
		pWorldInfo->SetAttribute( "AiEnabled",       GamePlay->GetAiEnabled() );
		pWorldInfo->SetAttribute( "CountryOneOwner", GamePlay->GetCountryOneOwner());
		pWorldInfo->SetAttribute( "CounrtyTwoOwner", GamePlay->GetCountryTwoOwner() );



		// Link the player node to the root node
		pName->LinkEndChild( pWorldInfo );

#pragma endregion

#pragma region TerritoriesOwned
		for(unsigned int i = 0; i < GamePlay->GetWorld()->GetGrid().size();++i)
		{
			for(unsigned int j = 0; j < GamePlay->GetWorld()->GetGrid()[i].size();++j)
			{

				TiXmlElement* pCountryOwner = new TiXmlElement("country_info" );

				pCountryOwner->SetAttribute( "Country", i);
				pCountryOwner->SetAttribute( "Territory", j);
				pCountryOwner->SetAttribute( "Owner", GamePlay->GetWorld()->GetGrid()[i][j]->GetOwner());


				pName->LinkEndChild( pCountryOwner );


			}
		}



#pragma endregion

#pragma region PlayerBuildinginfo
		for (unsigned int i = 0; i < Player->GetBuildings().size();++i)
		{
			if(Player->GetBuildings()[i]->GetBuildingID() == FARM)
			{
				TiXmlElement* pBuildingInfo = new TiXmlElement("humanfarmbuilding_info" );
				//pBuildingInfo->SetAttribute( "PosX",			 Player->GetBuildings()[i]->GetPosX());
				//pBuildingInfo->SetAttribute( "PosY",			 Player->GetBuildings()[i]->GetPosY());
				pBuildingInfo->SetAttribute( "BuildPosX",      Player->GetBuildings()[i]->GetPosBuildX());
				pBuildingInfo->SetAttribute( "BuildPosY",      Player->GetBuildings()[i]->GetPosBuildY());
				pName->LinkEndChild( pBuildingInfo );
			}
			if(Player->GetBuildings()[i]->GetBuildingID() == WOOD)
			{
				TiXmlElement* pBuildingInfo = new TiXmlElement("humanwoodbuilding_info" );
				pBuildingInfo->SetAttribute( "PosX",			(int)Player->GetBuildings()[i]->GetPosX());
				pBuildingInfo->SetAttribute( "PosY",			(int)Player->GetBuildings()[i]->GetPosY());
				pBuildingInfo->SetAttribute( "BuildPosX",		Player->GetBuildings()[i]->GetPosBuildX());
				pBuildingInfo->SetAttribute( "BuildPosY",		Player->GetBuildings()[i]->GetPosBuildY());
				pName->LinkEndChild( pBuildingInfo );
			}
			if(Player->GetBuildings()[i]->GetBuildingID() == MINE)
			{
				TiXmlElement* pBuildingInfo = new TiXmlElement("humanminebuilding_info" );
				pBuildingInfo->SetAttribute( "PosX",			(int)Player->GetBuildings()[i]->GetPosX());
				pBuildingInfo->SetAttribute( "PosY",			(int)Player->GetBuildings()[i]->GetPosY());
				pBuildingInfo->SetAttribute( "BuildPosX",		Player->GetBuildings()[i]->GetPosBuildX());
				pBuildingInfo->SetAttribute( "BuildPosY",		Player->GetBuildings()[i]->GetPosBuildY());
				pName->LinkEndChild( pBuildingInfo );
			}
			if(Player->GetBuildings()[i]->GetBuildingID() == BARRACKS)
			{
				TiXmlElement* pBuildingInfo = new TiXmlElement("humanbarracksbuilding_info" );
				//pBuildingInfo->SetAttribute( "PosX",			 Player->GetBuildings()[i]->GetPosX());
				//pBuildingInfo->SetAttribute( "PosY",			 Player->GetBuildings()[i]->GetPosY());
				pBuildingInfo->SetAttribute( "BuildPosX",      Player->GetBuildings()[i]->GetPosBuildX());
				pBuildingInfo->SetAttribute( "BuildPosY",      Player->GetBuildings()[i]->GetPosBuildY());
				pName->LinkEndChild( pBuildingInfo );
			}
			if(Player->GetBuildings()[i]->GetBuildingID() == CAPITOL)
			{
				TiXmlElement* pBuildingInfo = new TiXmlElement("humancapitalbuilding_info" );
				//pBuildingInfo->SetAttribute( "PosX",			 Player->GetBuildings()[i]->GetPosX());
				//pBuildingInfo->SetAttribute( "PosY",			 Player->GetBuildings()[i]->GetPosY());
				pBuildingInfo->SetAttribute( "BuildPosX",      Player->GetBuildings()[i]->GetPosBuildX());
				pBuildingInfo->SetAttribute( "BuildPosY",      Player->GetBuildings()[i]->GetPosBuildY());
				pName->LinkEndChild( pBuildingInfo );
			}
			if(Player->GetBuildings()[i]->GetBuildingID() == WARFACTORY)
			{
				TiXmlElement* pBuildingInfo = new TiXmlElement("humanwarfactorybuilding_info" );
				//pBuildingInfo->SetAttribute( "PosX",			 Player->GetBuildings()[i]->GetPosX());
				//pBuildingInfo->SetAttribute( "PosY",			 Player->GetBuildings()[i]->GetPosY());
				pBuildingInfo->SetAttribute( "BuildPosX",      Player->GetBuildings()[i]->GetPosBuildX());
				pBuildingInfo->SetAttribute( "BuildPosY",      Player->GetBuildings()[i]->GetPosBuildY());
				pName->LinkEndChild( pBuildingInfo );
			}
		}










#pragma endregion

#pragma region Enemybuildinginfo

		for (unsigned int i = 0; i < Enemy->GetBuildings().size();++i)
		{
			if(Enemy->GetBuildings()[i]->GetBuildingID() == FARM)
			{
				TiXmlElement* pBuildingInfo = new TiXmlElement("undeadfarmbuilding_info" );
				//pBuildingInfo->SetAttribute( "PosX",			 Enemy->GetBuildings()[i]->GetPosX());
				//pBuildingInfo->SetAttribute( "PosY",			 Enemy->GetBuildings()[i]->GetPosY());
				pBuildingInfo->SetAttribute( "BuildPosX",      Enemy->GetBuildings()[i]->GetPosBuildX());
				pBuildingInfo->SetAttribute( "BuildPosY",      Enemy->GetBuildings()[i]->GetPosBuildY());
				pName->LinkEndChild( pBuildingInfo );
			}
			if(Enemy->GetBuildings()[i]->GetBuildingID() == WOOD)
			{
				TiXmlElement* pBuildingInfo = new TiXmlElement("undeadwoodbuilding_info" );
				//pBuildingInfo->SetAttribute( "PosX",			 Enemy->GetBuildings()[i]->GetPosX());
				//pBuildingInfo->SetAttribute( "PosY",			 Enemy->GetBuildings()[i]->GetPosY());
				pBuildingInfo->SetAttribute( "BuildPosX",      Enemy->GetBuildings()[i]->GetPosBuildX());
				pBuildingInfo->SetAttribute( "BuildPosY",      Enemy->GetBuildings()[i]->GetPosBuildY());
				pName->LinkEndChild( pBuildingInfo );
			}
			if(Enemy->GetBuildings()[i]->GetBuildingID() == MINE)
			{
				TiXmlElement* pBuildingInfo = new TiXmlElement("undeadminebuilding_info" );

				pBuildingInfo->SetAttribute( "BuildPosX",      Enemy->GetBuildings()[i]->GetPosBuildX());
				pBuildingInfo->SetAttribute( "BuildPosY",      Enemy->GetBuildings()[i]->GetPosBuildY());
				pName->LinkEndChild( pBuildingInfo );
			}
			if(Enemy->GetBuildings()[i]->GetBuildingID() == BARRACKS)
			{
				TiXmlElement* pBuildingInfo = new TiXmlElement("undeadbarracksbuilding_info" );

				pBuildingInfo->SetAttribute( "BuildPosX",      Enemy->GetBuildings()[i]->GetPosBuildX());
				pBuildingInfo->SetAttribute( "BuildPosY",      Enemy->GetBuildings()[i]->GetPosBuildY());
				pName->LinkEndChild( pBuildingInfo );
			}
			if(Enemy->GetBuildings()[i]->GetBuildingID() == CAPITOL)
			{
				TiXmlElement* pBuildingInfo = new TiXmlElement("undeadcapitalbuilding_info" );

				pBuildingInfo->SetAttribute( "BuildPosX",      Enemy->GetBuildings()[i]->GetPosBuildX());
				pBuildingInfo->SetAttribute( "BuildPosY",      Enemy->GetBuildings()[i]->GetPosBuildY());
				pName->LinkEndChild( pBuildingInfo );
			}
			if(Enemy->GetBuildings()[i]->GetBuildingID() == WARFACTORY)
			{
				TiXmlElement* pBuildingInfo = new TiXmlElement("undeadwarfactorybuilding_info" );

				pBuildingInfo->SetAttribute( "BuildPosX",      Enemy->GetBuildings()[i]->GetPosBuildX());
				pBuildingInfo->SetAttribute( "BuildPosY",      Enemy->GetBuildings()[i]->GetPosBuildY());
				pName->LinkEndChild( pBuildingInfo );
			}
		}

#pragma endregion



#pragma region PlayerUnits
		for(unsigned int i = 0; i < Player->GetUnits().size() ; ++i)
		{
			if(Player->GetUnits()[i]->GetUnitID() == FOOTSOLDIERS)
			{
				TiXmlElement* pFootSoldierInfo = new TiXmlElement("footsoldier_info" );
				pFootSoldierInfo->SetAttribute( "Country",        Player->GetUnits()[i]->GetCountry());
				pFootSoldierInfo->SetAttribute( "Territory",      Player->GetUnits()[i]->GetTerritory() );
				pName->LinkEndChild( pFootSoldierInfo );
			}

			if(Player->GetUnits()[i]->GetUnitID() == PRIEST)
			{
				TiXmlElement* pPriestInfo = new TiXmlElement("priest_info" );
				pPriestInfo->SetAttribute( "Country",        Player->GetUnits()[i]->GetCountry());
				pPriestInfo->SetAttribute( "Territory",      Player->GetUnits()[i]->GetTerritory() );
				pName->LinkEndChild( pPriestInfo );
			}

			if(Player->GetUnits()[i]->GetUnitID() == WIZARD)
			{
				TiXmlElement* pWizardInfo = new TiXmlElement("wizard_info" );
				pWizardInfo->SetAttribute( "Country",        Player->GetUnits()[i]->GetCountry());
				pWizardInfo->SetAttribute( "Territory",      Player->GetUnits()[i]->GetTerritory() );
				pName->LinkEndChild( pWizardInfo );
			}

			if(Player->GetUnits()[i]->GetUnitID() == CALVARY)
			{
				TiXmlElement* pCalvaryInfo = new TiXmlElement("calvary_info" );
				pCalvaryInfo->SetAttribute( "Country",        Player->GetUnits()[i]->GetCountry());
				pCalvaryInfo->SetAttribute( "Territory",      Player->GetUnits()[i]->GetTerritory() );
				pName->LinkEndChild( pCalvaryInfo );
			}

			if(Player->GetUnits()[i]->GetUnitID() == CATAPULT)
			{
				TiXmlElement* pCatapultInfo = new TiXmlElement("catapult_info" );
				pCatapultInfo->SetAttribute( "Country",        Player->GetUnits()[i]->GetCountry());
				pCatapultInfo->SetAttribute( "Territory",      Player->GetUnits()[i]->GetTerritory() );
				pName->LinkEndChild( pCatapultInfo );
			}

			if(Player->GetUnits()[i]->GetUnitID() == GENERAL)
			{
				TiXmlElement* pGeneralInfo = new TiXmlElement("general_info" );
				pGeneralInfo->SetAttribute( "Country",        Player->GetUnits()[i]->GetCountry());
				pGeneralInfo->SetAttribute( "Territory",      Player->GetUnits()[i]->GetTerritory() );
				pName->LinkEndChild(pGeneralInfo );
			}
		}

#pragma endregion

#pragma region EnemyUnits
		for(unsigned int i = 0; i < Enemy->GetUnits().size() ; ++i)
		{
			if(Enemy->GetUnits()[i]->GetUnitID() == SKELETALWARRIOR)
			{
				TiXmlElement* pSkeletalWarriorInfo = new TiXmlElement("skeletalwarrior_info" );
				pSkeletalWarriorInfo->SetAttribute( "Country",        Enemy->GetUnits()[i]->GetCountry());
				pSkeletalWarriorInfo->SetAttribute( "Territory",      Enemy->GetUnits()[i]->GetTerritory() );
				pName->LinkEndChild( pSkeletalWarriorInfo );
			}

			if(Enemy->GetUnits()[i]->GetUnitID() == SHADOWPRIEST)
			{
				TiXmlElement* pShadowPriestInfo = new TiXmlElement("shadowpriest_info" );
				pShadowPriestInfo->SetAttribute( "Country",        Enemy->GetUnits()[i]->GetCountry());
				pShadowPriestInfo->SetAttribute( "Territory",      Enemy->GetUnits()[i]->GetTerritory() );
				pName->LinkEndChild( pShadowPriestInfo );
			}

			if(Enemy->GetUnits()[i]->GetUnitID() == NECROMANCER)
			{
				TiXmlElement* pNecromancerInfo = new TiXmlElement("necromancer_info" );
				pNecromancerInfo->SetAttribute( "Country",       Enemy->GetUnits()[i]->GetCountry());
				pNecromancerInfo->SetAttribute( "Territory",     Enemy->GetUnits()[i]->GetTerritory() );
				pName->LinkEndChild( pNecromancerInfo );
			}

			if(Enemy->GetUnits()[i]->GetUnitID() == DEATHKNIGHT)
			{
				TiXmlElement* pDeathKnightInfo = new TiXmlElement("deathknight_info" );
				pDeathKnightInfo->SetAttribute( "Country",        Enemy->GetUnits()[i]->GetCountry());
				pDeathKnightInfo->SetAttribute( "Territory",      Enemy->GetUnits()[i]->GetTerritory() );
				pName->LinkEndChild( pDeathKnightInfo );
			}

			if(Enemy->GetUnits()[i]->GetUnitID() == WARMACHINE)
			{
				TiXmlElement* pWarmachineInfo = new TiXmlElement("warmachine_info" );
				pWarmachineInfo->SetAttribute( "Country",        Enemy->GetUnits()[i]->GetCountry());
				pWarmachineInfo->SetAttribute( "Territory",      Enemy->GetUnits()[i]->GetTerritory() );
				pName->LinkEndChild( pWarmachineInfo );
			}

			if(Enemy->GetUnits()[i]->GetUnitID() == LICH)
			{
				TiXmlElement* pLichInfo = new TiXmlElement("lich_info" );
				pLichInfo->SetAttribute( "Country",        Enemy->GetUnits()[i]->GetCountry());
				pLichInfo->SetAttribute( "Territory",      Enemy->GetUnits()[i]->GetTerritory() );
				pName->LinkEndChild(pLichInfo );
			}
		}


#pragma endregion




	}

	char buffernamefile[128];
	wcstombs_s(nullptr,buffernamefile,128,szFilename,_TRUNCATE);
	// Save the XML Document to the file
	//std::wstring namefile = szFilename;
	doc.SaveFile(buffernamefile);

#pragma endregion		
}

//void CSaveGame::SavePlayer2	( const char* szFilename, 
//							 const vector< tPlayerSaveInfo2 >&	vPlayers )
//{	
//#pragma region saveslo2
//	// Create a TinyXML Document
//	TiXmlDocument doc;
//
//
//	// Create the TinyXML Declaration (version)
//	TiXmlDeclaration* pDecl = new TiXmlDeclaration( "1.0", "utf-8", "" );
//
//	// Link the declaration to the document
//	doc.LinkEndChild( pDecl );
//
//
//	// Create a root node ("players_list")
//	TiXmlElement* pRoot = new TiXmlElement( "World" );
//
//	// Link the root to the document
//	doc.LinkEndChild( pRoot );
//
//	// Loop through the vector of players
//	for( unsigned int i = 0; i < vPlayers.size(); i++ )
//	{
//
//#pragma region PlayerOneInfo
//		// Create a node for the player info ("player_info")
//		TiXmlElement* pPlayer = new TiXmlElement("playerone_info" );
//
//
//		// Create a Text node for the player's name (stored between tags)
//		//pPlayer->SetAttribute("Name",vPlayers[i].szName);
//		TiXmlText* pText = new TiXmlText( vPlayers[ i ].szName );
//
//		pPlayer->SetAttribute( "player1wood",  Player->GetWood());
//		pPlayer->SetAttribute( "player1food",  Player->GetFood() );
//		pPlayer->SetAttribute( "player1steel", Player->GetSteel() );
//		// Link the text to the player node
//		pPlayer->LinkEndChild( pText );
//		// Link the player node to the root node
//		pRoot->LinkEndChild( pPlayer );
//
//#pragma endregion
//
//#pragma region PlayerTwoInfo
//
//		TiXmlElement* pPlayerTwo = new TiXmlElement("playertwo_info" );
//		pPlayerTwo->SetAttribute( "player2wood",  Enemy->GetWood());
//		pPlayerTwo->SetAttribute( "player2food",  Enemy->GetFood() );
//		pPlayerTwo->SetAttribute( "player2steel", Enemy->GetSteel() );
//		// Link the player node to the root node
//		pRoot->LinkEndChild( pPlayerTwo );
//
//
//#pragma endregion
//
//#pragma region WorldInfo
//
//		TiXmlElement* pWorldInfo = new TiXmlElement("world_info" );
//		pWorldInfo->SetAttribute( "GameMode",        vPlayer1[i].CAMPAIGN );
//		pWorldInfo->SetAttribute( "TurnOrder",       GamePlay->GetTurnOrder());
//		pWorldInfo->SetAttribute( "TurnNumber",      GamePlay->GetTurnNumber() );
//		pWorldInfo->SetAttribute( "TurnCap",         GamePlay->GetTurnCap() );
//		pWorldInfo->SetAttribute( "AiEnabled",       GamePlay->GetAiEnabled() );
//		pWorldInfo->SetAttribute( "CountryOneOwner", GamePlay->GetCountryOneOwner());
//		pWorldInfo->SetAttribute( "CounrtyTwoOwner", GamePlay->GetCountryTwoOwner() );
//		// Link the player node to the root node
//		pRoot->LinkEndChild( pWorldInfo );
//
//#pragma endregion
//
//
//#pragma region PlayerBuildinginfo
//		for (unsigned int i = 0; i < Player->GetBuildings().size();++i)
//		{
//			if(Player->GetBuildings()[i]->GetBuildingID() == FARM)
//			{
//				TiXmlElement* pBuildingInfo = new TiXmlElement("humanfarmbuilding_info" );
//				//pBuildingInfo->SetAttribute( "PosX",			 Player->GetBuildings()[i]->GetPosX());
//				//pBuildingInfo->SetAttribute( "PosY",			 Player->GetBuildings()[i]->GetPosY());
//				pBuildingInfo->SetAttribute( "BuildPosX",      Player->GetBuildings()[i]->GetPosBuildX());
//				pBuildingInfo->SetAttribute( "BuildPosY",      Player->GetBuildings()[i]->GetPosBuildY());
//				pRoot->LinkEndChild( pBuildingInfo );
//			}
//			if(Player->GetBuildings()[i]->GetBuildingID() == WOOD)
//			{
//				TiXmlElement* pBuildingInfo = new TiXmlElement("humanwoodbuilding_info" );
//				pBuildingInfo->SetAttribute( "PosX",			(int)Player->GetBuildings()[i]->GetPosX());
//				pBuildingInfo->SetAttribute( "PosY",			(int)Player->GetBuildings()[i]->GetPosY());
//				pBuildingInfo->SetAttribute( "BuildPosX",      Player->GetBuildings()[i]->GetPosBuildX());
//				pBuildingInfo->SetAttribute( "BuildPosY",      Player->GetBuildings()[i]->GetPosBuildY());
//				pRoot->LinkEndChild( pBuildingInfo );
//			}
//			if(Player->GetBuildings()[i]->GetBuildingID() == MINE)
//			{
//				TiXmlElement* pBuildingInfo = new TiXmlElement("humanminebuilding_info" );
//				pBuildingInfo->SetAttribute( "PosX",			 (int)Player->GetBuildings()[i]->GetPosX());
//				pBuildingInfo->SetAttribute( "PosY",			 (int)Player->GetBuildings()[i]->GetPosY());
//				pBuildingInfo->SetAttribute( "BuildPosX",      Player->GetBuildings()[i]->GetPosBuildX());
//				pBuildingInfo->SetAttribute( "BuildPosY",      Player->GetBuildings()[i]->GetPosBuildY());
//				pRoot->LinkEndChild( pBuildingInfo );
//			}
//			if(Player->GetBuildings()[i]->GetBuildingID() == BARRACKS)
//			{
//				TiXmlElement* pBuildingInfo = new TiXmlElement("humanbarracksbuilding_info" );
//				//pBuildingInfo->SetAttribute( "PosX",			 Player->GetBuildings()[i]->GetPosX());
//				//pBuildingInfo->SetAttribute( "PosY",			 Player->GetBuildings()[i]->GetPosY());
//				pBuildingInfo->SetAttribute( "BuildPosX",      Player->GetBuildings()[i]->GetPosBuildX());
//				pBuildingInfo->SetAttribute( "BuildPosY",      Player->GetBuildings()[i]->GetPosBuildY());
//				pRoot->LinkEndChild( pBuildingInfo );
//			}
//			if(Player->GetBuildings()[i]->GetBuildingID() == CAPITOL)
//			{
//				TiXmlElement* pBuildingInfo = new TiXmlElement("humancapitalbuilding_info" );
//				//pBuildingInfo->SetAttribute( "PosX",			 Player->GetBuildings()[i]->GetPosX());
//				//pBuildingInfo->SetAttribute( "PosY",			 Player->GetBuildings()[i]->GetPosY());
//				pBuildingInfo->SetAttribute( "BuildPosX",      Player->GetBuildings()[i]->GetPosBuildX());
//				pBuildingInfo->SetAttribute( "BuildPosY",      Player->GetBuildings()[i]->GetPosBuildY());
//				pRoot->LinkEndChild( pBuildingInfo );
//			}
//			if(Player->GetBuildings()[i]->GetBuildingID() == WARFACTORY)
//			{
//				TiXmlElement* pBuildingInfo = new TiXmlElement("humanwarfactorybuilding_info" );
//				//pBuildingInfo->SetAttribute( "PosX",			 Player->GetBuildings()[i]->GetPosX());
//				//pBuildingInfo->SetAttribute( "PosY",			 Player->GetBuildings()[i]->GetPosY());
//				pBuildingInfo->SetAttribute( "BuildPosX",      Player->GetBuildings()[i]->GetPosBuildX());
//				pBuildingInfo->SetAttribute( "BuildPosY",      Player->GetBuildings()[i]->GetPosBuildY());
//				pRoot->LinkEndChild( pBuildingInfo );
//			}
//		}
//
//
//
//
//
//
//
//
//
//
//#pragma endregion
//
//#pragma region Enemybuildinginfo
//
//		for (unsigned int i = 0; i < Enemy->GetBuildings().size();++i)
//		{
//			if(Enemy->GetBuildings()[i]->GetBuildingID() == FARM)
//			{
//				TiXmlElement* pBuildingInfo = new TiXmlElement("undeadfarmbuilding_info" );
//				//pBuildingInfo->SetAttribute( "PosX",			 Enemy->GetBuildings()[i]->GetPosX());
//				//pBuildingInfo->SetAttribute( "PosY",			 Enemy->GetBuildings()[i]->GetPosY());
//				pBuildingInfo->SetAttribute( "BuildPosX",      Enemy->GetBuildings()[i]->GetPosBuildX());
//				pBuildingInfo->SetAttribute( "BuildPosY",      Enemy->GetBuildings()[i]->GetPosBuildY());
//				pRoot->LinkEndChild( pBuildingInfo );
//			}
//			if(Enemy->GetBuildings()[i]->GetBuildingID() == WOOD)
//			{
//				TiXmlElement* pBuildingInfo = new TiXmlElement("undeadwoodbuilding_info" );
//				//pBuildingInfo->SetAttribute( "PosX",			 Enemy->GetBuildings()[i]->GetPosX());
//				//pBuildingInfo->SetAttribute( "PosY",			 Enemy->GetBuildings()[i]->GetPosY());
//				pBuildingInfo->SetAttribute( "BuildPosX",      Enemy->GetBuildings()[i]->GetPosBuildX());
//				pBuildingInfo->SetAttribute( "BuildPosY",      Enemy->GetBuildings()[i]->GetPosBuildY());
//				pRoot->LinkEndChild( pBuildingInfo );
//			}
//			if(Enemy->GetBuildings()[i]->GetBuildingID() == MINE)
//			{
//				TiXmlElement* pBuildingInfo = new TiXmlElement("undeadminebuilding_info" );
//
//				pBuildingInfo->SetAttribute( "BuildPosX",      Enemy->GetBuildings()[i]->GetPosBuildX());
//				pBuildingInfo->SetAttribute( "BuildPosY",      Enemy->GetBuildings()[i]->GetPosBuildY());
//				pRoot->LinkEndChild( pBuildingInfo );
//			}
//			if(Enemy->GetBuildings()[i]->GetBuildingID() == BARRACKS)
//			{
//				TiXmlElement* pBuildingInfo = new TiXmlElement("undeadbarracksbuilding_info" );
//
//				pBuildingInfo->SetAttribute( "BuildPosX",      Enemy->GetBuildings()[i]->GetPosBuildX());
//				pBuildingInfo->SetAttribute( "BuildPosY",      Enemy->GetBuildings()[i]->GetPosBuildY());
//				pRoot->LinkEndChild( pBuildingInfo );
//			}
//			if(Enemy->GetBuildings()[i]->GetBuildingID() == CAPITOL)
//			{
//				TiXmlElement* pBuildingInfo = new TiXmlElement("undeadcapitalbuilding_info" );
//
//				pBuildingInfo->SetAttribute( "BuildPosX",      Enemy->GetBuildings()[i]->GetPosBuildX());
//				pBuildingInfo->SetAttribute( "BuildPosY",      Enemy->GetBuildings()[i]->GetPosBuildY());
//				pRoot->LinkEndChild( pBuildingInfo );
//			}
//			if(Enemy->GetBuildings()[i]->GetBuildingID() == WARFACTORY)
//			{
//				TiXmlElement* pBuildingInfo = new TiXmlElement("undeadwarfactorybuilding_info" );
//
//				pBuildingInfo->SetAttribute( "BuildPosX",      Enemy->GetBuildings()[i]->GetPosBuildX());
//				pBuildingInfo->SetAttribute( "BuildPosY",      Enemy->GetBuildings()[i]->GetPosBuildY());
//				pRoot->LinkEndChild( pBuildingInfo );
//			}
//		}
//
//#pragma endregion
//
//
//#pragma region PlayerUnits
//		for(unsigned int i = 0; i < Player->GetUnits().size() ; ++i)
//		{
//			if(Player->GetUnits()[i]->GetUnitID() == FOOTSOLDIERS)
//			{
//				TiXmlElement* pFootSoldierInfo = new TiXmlElement("footsoldier_info" );
//				pFootSoldierInfo->SetAttribute( "Country",        Player->GetUnits()[i]->GetCountry());
//				pFootSoldierInfo->SetAttribute( "Territory",      Player->GetUnits()[i]->GetTerritory() );
//				pRoot->LinkEndChild( pFootSoldierInfo );
//			}
//
//			if(Player->GetUnits()[i]->GetUnitID() == PRIEST)
//			{
//				TiXmlElement* pPriestInfo = new TiXmlElement("priest_info" );
//				pPriestInfo->SetAttribute( "Country",        Player->GetUnits()[i]->GetCountry());
//				pPriestInfo->SetAttribute( "Territory",      Player->GetUnits()[i]->GetTerritory() );
//				pRoot->LinkEndChild( pPriestInfo );
//			}
//
//			if(Player->GetUnits()[i]->GetUnitID() == WIZARD)
//			{
//				TiXmlElement* pWizardInfo = new TiXmlElement("wizard_info" );
//				pWizardInfo->SetAttribute( "Country",        Player->GetUnits()[i]->GetCountry());
//				pWizardInfo->SetAttribute( "Territory",      Player->GetUnits()[i]->GetTerritory() );
//				pRoot->LinkEndChild( pWizardInfo );
//			}
//
//			if(Player->GetUnits()[i]->GetUnitID() == CALVARY)
//			{
//				TiXmlElement* pCalvaryInfo = new TiXmlElement("calvary_info" );
//				pCalvaryInfo->SetAttribute( "Country",        Player->GetUnits()[i]->GetCountry());
//				pCalvaryInfo->SetAttribute( "Territory",      Player->GetUnits()[i]->GetTerritory() );
//				pRoot->LinkEndChild( pCalvaryInfo );
//			}
//
//			if(Player->GetUnits()[i]->GetUnitID() == CATAPULT)
//			{
//				TiXmlElement* pCatapultInfo = new TiXmlElement("catapult_info" );
//				pCatapultInfo->SetAttribute( "Country",        Player->GetUnits()[i]->GetCountry());
//				pCatapultInfo->SetAttribute( "Territory",      Player->GetUnits()[i]->GetTerritory() );
//				pRoot->LinkEndChild( pCatapultInfo );
//			}
//
//			if(Player->GetUnits()[i]->GetUnitID() == GENERAL)
//			{
//				TiXmlElement* pGeneralInfo = new TiXmlElement("general_info" );
//				pGeneralInfo->SetAttribute( "Country",        Player->GetUnits()[i]->GetCountry());
//				pGeneralInfo->SetAttribute( "Territory",      Player->GetUnits()[i]->GetTerritory() );
//				pRoot->LinkEndChild(pGeneralInfo );
//			}
//		}
//
//#pragma endregion
//
//#pragma region EnemyUnits
//		for(unsigned int i = 0; i < Enemy->GetUnits().size() ; ++i)
//		{
//			if(Enemy->GetUnits()[i]->GetUnitID() == SKELETALWARRIOR)
//			{
//				TiXmlElement* pSkeletalWarriorInfo = new TiXmlElement("skeletalwarrior_info" );
//				pSkeletalWarriorInfo->SetAttribute( "Country",        Enemy->GetUnits()[i]->GetCountry());
//				pSkeletalWarriorInfo->SetAttribute( "Territory",      Enemy->GetUnits()[i]->GetTerritory() );
//				pRoot->LinkEndChild( pSkeletalWarriorInfo );
//			}
//
//			if(Enemy->GetUnits()[i]->GetUnitID() == SHADOWPRIEST)
//			{
//				TiXmlElement* pShadowPriestInfo = new TiXmlElement("shadowpriest_info" );
//				pShadowPriestInfo->SetAttribute( "Country",        Enemy->GetUnits()[i]->GetCountry());
//				pShadowPriestInfo->SetAttribute( "Territory",      Enemy->GetUnits()[i]->GetTerritory() );
//				pRoot->LinkEndChild( pShadowPriestInfo );
//			}
//
//			if(Enemy->GetUnits()[i]->GetUnitID() == NECROMANCER)
//			{
//				TiXmlElement* pNecromancerInfo = new TiXmlElement("necromancer_info" );
//				pNecromancerInfo->SetAttribute( "Country",       Enemy->GetUnits()[i]->GetCountry());
//				pNecromancerInfo->SetAttribute( "Territory",     Enemy->GetUnits()[i]->GetTerritory() );
//				pRoot->LinkEndChild( pNecromancerInfo );
//			}
//
//			if(Enemy->GetUnits()[i]->GetUnitID() == DEATHKNIGHT)
//			{
//				TiXmlElement* pDeathKnightInfo = new TiXmlElement("deathknight_info" );
//				pDeathKnightInfo->SetAttribute( "Country",        Enemy->GetUnits()[i]->GetCountry());
//				pDeathKnightInfo->SetAttribute( "Territory",      Enemy->GetUnits()[i]->GetTerritory() );
//				pRoot->LinkEndChild( pDeathKnightInfo );
//			}
//
//			if(Enemy->GetUnits()[i]->GetUnitID() == WARMACHINE)
//			{
//				TiXmlElement* pWarmachineInfo = new TiXmlElement("warmachine_info" );
//				pWarmachineInfo->SetAttribute( "Country",        Enemy->GetUnits()[i]->GetCountry());
//				pWarmachineInfo->SetAttribute( "Territory",      Enemy->GetUnits()[i]->GetTerritory() );
//				pRoot->LinkEndChild( pWarmachineInfo );
//			}
//
//			if(Enemy->GetUnits()[i]->GetUnitID() == LICH)
//			{
//				TiXmlElement* pLichInfo = new TiXmlElement("lich_info" );
//				pLichInfo->SetAttribute( "Country",        Enemy->GetUnits()[i]->GetCountry());
//				pLichInfo->SetAttribute( "Territory",      Enemy->GetUnits()[i]->GetTerritory() );
//				pRoot->LinkEndChild(pLichInfo );
//			}
//		}
//
//
//#pragma endregion
//
//	}
//
//	// Save the XML Document to the file
//	doc.SaveFile( szFilename );
//#pragma endregion
//}
//
//void CSaveGame::SavePlayer3	( const char* szFilename, 
//							 const vector< tPlayerSaveInfo3 >&	vPlayers )
//{	
//#pragma region saveslo3
//	// Create a TinyXML Document
//	TiXmlDocument doc;
//
//
//	// Create the TinyXML Declaration (version)
//	TiXmlDeclaration* pDecl = new TiXmlDeclaration( "1.0", "utf-8", "" );
//
//	// Link the declaration to the document
//	doc.LinkEndChild( pDecl );
//
//
//	// Create a root node ("players_list")
//	TiXmlElement* pRoot = new TiXmlElement( "World" );
//
//	// Link the root to the document
//	doc.LinkEndChild( pRoot );
//
//	// Loop through the vector of players
//	for( unsigned int i = 0; i < vPlayers.size(); i++ )
//	{
//
//#pragma region PlayerOneInfo
//		// Create a node for the player info ("player_info")
//		TiXmlElement* pPlayer = new TiXmlElement("playerone_info" );
//
//
//		// Create a Text node for the player's name (stored between tags)
//		//pPlayer->SetAttribute("Name",vPlayers[i].szName);
//		TiXmlText* pText = new TiXmlText( vPlayers[ i ].szName );
//
//		pPlayer->SetAttribute( "player1wood",  Player->GetWood());
//		pPlayer->SetAttribute( "player1food",  Player->GetFood() );
//		pPlayer->SetAttribute( "player1steel", Player->GetSteel() );
//		// Link the text to the player node
//		pPlayer->LinkEndChild( pText );
//		// Link the player node to the root node
//		pRoot->LinkEndChild( pPlayer );
//
//#pragma endregion
//
//#pragma region PlayerTwoInfo
//
//		TiXmlElement* pPlayerTwo = new TiXmlElement("playertwo_info" );
//		pPlayerTwo->SetAttribute( "player2wood",  Enemy->GetWood());
//		pPlayerTwo->SetAttribute( "player2food",  Enemy->GetFood() );
//		pPlayerTwo->SetAttribute( "player2steel", Enemy->GetSteel() );
//		// Link the player node to the root node
//		pRoot->LinkEndChild( pPlayerTwo );
//
//
//#pragma endregion
//
//#pragma region WorldInfo
//
//		TiXmlElement* pWorldInfo = new TiXmlElement("world_info" );
//		pWorldInfo->SetAttribute( "GameMode",        vPlayer1[i].CAMPAIGN );
//		pWorldInfo->SetAttribute( "TurnOrder",       GamePlay->GetTurnOrder());
//		pWorldInfo->SetAttribute( "TurnNumber",      GamePlay->GetTurnNumber() );
//		pWorldInfo->SetAttribute( "TurnCap",         GamePlay->GetTurnCap() );
//		pWorldInfo->SetAttribute( "AiEnabled",       GamePlay->GetAiEnabled() );
//		pWorldInfo->SetAttribute( "CountryOneOwner", GamePlay->GetCountryOneOwner());
//		pWorldInfo->SetAttribute( "CounrtyTwoOwner", GamePlay->GetCountryTwoOwner() );
//		// Link the player node to the root node
//		pRoot->LinkEndChild( pWorldInfo );
//
//#pragma endregion
//
//
//#pragma region PlayerBuildinginfo
//		for (unsigned int i = 0; i < Player->GetBuildings().size();++i)
//		{
//			if(Player->GetBuildings()[i]->GetBuildingID() == FARM)
//			{
//				TiXmlElement* pBuildingInfo = new TiXmlElement("humanfarmbuilding_info" );
//				//pBuildingInfo->SetAttribute( "PosX",			 Player->GetBuildings()[i]->GetPosX());
//				//pBuildingInfo->SetAttribute( "PosY",			 Player->GetBuildings()[i]->GetPosY());
//				pBuildingInfo->SetAttribute( "BuildPosX",      Player->GetBuildings()[i]->GetPosBuildX());
//				pBuildingInfo->SetAttribute( "BuildPosY",      Player->GetBuildings()[i]->GetPosBuildY());
//				pRoot->LinkEndChild( pBuildingInfo );
//			}
//			if(Player->GetBuildings()[i]->GetBuildingID() == WOOD)
//			{
//				TiXmlElement* pBuildingInfo = new TiXmlElement("humanwoodbuilding_info" );
//				pBuildingInfo->SetAttribute( "PosX",			 (int)Player->GetBuildings()[i]->GetPosX());
//				pBuildingInfo->SetAttribute( "PosY",			 (int)Player->GetBuildings()[i]->GetPosY());
//				pBuildingInfo->SetAttribute( "BuildPosX",      Player->GetBuildings()[i]->GetPosBuildX());
//				pBuildingInfo->SetAttribute( "BuildPosY",      Player->GetBuildings()[i]->GetPosBuildY());
//				pRoot->LinkEndChild( pBuildingInfo );
//			}
//			if(Player->GetBuildings()[i]->GetBuildingID() == MINE)
//			{
//				TiXmlElement* pBuildingInfo = new TiXmlElement("humanminebuilding_info" );
//				pBuildingInfo->SetAttribute( "PosX",			 (int)Player->GetBuildings()[i]->GetPosX());
//				pBuildingInfo->SetAttribute( "PosY",			 (int)Player->GetBuildings()[i]->GetPosY());
//				pBuildingInfo->SetAttribute( "BuildPosX",      Player->GetBuildings()[i]->GetPosBuildX());
//				pBuildingInfo->SetAttribute( "BuildPosY",      Player->GetBuildings()[i]->GetPosBuildY());
//				pRoot->LinkEndChild( pBuildingInfo );
//			}
//			if(Player->GetBuildings()[i]->GetBuildingID() == BARRACKS)
//			{
//				TiXmlElement* pBuildingInfo = new TiXmlElement("humanbarracksbuilding_info" );
//				//pBuildingInfo->SetAttribute( "PosX",			 Player->GetBuildings()[i]->GetPosX());
//				//pBuildingInfo->SetAttribute( "PosY",			 Player->GetBuildings()[i]->GetPosY());
//				pBuildingInfo->SetAttribute( "BuildPosX",      Player->GetBuildings()[i]->GetPosBuildX());
//				pBuildingInfo->SetAttribute( "BuildPosY",      Player->GetBuildings()[i]->GetPosBuildY());
//				pRoot->LinkEndChild( pBuildingInfo );
//			}
//			if(Player->GetBuildings()[i]->GetBuildingID() == CAPITOL)
//			{
//				TiXmlElement* pBuildingInfo = new TiXmlElement("humancapitalbuilding_info" );
//				//pBuildingInfo->SetAttribute( "PosX",			 Player->GetBuildings()[i]->GetPosX());
//				//pBuildingInfo->SetAttribute( "PosY",			 Player->GetBuildings()[i]->GetPosY());
//				pBuildingInfo->SetAttribute( "BuildPosX",      Player->GetBuildings()[i]->GetPosBuildX());
//				pBuildingInfo->SetAttribute( "BuildPosY",      Player->GetBuildings()[i]->GetPosBuildY());
//				pRoot->LinkEndChild( pBuildingInfo );
//			}
//			if(Player->GetBuildings()[i]->GetBuildingID() == WARFACTORY)
//			{
//				TiXmlElement* pBuildingInfo = new TiXmlElement("humanwarfactorybuilding_info" );
//				//pBuildingInfo->SetAttribute( "PosX",			 Player->GetBuildings()[i]->GetPosX());
//				//pBuildingInfo->SetAttribute( "PosY",			 Player->GetBuildings()[i]->GetPosY());
//				pBuildingInfo->SetAttribute( "BuildPosX",      Player->GetBuildings()[i]->GetPosBuildX());
//				pBuildingInfo->SetAttribute( "BuildPosY",      Player->GetBuildings()[i]->GetPosBuildY());
//				pRoot->LinkEndChild( pBuildingInfo );
//			}
//		}
//
//
//
//
//
//
//
//
//
//
//#pragma endregion
//
//#pragma region Enemybuildinginfo
//
//		for (unsigned int i = 0; i < Enemy->GetBuildings().size();++i)
//		{
//			if(Enemy->GetBuildings()[i]->GetBuildingID() == FARM)
//			{
//				TiXmlElement* pBuildingInfo = new TiXmlElement("undeadfarmbuilding_info" );
//				//pBuildingInfo->SetAttribute( "PosX",			 Enemy->GetBuildings()[i]->GetPosX());
//				//pBuildingInfo->SetAttribute( "PosY",			 Enemy->GetBuildings()[i]->GetPosY());
//				pBuildingInfo->SetAttribute( "BuildPosX",      Enemy->GetBuildings()[i]->GetPosBuildX());
//				pBuildingInfo->SetAttribute( "BuildPosY",      Enemy->GetBuildings()[i]->GetPosBuildY());
//				pRoot->LinkEndChild( pBuildingInfo );
//			}
//			if(Enemy->GetBuildings()[i]->GetBuildingID() == WOOD)
//			{
//				TiXmlElement* pBuildingInfo = new TiXmlElement("undeadwoodbuilding_info" );
//				//pBuildingInfo->SetAttribute( "PosX",			 Enemy->GetBuildings()[i]->GetPosX());
//				//pBuildingInfo->SetAttribute( "PosY",			 Enemy->GetBuildings()[i]->GetPosY());
//				pBuildingInfo->SetAttribute( "BuildPosX",      Enemy->GetBuildings()[i]->GetPosBuildX());
//				pBuildingInfo->SetAttribute( "BuildPosY",      Enemy->GetBuildings()[i]->GetPosBuildY());
//				pRoot->LinkEndChild( pBuildingInfo );
//			}
//			if(Enemy->GetBuildings()[i]->GetBuildingID() == MINE)
//			{
//				TiXmlElement* pBuildingInfo = new TiXmlElement("undeadminebuilding_info" );
//
//				pBuildingInfo->SetAttribute( "BuildPosX",      Enemy->GetBuildings()[i]->GetPosBuildX());
//				pBuildingInfo->SetAttribute( "BuildPosY",      Enemy->GetBuildings()[i]->GetPosBuildY());
//				pRoot->LinkEndChild( pBuildingInfo );
//			}
//			if(Enemy->GetBuildings()[i]->GetBuildingID() == BARRACKS)
//			{
//				TiXmlElement* pBuildingInfo = new TiXmlElement("undeadbarracksbuilding_info" );
//
//				pBuildingInfo->SetAttribute( "BuildPosX",      Enemy->GetBuildings()[i]->GetPosBuildX());
//				pBuildingInfo->SetAttribute( "BuildPosY",      Enemy->GetBuildings()[i]->GetPosBuildY());
//				pRoot->LinkEndChild( pBuildingInfo );
//			}
//			if(Enemy->GetBuildings()[i]->GetBuildingID() == CAPITOL)
//			{
//				TiXmlElement* pBuildingInfo = new TiXmlElement("undeadcapitalbuilding_info" );
//
//				pBuildingInfo->SetAttribute( "BuildPosX",      Enemy->GetBuildings()[i]->GetPosBuildX());
//				pBuildingInfo->SetAttribute( "BuildPosY",      Enemy->GetBuildings()[i]->GetPosBuildY());
//				pRoot->LinkEndChild( pBuildingInfo );
//			}
//			if(Enemy->GetBuildings()[i]->GetBuildingID() == WARFACTORY)
//			{
//				TiXmlElement* pBuildingInfo = new TiXmlElement("undeadwarfactorybuilding_info" );
//
//				pBuildingInfo->SetAttribute( "BuildPosX",      Enemy->GetBuildings()[i]->GetPosBuildX());
//				pBuildingInfo->SetAttribute( "BuildPosY",      Enemy->GetBuildings()[i]->GetPosBuildY());
//				pRoot->LinkEndChild( pBuildingInfo );
//			}
//		}
//
//#pragma endregion
//
//
//#pragma region PlayerUnits
//		for(unsigned int i = 0; i < Player->GetUnits().size() ; ++i)
//		{
//			if(Player->GetUnits()[i]->GetUnitID() == FOOTSOLDIERS)
//			{
//				TiXmlElement* pFootSoldierInfo = new TiXmlElement("footsoldier_info" );
//				pFootSoldierInfo->SetAttribute( "Country",        Player->GetUnits()[i]->GetCountry());
//				pFootSoldierInfo->SetAttribute( "Territory",      Player->GetUnits()[i]->GetTerritory() );
//				pRoot->LinkEndChild( pFootSoldierInfo );
//			}
//
//			if(Player->GetUnits()[i]->GetUnitID() == PRIEST)
//			{
//				TiXmlElement* pPriestInfo = new TiXmlElement("priest_info" );
//				pPriestInfo->SetAttribute( "Country",        Player->GetUnits()[i]->GetCountry());
//				pPriestInfo->SetAttribute( "Territory",      Player->GetUnits()[i]->GetTerritory() );
//				pRoot->LinkEndChild( pPriestInfo );
//			}
//
//			if(Player->GetUnits()[i]->GetUnitID() == WIZARD)
//			{
//				TiXmlElement* pWizardInfo = new TiXmlElement("wizard_info" );
//				pWizardInfo->SetAttribute( "Country",        Player->GetUnits()[i]->GetCountry());
//				pWizardInfo->SetAttribute( "Territory",      Player->GetUnits()[i]->GetTerritory() );
//				pRoot->LinkEndChild( pWizardInfo );
//			}
//
//			if(Player->GetUnits()[i]->GetUnitID() == CALVARY)
//			{
//				TiXmlElement* pCalvaryInfo = new TiXmlElement("calvary_info" );
//				pCalvaryInfo->SetAttribute( "Country",        Player->GetUnits()[i]->GetCountry());
//				pCalvaryInfo->SetAttribute( "Territory",      Player->GetUnits()[i]->GetTerritory() );
//				pRoot->LinkEndChild( pCalvaryInfo );
//			}
//
//			if(Player->GetUnits()[i]->GetUnitID() == CATAPULT)
//			{
//				TiXmlElement* pCatapultInfo = new TiXmlElement("catapult_info" );
//				pCatapultInfo->SetAttribute( "Country",        Player->GetUnits()[i]->GetCountry());
//				pCatapultInfo->SetAttribute( "Territory",      Player->GetUnits()[i]->GetTerritory() );
//				pRoot->LinkEndChild( pCatapultInfo );
//			}
//
//			if(Player->GetUnits()[i]->GetUnitID() == GENERAL)
//			{
//				TiXmlElement* pGeneralInfo = new TiXmlElement("general_info" );
//				pGeneralInfo->SetAttribute( "Country",        Player->GetUnits()[i]->GetCountry());
//				pGeneralInfo->SetAttribute( "Territory",      Player->GetUnits()[i]->GetTerritory() );
//				pRoot->LinkEndChild(pGeneralInfo );
//			}
//		}
//
//#pragma endregion
//
//#pragma region EnemyUnits
//		for(unsigned int i = 0; i < Enemy->GetUnits().size() ; ++i)
//		{
//			if(Enemy->GetUnits()[i]->GetUnitID() == SKELETALWARRIOR)
//			{
//				TiXmlElement* pSkeletalWarriorInfo = new TiXmlElement("skeletalwarrior_info" );
//				pSkeletalWarriorInfo->SetAttribute( "Country",        Enemy->GetUnits()[i]->GetCountry());
//				pSkeletalWarriorInfo->SetAttribute( "Territory",      Enemy->GetUnits()[i]->GetTerritory() );
//				pRoot->LinkEndChild( pSkeletalWarriorInfo );
//			}
//
//			if(Enemy->GetUnits()[i]->GetUnitID() == SHADOWPRIEST)
//			{
//				TiXmlElement* pShadowPriestInfo = new TiXmlElement("shadowpriest_info" );
//				pShadowPriestInfo->SetAttribute( "Country",        Enemy->GetUnits()[i]->GetCountry());
//				pShadowPriestInfo->SetAttribute( "Territory",      Enemy->GetUnits()[i]->GetTerritory() );
//				pRoot->LinkEndChild( pShadowPriestInfo );
//			}
//
//			if(Enemy->GetUnits()[i]->GetUnitID() == NECROMANCER)
//			{
//				TiXmlElement* pNecromancerInfo = new TiXmlElement("necromancer_info" );
//				pNecromancerInfo->SetAttribute( "Country",       Enemy->GetUnits()[i]->GetCountry());
//				pNecromancerInfo->SetAttribute( "Territory",     Enemy->GetUnits()[i]->GetTerritory() );
//				pRoot->LinkEndChild( pNecromancerInfo );
//			}
//
//			if(Enemy->GetUnits()[i]->GetUnitID() == DEATHKNIGHT)
//			{
//				TiXmlElement* pDeathKnightInfo = new TiXmlElement("deathknight_info" );
//				pDeathKnightInfo->SetAttribute( "Country",        Enemy->GetUnits()[i]->GetCountry());
//				pDeathKnightInfo->SetAttribute( "Territory",      Enemy->GetUnits()[i]->GetTerritory() );
//				pRoot->LinkEndChild( pDeathKnightInfo );
//			}
//
//			if(Enemy->GetUnits()[i]->GetUnitID() == WARMACHINE)
//			{
//				TiXmlElement* pWarmachineInfo = new TiXmlElement("warmachine_info" );
//				pWarmachineInfo->SetAttribute( "Country",        Enemy->GetUnits()[i]->GetCountry());
//				pWarmachineInfo->SetAttribute( "Territory",      Enemy->GetUnits()[i]->GetTerritory() );
//				pRoot->LinkEndChild( pWarmachineInfo );
//			}
//
//			if(Enemy->GetUnits()[i]->GetUnitID() == LICH)
//			{
//				TiXmlElement* pLichInfo = new TiXmlElement("lich_info" );
//				pLichInfo->SetAttribute( "Country",        Enemy->GetUnits()[i]->GetCountry());
//				pLichInfo->SetAttribute( "Territory",      Enemy->GetUnits()[i]->GetTerritory() );
//				pRoot->LinkEndChild(pLichInfo );
//			}
//		}
//
//
//#pragma endregion
//
//	}
//
//	// Save the XML Document to the file
//	doc.SaveFile( szFilename );
//#pragma endregion
//}



bool	CSaveGame::LoadSlot(const wchar_t* szFilename,  vector< tPlayerSaveInfo >& vPlayers  )
{

#pragma region slot1
	// Create a TinyXML Document
	TiXmlDocument doc;
	char buffernamefile[128];
	wcstombs_s(nullptr,buffernamefile,128,szFilename,_TRUNCATE);
	// Attempt to load
	if( doc.LoadFile( buffernamefile ) == false )
		return false;


	// Access the root node ("players_list")
	TiXmlElement* pRoot = doc.RootElement();

	if( pRoot == nullptr )
		return false;

	// Iterate through the children nodes of the root ("player_info")
	TiXmlElement* pPlayer = pRoot->FirstChildElement( "Name" );

	while( pPlayer != nullptr )
	{
		// Read the info from the node
		tPlayerSaveInfo info = { };


		// Read the name (Text node)
		const char* pText = pPlayer->GetText();
		if( pText != nullptr )
			strcpy_s( info.szName, 32, pText );


		// Save the info into the vector
		vPlayers.push_back( info );


		// Move to the next sibling ("player_info")
		pPlayer = pPlayer->NextSiblingElement( "Name" );
	}


	// Did we load anything?
	if( vPlayers.size() > 0 )
		return true;

	return false;

#pragma endregion


}

void CSaveGame::SaveDefault	(  const wchar_t* szFilename)
{

#pragma region saveslot1
	// Create a TinyXML Document
	TiXmlDocument doc;


	// Create the TinyXML Declaration (version)
	TiXmlDeclaration* pDecl = new TiXmlDeclaration( "1.0", "utf-8", "" );

	// Link the declaration to the document
	doc.LinkEndChild( pDecl );



	// Create a root node ("players_list")
	TiXmlElement* pRoot = new TiXmlElement( "World" );

	// Link the root to the document
	doc.LinkEndChild( pRoot);

	// Loop through the vector of players
	for( unsigned int i = 0; i < 3; i++ )
	{
		TiXmlElement* pName = new TiXmlElement( "Name" );
		//pName->SetAttribute( "name",  vPlayers[ i ].szName);
		TiXmlText* pText = new TiXmlText( "" );
		pName->LinkEndChild( pText );
		pRoot->LinkEndChild( pName );

#pragma region PlayerOneInfo
		// Create a node for the player info ("player_info")
		TiXmlElement* pPlayer = new TiXmlElement("playerone_info" );


		// Create a Text node for the player's name (stored between tags)
		//pPlayer->SetAttribute("Name",vPlayers[i].szName);

		//vPlayers[i].szName = Namebuffer1;



		pPlayer->SetAttribute( "player1wood",  300);
		pPlayer->SetAttribute( "player1food",  100 );
		pPlayer->SetAttribute( "player1steel", 100 );
		// Link the text to the player node
		//pPlayer->LinkEndChild( pText );
		// Link the player node to the root node
		pName->LinkEndChild( pPlayer );

#pragma endregion

#pragma region PlayerTwoInfo

		TiXmlElement* pPlayerTwo = new TiXmlElement("playertwo_info" );
		pPlayerTwo->SetAttribute( "player2wood", 300);
		pPlayerTwo->SetAttribute( "player2food",  100 );
		pPlayerTwo->SetAttribute( "player2steel", 100 );
		// Link the player node to the root node
		pName->LinkEndChild( pPlayerTwo );


#pragma endregion

#pragma region WorldInfo

		TiXmlElement* pWorldInfo = new TiXmlElement("world_info" );
		pWorldInfo->SetAttribute( "GameMode",        0 );
		pWorldInfo->SetAttribute( "TurnOrder",       0);
		pWorldInfo->SetAttribute( "TurnNumber",      0 );
		pWorldInfo->SetAttribute( "TurnCap",         -1 );
		pWorldInfo->SetAttribute( "AiEnabled",       1 );
		pWorldInfo->SetAttribute( "CountryOneOwner", 0);
		pWorldInfo->SetAttribute( "CounrtyTwoOwner", 0 );



		// Link the player node to the root node
		pName->LinkEndChild( pWorldInfo );

#pragma endregion

#pragma region TerritoriesOwned


		TiXmlElement* pCountryOwner = new TiXmlElement("country_info" );

		pCountryOwner->SetAttribute( "Country", 0);
		pCountryOwner->SetAttribute( "Territory", 0);
		pCountryOwner->SetAttribute( "Owner", 0);


		pName->LinkEndChild( pCountryOwner );

		TiXmlElement* pCountryOwner2 = new TiXmlElement("country_info" );

		pCountryOwner2->SetAttribute( "Country", 1);
		pCountryOwner2->SetAttribute( "Territory", 0);
		pCountryOwner2->SetAttribute( "Owner", 2);


		pName->LinkEndChild( pCountryOwner2 );
		TiXmlElement* pCountryOwner3 = new TiXmlElement("country_info" );

		pCountryOwner3->SetAttribute( "Country", 2);
		pCountryOwner3->SetAttribute( "Territory", 0);
		pCountryOwner3->SetAttribute( "Owner", 1);


		pName->LinkEndChild( pCountryOwner3 );
		TiXmlElement* pCountryOwner4 = new TiXmlElement("country_info" );

		pCountryOwner4->SetAttribute( "Country", 3);
		pCountryOwner4->SetAttribute( "Territory", 0);
		pCountryOwner4->SetAttribute( "Owner", 0);

		pName->LinkEndChild( pCountryOwner4 );




#pragma endregion

#pragma region PlayerUnits

		for(int i = 0; i < 7; ++i)
		{
			TiXmlElement* pFootSoldierInfo = new TiXmlElement("footsoldier_info" );
			pFootSoldierInfo->SetAttribute( "Country",        2);
			pFootSoldierInfo->SetAttribute( "Territory",      0 );
			pName->LinkEndChild( pFootSoldierInfo );
		}


		TiXmlElement* pPriestInfo = new TiXmlElement("priest_info" );
		pPriestInfo->SetAttribute( "Country",        2);
		pPriestInfo->SetAttribute( "Territory",      0 );
		pName->LinkEndChild( pPriestInfo );



		TiXmlElement* pWizardInfo = new TiXmlElement("wizard_info" );
		pWizardInfo->SetAttribute( "Country",        2);
		pWizardInfo->SetAttribute( "Territory",      0 );
		pName->LinkEndChild( pWizardInfo );


		TiXmlElement* pCalvaryInfo = new TiXmlElement("calvary_info" );
		pCalvaryInfo->SetAttribute( "Country",       2);
		pCalvaryInfo->SetAttribute( "Territory",      0 );
		pName->LinkEndChild( pCalvaryInfo );



		TiXmlElement* pCatapultInfo = new TiXmlElement("catapult_info" );
		pCatapultInfo->SetAttribute( "Country",        2);
		pCatapultInfo->SetAttribute( "Territory",      0 );
		pName->LinkEndChild( pCatapultInfo );



		TiXmlElement* pGeneralInfo = new TiXmlElement("general_info" );
		pGeneralInfo->SetAttribute( "Country",        2);
		pGeneralInfo->SetAttribute( "Territory",      0 );
		pName->LinkEndChild(pGeneralInfo );



#pragma endregion

#pragma region EnemyUnits


		for(int i = 0; i < 7; ++i)
		{
			TiXmlElement* pSkeletalWarriorInfo = new TiXmlElement("skeletalwarrior_info" );
			pSkeletalWarriorInfo->SetAttribute( "Country",        1);
			pSkeletalWarriorInfo->SetAttribute( "Territory",     0 );
			pName->LinkEndChild( pSkeletalWarriorInfo );
		}


		TiXmlElement* pShadowPriestInfo = new TiXmlElement("shadowpriest_info" );
		pShadowPriestInfo->SetAttribute( "Country",        1);
		pShadowPriestInfo->SetAttribute( "Territory",     0 );
		pName->LinkEndChild( pShadowPriestInfo );



		TiXmlElement* pNecromancerInfo = new TiXmlElement("necromancer_info" );
		pNecromancerInfo->SetAttribute( "Country",       1);
		pNecromancerInfo->SetAttribute( "Territory",     0 );
		pName->LinkEndChild( pNecromancerInfo );



		TiXmlElement* pDeathKnightInfo = new TiXmlElement("deathknight_info" );
		pDeathKnightInfo->SetAttribute( "Country",        1);
		pDeathKnightInfo->SetAttribute( "Territory",      0 );
		pName->LinkEndChild( pDeathKnightInfo );



		TiXmlElement* pWarmachineInfo = new TiXmlElement("warmachine_info" );
		pWarmachineInfo->SetAttribute( "Country",        1);
		pWarmachineInfo->SetAttribute( "Territory",      0 );
		pName->LinkEndChild( pWarmachineInfo );



		TiXmlElement* pLichInfo = new TiXmlElement("lich_info" );
		pLichInfo->SetAttribute( "Country",       1);
		pLichInfo->SetAttribute( "Territory",      0 );
		pName->LinkEndChild(pLichInfo );




#pragma endregion

	}

	char buffernamefile[128];
	wcstombs_s(nullptr,buffernamefile,128,szFilename,_TRUNCATE);
	// Save the XML Document to the file
	//std::wstring namefile = szFilename;
	doc.SaveFile(buffernamefile);

#pragma endregion		




}
bool CSaveGame::LoadSlotGame		(const wchar_t* szFilename,  vector< tPlayerSaveInfo >& vPlayers ,int m_nCursor )
{
	
	//GamePlay->Enter();
	// Create a TinyXML Document
	TiXmlDocument doc;
	char buffernamefile[128];
	wcstombs_s(nullptr,buffernamefile,128,szFilename,_TRUNCATE);
	// Attempt to load
	if( doc.LoadFile( buffernamefile ) == false )
		return false;


	// Access the root node ("players_list")
	TiXmlElement* pRoot = doc.RootElement();

	if( pRoot == nullptr )
		return false;

	//vPlayer1.clear();


	TiXmlElement* pName				    = pRoot->FirstChildElement( "Name" );
	for(int i = 0; i < m_nCursor; ++i,pName = pName->NextSiblingElement("Name")); // update element as well

	TiXmlElement* pPlayer1				= pName->FirstChildElement( "playerone_info" );
	TiXmlElement* pPlayer2				= pName->FirstChildElement( "playertwo_info" );
	TiXmlElement* pWorld				= pName->FirstChildElement( "world_info" );
	TiXmlElement* pCountry				= pName->FirstChildElement( "country_info" );
	TiXmlElement* pFootSoldier			= pName->FirstChildElement( "footsoldier_info" );
	TiXmlElement* pPriest				= pName->FirstChildElement( "priest_info" );
	TiXmlElement* pWizard				= pName->FirstChildElement( "wizard_info" );
	TiXmlElement* pCalvary				= pName->FirstChildElement( "calvary_info" );
	TiXmlElement* pCatapult				= pName->FirstChildElement( "catapult_info" );
	TiXmlElement* pGeneral				= pName->FirstChildElement( "general_info" );
	TiXmlElement* pSkeletalWarrior		= pName->FirstChildElement( "skeletalwarrior_info" );
	TiXmlElement* pNecromancer			= pName->FirstChildElement( "necromancer_info" );
	TiXmlElement* pShadowpriest			= pName->FirstChildElement( "shadowpriest_info" );
	TiXmlElement* pWarmachine			= pName->FirstChildElement( "warmachine_info" );
	TiXmlElement* pDeathknight			= pName->FirstChildElement( "deathknight_info" );
	TiXmlElement* pLich					= pName->FirstChildElement( "lich_info" );

	TiXmlElement* pHBarracks			= pName->FirstChildElement( "humanbarracksbuilding_info" );
	TiXmlElement* pHCapital				= pName->FirstChildElement( "humancaptialbuilding_info" );
	TiXmlElement* pHMine				= pName->FirstChildElement( "humanminebuilding_info" );
	TiXmlElement* pHFarm				= pName->FirstChildElement( "humanfarmbuilding_info" );
	TiXmlElement* pHWood				= pName->FirstChildElement( "humanwoodbuilding_info" );
	TiXmlElement* pHWarfactory			= pName->FirstChildElement( "humanwarfactorybuilding_info" );
	TiXmlElement* pUBarracks			= pName->FirstChildElement( "undeadbarracksbuilding_info" );
	TiXmlElement* pUCapital				= pName->FirstChildElement( "undeadcaptialbuilding_info" );
	TiXmlElement* pUMine				= pName->FirstChildElement( "undeadminebuilding_info" );
	TiXmlElement* pUFarm				= pName->FirstChildElement( "undeadfarmbuilding_info" );
	TiXmlElement* pUWood				= pName->FirstChildElement( "undeadwoodbuilding_info" );
	TiXmlElement* pUWarfactory			= pName->FirstChildElement( "undeadwarfactorybuilding_info" );


	tPlayerSaveInfo info = { };


	const char* pText = pName->GetText();
		if( pText != nullptr )
			strcpy_s( info.szName, 32, pText );

		
#pragma region LoadingPlayersinfo
	if( pPlayer1 != nullptr )
	{
		// Read the info from the node


		// Read the name (Text node)

		pPlayer1->Attribute( "player1wood", &info.player1wood );
		pPlayer1->Attribute( "player1food", &info.player1food );
		pPlayer1->Attribute( "player1steel", &info.player1steel );	

		Player->SetWood(info.player1wood);
		Player->SetFood(info.player1food);
		Player->SetSteel(info.player1steel);

	}
	if(pPlayer2 != nullptr)
	{
		// Read the info from the node
		pPlayer2->Attribute( "player2wood", &info.player2wood );
		pPlayer2->Attribute( "player2food", &info.player2food );
		pPlayer2->Attribute( "player2steel", &info.player2steel );

		Enemy->SetWood(info.player2wood);
		Enemy->SetFood(info.player2food);
		Enemy->SetSteel(info.player2steel);
	}

#pragma endregion

#pragma region LoadingWorldInfo
	if( pWorld != nullptr )
	{
		// Read the info from the node
		//int gamemode;
		//int turncap;
		//int aienabled;

		pWorld->Attribute( "GameMode", &info.gamemode );
		pWorld->Attribute( "TurnOrder", &info.m_nTurnOrder );
		pWorld->Attribute( "TurnNumber", &info.m_nTurnNumber );
		pWorld->Attribute( "TurnCap", &info.turncap );
		pWorld->Attribute( "AiEnabled", &info.aienabled );
		pWorld->Attribute( "CountryOneOwner", &info.m_nCountryOneOwner );
		pWorld->Attribute( "CounrtyTwoOwner", &info.m_nCountryTwoOwner );


		GamePlay->SetTurnOrder(info.m_nTurnOrder);
		GamePlay->SetTurnNumber(info.m_nTurnNumber);
		GamePlay->SetTurnCap(info.turncap);

		if(info.aienabled == 0)
			GamePlay->SetAiEnabled(false);
		else
			GamePlay->SetAiEnabled(true);

		GamePlay->SetCountryOneOwner(info.m_nCountryOneOwner);
		GamePlay->SetCountryTwoOwner(info.m_nCountryTwoOwner);


	}






#pragma endregion

#pragma region LoadingCountryowner

	while(pCountry != nullptr)
	{
		int country;
		int territory;
		int owner; 

		pCountry->Attribute( "Country", &country );
		pCountry->Attribute( "Territory", &territory );
		pCountry->Attribute( "Owner", &owner );
		//GamePlay->GetWorld()->GetGrid()[country][territory]->SetOwner(owner);
		pCountry = pCountry->NextSiblingElement( "country_info" );

	}




#pragma endregion


#pragma region Loading Units


	




#pragma region HumanUnits

	while(pFootSoldier != nullptr)
	{
		int Country;
		int Territory;
		info.soldierCount++;

		// Read the info from the node
		pFootSoldier->Attribute( "Country", &Country );
		pFootSoldier->Attribute( "Territory", &Territory );

		

		Country = 0;
		Territory = 0;

		pFootSoldier = pFootSoldier->NextSiblingElement( "footsoldier_info" );

	}

	while(pPriest != nullptr)
	{
		int Country;
		int Territory;
		info.priestCount++;
		// Read the info from the node
		pPriest->Attribute( "Country", &Country );
		pPriest->Attribute( "Territory", &Territory );

		

		Country = 0;
		Territory = 0;

		// Move to the next sibling ("player_info")
		pPriest = pPriest->NextSiblingElement( "priest_info" );

	}

	while(pWizard != nullptr)
	{
		int Country;
		int Territory;
		info.wizardCount++;
		// Read the info from the node
		pWizard->Attribute( "Country", &Country );
		pWizard->Attribute( "Territory", &Territory );

		
		Country = 0;
		Territory = 0;

		// Move to the next sibling ("player_info")
		pWizard = pWizard->NextSiblingElement( "wizard_info" );

	}

	while(pCalvary != nullptr)
	{
		int Country;
		int Territory;
		info.cavalryCount++;
		// Read the info from the node
		pCalvary->Attribute( "Country", &Country );
		pCalvary->Attribute( "Territory", &Territory );

		

		Country = 0;
		Territory = 0;

		// Move to the next sibling ("player_info")
		pCalvary = pCalvary->NextSiblingElement( "calvary_info" );

	}

	while(pCatapult != nullptr)
	{
		int Country;
		int Territory;
		info.catapultCount++;
		// Read the info from the node
		pCatapult->Attribute( "Country", &Country );
		pCatapult->Attribute( "Territory", &Territory );

		

		Country = 0;
		Territory = 0;

		// Move to the next sibling ("player_info")
		pCatapult = pCatapult->NextSiblingElement( "catapult_info" );

	}

	while(pGeneral != nullptr)
	{
		int Country;
		int Territory;
		info.generalPresent = true;
		// Read the info from the node
		pGeneral->Attribute( "Country", &Country );
		pGeneral->Attribute( "Territory", &Territory );

		

		Country = 0;
		Territory = 0;

		// Move to the next sibling ("player_info")
		pGeneral = pGeneral->NextSiblingElement( "general_info" );

	}

#pragma endregion

#pragma region UndeadUnits
	while(pSkeletalWarrior != nullptr)
	{
		int Country;
		int Territory;
		info.skeletonCount++;
		// Read the info from the node
		pSkeletalWarrior->Attribute( "Country", &Country );
		pSkeletalWarrior->Attribute( "Territory", &Territory );

		

		Country = 0;
		Territory = 0;

		// Move to the next sibling ("player_info")
		pSkeletalWarrior = pSkeletalWarrior->NextSiblingElement( "skeletalwarrior_info" );

	}

	while(pNecromancer != nullptr)
	{
		int Country;
		int Territory;
		info.necroCount++;
		// Read the info from the node
		pNecromancer->Attribute( "Country", &Country );
		pNecromancer->Attribute( "Territory", &Territory );

		
		Country = 0;
		Territory = 0;

		// Move to the next sibling ("player_info")
		pNecromancer = pNecromancer->NextSiblingElement( "necromancer_info" );

	}

	while(pShadowpriest != nullptr)
	{
		int Country;
		int Territory;
		info.spriestCount++;
		// Read the info from the node
		pShadowpriest->Attribute( "Country", &Country );
		pShadowpriest->Attribute( "Territory", &Territory );

		
		Country = 0;
		Territory = 0;

		// Move to the next sibling ("player_info")
		pShadowpriest = pShadowpriest->NextSiblingElement( "shadowpriest_info" );

	}

	while(pWarmachine != nullptr)
	{
		int Country;
		int Territory;
		info.wmCount++;
		// Read the info from the node
		pWarmachine->Attribute( "Country", &Country );
		pWarmachine->Attribute( "Territory", &Territory );

		
		Country = 0;
		Territory = 0;

		// Move to the next sibling ("player_info")
		pWarmachine = pWarmachine->NextSiblingElement( "warmachine_info" );

	}

	while(pDeathknight != nullptr)
	{
		int Country;
		int Territory;
		info.dkCount++;
		// Read the info from the node
		pDeathknight->Attribute( "Country", &Country );
		pDeathknight->Attribute( "Territory", &Territory );

		

		Country = 0;
		Territory = 0;

		// Move to the next sibling ("player_info")
		pDeathknight = pDeathknight->NextSiblingElement( "deathknight_info" );

	}

	while(pLich != nullptr)
	{
		int Country;
		int Territory;
		info.lichPresent = true;
		// Read the info from the node
		pLich->Attribute( "Country", &Country );
		pLich->Attribute( "Territory", &Territory );

		
		Country = 0;
		Territory = 0;

		// Move to the next sibling ("player_info")
		pLich = pLich->NextSiblingElement( "lich_info" );

	}

#pragma endregion


#pragma endregion


#pragma region LoadingBuildings

#pragma region Humanbuildings
	while(pHBarracks != nullptr)
	{

		int BuildPosX;
		int BuildPosY;

		pHBarracks->Attribute("BuildPosX", &BuildPosX);
		pHBarracks->Attribute("BuildPosY", &BuildPosY);

		
		pHBarracks = pHBarracks->NextSiblingElement( "humanbarracksbuilding_info" );

	}

	while(pHCapital != nullptr)
	{

		int BuildPosX;
		int BuildPosY;


		pHCapital->Attribute("BuildPosX", &BuildPosX);
		pHCapital->Attribute("BuildPosY", &BuildPosY);

		
		pHCapital = pHCapital->NextSiblingElement( "humancapitalbuilding_info" );

	}
	while(pHWood != nullptr)
	{

		int BuildPosX;
		int BuildPosY;


		pHWood->Attribute("BuildPosX", &BuildPosX);
		pHWood->Attribute("BuildPosY", &BuildPosY);

		pHWood = pHWood->NextSiblingElement( "humanwoodbuilding_info" );

	}
	while(pHFarm != nullptr)
	{

		int BuildPosX;
		int BuildPosY;


		pHFarm->Attribute("BuildPosX", &BuildPosX);
		pHFarm->Attribute("BuildPosY", &BuildPosY);

		
		pHFarm = pHFarm->NextSiblingElement( "humanfarmbuilding_info" );

	}
	while(pHMine != nullptr)
	{

		int BuildPosX;
		int BuildPosY;


		pHMine->Attribute("BuildPosX", &BuildPosX);
		pHMine->Attribute("BuildPosY", &BuildPosY);

		
		pHMine = pHMine->NextSiblingElement( "humanminebuilding_info" );

	}
	while(pHWarfactory != nullptr)
	{

		int BuildPosX;
		int BuildPosY;


		pHWarfactory->Attribute("BuildPosX", &BuildPosX);
		pHWarfactory->Attribute("BuildPosY", &BuildPosY);

		
		pHWarfactory = pHWarfactory->NextSiblingElement( "humanwarfactorybuilding_info" );

	}

#pragma endregion


#pragma region Undeadbuildings
	while(pUBarracks != nullptr)
	{

		int BuildPosX;
		int BuildPosY;


		pUBarracks->Attribute("BuildPosX", &BuildPosX);
		pUBarracks->Attribute("BuildPosY", &BuildPosY);

		pUBarracks = pUBarracks->NextSiblingElement( "undeadbarracksbuilding_info" );

	}

	while(pUCapital != nullptr)
	{

		int BuildPosX;
		int BuildPosY;


		pUCapital->Attribute("BuildPosX", &BuildPosX);
		pUCapital->Attribute("BuildPosY", &BuildPosY);

		
		pUCapital = pUCapital->NextSiblingElement( "undeadcapitalbuilding_info" );

	}
	while(pUWood != nullptr)
	{

		int BuildPosX;
		int BuildPosY;


		pUWood->Attribute("BuildPosX", &BuildPosX);
		pUWood->Attribute("BuildPosY", &BuildPosY);

		pUWood = pUWood->NextSiblingElement( "undeadwoodbuilding_info" );

	}
	while(pUFarm != nullptr)
	{

		int BuildPosX;
		int BuildPosY;


		pUFarm->Attribute("BuildPosX", &BuildPosX);
		pUFarm->Attribute("BuildPosY", &BuildPosY);

		pUFarm = pUFarm->NextSiblingElement( "undeadfarmbuilding_info" );

	}
	while(pUMine != nullptr)
	{

		int BuildPosX;
		int BuildPosY;


		pUMine->Attribute("BuildPosX", &BuildPosX);
		pUMine->Attribute("BuildPosY", &BuildPosY);

		pUMine = pUMine->NextSiblingElement( "undeadminebuilding_info" );

	}
	while(pUWarfactory != nullptr)
	{

		int BuildPosX;
		int BuildPosY;


		pUWarfactory->Attribute("BuildPosX", &BuildPosX);
		pUWarfactory->Attribute("BuildPosY", &BuildPosY);

		pUMine = pUMine->NextSiblingElement( "undeadwarfactorybuilding_info" );

	}

#pragma endregion


#pragma endregion

	// Save the info into the vector
	//vPlayer1.push_back( info );
	vPlayers[m_nCursor] = info;
	//pName = pName->NextSiblingElement( "Name" );
	//}
	//}

	//}
	// Did we load anything?
	if( vPlayers.size() > 0 )
		return true;


	return false;


}
