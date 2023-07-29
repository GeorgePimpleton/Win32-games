// "Talk to me like I'm a 3 year old!" Programming Lessons - Windows Games

// Fore 2.hpp - Game Header

#pragma once

#include <windows.h>
#include <vector>
#include "resource.h"
#include "Bitmap.hpp"
#include "Sprite.hpp"
#include "GameEngine.hpp"

GameEngine* g_pGame;
Bitmap*     g_pForestBitmap;
Bitmap*     g_pGolfBallBitmap;
HDC         g_hOffscreenDC;
HBITMAP     g_hOffscreenBitmap;
Sprite*     g_pDragSprite;
