#pragma once

#include <windows.h>
#include "Resource.h"
#include "GameEngine.h"
#include "Bitmap.h"
#include "Sprite.h"
#include "ScrollingBackground.h"
#include "PersonSprite.h"

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
