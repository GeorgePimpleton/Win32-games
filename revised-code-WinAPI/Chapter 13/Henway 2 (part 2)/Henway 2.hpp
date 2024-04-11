#pragma once

#include <windows.h>
#include <memory>
#include <array>
#include "random_toolkit_v131.hpp"
#include "resource.h"
#include "Bitmap.hpp"
#include "Sprite.hpp"
#include "GameEngine.hpp"

std::unique_ptr<GameEngine>            g_game;
std::unique_ptr<Bitmap>                g_highwayBitmap;
std::unique_ptr<Bitmap>                g_chickenBitmap;
std::array<std::unique_ptr<Bitmap>, 4> g_carBitmaps;
std::unique_ptr<Bitmap>                g_chickenHeadBitmap;
std::unique_ptr<Sprite>                g_chickenSprite;

BOOL    g_gameOver;
HDC     g_offscreenDC;
HBITMAP g_offscreenBitmap;
UINT    g_inputDelay;
UINT    g_numLives;
UINT    g_score;

void MoveChicken( int, int );
