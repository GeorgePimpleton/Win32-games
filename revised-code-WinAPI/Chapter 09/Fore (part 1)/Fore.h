#pragma once

#include <windows.h>
#include "Resource.h"
#include "GameEngine.h"
#include "Bitmap.h"
#include "Sprite.h"

HINSTANCE   _hInstance;
GameEngine* _pGame;
Bitmap*     _pForestBitmap;
Bitmap*     _pGolfBallBitmap;
Sprite*     _pGolfBallSprite[3];
BOOL        _bDragging;
int         _iDragBall;
