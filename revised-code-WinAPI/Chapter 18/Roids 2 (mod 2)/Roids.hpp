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
Bitmap*           g_pAsteroidBitmap;
Bitmap*           g_pSaucerBitmap;
StarryBackground* g_pBackground;
Sprite*           g_pAsteroids[ 3 ];
Sprite*           g_pSaucer;

void UpdateSaucer( );
