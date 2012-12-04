#pragma once
#include "stdafx.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "Entity.h"
#include "Projectile.h"
#include "Powerup.h"
//player - will get fancier later
class Wizard : public Entity{
public:
	Wizard();
	~Wizard();
	//basic attack (fire)
	Projectile					Shoot(int xVelocity);
	//check for collision with powerup
	bool						CheckCollision(Powerup pup);
	//change the image
	void						SwapImage();
	void						Tick();
	//image and texture for sprite
	LPCWSTR						m_Image;
	LPDIRECT3DTEXTURE9			m_Texture;
	//spells
	char*						m_Spell;
	LPCWSTR						m_SpellImage;
	//hp
	int							m_Health;
	int							m_dmgCooldown;
private:
	//time until next attack allowed
	int							m_CoolDown;
};