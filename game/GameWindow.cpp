
// The window that the game will appear in.  Encapsulates the DirectX implementation details, so we 
//	don't have to worry about them.
#include "stdafx.h"
#include "GameWindow.h"
#include <iostream>
#include <string>
#include <string.h>
#include <sstream>

// configuration constants
//could not load these from xml due to a compatibility error with LPCWSTR and consts
const int kScreenWidth = 1024;
const int kScreenHeight = 768;
const LPCWSTR kApplicationTitle = L"Snape Simulator 2012";
const LPCWSTR kBackground = L"images\\bkg.png";
const LPCWSTR kStartGame = L"images\\startgame.png";

// black magic that tells me if a particular key is down
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

//load defaults from xml
GameWindow::GameWindow(){
	//main menu
	m_bInGame = false;
	//grab data from xml
	TiXmlDocument doc("Data.xml");
	TiXmlHandle docHandle( &doc );
	bool loadOkay = doc.LoadFile();
	if (loadOkay){
		//high score
		TiXmlElement* hs = docHandle.FirstChild( "data" ).FirstChild( "scores" ).ToElement();
		m_Highscore = atoi(hs->Attribute( "best"));
		//start score
		m_Score = atoi(hs->Attribute("start"));
		//platforms
		TiXmlElement* platform = docHandle.FirstChild( "data" ).FirstChild( "platforms" ).FirstChild("plat0").ToElement();
		int i = 0;
		for(platform; platform; platform=platform->NextSiblingElement()){
			m_pPlatformList[i].m_X = atof(platform->Attribute("x"));
			m_pPlatformList[i].m_Y = atof(platform->Attribute("y"));
			m_pPlatformList[i].m_Width = atoi(platform->Attribute("width"));
			m_pPlatformList[i].m_Height = atoi(platform->Attribute("height"));
			i++;
		}
		m_PlatformCount = i;

		//first enemy spawn
		TiXmlElement* firstenemy = docHandle.FirstChild( "data" ).FirstChild( "firstenemy" ).ToElement();
		m_eAllEnemies[0].m_X = atof(firstenemy->Attribute("x"));
		m_eAllEnemies[0].m_Y = atof(firstenemy->Attribute("y"));
		m_eAllEnemies[0].m_xVelocity = atoi(firstenemy->Attribute("xvel"));
		m_eAllEnemies[0].m_yVelocity = atoi(firstenemy->Attribute("yvel"));
		m_eAllEnemies[0].m_Width = atoi(firstenemy->Attribute("width"));
		m_eAllEnemies[0].m_Height = atoi(firstenemy->Attribute("height"));
		//visual studio wouldnt let me import this from the xml (due to LPCWSTR compatibility issue)
		m_eAllEnemies[0].m_Image = L"images\\globbly.l1.png";
		if(firstenemy->Attribute("onscreen") == "true"){
			m_eAllEnemies[0].m_bOnScreen = true;
		}
		m_eAllEnemies[0].m_HP = atoi(firstenemy->Attribute("hp"));
		//start hp
		TiXmlElement* hp = docHandle.FirstChild( "data" ).FirstChild( "starthp" ).ToElement();
		m_wPlayer.m_Health = atoi(hp->Attribute("hp"));
		//visual studio wouldnt let me import this from the xml (due to LPCWSTR compatibility issue)
		m_HpImg = L"images\\health.png";
		//UI Rects
		TiXmlElement* uiRects = docHandle.FirstChild( "data" ).FirstChild( "ui" ).FirstChild("current").ToElement();
		m_rCurrScore.left = atoi(uiRects->Attribute("l"));
		m_rCurrScore.top = atoi(uiRects->Attribute("t"));
		m_rCurrScore.right = atoi(uiRects->Attribute("r"));
		m_rCurrScore.bottom = atoi(uiRects->Attribute("b"));
		uiRects = uiRects->NextSiblingElement();
		m_rBest.left = atoi(uiRects->Attribute("l"));
		m_rBest.top = atoi(uiRects->Attribute("t"));
		m_rBest.right = atoi(uiRects->Attribute("r"));
		m_rBest.bottom = atoi(uiRects->Attribute("b"));
		uiRects = uiRects->NextSiblingElement();
		m_rTitle.left = atoi(uiRects->Attribute("l"));
		m_rTitle.top = atoi(uiRects->Attribute("t"));
		m_rTitle.right = atoi(uiRects->Attribute("r"));
		m_rTitle.bottom = atoi(uiRects->Attribute("b"));
		
		//powerup
		TiXmlElement* pup = docHandle.FirstChild( "data" ).FirstChild( "powerup").ToElement();
		m_pPowerup.m_X = atoi(pup->Attribute("x"));
		m_pPowerup.m_Y = atoi(pup->Attribute("y"));
	}
	else{
		return ;
	}
}
//deconstruct
GameWindow::~GameWindow(){
	if (m_d3d)
		m_d3d->Release();

	if (m_d3ddev)
		m_d3ddev->Release();

	if (m_Backbuffer)
		m_Backbuffer->Release();

	if (m_Surface)
		m_Surface->Release();
	
	if (m_Sprite)
		m_Sprite->Release();
}

