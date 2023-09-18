#pragma once

#include <windows.h>
#include "resource.h"
#include "GameEngine.hpp"
#include "Bitmap.hpp"
#include "Sprite.hpp"
#include "Background.hpp"
#include "AlienSprite.hpp"

HINSTANCE         g_inst;
GameEngine*       g_pGame;
HDC               g_hOffscreenDC;
HBITMAP           g_hOffscreenBitmap;
Bitmap*           g_pDesertBitmap;
Bitmap*           g_pCarBitmap;
Bitmap*           g_pSmCarBitmap;
Bitmap*           g_pMissileBitmap;
Bitmap*           g_pBlobboBitmap;
Bitmap*           g_pBMissileBitmap;
Bitmap*           g_pJellyBitmap;
Bitmap*           g_pJMissileBitmap;
Bitmap*           g_pTimmyBitmap;
Bitmap*           g_pTMissileBitmap;
Bitmap*           g_pSmExplosionBitmap;
Bitmap*           g_pLgExplosionBitmap;
Bitmap*           g_pGameOverBitmap;
StarryBackground* g_pBackground;
Sprite*           g_pCarSprite;
int               g_iFireInputDelay;
int               g_iNumLives;
int               g_iScore;
int               g_iDifficulty;
BOOL              g_bGameOver;

void NewGame( );
void AddAlien( );
