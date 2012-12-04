//Brett Davis
//Assignment 3
#include "stdafx.h"
#include "GameManager.h"
#include <time.h>

float startTime = (float)clock();


//main
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow){
	GameManager& gameMgr = GameManager::Get();

	if (!gameMgr.Init(hInstance, nCmdShow))
		return 0;

    // main message loop
	MSG message;
	while (!gameMgr.IsGameOver())
    {
		// Handle any windows messages
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) 
		{
		    TranslateMessage(&message);
			DispatchMessage(&message);
		}
        // Update the game - 30FPS
		if(((float)clock() - startTime) > 3.33f){
			startTime = (float)clock();
			gameMgr.Update();
		}
	}

	gameMgr.Clear();

	return message.wParam;
}