/**
 ** Windows event handling function
 **/
LRESULT WINAPI WinProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ){
    switch( msg ){
        case WM_DESTROY:
            GameManager::Get().SetGameOver();
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc( hWnd, msg, wParam, lParam );
}

bool GameWindow::Init(HINSTANCE hInstance, int nCmdShow){
	//initialize window settings
    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX); 
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = (WNDPROC)WinProc;
    wc.cbClsExtra	 = 0;
    wc.cbWndExtra	 = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = NULL;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = kApplicationTitle;
    wc.hIconSm       = NULL;
    RegisterClassEx(&wc);

	//create a new window
    m_Window = CreateWindow(kApplicationTitle, kApplicationTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 
							CW_USEDEFAULT, kScreenWidth, kScreenHeight, NULL, NULL, hInstance, NULL);

    //was there an error creating the window?
    if (!m_Window) 
		return false;

    //display the window
    ShowWindow(m_Window, nCmdShow);
    UpdateWindow(m_Window);			// TODO: should I call this every update?  The name certainly suggests that...

	//initialize Direct3D
    m_d3d = Direct3DCreate9(D3D_SDK_VERSION);
    if (!m_d3d){
        MessageBox(m_Window, L"Error initializing Direct3D", L"Error", MB_OK);
        return false;
    }

    //set Direct3D presentation parameters
    D3DPRESENT_PARAMETERS d3dpp; 
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dpp.BackBufferCount = 1;
    d3dpp.BackBufferWidth = kScreenWidth;
    d3dpp.BackBufferHeight = kScreenHeight;
    d3dpp.hDeviceWindow = m_Window;

    //create Direct3D device
    m_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_Window,
						D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_d3ddev);

    if (!m_d3ddev){
        MessageBox(m_Window, L"Error creating Direct3D device", L"Error", MB_OK);
        return 0;
    }

    //clear the m_Backbuffer to black
    m_d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);
    
    //create m_Surface
    HRESULT result = m_d3ddev->CreateOffscreenPlainSurface(
        kScreenWidth,			//width of the m_Surface
        kScreenHeight,			//height of the m_Surface
        D3DFMT_X8R8G8B8,		//m_Surface format
        D3DPOOL_DEFAULT,		//memory pool to use
        &m_Surface,				//pointer to the m_Surface
        NULL);					//reserved (always NULL)
    
	if (!SUCCEEDED(result)) 
		return false;

    //load m_Surface from file into newly created m_Surface
    result = D3DXLoadSurfaceFromFile(
        m_Surface,				//destination m_Surface
        NULL,					//destination palette
        NULL,					//destination rectangle
        kBackground,		//source filename
        NULL,					//source rectangle
        D3DX_DEFAULT,			//controls how image is filtered
        0,						//for transparency (0 for none)
        NULL);					//source image info (usually NULL)

    //make sure file was loaded okay
    if (!SUCCEEDED(result)) 
		return false;
	
	//create sprite
	D3DXCreateSprite(m_d3ddev, &m_Sprite);
	//create font for scores
	D3DXCreateFont(m_d3ddev, 45, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,	DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
	TEXT("OCR A Std"), &m_Font);

	//load startgame texture
	m_StartGameTexture = Load_Texture(kStartGame);
	//load hp texture
	m_HpTexture = Load_Texture(m_HpImg);
	//powerup texture- static image
	m_pPowerup.m_Texture = Load_Texture(m_pPowerup.m_Image);
	//for respawn rate
	m_Frame = 0;

	//create sound
	m_cSound = new CSoundManager();
	m_cSound->Initialize(m_Window, DSSCL_PRIORITY);
	m_cSound->SetPrimaryBufferFormat(2, 22050, 16);
	//sound effects
	m_cSound->Create(&m_pJump, L"sounds\\jump.wav");
	m_cSound->Create(&m_pKill, L"sounds\\kill.wav");
	m_cSound->Create(&m_pDamaged, L"sounds\\damaged.wav");
	m_cSound->Create(&m_pShoot, L"sounds\\shoot.wav");
	m_cSound->Create(&m_pMusic, L"sounds\\music.wav");
	m_cSound->Create(&m_pSelect, L"sounds\\select.wav");
	m_cSound->Create(&m_pPickup, L"sounds\\pickup.wav");
	//play music loop
	m_pMusic->Play(0, DSBPLAY_LOOPING);

    return true;
}

