////////////////////////////////////////////////////
// File:	CLoadGame.cpp
// Purpose:	Handles all the Loading information.
//			
//
// Author:	Roberto Crespo (Cresic95@fullsail.edu)
// Course:	SGP 1310
////////////////////////////////////////////////////
#include "LoadGame.h"
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
#include "BaseUnit.h"
#include "Enemy.h"
#include "Player.h"
#include "BaseBuilding.h"
#include "Farm.h"
#include "Capitol.h"
#include "Barracks.h"
#include "WoodMill.h"
#include "Mine.h"
#include "WarFactory.h"
#include "MainMenu.h"
#include "ArcadeGamePlay.h"
#include "TinyXML\tinyxml.h"
#include "MainMenu.h"
#include "TileSystem\WorldGrid.h"
#include <Shlobj.h>


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



////////////////////////////////////////////////////
//Singleton
CLoadGame* CLoadGame::GetInstance( void )
{
	//Global Instance
	static CLoadGame s_Instance;
	return &s_Instance;
}



//Functions for every state
bool	CLoadGame::Input			( )					 
{
	CSGD_DirectInput*	pDI		=	CSGD_DirectInput::GetInstance();
	CSGD_Direct3D*		p3D		=	CSGD_Direct3D::GetInstance();

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

	if( ( pDI->KeyPressed( DIK_TAB ) || pDI->JoystickButtonPressed( 1, JOYSTICKID1 ) ) || (pDI->KeyPressed( DIK_RETURN ) || pDI->JoystickButtonPressed( 1, JOYSTICKID2 )  ) )
	{
		// What selection?
		switch( m_nCursor )
		{

		case 0:
			{
				TCHAR * wszPath = NULL;

				HRESULT hr = SHGetKnownFolderPath( FOLDERID_LocalAppData, 0, 0, &wszPath);

				
				CMainMenu::GetInstance()->SetCampaign(true);
				CMainMenu::GetInstance()->SetInGame(true);
				CGame::GetInstance()->ChangeState( CGamePlay::GetInstance() );
			

				if(SUCCEEDED(hr))
				{
					std::wstring name = wszPath;
					name += L"\\ArchaicDomination\\Slots.xml";
					LoadSlotGame(name.c_str(),vPlayer1, m_nCursor );
				}
				else 
				{
					CMainMenu::GetInstance()->SetCampaign(false);
					CMainMenu::GetInstance()->SetInGame(false);
					CGame::GetInstance()->PopState();
				}



			}
			break;

		case 1:
			{

				TCHAR * wszPath = NULL;

				HRESULT hr = SHGetKnownFolderPath( FOLDERID_LocalAppData, 0, 0, &wszPath);
				
				CMainMenu::GetInstance()->SetCampaign(true);
				CMainMenu::GetInstance()->SetInGame(true);
				CGame::GetInstance()->ChangeState( CGamePlay::GetInstance() );
			

				if(SUCCEEDED(hr))
				{
					std::wstring name = wszPath;
					name += L"\\ArchaicDomination\\Slots.xml";
					LoadSlotGame(name.c_str(),vPlayer1, m_nCursor );
				}
				else 
				{
					CMainMenu::GetInstance()->SetCampaign(false);
					CMainMenu::GetInstance()->SetInGame(false);
					CGame::GetInstance()->PopState();
				}
				
			}
			break;

		case 2:
			{
				TCHAR * wszPath = NULL;

				HRESULT hr = SHGetKnownFolderPath( FOLDERID_LocalAppData, 0, 0, &wszPath);

				

				CMainMenu::GetInstance()->SetCampaign(true);
				CMainMenu::GetInstance()->SetInGame(true);
				CGame::GetInstance()->ChangeState( CGamePlay::GetInstance() );
			

				if(SUCCEEDED(hr))
				{
					std::wstring name = wszPath;
					name += L"\\ArchaicDomination\\Slots.xml";
					LoadSlotGame(name.c_str(),vPlayer1, m_nCursor );
				}
				else 
				{
					CMainMenu::GetInstance()->SetCampaign(false);
					CMainMenu::GetInstance()->SetInGame(false);
					CGame::GetInstance()->PopState();
				}
				
				
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
	}

	///Escape to exit
	if (pDI->JoystickButtonPressed( 3, 0 ) || pDI->JoystickButtonPressed( 3, 1 ) )
	{
		m_nCursor = 3;
		CGame::GetInstance()->PlayCancelSFX();
		return true;
	}
	else if (pDI->KeyPressed( DIK_ESCAPE ) == true || pDI->JoystickButtonPressed( 2, JOYSTICKID1 ) == true)
	{
		m_nCursor = 3;
		CGame::GetInstance()->PlayCancelSFX();
		return true;
	}

	// Keep running
	return true;

}
void	CLoadGame::Update			( float fElapsedTime ) 
{
}
void	CLoadGame::Render			( )			
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

		RECT rect = {0,0,400,200};

		pTM->Draw( m_nSlotImageID, 40, 140);

		pTM->Draw( m_nSlotImageID, 40, 260);

		pTM->Draw( m_nSlotImageID, 40, 380);

		pFont->Draw( _T("Load" ),340, 10,2.0f, Blue);





#pragma region Name of slots
		TOSTRINGSTREAM sname1;

		sname1 << vPlayer1[0].szName;

		pFont->Draw(sname1.str().c_str(), 300, 170 + (0),1.0f, White );


		TOSTRINGSTREAM sname2;

		sname2 << vPlayer1[1].szName;

		pFont->Draw(sname2.str().c_str(), 300, 170 + (120),1.0f, White );

		TOSTRINGSTREAM sname3;

		sname3 << vPlayer1[2].szName;

		pFont->Draw(sname3.str().c_str(), 300, 170 + (240),1.0f, White );

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
		/*rSelection.left		= 700;
		rSelection.top		= 550;
		rSelection.right	= 720;
		rSelection.bottom	= 570;
		m_D3D->DrawRect( rSelection, Red );*/
		pTM->Draw( m_nCursorID, 550, 545);
		//pFont->Draw( _T("->"),685, 545, 1.0f, Red );	
	}

#pragma endregion

}
void	CLoadGame::Enter			( )		
{
	//Count
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

	//ImageIds
	m_nBackgroundImageID =  CSGD_TextureManager::GetInstance()->LoadTexture( _T( "resources/graphics/SaveLoadBackground.png") );
	m_nSlotImageID = CSGD_TextureManager::GetInstance()->LoadTexture( _T( "resources/graphics/Slot.png") );
	m_nCursorID = CSGD_TextureManager::GetInstance()->LoadTexture( _T( "resources/graphics/Cursor_MainMenu.png") );
	//Cursor Image ID Set
	cursorImageID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/Cursor_Battle.png"));
	//load names of files

	TCHAR * wszPath = NULL;

	HRESULT hr = SHGetKnownFolderPath( FOLDERID_LocalAppData, 0, 0, &wszPath);


	if(SUCCEEDED(hr))
	{
		std::wstring name = wszPath;
		name += L"\\ArchaicDomination\\Slots.xml";

		DWORD path = GetFileAttributes(name.c_str());

		if(path == INVALID_FILE_ATTRIBUTES)
		{

			//name += L"\\Slots.xml";
			SaveDefault(name.c_str());
			LoadSlotName(name.c_str(), vPlayer1);
		}
		else
		{
			//std::wstring namefile = wszPath;
			//namefile += L"\\ArchaicDomination\\Slots.xml";
			LoadSlotName(name.c_str(), vPlayer1);
		}






	}

	//LoadSlotName2("Slot2.xml", vPlayer2);
	//LoadSlotName3("Slot3.xml", vPlayer3);
}
void	CLoadGame::Exit			( )	
{

}


