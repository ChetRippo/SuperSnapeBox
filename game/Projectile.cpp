#include "stdafx.h"
#include "Projectile.h"

Projectile::Projectile(){
	m_Width = 64;
	m_Height = 64;
	m_xVelocity = 16;
	m_bOnScreen = false;
	m_Image = L"images\\meteor.l.png";
	m_bDark = false;
	m_bLight = false;
	m_DarkImages[0]=L"images\\dark.1.png";
	m_DarkImages[1]=L"images\\dark.2.png";
	m_DarkImages[2]=L"images\\dark.3.png";
	m_DarkImages[3]=L"images\\dark.4.png";
	m_DarkImages[4]=L"images\\dark.5.png";
	m_LightImages[0]=L"images\\lightning.1.png";
	m_LightImages[1]=L"images\\lightning.2.png";
	m_LightImages[2]=L"images\\lightning.3.png";
	m_ImageIndex = 0;
}

//deconstruct
Projectile::~Projectile(){}

//move meteor
void Projectile::Movement(){
	if(m_bOnScreen == true){
		m_X+=m_xVelocity;
		if(m_ImageIndex >= 5 || m_X+m_Width < 0 || m_X > kScreenWidth){
			m_xVelocity = 0;
			m_bOnScreen = false;
			m_X = -500;
			m_Y = -500;
			m_ImageIndex = 0;
			return;
		}
		SwapImage();
	}
}
//if dark spell, increment image
void Projectile::SwapImage(){
	if(m_bDark && m_bOnScreen){
		m_ImageIndex+=0.15;
		//reset
		if(m_ImageIndex >= 5){
			m_ImageIndex = 0;
			m_xVelocity = 0;
			m_bOnScreen = false;
			m_X = -500;
			m_Y = -500;
			return;
		}
		m_Image = m_DarkImages[(int)floor((float)m_ImageIndex)];
	}
	else if(m_bLight && m_bOnScreen){
		m_ImageIndex+=0.1;
		//reset
		if(m_ImageIndex >= 3){
			m_ImageIndex = 0;
			m_xVelocity = 0;
			m_bOnScreen = false;
			m_X = -500;
			m_Y = -500;
			return;
		}
		m_Image = m_LightImages[(int)floor((float)m_ImageIndex)];
	}
	else{
		m_ImageIndex = 0;
	}
}