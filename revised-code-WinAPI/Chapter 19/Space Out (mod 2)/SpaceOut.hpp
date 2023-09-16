#pragma once

#include <windows.h>
#include "resource.h"
#include "random_toolkit.hpp"
#include "GameEngine.hpp"
#include "Bitmap.hpp"
#include "Sprite.hpp"
#include "Background.hpp"
#include "AlienSprite.hpp"

HINSTANCE         g_inst;
GameEngine*       g_game;
HDC               g_offscreenDC;
HBITMAP           g_offscreenBitmap;
Bitmap*           g_desertBitmap;
Bitmap*           g_carBitmap;
Bitmap*           g_smallCarBitmap;
Bitmap*           g_missileBitmap;
Bitmap*           g_BlobboBitmap;
Bitmap*           g_BMissileBitmap;
Bitmap*           g_JellyBitmap;
Bitmap*           g_JMissileBitmap;
Bitmap*           g_TimmyBitmap;
Bitmap*           g_TMissileBitmap;
Bitmap*           g_smallExplosionBitmap;
Bitmap*           g_largeExplosionBitmap;
Bitmap*           g_gameOverBitmap;
StarryBackground* g_background;
Sprite*           g_carSprite;
int               g_fireInputDelay;
int               g_numLives;
int               g_score;
int               g_difficulty;
BOOL              g_gameOver;

void AddAlien( );
