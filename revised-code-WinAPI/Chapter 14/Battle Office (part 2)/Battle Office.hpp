#pragma once

#include <windows.h>
#include <memory>
#include <vector>
#include "random_toolkit.hpp"
#include "resource.h"
#include "Bitmap.hpp"
#include "Sprite.hpp"
#include "GameEngine.hpp"

std::unique_ptr<GameEngine>          g_game;
std::unique_ptr<Bitmap>              g_officeBitmap;
std::unique_ptr<Bitmap>              g_powBitmap;
std::unique_ptr<Bitmap>              g_guyBitmaps[5];
std::unique_ptr<Bitmap>              g_smallGuyBitmap;
std::unique_ptr<Bitmap>              g_gameOverBitmap;
BOOL                                 g_gameOver;
HDC                                  g_offscreenDC;
HBITMAP                              g_offscreenBitmap;
std::vector<int>                     g_guyDelay(5);
int                                  g_guyMasterDelay;
int                                  g_hits;
int                                  g_misses;
std::unique_ptr<Sprite>              g_powSprite;
std::vector<std::unique_ptr<Sprite>> g_guySprites(5);
