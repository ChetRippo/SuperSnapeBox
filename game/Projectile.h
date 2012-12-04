#pragma once
#include "stdafx.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "Entity.h"
//#include "Enemy.h"
//player - will get fancier later
class Projectile{
public:
	Projectile();
	~Projectile();
	void						Movement();
	float						m_X;
	float						m_Y;
	int							m_Width;
	int							m_Height;
	int							m_xVelocity;
	bool						m_bOnScreen;
	//spell types
	bool						m_bDark;
	bool						m_bLight;
	//image and texture for sprite
	LPCWSTR						m_Image;
	double						m_ImageIndex;
	LPDIRECT3DTEXTURE9			m_Texture;
	//dark images
	LPCWSTR						m_DarkImages[5];
	//lightning images
	LPCWSTR						m_LightImages[3];

private:
	void						SwapImage();
};