//Loads the given file into a texture (I use textures for the sprites to allow transparency)
LPDIRECT3DTEXTURE9 GameWindow::Load_Texture(LPCWSTR filename){  
    LPDIRECT3DTEXTURE9 texture = NULL;

    //get width and height from bitmap file
    D3DXIMAGE_INFO info;
    HRESULT result = D3DXGetImageInfoFromFile(filename, &info);
    if (result != D3D_OK) return NULL;
    //create the new texture by loading a bitmap image file
	D3DXCreateTextureFromFileEx( 
        m_d3ddev,                //Direct3D device object
        filename,				 //bitmap filename
        info.Width,            //bitmap image width
        info.Height,           //bitmap image height
        1,                     //mip-map levels (1 for no chain)
        D3DPOOL_DEFAULT,       //the type of surface (standard)
        D3DFMT_UNKNOWN,        //surface format (default)
        D3DPOOL_DEFAULT,       //memory class for the texture
        D3DX_DEFAULT,          //image filter
        D3DX_DEFAULT,          //mip filter
        D3DCOLOR_XRGB(255,0,255),            //color key for transparency
        &info,                 //bitmap file info (from loaded file)
        NULL,                  //color palette
        &texture );            //destination texture

    //make sure the bitmap textre was loaded correctly
    if (result != D3D_OK) return NULL;

	return texture;
}

