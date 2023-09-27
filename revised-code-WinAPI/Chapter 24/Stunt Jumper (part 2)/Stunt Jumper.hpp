#pragma once

#include <windows.h>
#include "resource.h"
#include "GameEngine.hpp"
#include "Bitmap.hpp"
#include "Sprite.hpp"
#include "ScrollingBackground.hpp"
#include "MotorcycleSprite.hpp"

HINSTANCE             g_inst;
GameEngine*           g_pGame;
HDC                   g_hOffscreenDC;
HBITMAP               g_hOffscreenBitmap;
Bitmap*               g_pSplashBitmap;
BackgroundLayer*      g_pBGRoadLayer;
BackgroundLayer*      g_pBGTreesLayer;
BackgroundLayer*      g_pBGMountainsLayer;
BackgroundLayer*      g_pBGSkyLayer;
Bitmap*               g_pJumperBitmap;
Bitmap*               g_pBusBitmap;
Bitmap*               g_pRampBitmap[2];
Bitmap*               g_pGameOverBitmap;
ScrollingBackground*  g_pBackground;
MotorcycleSprite*     g_pJumperSprite;
Sprite*               g_pLaunchRampSprite;
Sprite*               g_pLandingRampSprite;
Sprite*               g_pBusSprite[7];
int                   g_iInputDelay;
BOOL                  g_bGameOver;
BOOL                  g_bSplash;

void NewGame();
void NewJump(int iNumBuses);
