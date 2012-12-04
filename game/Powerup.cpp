#include "stdafx.h"
#include "Powerup.h"

Powerup::Powerup(){
	m_X = 0.0f;
	m_Y = 0.0f;
	m_Width = 38;
	m_Height = 38;
	m_bOnScreen = true;
	m_Image = L"images\\powerup.png";

}

//deconstruct
Powerup::~Powerup(){}