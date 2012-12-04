#pragma once
#include "stdafx.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "Entity.h"
#include "Wizard.h"
#include "Platform.h"

//player
class Enemy : public Entity{
public:
	//constructor
	Enemy();
	//destructor
	~Enemy();
	void				SwapImage();
	//detect when colliding with the player
	void				AI(Platform* platforms, int platformNum, Entity m_wPlayer);
	bool				CheckCollision(Projectile projList);
	int					m_HP;
	LPCWSTR				m_HPimg;
	LPDIRECT3DTEXTURE9	m_HPTexture;
	//image and texture for sprite
	LPCWSTR				m_Image;
	LPDIRECT3DTEXTURE9	m_Texture;

private:
	//image arrays
	LPCWSTR				m_LeftImageList[3];
	LPCWSTR				m_RightImageList[3];
	//for looping through animation
	double				m_ImageIndex;
	//direction
	bool				m_IsLeft;
};