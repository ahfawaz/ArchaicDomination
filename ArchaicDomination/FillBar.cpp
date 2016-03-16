#include "FillBar.h"
#include "SGD Wrappers\CSGD_TextureManager.h"
#include "SGD Wrappers\CSGD_DirectInput.h"
#include "SGD Wrappers\CSGD_Direct3D.h"
#include "MainMenu.h"
#include "BattlePlay.h"
#include "Game.h"


#define POWERBAR_INTERVAL 0.1f



FillBar::FillBar(void)
{
	SetCast(false);

	m_fswitchTime = 0;
	m_fBarTimer = 6.0f;

	pressX = false;
	m_nMinSuccess = std::rand() % (100 - 50) + 50;

	if(CMainMenu::GetInstance()->GetArcade() == false && CSGD_DirectInput::GetInstance()->JoystickIsUnplugged() == true )
	{
		imagePressXID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/PressX.png"));
		imagePressZID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/PressZ.png"));
	}
}


FillBar::~FillBar(void)
{
}



void FillBar::Upate(float fElapsedTime)
{
	m_fswitchTime += fElapsedTime;
	m_fBarTimer -= fElapsedTime;

	if(m_fBarTimer <= 0)
	{

		if(GetDamageScale() >= m_nMinSuccess)
			SetCast(true);

		SetActive(false);

	}




	if(((CSGD_DirectInput::GetInstance()->KeyPressed(DIK_Z) || CSGD_DirectInput::GetInstance()->KeyPressed(DIK_X)) || (CSGD_DirectInput::GetInstance()->JoystickButtonPressed( 4, JOYSTICKID1 ) || CSGD_DirectInput::GetInstance()->JoystickButtonPressed(5,JOYSTICKID1))) || 
		((CSGD_DirectInput::GetInstance()->KeyPressed(DIK_PERIOD) || CSGD_DirectInput::GetInstance()->KeyPressed(DIK_COMMA)) || (CSGD_DirectInput::GetInstance()->JoystickButtonPressed( 4, JOYSTICKID2 ) || CSGD_DirectInput::GetInstance()->JoystickButtonPressed(5, JOYSTICKID2))  ) )
	{
		DamageScalePlus(3);

		if(GetDamageScale() >= 99)
			SetDamageScale(100);
	}


	if(GetPowerBarTimer() >= POWERBAR_INTERVAL)
	{

		this->DamageScaleMinus(2);
		if(GetDamageScale() <= 0)
			SetDamageScale(0);

		SetPowerBarTimer(0);
	}
	else
		SetPowerBarTimer(GetPowerBarTimer() + fElapsedTime);

	if(m_fswitchTime > 0.2f)
	{
		pressX = !pressX;
		m_fswitchTime = 0;
	}

}


void FillBar::Render(void)
{
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();
	CBitmapFont* pFont = CGame::GetInstance()->GetFont();

	TOSTRINGSTREAM display;
	display << "Fill to Green";
	pFont->Draw(display.str().c_str(), 320, 470, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 255));

	display.clear();
	display.str(L"");
	display << "Countdown: " << (int)m_fBarTimer;
	pFont->Draw(display.str().c_str(), 96, 470, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 255));

	RECT rSource = {96,504,496,536};

	pD3D->DrawHollowRect(rSource,D3DCOLOR_ARGB(255,25,255,25),5);

	RECT rSource2 = {m_nMinSuccess*4,504,496,536};
	pD3D->DrawRect(rSource2,D3DCOLOR_ARGB(255,0,255,0));


	RECT rCursor = { rSource.left, rSource.top, rSource.left + GetDamageScale()*4, rSource.bottom };
	pD3D->DrawRect(rCursor, D3DCOLOR_ARGB(255,0,0,255));

	if(CMainMenu::GetInstance()->GetArcade() == true)
	{
		if(pressX == true)
		{
			RECT rSource = { 32, 64, 64, 96 };
			CSGD_TextureManager::GetInstance()->Draw(CBattlePlay::GetInstance()->GetArcadeImage(),316,386,2.0f,2.0f, &rSource);
		}
		else
		{
			RECT rSource = { 32, 96, 64, 128 };
			CSGD_TextureManager::GetInstance()->Draw(CBattlePlay::GetInstance()->GetArcadeImage(),316,386,2.0f,2.0f, &rSource);
		}
	}
	else if( CSGD_DirectInput::GetInstance()->JoystickIsUnplugged() == false )
	{
		if(pressX == true)
		{
			RECT rButtonY = { 0, 32, 32, 64 };
			RECT rButtonB = { 32, 0, 64, 32 };
			CSGD_TextureManager::GetInstance()->Draw(CBattlePlay::GetInstance()->GetControllerAbilityImageID(),316,386,1.0f,1.0f, &rButtonY);
			CSGD_TextureManager::GetInstance()->Draw(CBattlePlay::GetInstance()->GetControllerAbilityImageID(),348,386,1.0f,1.0f, &rButtonB);
		}
		else
		{
			RECT rButtonY = { 0, 0, 32, 32 };
			RECT rButtonB = { 32, 32, 64, 64 };
			CSGD_TextureManager::GetInstance()->Draw(CBattlePlay::GetInstance()->GetControllerAbilityImageID(),316,386,1.0f,1.0f, &rButtonY);
			CSGD_TextureManager::GetInstance()->Draw(CBattlePlay::GetInstance()->GetControllerAbilityImageID(),348,386,1.0f,1.0f, &rButtonB);
		}
	}
	else
	{
		if(pressX == true)
			CSGD_TextureManager::GetInstance()->Draw(imagePressXID,316,386);
		else
			CSGD_TextureManager::GetInstance()->Draw(imagePressZID,316,386);
	}
}

