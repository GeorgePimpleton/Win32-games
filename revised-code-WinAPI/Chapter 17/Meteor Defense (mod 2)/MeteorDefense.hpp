#pragma once

#include <windows.h>
#include "resource.h"
#include "GameEngine.hpp"
#include "Bitmap.hpp"
#include "Sprite.hpp"
#include "Background.hpp"

HINSTANCE         g_hInstance;
GameEngine*       g_pGame;
HDC               g_hOffscreenDC;
HBITMAP           g_hOffscreenBitmap;
Bitmap*           g_pGroundBitmap;
Bitmap*           g_pCityBitmap;
Bitmap*           g_pMeteorBitmap;
Bitmap*           g_pMissileBitmap;
Bitmap*           g_pExplosionBitmap;
Bitmap*           g_pGameOverBitmap;
StarryBackground* g_pBackground;
int               g_iNumCities;
int               g_iScore;
int               g_iDifficulty;
BOOL              g_bGameOver;

void NewGame();
void AddMeteor();
