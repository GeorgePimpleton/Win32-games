#pragma once

#include <windows.h>
#include <memory>
#include <array>
#include "resource.h"
#include "Bitmap.hpp"
#include "Sprite.hpp"
#include "GameEngine.hpp"

HINSTANCE                              g_inst;
std::unique_ptr<GameEngine>            g_game;
std::unique_ptr<Bitmap>                g_officeBitmap;
std::unique_ptr<Bitmap>                g_powBitmap;
std::array<std::unique_ptr<Bitmap>, 5> g_guyBitmaps;
std::unique_ptr<Bitmap>                g_smallGuyBitmap;
std::unique_ptr<Bitmap>                g_gameOverBitmap;
BOOL                                   g_gameOver;
HDC                                    g_offscreenDC;
HBITMAP                                g_offscreenBitmap;
std::array<INT, 5>                     g_guyDelay;
int                                    g_guyMasterDelay;
int                                    g_hits;
int                                    g_misses;
std::unique_ptr<Sprite>                g_powSprite;
std::array<std::unique_ptr<Sprite>, 5> g_guySprites;
