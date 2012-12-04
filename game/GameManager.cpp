
// The main game object, which contains the entire game.
#include "stdafx.h"
#include "GameManager.h"


GameManager* GameManager::ms_pGameManager = NULL;

GameManager::GameManager()
: m_pGameWindow(new GameWindow)
, m_bIsGameOver(false)
{
}

GameManager::~GameManager()
{
	delete m_pGameWindow;
}

GameManager& GameManager::Get()
{
	if (!ms_pGameManager)
		ms_pGameManager = new GameManager;
	return *ms_pGameManager;
}

bool GameManager::Init(HINSTANCE hInstance, int nCmdShow)
{
	bool bSuccess = true;

	bSuccess = bSuccess && m_pGameWindow->Init(hInstance, nCmdShow);

	return bSuccess;
}

void GameManager::Clear()
{
	delete ms_pGameManager;
	ms_pGameManager = NULL;
}


void GameManager::Update()
{
	m_pGameWindow->Update();
}

