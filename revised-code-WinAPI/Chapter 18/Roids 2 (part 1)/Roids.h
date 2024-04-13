#pragma once

#include <windows.h>
#include "Resource.h"
#include "GameEngine.h"
#include "Bitmap.h"
#include "Sprite.h"
#include "Background.h"

HINSTANCE         g_hInstance;
GameEngine*       g_pGame;
HDC               g_hOffscreenDC;
HBITMAP           g_hOffscreenBitmap;
Bitmap*           g_pAsteroidBitmap;
Bitmap*           g_pSaucerBitmap;
StarryBackground* g_pBackground;
Sprite*           g_pAsteroids[3];
Sprite*           g_pSaucer;

void UpdateSaucer( );
