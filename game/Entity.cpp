#include "stdafx.h"
#include "Entity.h"

Entity::Entity(){
	m_X = -500.0f;
	m_Y = -500.0f;
	m_xVelocity = 0;
	m_yVelocity = 0;
	m_Width = 0;
	m_Height = 0;
	m_bGrounded = false;
	m_bOnScreen = false;
};

Entity::~Entity(){};

//Apply gravity to Entity
void Entity::Gravity(Platform* platforms, int platformNum){
	m_Y+=4;
	m_Y+=m_yVelocity;
	int i = 0;
	bool justLanded = false;
	//check to see if the Entity is on a platform
	while(i < platformNum){
		//if entity is inside a platform, put them on top of it
		if((m_X + m_Width >= platforms[i].m_X) && (m_X <= platforms[i].m_X + platforms[i].m_Width)){
			if(m_Y + m_Height >= platforms[i].m_Y && m_Y + m_Height <= platforms[i].m_Y+platforms[i].m_Height && m_Y <= platforms[i].m_Y){
				m_Y = platforms[i].m_Y-m_Height;
				//no longer in the air
				m_bGrounded = true;
				justLanded = true;
				break;
			}
			else if(m_Y < 0 || (m_Y+m_Height >= platforms[i].m_Y+platforms[i].m_Height && m_Y >= platforms[i].m_Y && m_Y + m_yVelocity <= platforms[i].m_Y+platforms[i].m_Height)){
				m_yVelocity = 0;
				break;
			}
		}
		i++;
	}
	//If they are not on a platform, they are in the air
	if(!justLanded){
		m_bGrounded = false;
	}
	//If in the air, slow down velocity
	if(!m_bGrounded){
		m_yVelocity+=0.5;
		if(m_yVelocity >= 8){
			m_yVelocity = 8;
		}
	}
    return;
}

//Move entity- check for horizontal collisions with platforms, and with map boundaries
void Entity::Movement(Platform* platforms, int platformNum){
	if(m_xVelocity < 0){
		//if an entity is inside a platform, move them out
		for(int i = 0; i < platformNum; i++){
			if(m_Y + m_Height > platforms[i].m_Y && m_Y < platforms[i].m_Y+platforms[i].m_Height
				&& m_X < platforms[i].m_X+platforms[i].m_Width && m_X > platforms[i].m_X){
					m_X = platforms[i].m_X+platforms[i].m_Width+4;
					break;
			}
		}
		//if out of bounds
		if(m_X>0){
			m_X+=m_xVelocity;
			if(m_X < 0){
				m_X = 0;
			}
		}
	}
	else if(m_xVelocity > 0){
		//if an entity is inside a platform, move them out
		for(int i = 0; i < platformNum; i++){
			if(m_Y + m_Height> platforms[i].m_Y && m_Y < platforms[i].m_Y+platforms[i].m_Height
				&& m_X + m_Width > platforms[i].m_X && m_X < platforms[i].m_X + platforms[i].m_Width){
					m_X = platforms[i].m_X-m_Width-4;
					break;
			}
		}
		//if out of bounds
		if(m_X<kScreenWidth){
			m_X+=m_xVelocity;
			if(m_X+m_Width > kScreenWidth){
				m_X = (float)kScreenWidth-m_Width+1;
			}
		}
	}
}

void Entity::SwapImage(){};