#pragma once

#include <windows.h>
#include "random_toolkit.hpp"
#include "resource.h"
#include "Bitmap.hpp"
#include "Sprite.hpp"
#include "GameEngine.hpp"

GameEngine* g_game;
Bitmap*     g_officeBitmap;
Bitmap*     g_powBitmap;
Bitmap*     g_guyBitmaps[5];
Bitmap*     g_smallGuyBitmap;
Bitmap*     g_gameOverBitmap;
BOOL        g_gameOver;
HDC         g_offscreenDC;
HBITMAP     g_offscreenBitmap;
int         g_guyDelay[5];
int         g_guyMasterDelay;
int         g_hits;
int         g_misses;
Sprite*     g_powSprite;
Sprite*     g_guySprites[5];
