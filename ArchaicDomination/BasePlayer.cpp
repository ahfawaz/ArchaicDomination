#include "BasePlayer.h"
#include "SGD Wrappers\CSGD_TextureManager.h"

CBasePlayer::CBasePlayer(void)
{
}


CBasePlayer::~CBasePlayer(void)
{
}


bool CBasePlayer::Input(void)
{
	return true;
}

void CBasePlayer::Render(void)
{
	CSGD_TextureManager * pTM = CSGD_TextureManager::GetInstance();
	

	pTM->Draw( m_nGeneralImageID, (int)m_fPosX, (int)m_fPosY, 0.15f, 0.15f );
}

std::vector<CBaseUnit*> CBasePlayer::GetUnits(void)
{
	std::vector<CBaseUnit*> temp;
	return temp;
}

void CBasePlayer::Update(void)
{

}
