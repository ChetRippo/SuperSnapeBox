#pragma once
#include "stdafx.h"
#include "Platform.h"
extern const int kScreenWidth;
extern const int kScreenHeight;
//main class for AI or player controlled objects
class Entity{
public:
	Entity();
	~Entity();
	//move entities vertically and horizontally
	void					Gravity(Platform* platforms, int platformNum);
	void					Movement(Platform* platforms, int platformNum);
	//can be overwritten. for entities that animate or have multiple images
	virtual void			SwapImage();
	//coordinates
	float					m_X;
	float					m_Y;
	//size
	int						m_Width;
	int						m_Height;
	//movement speed
	float					m_xVelocity;
	float					m_yVelocity;
	//whether or not the entity is in the air
	bool					m_bGrounded;

	bool					m_bOnScreen;
private:

};