#pragma once
#include "stdafx.h"
#include <d3d9.h>
#include <d3dx9.h>
//powerup - grab to shift elements
class Powerup{
public:
	Powerup();
	~Powerup();
	float						m_X;
	float						m_Y;
	int							m_Width;
	int							m_Height;
	bool						m_bOnScreen;
	//image and texture for sprite
	LPCWSTR						m_Image;
	LPDIRECT3DTEXTURE9			m_Texture;
private:
};