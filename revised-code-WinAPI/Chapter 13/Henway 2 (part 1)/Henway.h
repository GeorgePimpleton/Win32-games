#pragma once

#include <windows.h>
#include "resource.h"
#include "GameEngine.h"
#include "Bitmap.h"
#include "Sprite.h"

HINSTANCE   _hInstance;
GameEngine* _pGame;
HDC         _hOffscreenDC;
HBITMAP     _hOffscreenBitmap;
Bitmap*     _pHighwayBitmap;
Bitmap*     _pChickenBitmap;
Bitmap*     _pCarBitmaps[4];
Bitmap*     _pChickenHeadBitmap;
Sprite*     _pChickenSprite;
int         _iInputDelay;
int         _iNumLives;
int         _iScore;
BOOL        _bGameOver;

void MoveChicken( int iXDistance, int iYDistance );