// An Update() helper that draws the image.
void GameWindow::Draw()
{
    //make sure the Direct3D device is valid
    if (!m_d3ddev) return;

    //create pointer to the back buffer
    m_d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &m_Backbuffer);
	//if in game
	if(m_bInGame){
		//load sprite textures
		m_wPlayer.m_Texture = Load_Texture(m_wPlayer.m_Image);
		for(int i = 0; i < 7; i++){
			if(m_eAllEnemies[i].m_bOnScreen){
				m_eAllEnemies[i].m_Texture = Load_Texture(m_eAllEnemies[i].m_Image);
				m_eAllEnemies[i].m_HPTexture = Load_Texture(m_eAllEnemies[i].m_HPimg);
			}
		}
		for(int i = 0; i < 7; i++){
			if(m_pProjectileList[i].m_bOnScreen){
				m_pProjectileList[i].m_Texture = Load_Texture(m_pProjectileList[i].m_Image);
			}
		}
	}
    //start rendering
    if (m_d3ddev->BeginScene())
    {
		
        //draw m_Surface to the m_Backbuffer
		m_d3ddev->StretchRect(m_Surface, NULL, m_Backbuffer, NULL, D3DTEXF_NONE);
		//begin drawing sprites
		m_Sprite->Begin(D3DXSPRITE_ALPHABLEND);
		//if in game
		if(m_bInGame){
			
			//Turn x and y into a vector. Eventually I will set all coordinates to be vectors
			D3DXVECTOR3 pos1(m_wPlayer.m_X, m_wPlayer.m_Y, 0);
			
			//draw player
			m_Sprite->Draw(m_wPlayer.m_Texture, NULL, NULL, &pos1, D3DCOLOR_XRGB(255,255,255));
			
			//draw player's spell icon
			m_wPlayer.m_Texture = Load_Texture(m_wPlayer.m_SpellImage);
			D3DXVECTOR3 spellpos(m_wPlayer.m_X+0.25f*m_wPlayer.m_Width, m_wPlayer.m_Y-48, 0);
			m_Sprite->Draw(m_wPlayer.m_Texture, NULL, NULL, &spellpos, D3DCOLOR_XRGB(255,255,255));
			
			//draw all enemies
			for(int i = 0; i < 7; i++){
				if(m_eAllEnemies[i].m_bOnScreen){
					D3DXVECTOR3 pos2(m_eAllEnemies[i].m_X, m_eAllEnemies[i].m_Y, 0);
					m_Sprite->Draw(m_eAllEnemies[i].m_Texture, NULL, NULL, &pos2, D3DCOLOR_XRGB(255,255,255));
					//draw hp icons
					D3DXVECTOR3 pos4(m_eAllEnemies[i].m_X, m_eAllEnemies[i].m_Y- 32, 0);
					m_Sprite->Draw(m_eAllEnemies[i].m_HPTexture, NULL, NULL, &pos4, D3DCOLOR_XRGB(255,255,255));
				}
			}
			
			//draw projectile spells 
			for(int i = 0; i < 10; i++){
				if(m_pProjectileList[i].m_bOnScreen){
					D3DXVECTOR3 pos3(m_pProjectileList[i].m_X, m_pProjectileList[i].m_Y, 0);
					m_Sprite->Draw(m_pProjectileList[i].m_Texture, NULL, NULL, &pos3, D3DCOLOR_XRGB(255,255,255));
				}
			}

			//draw powerups
			D3DXVECTOR3 pos5(m_pPowerup.m_X, m_pPowerup.m_Y, 0);
			m_Sprite->Draw(m_pPowerup.m_Texture, NULL, NULL, &pos5, D3DCOLOR_XRGB(255, 255, 255));
			//draw hp ui
			D3DXVECTOR3 HpPos(16, 684, 0);
			m_Sprite->Draw(m_HpTexture, NULL, NULL, &HpPos, D3DCOLOR_XRGB(255,255,255));
			if(m_wPlayer.m_Health > 1){
				D3DXVECTOR3 HpPos(48, 684, 0);
				m_Sprite->Draw(m_HpTexture, NULL, NULL, &HpPos, D3DCOLOR_XRGB(255,255,255));
			}
			if(m_wPlayer.m_Health > 2){
				D3DXVECTOR3 HpPos(80, 684, 0);
				m_Sprite->Draw(m_HpTexture, NULL, NULL, &HpPos, D3DCOLOR_XRGB(255,255,255));
			}
		}
		else{
			//write title
			m_Font->DrawTextA(NULL, "Snape Simulator 2012", -1, &m_rTitle, DT_RIGHT, D3DCOLOR_XRGB(0, 0, 70));
			//draw "press space to start" in center of screen
			D3DXVECTOR3 StartPos(356, 256, 0);
			m_Sprite->Draw(m_StartGameTexture, NULL, NULL, &StartPos, D3DCOLOR_XRGB(255, 255, 255));
		}
		//UI- draw scores
		std::stringstream ss;//create a stringstream
		ss << m_Score;//add number to the stream
		std::string j = ss.str();
		m_Font->DrawTextA(NULL, j.c_str(), -1, &m_rCurrScore, DT_RIGHT, D3DCOLOR_ARGB(255, 255, 255, 255));

		std::stringstream ss2;
		ss2 << m_Highscore;
		j = ss2.str();
		m_Font->DrawTextA(NULL, j.c_str(), -1, &m_rBest, DT_RIGHT, D3DCOLOR_ARGB(255, 255, 255, 255));

        //stop rendering
        m_Sprite->End();
		m_d3ddev->EndScene();
        m_d3ddev->Present(NULL, NULL, NULL, NULL);

	}
}

