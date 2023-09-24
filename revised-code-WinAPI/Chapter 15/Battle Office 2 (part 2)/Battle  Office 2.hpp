#pragma once

#include <windows.h>
#include <memory>
#include <vector>
#include "resource.h"
#include "GameEngine.hpp"
#include "Bitmap.hpp"
#include "Sprite.hpp"
#include "random_toolkit.hpp"

HINSTANCE                            g_inst;
std::unique_ptr<GameEngine>          g_game;
HDC                                  g_offscreenDC;
HBITMAP                              g_offscreenBitmap;
std::unique_ptr<Bitmap>              g_officeBitmap;
std::unique_ptr<Bitmap>              g_powBitmap;
std::vector<std::unique_ptr<Bitmap>> g_guyBitmaps(5);
std::unique_ptr<Bitmap>              g_smallGuyBitmap;
std::unique_ptr<Bitmap>              g_gameOverBitmap;
std::unique_ptr<Sprite>              g_powSprite;
std::vector<std::unique_ptr<Sprite>> g_guySprites(5);
std::vector<int>                     g_guyDelay(5);
int                                  g_guyMasterDelay;
int                                  g_hits;
int                                  g_misses;
BOOL                                 g_gameOver;
