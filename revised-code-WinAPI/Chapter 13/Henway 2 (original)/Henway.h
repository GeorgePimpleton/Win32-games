//-----------------------------------------------------------------
// Henway 2 Application
// C++ Header - Henway.h
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include <windows.h>
#include "Resource.h"
#include "GameEngine.h"
#include "Bitmap.h"
#include "Sprite.h"

//-----------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------
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

//-----------------------------------------------------------------
// Function Declarations
//-----------------------------------------------------------------
void MoveChicken(int iXDistance, int iYDistance);
