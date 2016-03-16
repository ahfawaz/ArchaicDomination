#pragma once

#include <string>
#include "SGD Wrappers\CSGD_Direct3D.h"

enum TextType {DAMAGE, HEAL, BUFF, DEBUFF, };



class CFloatingText
{
public:
	void Update(float fElapsedTime);
	void Render(void);

	CFloatingText(std::string text, float x, float y, TextType type);
	~CFloatingText(void);
	CFloatingText(void);
	bool IsDead(void);

private:
	float m_nPosX;
	float m_nPosY;

	TextType type;

	std::string text;

	float lifeTimer;
	float deathTimer;



};

