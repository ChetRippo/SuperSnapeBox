#pragma once
#include "stdafx.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "Wizard.h"
#include "Platform.h"
#include "GameManager.h"
#include "Enemy.h"
#include "Projectile.h"
#include "tinyxml.h"
#include "DirectSound.h"
#include "Powerup.h"
class GameWindow
{
public:
	GameWindow();
	virtual ~GameWindow();

	// Initializes the game.  Returns false if initialization fails.
	// ARGUMENTS:
	//	hInstance - the window instance passed in to WinMain()
	//	cmdLine - the commandline arguments passed in to WinMain()
	//	nCmdShow - I'm not sure, to be honest - also passed in to WinMain
	bool					Init(HINSTANCE hInstance, int nCmdShow);

	// Called every frame.  Returns true if the game should keep running, false if it's time to exit.
	void					Update();

private:
	// an Update() helper that handles the input and manipulates the image accordingly
	void					HandleInput();

	// An Update() helper that draws the image.
	void					Draw();
	//load texture for sprites
	LPDIRECT3DTEXTURE9		Load_Texture(LPCWSTR filename);
	//ends the game
	void					EndGame();
	//apply gravity and move entities
	void					Gravity();
	void					Movement();
	//reset game
	void					Reset();
	//check for damage
	void					Damage();
	//create enemies
	void					Spawn();
	// the window object that we're managing
	HWND					m_Window;

	//Direct3D objects
	LPDIRECT3D9				m_d3d; 
	LPDIRECT3DDEVICE9		m_d3ddev; 
	LPDIRECT3DSURFACE9		m_Backbuffer;
	LPDIRECT3DSURFACE9		m_Surface;
	LPDIRECT3DTEXTURE9		m_StartGameTexture;
	LPD3DXSPRITE			m_Sprite;
	//font/ui
	RECT					m_rCurrScore;
	RECT					m_rBest;
	RECT					m_rTitle;
	ID3DXFont*				m_Font;
	//sound
	CSoundManager*			m_cSound;
	CSound					*m_pJump;
	CSound					*m_pKill;
	CSound					*m_pDamaged;
	CSound					*m_pShoot;
	CSound					*m_pMusic;
	CSound					*m_pPickup;
	CSound					*m_pSelect;

	//health pic
	LPCWSTR					m_HpImg;
	LPDIRECT3DTEXTURE9		m_HpTexture;

private:
	//objects
	Wizard					m_wPlayer;
	Enemy					m_eAllEnemies[7];
	Projectile				m_pProjectileList[10];
	Powerup					m_pPowerup;
	//spawn handling
	int						m_SpawnTimer;
	int						m_Frame;
	//main menu handling
	bool					m_bInGame;
public:
	Platform				m_pPlatformList[6];
	int						m_PlatformCount;
	//score
	int						m_Score;
	int						m_Highscore;
private:
	// DELIBERATELY UNDEFINED
	GameWindow(const GameWindow& rhs);
	GameWindow&	operator=(const GameWindow& rhs);
	bool operator==(const GameWindow& rhs);
	bool operator<(const GameWindow& rhs);
};

