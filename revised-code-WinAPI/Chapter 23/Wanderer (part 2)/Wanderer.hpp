#pragma once

#include <windows.h>
#include "resource.h"
#include "GameEngine.hpp"
#include "Bitmap.hpp"
#include "Sprite.hpp"
#include "ScrollingBackground.hpp"
#include "PersonSprite.hpp"

HINSTANCE            g_hInstance;
GameEngine*          g_pGame;
HDC                  g_hOffscreenDC;
HBITMAP              g_hOffscreenBitmap;
BackgroundLayer*     g_pBGLandscapeLayer;
BackgroundLayer*     g_pFGCloudsLayer;
ScrollingBackground* g_pBackground;
ScrollingBackground* g_pForeground;
Bitmap*              g_pPersonBitmap;
PersonSprite*        g_pPersonSprite;
int                  g_iInputDelay;
