#include "FloatingText.h"
#include "SGD Wrappers\SGD_String.h"



CFloatingText::CFloatingText(void)
{

}

CFloatingText::CFloatingText(std::string text, float x, float y, TextType type)
{
	this->text = text;
	m_nPosX = x;
	m_nPosY = y;
	lifeTimer = 0.0f;
	deathTimer = 2.0f;
	this->type = type;

}


CFloatingText::~CFloatingText(void)
{
}


void CFloatingText::Update(float fElapsedTime)
{
	lifeTimer += fElapsedTime;

	m_nPosX -= 0.1f;
	m_nPosY -= 0.1f;

}


void CFloatingText::Render(void)
{
	TOSTRINGSTREAM output;
	output << text.c_str();

	switch(type)
	{
	case DAMAGE:
		CSGD_Direct3D::GetInstance()->DrawText( output.str().c_str(), (int)m_nPosX, (int)m_nPosY-10, D3DCOLOR_XRGB(255, 0, 0));
		break;
	case HEAL:
		CSGD_Direct3D::GetInstance()->DrawText( output.str().c_str(), (int)m_nPosX, (int)m_nPosY-10, D3DCOLOR_XRGB(0, 255, 0));
		break;
	case BUFF:
		CSGD_Direct3D::GetInstance()->DrawText( output.str().c_str(), (int)m_nPosX, (int)m_nPosY-10, D3DCOLOR_XRGB(125, 249, 255));
		break;
	case DEBUFF:
		CSGD_Direct3D::GetInstance()->DrawText( output.str().c_str(), (int)m_nPosX, (int)m_nPosY-10, D3DCOLOR_XRGB(255, 0, 255));
		break;
	}
}


bool CFloatingText::IsDead(void)
{
	return (lifeTimer >= deathTimer);
}
