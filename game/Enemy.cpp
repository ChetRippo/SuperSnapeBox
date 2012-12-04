#include "stdafx.h"
#include "Enemy.h"

//create enemy - this will be filled in with more later when there are more enemies
Enemy::Enemy(){
	//initialize array of images
	m_LeftImageList[0] = L"images\\globbly.l1.png";
	m_LeftImageList[1] = L"images\\globbly.l2.png";
	m_LeftImageList[2] = L"images\\globbly.l3.png";
	m_RightImageList[0] = L"images\\globbly.r1.png";
	m_RightImageList[1] = L"images\\globbly.r2.png";
	m_RightImageList[2] = L"images\\globbly.r3.png";
	m_HPimg = L"images\\hp2.png";
	m_Image = L"images\\globbly.l1.png";
	m_ImageIndex = 0;
	m_IsLeft = true;
	m_X = rand()%500 + 400;
	m_Y = 0;
	m_Width = 64;
	m_Height = 64;
	m_bOnScreen = false;
	m_HP = 2;
};

//deconstruct
Enemy::~Enemy(){}

//change image
void Enemy::SwapImage(){
	m_ImageIndex+=0.15;
	if(m_ImageIndex > 3){
		m_ImageIndex = 0;
	}
	//if moving right
	if(m_xVelocity > 0){
		m_Image = m_RightImageList[(int)floor(m_ImageIndex)];
		m_IsLeft = false;
	}
	//if moving left
	else if(m_xVelocity < 0){
		m_Image = m_LeftImageList[(int)floor(m_ImageIndex)];
		m_IsLeft = true;
	}
	//if facing left
	else if(m_IsLeft){
		m_Image = m_LeftImageList[(int)floor(m_ImageIndex)];
	}
	else{
		m_Image = m_RightImageList[(int)floor(m_ImageIndex)];
	}
	if(m_HP == 3){
		m_HPimg = L"images\\hp3.png";
	}
	else if(m_HP == 2){
		m_HPimg = L"images\\hp2.png";
	}
	else{
		m_HPimg = L"images\\hp1.png";
	}
}

//determine when there is a collision between this and the player, or if at the end of the platform
void Enemy::AI(Platform* platforms, int platformNum, Entity m_wPlayer){
	//swap x vel if at edge of map
	if(m_X+m_Width >= kScreenWidth || m_X <= 0){
		m_xVelocity*=-1;
	}
	if(m_Y > kScreenHeight){
		m_X = 500;
		m_Y = 0;
		m_bOnScreen = true;
		m_xVelocity = 8;
		m_HP = 3;
		int vel = rand()%2 + 1;
		if(vel == 2){
			m_xVelocity*=-1;
		}
		return;
	}
	//change image
	SwapImage();
}
//check for collisions with meteors. if so, delete enemy
bool Enemy::CheckCollision(Projectile projList){
	if(projList.m_bOnScreen){
		if(projList.m_X <= m_X + m_Width && m_X <= projList.m_X + projList.m_Width
			&& projList.m_Y <= m_Y + m_Height && m_Y <= projList.m_Y + projList.m_Height){
			m_HP-=1;
			if(m_HP<=0){
				m_X = 2000;
				m_Y = 2000;
				m_bOnScreen = false;
				m_xVelocity = 4;
			}
			return true;
		}
	}
	return false;
}