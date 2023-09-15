#pragma once

#include <windows.h>
#include <memory>
#include "resource.h"
#include "random_toolkit.hpp"
#include "GameEngine.hpp"
#include "Bitmap.hpp"
#include "Sprite.hpp"
#include "Background.hpp"

HINSTANCE         g_inst;
std::unique_ptr<GameEngine>       g_game;
HDC               g_offscreenDC;
HBITMAP           g_offscreenBitmap;
Bitmap*           g_groundBitmap;
Bitmap*           g_cityBitmap;
Bitmap*           g_meteorBitmap;
Bitmap*           g_missileBitmap;
Bitmap*           g_explosionBitmap;
Bitmap*           g_gameOverBitmap;
StarryBackground* g_background;
int               g_numCities;
int               g_score;
int               g_difficulty;
BOOL              g_gameOver;

void AddMeteor();
