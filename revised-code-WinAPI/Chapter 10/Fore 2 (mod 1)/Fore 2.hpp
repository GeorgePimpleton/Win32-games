#pragma once

#include <windows.h>
#include <vector>
#include "resource.h"
#include "Bitmap.hpp"
#include "Sprite.hpp"
#include "GameEngine.hpp"

GameEngine* g_game;
Bitmap*     g_forestBitmap;
Bitmap*     g_golfBallBitmap;
HDC         g_offscreenDC;
HBITMAP     g_offscreenBitmap;
Sprite*     g_dragSprite;