// an Update() helper that handles the input
void GameWindow::HandleInput(){
	//check for escape key (to exit program)
    if (KEY_DOWN(VK_ESCAPE))
        PostMessage(m_Window, WM_DESTROY, 0, 0);
	//start game
	if(!m_bInGame){
		if(KEY_DOWN(VK_SPACE) || KEY_DOWN(VK_RETURN)){
			m_bInGame = true;
			m_pSelect->Play();
		}
	}
	else{
		//A or LeftArrow move left
		if (KEY_DOWN(VK_LEFT) || KEY_DOWN(0x41)){
			m_wPlayer.m_xVelocity = -6;
		}

		//D or RightArrow move right
		if (KEY_DOWN(VK_RIGHT) || KEY_DOWN(0x44)){
			m_wPlayer.m_xVelocity = 6;
		}
		//W or UpArrow to jump
		if(KEY_DOWN(VK_UP) || KEY_DOWN(0x57)){
			if(m_wPlayer.m_bGrounded){
				m_wPlayer.m_bGrounded = false;
				m_wPlayer.m_yVelocity = -18;
				//play sound
				m_pJump->Play();
			}
		}
		//space to cast spell
		if(KEY_DOWN(VK_SPACE)){
			//for making sure fire spell shoots in correct directions
			int vel = 1;
			//cast fire
			if(m_wPlayer.m_Spell == "Fire"){
				//(for making sure fire spell goes in correct direction
				if(m_wPlayer.m_Image == L"images\\wiz.l.png"){
					vel = -1;
				}
				//make a new projectile and shoot it
				Projectile newProj = m_wPlayer.Shoot(vel);
				if(newProj.m_bOnScreen){
					for(int i = 0; i < 10; i++){
						if(m_pProjectileList[i].m_bOnScreen == false){
							m_pProjectileList[i] = newProj;
							m_pProjectileList[i].m_bOnScreen = true;
							//play sound
							m_pShoot->Play();
							break;
						}
					}
				}
			}
			//cast dark or lightning
			else if(m_wPlayer.m_Spell == "Lightning" || (m_wPlayer.m_Spell == "Dark" && m_wPlayer.m_bGrounded)){
				Projectile newProj = m_wPlayer.Shoot(vel);
				if(newProj.m_bOnScreen){
					for(int i = 0; i < 10; i++){
						if(m_pProjectileList[i].m_bOnScreen == false){
							m_pProjectileList[i] = newProj;
							m_pProjectileList[i].m_bOnScreen = true;
							if(m_wPlayer.m_Spell == "Dark"){
								//play sound
								m_pShoot->Play();
							}
							else{
								m_pKill->Play();
							}
							break;
						}
					}
				}
			}
		}
	}
}

//Apply gravity to sprites
void GameWindow::Gravity(){
	m_wPlayer.Gravity(m_pPlatformList, m_PlatformCount);
	for(int i = 0; i < 7; i++){
		if(m_eAllEnemies[i].m_bOnScreen){
			m_eAllEnemies[i].Gravity(m_pPlatformList, m_PlatformCount);
		}
	}
}

//Move all sprites
void GameWindow::Movement(){
	m_wPlayer.Movement(m_pPlatformList, m_PlatformCount);
	//if powerup is taken, move to another platform at a random spot and increment score
	if(m_wPlayer.CheckCollision(m_pPowerup)){
		int i = rand()%5 + 0;
		m_pPowerup.m_X = m_pPlatformList[i].m_X + rand()%m_pPlatformList[i].m_Width;
		m_pPowerup.m_Y = m_pPlatformList[i].m_Y-32;
		//make sure it isnt offscreen
		if(m_pPowerup.m_X+m_pPowerup.m_Width > kScreenWidth){
			m_pPowerup.m_X = kScreenWidth-m_pPowerup.m_Width;
		}
		if(m_pPowerup.m_X < 0){
			m_pPowerup.m_X = 0;
		}
		m_Score+=1;
		m_pPickup->Play();
	}
	//enemies
	for(int i = 0; i < 7; i++){
		if(m_eAllEnemies[i].m_bOnScreen){
			m_eAllEnemies[i].Movement(m_pPlatformList, m_PlatformCount);
			for(int g = 0; g < 10; g++){
				if(m_eAllEnemies[i].CheckCollision(m_pProjectileList[g])){
					//reset projectile if player is using fire
					if(m_wPlayer.m_Spell == "Fire"){
						m_pProjectileList[g].m_bOnScreen = false;
						m_pProjectileList[g].m_X = -500;
						m_pProjectileList[g].m_Y = -500;
					}
					if(m_eAllEnemies[i].m_HP <=0){
						//play sound
						m_pKill->Play();
					}
					break;
				}
			}
		}
		if(m_pProjectileList[i].m_bOnScreen == true){
			m_pProjectileList[i].Movement();
		}
	}
}

//spawn enemies
void GameWindow::Spawn(){
	m_Frame++;
	if(m_SpawnTimer <= 0){
		for(int i = 0; i < 7; i++){
			//set the enemy to be on screen
			if(!m_eAllEnemies[i].m_bOnScreen){
				Enemy newEnemy;
				m_eAllEnemies[i].m_X = newEnemy.m_X;
				m_eAllEnemies[i].m_Y = newEnemy.m_Y;
				m_eAllEnemies[i].m_HP = 2;
				int vel = rand()%2 + 1;
				if(vel == 2){
					m_eAllEnemies[i].m_xVelocity = -4;
				}
				else{
					m_eAllEnemies[i].m_xVelocity = 4;
				}
				m_eAllEnemies[i].m_bOnScreen = true;
				break;
			}
		}
		//slowly decrease spawn time
		m_SpawnTimer = 240-0.05*m_Frame;
		if(m_SpawnTimer <=0){
			m_SpawnTimer = 10;
		}
	}
	else{
		m_SpawnTimer--;
	}
}

