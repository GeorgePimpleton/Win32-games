#pragma once

#include <windows.h>
#include <memory>
#include <array>
#include "random_toolkit.hpp"
#include "resource.h"
#include "Bitmap.hpp"
#include "Sprite.hpp"
#include "GameEngine.hpp"

std::unique_ptr<GameEngine>            g_game;
std::unique_ptr<Bitmap>                g_highwayBitmap;
std::unique_ptr<Bitmap>                g_chickenBitmap;
std::array<std::unique_ptr<Bitmap>, 4> g_carBitmaps;
std::unique_ptr<Bitmap>                g_chickenHeadBitmap;
BOOL                                   g_gameOver;
HDC                                    g_offscreenDC;
HBITMAP                                g_offscreenBitmap;
UINT                                   g_inputDelay;
UINT                                   g_numLives;
UINT                                   g_score;
std::unique_ptr<Sprite>                g_chickenSprite;

void MoveChicken(int, int);

