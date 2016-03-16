#include "SpamBar.h"
#include "SGD Wrappers\CSGD_TextureManager.h"
#include "SGD Wrappers\CSGD_DirectInput.h"
#include "SGD Wrappers\CSGD_Direct3D.h"
#include "MainMenu.h"
#include "BattlePlay.h"
#include "Game.h"



#define POWERBAR_INTERVAL 0.1f




SpamBar::SpamBar(void)
{
	m_fTimeLimit = 6;
	m_fswitchTime = 0;
	pressX = false;

	if(CMainMenu::GetInstance()->GetArcade() == true)
	{

	}
	else if( CSGD_DirectInput::GetInstance()->JoystickIsUnplugged() == false )
	{

	}
	else
	{
		imagePressXID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/PressX.png"));
		imagePressZID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/PressZ.png"));
	}
}


SpamBar::~SpamBar(void)
{
}




void SpamBar::Upate(float fElapsedTime)
{
	m_fTimeLimit -= fElapsedTime;
	m_fswitchTime += fElapsedTime;

	if(m_fTimeLimit <= 1.0f)
	{
		SetActive(false);
		return;
	}

	if(((CSGD_DirectInput::GetInstance()->KeyPressed(DIK_Z) || CSGD_DirectInput::GetInstance()->KeyPressed(DIK_X)) || (CSGD_DirectInput::GetInstance()->JoystickButtonPressed( 4, JOYSTICKID1 ) || CSGD_DirectInput::GetInstance()->JoystickButtonPressed(5,JOYSTICKID1))) || 
		((CSGD_DirectInput::GetInstance()->KeyPressed(DIK_PERIOD) || CSGD_DirectInput::GetInstance()->KeyPressed(DIK_COMMA)) || (CSGD_DirectInput::GetInstance()->JoystickButtonPressed( 4, JOYSTICKID2 ) || CSGD_DirectInput::GetInstance()->JoystickButtonPressed(5, JOYSTICKID2))  ) )
	{
		DamageScalePlus(3);

		if(GetDamageScale() >= 100)
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




void SpamBar::Render(void)
{
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();
	CBitmapFont* pFont = CGame::GetInstance()->GetFont();

	TOSTRINGSTREAM timer;

	timer << "Countdown: " << (int)m_fTimeLimit;

	pFont->Draw(timer.str().c_str(), 256, 470, 1.0f, D3DCOLOR_ARGB(255, 0, 0, 255));


	RECT rSource = {96,504,496,536};
	pD3D->DrawHollowRect(rSource,D3DCOLOR_ARGB(255,25,255,25),5);

	pD3D->DrawLine(96,504,96,490,D3DCOLOR_ARGB(255,25,255,25),2);
	pD3D->DrawText(_T("0%"),96,470,D3DCOLOR_ARGB(255,25,255,25));

	pD3D->DrawText(_T("Fill the Bar"), 256, 470, D3DCOLOR_ARGB(255,25,255,150));

	pD3D->DrawLine(496,504,496,490,D3DCOLOR_ARGB(255,25,255,25),2);
	pD3D->DrawText(_T("100%"),496,470,D3DCOLOR_ARGB(255,25,255,25));

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
			CSGD_TextureManager::GetInstance()->Draw(CBattlePlay::GetInstance()->GetControllerAbilityImageID(),316,386,2.0f,2.0f, &rButtonY);
			CSGD_TextureManager::GetInstance()->Draw(CBattlePlay::GetInstance()->GetControllerAbilityImageID(),380,386,2.0f,2.0f, &rButtonB);
		}
		else
		{
			RECT rButtonY = { 0, 0, 32, 32 };
			RECT rButtonB = { 32, 32, 64, 64 };
			CSGD_TextureManager::GetInstance()->Draw(CBattlePlay::GetInstance()->GetControllerAbilityImageID(),316,386,2.0f,2.0f, &rButtonY);
			CSGD_TextureManager::GetInstance()->Draw(CBattlePlay::GetInstance()->GetControllerAbilityImageID(),380,386,2.0f,2.0f, &rButtonB);
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