//check for damage
void GameWindow::Damage(){
	if(m_wPlayer.m_dmgCooldown <=0){
		for(int i = 0; i < 7; i++){
			if(m_eAllEnemies[i].m_bOnScreen && m_eAllEnemies[i].m_X <= m_wPlayer.m_X + m_wPlayer.m_Width && m_wPlayer.m_X <= m_eAllEnemies[i].m_X + m_eAllEnemies[i].m_Width
				&& m_eAllEnemies[i].m_Y <= m_wPlayer.m_Y + m_wPlayer.m_Height && m_wPlayer.m_Y <= m_eAllEnemies[i].m_Y + m_eAllEnemies[i].m_Height){
				m_wPlayer.m_Health--;
				m_wPlayer.m_dmgCooldown = 60;
				//knockback
				if(m_eAllEnemies[i].m_xVelocity > 0){
					m_wPlayer.m_X+=32;
				}
				else{
					m_wPlayer.m_X-=32;
				}
				//play sound
				m_pDamaged->Play();
			}
		}
	}
	else{
		m_wPlayer.m_dmgCooldown--;
	}
}

//update the game
void GameWindow::Update()
{
	//if in game
	if(m_bInGame){
		//Spawn Enemies
		Spawn();
		//check for damage
		Damage();
		//run enemy ai
		for(int i = 0; i < 7; i++){
			if(m_eAllEnemies[i].m_bOnScreen){
				m_eAllEnemies[i].AI(m_pPlatformList, m_PlatformCount, m_wPlayer);
			}
		}
		//move all entities
		Movement();
		//check players displayed image
		m_wPlayer.SwapImage();
		m_wPlayer.Tick();
		//apply gravity to all entities
		Gravity();
		//reset player velocity
		m_wPlayer.m_xVelocity = 0;
	}
	//handle key input
	HandleInput();
	//draw all images and sprites
	Draw();
	//check if game over
	EndGame();
}

//check for game end 
void GameWindow::EndGame(){
	//if player is offscreen (fell into pit)
	if(!m_bInGame || m_wPlayer.m_Y >= kScreenHeight || m_wPlayer.m_Health <=0){
		//write high score
		if(m_Score > m_Highscore){
			TiXmlDocument doc("Data.xml");
			TiXmlHandle docHandle( &doc );
			bool loadOkay = doc.LoadFile();
			if (loadOkay){
				TiXmlElement* child = docHandle.FirstChild( "data" ).FirstChild( "scores" ).ToElement();
				child->SetAttribute("best", m_Score);
				doc.SaveFile("Data.xml");
				m_Highscore = m_Score;
			}
		}
		m_bInGame = false;
		Reset();
	}
}

//reset game- none of this data could load from xml; kept getting memory errors.
void GameWindow::Reset(){
	//reset player
	Wizard temp;
	m_wPlayer = temp;
	m_wPlayer.m_Health = 3;
	//reset enemies
	for(int i = 0; i < 7; i++){
		m_eAllEnemies[i].m_X = 2000;
		m_eAllEnemies[i].m_Y = 2000;
		m_eAllEnemies[i].m_bOnScreen = false;
		m_eAllEnemies[i].m_HP = 2;
		m_eAllEnemies[i].m_xVelocity = 4;
	}
	//set enemy1
	m_eAllEnemies[0].m_X = 500.0f;
	m_eAllEnemies[0].m_Y = 0.0f;
	m_eAllEnemies[0].m_xVelocity = 4;
	m_eAllEnemies[0].m_yVelocity = 0;
	//reset all projectiles
	for(int i = 0; i < 10; i++){
		m_pProjectileList[i].m_Width = 64;
		m_pProjectileList[i].m_Height = 64;
		m_pProjectileList[i].m_xVelocity = 16;
		m_pProjectileList[i].m_bOnScreen = false;
		m_pProjectileList[i].m_Image = L"images\\meteor.l.png";
		m_pProjectileList[i].m_bDark = false;
		m_pProjectileList[i].m_ImageIndex = 0;
		m_pProjectileList[i].m_X = -500;
		m_pProjectileList[i].m_Y = -500;
	}
	//reset game vars
	m_Frame = 0;
	m_SpawnTimer = 240;
	m_Score = 0;
	//reset powerup
	m_pPowerup.m_X = 512;
	m_pPowerup.m_Y = 492;
}