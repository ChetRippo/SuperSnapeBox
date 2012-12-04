#include "stdafx.h"
#include "Wizard.h"

Wizard::Wizard(){
	m_X = 128.0f;
	m_Y = 542.0f;
	m_xVelocity = 0;
	m_yVelocity = 0;
	m_Width = 64;
	m_Height = 64;
	m_Image = L"images\\wiz.l.png";
	m_bGrounded = false;
	m_CoolDown = 0;
	m_Spell = "Fire";
	m_SpellImage = L"images\\fire.icon.png";
}

//deconstruct
Wizard::~Wizard(){}

//change wizard's image
void Wizard::SwapImage(){
	if(m_xVelocity < 0){
		m_Image = L"images\\wiz.l.png";
	}
	else if(m_xVelocity > 0){
		m_Image = L"images\\wiz.r.png";
	}
	if(m_Spell == "Fire"){
		m_SpellImage = L"images\\fire.icon.png";
	}
	else if(m_Spell == "Dark"){
		m_SpellImage = L"images\\dark.icon.png";
	}
	else{
		m_SpellImage = L"images\\lightning.icon.png";
	}
}

//increment spell recharge
void Wizard::Tick(){
	if(m_CoolDown > 0){
		m_CoolDown--;
	}
}

//create a new projectile
Projectile Wizard::Shoot(int xVelocity){
	Projectile newProj;
	if(m_CoolDown <= 0){
		if(m_Spell == "Fire"){
			m_CoolDown = 30;
			newProj.m_X = m_X;
			newProj.m_Y = m_Y;
			newProj.m_bDark = false;
			newProj.m_bLight = false;
			newProj.m_Image = L"images\\meteor.r.png";
			//change direction based on wizards current direction
			if(xVelocity < 0){
				newProj.m_xVelocity*=-1;
				newProj.m_Image = L"images\\meteor.l.png";
			}
			newProj.m_bOnScreen = true;
			return newProj;
		}
		//dark spell
		else if(m_Spell == "Dark"){
			m_CoolDown = 60;
			newProj.m_X = m_X-64;
			newProj.m_Y = m_Y-32;
			newProj.m_Width=192;
			newProj.m_xVelocity = 0;
			newProj.m_Image = newProj.m_DarkImages[0];
			newProj.m_bDark = true;
			newProj.m_bLight = false;
			newProj.m_bOnScreen = true;
			return newProj;
		}
		//lightning spell = "Lightning"
		else{
			m_CoolDown = 45;
			newProj.m_X = m_X;
			newProj.m_Y = 0;
			newProj.m_Height = 768;
			newProj.m_xVelocity = 0;
			newProj.m_Image = newProj.m_LightImages[0];
			newProj.m_bDark = false;
			newProj.m_bLight = true;
			newProj.m_bOnScreen = true;
			return newProj;
		}
	}
	//if cooldown is not ready, make it a zero velocity projectile, which is not drawn
	else{
		newProj.m_xVelocity = 0;
		return newProj;
	}
}

//check if colliding with powerup- if so, change spell
bool Wizard::CheckCollision(Powerup pup){
	if(pup.m_X <= m_X + m_Width && m_X <= pup.m_X + pup.m_Width
		&& pup.m_Y <= m_Y + m_Height && m_Y <= pup.m_Y + pup.m_Height){
		//pick random spell
		int spell = rand()%3 + 1;
		if(spell == 1){
			m_Spell = "Fire";
		}
		else if(spell == 2){
			m_Spell = "Dark";
		}
		else{
			m_Spell = "Lightning";
		}
		return true;
	}
	return false;
}