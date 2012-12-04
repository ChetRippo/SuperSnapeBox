
// The game manager - a singleton that contains the entire game.

#pragma once
#include "stdafx.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "GameWindow.h"

class GameWindow;

class GameManager
{
private:
	GameManager();
	virtual ~GameManager();

public:
	static GameManager&			Get();

	// Initializes the game.  Returns false if initialization fails.
	// ARGUMENTS:
	//	hInstance - the window instance passed in to WinMain()
	//	cmdLine - the commandline arguments passed in to WinMain()
	//	nCmdShow - I'm not sure, to be honest - also passed in to WinMain
	bool						Init(HINSTANCE hInstance, int nCmdShow);
	void						Clear();

	// Called every frame.  Returns true if the game should keep running, false if it's time to exit.
	void						Update();

	// Used to keep track of whether it's time to exit the game.
	void						SetGameOver()							{ m_bIsGameOver = true; }
	bool						IsGameOver() const						{ return m_bIsGameOver; }

	// The main game window
	GameWindow*					m_pGameWindow;

private:
	// Storage for our singleton game manager
	static GameManager*			ms_pGameManager;

	

	// Keeps track of whether the game is over.
	bool						m_bIsGameOver;

private:
	// DELIBERATELY UNDEFINED
	GameManager(const GameManager& rhs);
	GameManager&				operator=(const GameManager& rhs);
	bool						operator==(const GameManager& rhs);
	bool						operator<(const GameManager& rhs);
};

