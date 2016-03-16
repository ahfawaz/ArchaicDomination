#include "AccuracyBar.h"
#include "SGD Wrappers\CSGD_TextureManager.h"
#include "SGD Wrappers\CSGD_DirectInput.h"
#include "SGD Wrappers\CSGD_Direct3D.h"
#include "MainMenu.h"
#include "BattlePlay.h"

#define POWERBAR_INTERVAL 1.0f

AccuracyBar::AccuracyBar(void)
{
	barImageID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Graphics/AccuracyBar.png"));
}


AccuracyBar::~AccuracyBar(void)
{
	CSGD_TextureManager::GetInstance()->UnloadTexture(barImageID);
}


void AccuracyBar::Upate(float fElapsedTime)
{
	if( ( CSGD_DirectInput::GetInstance()->KeyPressed( DIK_TAB ) || CSGD_DirectInput::GetInstance()->JoystickButtonPressed( 1, JOYSTICKID1 ) ) || (CSGD_DirectInput::GetInstance()->KeyPressed( DIK_RETURN ) || CSGD_DirectInput::GetInstance()->JoystickButtonPressed( 1, JOYSTICKID2 )  ) )
	{
		if(GetDamageScale() >= 40 && GetDamageScale() <= 60)
			SetDamageScale(100);
		else if((GetDamageScale() <= 39 && GetDamageScale() >= 26) || (GetDamageScale() >= 61 && GetDamageScale() <= 74))
			SetDamageScale(50);
		else if((GetDamageScale() <= 25 && GetDamageScale() >= 11) || (GetDamageScale() >= 75 && GetDamageScale() <= 89))
			SetDamageScale(25);
		else if(GetDamageScale() <= 10 || GetDamageScale() >= 90)
			SetDamageScale(0);

		SetActive(false);
		return;
	}

	if(GetPowerBarTimer() >= POWERBAR_INTERVAL)
	{
		if(GetDamageScale() >= 100 || GetDamageScale() <= 0)
			SetIsScaling(!GetIsScaling());

		if(GetIsScaling())
			this->DamageScalePlus();
		else
			this->DamageScaleMinus();
	}
	else
		SetPowerBarTimer(GetPowerBarTimer() + fElapsedTime);
}


void AccuracyBar::Render()
{
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();

	RECT rSource = {96,494,496,526};

	CSGD_TextureManager::GetInstance()->Draw(barImageID,96,494);
	pD3D->DrawHollowRect(rSource,D3DCOLOR_ARGB(255,25,255,25),2);

	pD3D->DrawLine(96,504,96,480,D3DCOLOR_ARGB(255,25,255,25),2);
	pD3D->DrawText(_T("0%"),98,470,D3DCOLOR_ARGB(255,25,255,25));

	pD3D->DrawLine(296,504,296,480,D3DCOLOR_ARGB(255,25,255,25),2);
	pD3D->DrawText(_T("100%"),298,470,D3DCOLOR_ARGB(255,25,255,25));

	pD3D->DrawLine(496,504,496,480,D3DCOLOR_ARGB(255,25,255,25),2);
	pD3D->DrawText(_T("0%"),498,470,D3DCOLOR_ARGB(255,25,255,25));


	if(CMainMenu::GetInstance()->GetArcade() == true)
	{
		pD3D->DrawText(_T("Hit"),256,536,D3DCOLOR_ARGB(255,150,255,150));
		RECT rButton = { 0, 64, 32, 96 };
		CSGD_TextureManager::GetInstance()->Draw(CBattlePlay::GetInstance()->GetArcadeImage(), 292, 532, 1.0f, 1.0f, &rButton);
	}
	else if( CSGD_DirectInput::GetInstance()->JoystickIsUnplugged() == false )
	{
		pD3D->DrawText(_T("Hit"),256,536,D3DCOLOR_ARGB(255,150,255,150));
		CSGD_TextureManager::GetInstance()->Draw(CBattlePlay::GetInstance()->GetControllerAButtonID(), 292, 532, 0.75f, 0.75f);
	}
	else
	{
		if(CBattlePlay::GetInstance()->GetCurrentUnit()->GetUnitID() < 6) 
			pD3D->DrawText(_T("Hit 'TAB'"),256,536,D3DCOLOR_ARGB(255,150,255,150));
		else
			pD3D->DrawText(_T("Hit 'RETURN'"),256,536,D3DCOLOR_ARGB(255,150,255,150));
	}

	RECT rCursor = { rSource.left + GetDamageScale() * 4, rSource.top, rSource.left + GetDamageScale() * 4 + 5, rSource.bottom };
	pD3D->DrawRect(rCursor, D3DCOLOR_ARGB(255,255,255,255));
}