bool	CLoadGame::LoadSlotName(const wchar_t* szFilename,  vector< tPlayerLoadInfo >& vPlayer1  )
{
	vPlayer1.clear();
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
		tPlayerLoadInfo info = { };

		// Read the name (Text node)
		const char* pText = pPlayer->GetText();
		if( pText != nullptr )
			strcpy_s( info.szName, 32, pText );


		// Save the info into the vector
		vPlayer1.push_back( info );


		// Move to the next sibling ("player_info")
		pPlayer = pPlayer->NextSiblingElement( "Name" );
	}


	// Did we load anything?
	if( vPlayer1.size() > 0 )
		return true;

	return false;

#pragma endregion


}
//bool	CLoadGame::LoadSlotName(const char* szFilename,  vector< tPlayerLoadInfo >& vPlayer2  )
//{
//
//#pragma region slot2
//	// Create a TinyXML Document
//	TiXmlDocument doc;
//
//	// Attempt to load
//	if( doc.LoadFile( szFilename ) == false )
//		return false;
//
//
//	// Access the root node ("players_list")
//	TiXmlElement* pRoot = doc.RootElement();
//
//	if( pRoot == nullptr )
//		return false;
//
//
//	// Clear out the vector
//	//vPlayers.clear();
//
//
//	// Iterate through the children nodes of the root ("player_info")
//	TiXmlElement* pPlayer = pRoot->FirstChildElement( "playerone_info" );
//
//	while( pPlayer != nullptr )
//	{
//		// Read the info from the node
//		tPlayerLoadInfo2 info = { };
//
//
//		// Read the name (Text node)
//		const char* pText = pPlayer->GetText();
//		if( pText != nullptr )
//			strcpy_s( info.szName, 32, pText );
//
//		// Save the info into the vector
//		vPlayer2.push_back( info );
//
//
//		// Move to the next sibling ("player_info")
//		pPlayer = pPlayer->NextSiblingElement( "playerone_info" );
//	}
//
//	// Did we load anything?
//	if( vPlayer2.size() > 0 )
//		return true;
//
//	return false;
//#pragma endregion
//
//
//}
//bool	CLoadGame::LoadSlotName3(const char* szFilename,  vector< tPlayerLoadInfo3 >& vPlayer3  )
//{
//
//#pragma region slot3
//	// Create a TinyXML Document
//	TiXmlDocument doc;
//
//	// Attempt to load
//	if( doc.LoadFile( szFilename ) == false )
//		return false;
//
//
//	// Access the root node ("players_list")
//	TiXmlElement* pRoot = doc.RootElement();
//
//	if( pRoot == nullptr )
//		return false;
//
//
//	// Clear out the vector
//	//vPlayers.clear();
//
//
//	// Iterate through the children nodes of the root ("player_info")
//	TiXmlElement* pPlayer = pRoot->FirstChildElement( "playerone_info" );
//
//	while( pPlayer != nullptr )
//	{
//		// Read the info from the node
//		tPlayerLoadInfo3 info = { };
//
//
//		// Read the name (Text node)
//		const char* pText = pPlayer->GetText();
//		if( pText != nullptr )
//			strcpy_s( info.szName, 32, pText );
//
//
//		// Save the info into the vector
//		vPlayer3.push_back( info );
//
//
//		// Move to the next sibling ("player_info")
//		pPlayer = pPlayer->NextSiblingElement( "playerone_info" );
//	}
//
//
//	// Did we load anything?
//	if( vPlayer3.size() > 0 )
//		return true;
//
//	return false;
//#pragma endregion
//
//}




