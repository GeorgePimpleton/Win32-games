#pragma once

#include <windows.h>
#include "Resource.h"
#include "GameEngine.h"
#include "Bitmap.h"
#include "Sprite.h"

HINSTANCE   _hInstance;
GameEngine* _pGame;
HDC         _hOffscreenDC;
HBITMAP     _hOffscreenBitmap;
Bitmap*     _pForestBitmap;
Bitmap*     _pGolfBallBitmap;
Sprite*     _pDragSprite;