bool CLoadGame::LoadSlotGame		(const wchar_t* szFilename,  vector< tPlayerLoadInfo >& vPlayer1 ,int m_nCursor )
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


	tPlayerLoadInfo info = { };




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
		GamePlay->GetWorld()->GetGrid()[country][territory]->SetOwner(owner);
		pCountry = pCountry->NextSiblingElement( "country_info" );

	}




#pragma endregion


#pragma region Loading Units


	Player->ClearPlayerinfo();
	Enemy->ClearEnemyinfo();




#pragma region HumanUnits

	while(pFootSoldier != nullptr)
	{
		int Country;
		int Territory;
		info.soldierCount++;

		// Read the info from the node
		pFootSoldier->Attribute( "Country", &Country );
		pFootSoldier->Attribute( "Territory", &Territory );

		CBaseUnit* footsoldier = new CBaseUnit();
		footsoldier->CreateUnit(FOOTSOLDIERS,Country, Territory);
		Player->AddUnit(footsoldier);


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

		CBaseUnit* priest = new CBaseUnit();
		priest->CreateUnit(PRIEST,Country, Territory);
		Player->AddUnit(priest);

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

		CBaseUnit* wizard = new CBaseUnit();
		wizard->CreateUnit(WIZARD,Country, Territory);
		Player->AddUnit(wizard);

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

		CBaseUnit* calvary = new CBaseUnit();
		calvary->CreateUnit(CALVARY,Country, Territory);
		Player->AddUnit(calvary);

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

		CBaseUnit* catapult = new CBaseUnit();
		catapult->CreateUnit(CATAPULT,Country, Territory);
		Player->AddUnit(catapult);

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

		CBaseUnit* general = new CBaseUnit();
		general->CreateUnit(GENERAL,Country, Territory);
		Player->AddUnit(general);

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

		CBaseUnit* skeletalwarrior = new CBaseUnit();
		skeletalwarrior->CreateUnit(SKELETALWARRIOR,Country, Territory);
		Enemy->AddUnit(skeletalwarrior);

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

		CBaseUnit* necromancer = new CBaseUnit();
		necromancer->CreateUnit(NECROMANCER,Country, Territory);
		Enemy->AddUnit(necromancer);

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

		CBaseUnit* shadowpriest = new CBaseUnit();
		shadowpriest->CreateUnit(SHADOWPRIEST,Country, Territory);
		Enemy->AddUnit(shadowpriest);

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

		CBaseUnit* warmachine = new CBaseUnit();
		warmachine->CreateUnit(WARMACHINE,Country, Territory);
		Enemy->AddUnit(warmachine);

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

		CBaseUnit* deathknight = new CBaseUnit();
		deathknight->CreateUnit(DEATHKNIGHT,Country, Territory);
		Enemy->AddUnit(deathknight);

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

		CBaseUnit* lich = new CBaseUnit();
		lich->CreateUnit(LICH,Country, Territory);
		Enemy->AddUnit(lich);

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

		CBaseBuilding* humanbarracks = new Barracks(1,BuildPosX,BuildPosY);


		Player->AddBuilding(humanbarracks);
		pHBarracks = pHBarracks->NextSiblingElement( "humanbarracksbuilding_info" );

	}

	while(pHCapital != nullptr)
	{

		int BuildPosX;
		int BuildPosY;


		pHCapital->Attribute("BuildPosX", &BuildPosX);
		pHCapital->Attribute("BuildPosY", &BuildPosY);

		CBaseBuilding* humancapital = new Capitol(0,BuildPosX,BuildPosY);


		Player->AddBuilding(humancapital);
		pHCapital = pHCapital->NextSiblingElement( "humancapitalbuilding_info" );

	}
	while(pHWood != nullptr)
	{

		int BuildPosX;
		int BuildPosY;


		pHWood->Attribute("BuildPosX", &BuildPosX);
		pHWood->Attribute("BuildPosY", &BuildPosY);

		CBaseBuilding* humanwood = new WoodMill(0,BuildPosX,BuildPosY);


		Player->AddBuilding(humanwood);
		pHWood = pHWood->NextSiblingElement( "humanwoodbuilding_info" );

	}
	while(pHFarm != nullptr)
	{

		int BuildPosX;
		int BuildPosY;


		pHFarm->Attribute("BuildPosX", &BuildPosX);
		pHFarm->Attribute("BuildPosY", &BuildPosY);

		CBaseBuilding* humanfarm = new Farm(0,BuildPosX,BuildPosY);


		Player->AddBuilding(humanfarm);
		pHFarm = pHFarm->NextSiblingElement( "humanfarmbuilding_info" );

	}
	while(pHMine != nullptr)
	{

		int BuildPosX;
		int BuildPosY;


		pHMine->Attribute("BuildPosX", &BuildPosX);
		pHMine->Attribute("BuildPosY", &BuildPosY);

		CBaseBuilding* humanmine = new Mine(0,BuildPosX,BuildPosY);


		Player->AddBuilding(humanmine);
		pHMine = pHMine->NextSiblingElement( "humanminebuilding_info" );

	}
	while(pHWarfactory != nullptr)
	{

		int BuildPosX;
		int BuildPosY;


		pHWarfactory->Attribute("BuildPosX", &BuildPosX);
		pHWarfactory->Attribute("BuildPosY", &BuildPosY);

		CBaseBuilding* humanwarfactory = new Mine(0,BuildPosX,BuildPosY);


		Player->AddBuilding(humanwarfactory);
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

		CBaseBuilding* undeadbarracks = new Barracks(2,BuildPosX,BuildPosY);


		Enemy->AddBuilding(undeadbarracks);
		pUBarracks = pUBarracks->NextSiblingElement( "undeadbarracksbuilding_info" );

	}

	while(pUCapital != nullptr)
	{

		int BuildPosX;
		int BuildPosY;


		pUCapital->Attribute("BuildPosX", &BuildPosX);
		pUCapital->Attribute("BuildPosY", &BuildPosY);

		CBaseBuilding* undeadcapital = new Capitol(2,BuildPosX,BuildPosY);


		Enemy->AddBuilding(undeadcapital);
		pUCapital = pUCapital->NextSiblingElement( "undeadcapitalbuilding_info" );

	}
	while(pUWood != nullptr)
	{

		int BuildPosX;
		int BuildPosY;


		pUWood->Attribute("BuildPosX", &BuildPosX);
		pUWood->Attribute("BuildPosY", &BuildPosY);

		CBaseBuilding* undeadwood = new WoodMill(2,BuildPosX,BuildPosY);


		Enemy->AddBuilding(undeadwood);
		pUWood = pUWood->NextSiblingElement( "undeadwoodbuilding_info" );

	}
	while(pUFarm != nullptr)
	{

		int BuildPosX;
		int BuildPosY;


		pUFarm->Attribute("BuildPosX", &BuildPosX);
		pUFarm->Attribute("BuildPosY", &BuildPosY);

		CBaseBuilding* undeadfarm = new Farm(2,BuildPosX,BuildPosY);


		Enemy->AddBuilding(undeadfarm);
		pUFarm = pUFarm->NextSiblingElement( "undeadfarmbuilding_info" );

	}
	while(pUMine != nullptr)
	{

		int BuildPosX;
		int BuildPosY;


		pUMine->Attribute("BuildPosX", &BuildPosX);
		pUMine->Attribute("BuildPosY", &BuildPosY);

		CBaseBuilding* undeadmine = new Mine(2,BuildPosX,BuildPosY);



		Enemy->AddBuilding(undeadmine);
		pUMine = pUMine->NextSiblingElement( "undeadminebuilding_info" );

	}
	while(pUWarfactory != nullptr)
	{

		int BuildPosX;
		int BuildPosY;


		pUWarfactory->Attribute("BuildPosX", &BuildPosX);
		pUWarfactory->Attribute("BuildPosY", &BuildPosY);

		CBaseBuilding* undeadwarfactory = new WarFactory(2,BuildPosX,BuildPosY);



		Enemy->AddBuilding(undeadwarfactory);
		pUMine = pUMine->NextSiblingElement( "undeadwarfactorybuilding_info" );

	}

#pragma endregion


#pragma endregion

	// Save the info into the vector
	//vPlayer1.push_back( info );
	vPlayer1[m_nCursor] = info;
	//pName = pName->NextSiblingElement( "Name" );
	//}
	//}

	//}
	// Did we load anything?
	if( vPlayer1.size() > 0 )
		return true;


	return false;

}
void CLoadGame::SaveDefault	(  const wchar_t* szFilename)
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