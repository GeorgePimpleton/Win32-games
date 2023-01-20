// "Talk to me like I'm a 3 year old!" Programming Lessons - Windows Games

// Henway 2.hpp - Game Header

#pragma once

#include <windows.h>
#include "random_toolkit.hpp"
#include "resource.h"
#include "Bitmap.hpp"
#include "Sprite.hpp"
#include "GameEngine.hpp"

GameEngine* _pGame;
Bitmap*     _pHighwayBitmap;
Bitmap*     _pChickenBitmap;
Bitmap*     _pCarBitmaps[4];
Bitmap*     _pChickenHeadBitmap;
BOOL        _bGameOver;
HDC         _hOffscreenDC;
HBITMAP     _hOffscreenBitmap;
UINT        _iInputDelay;
UINT        _iNumLives;
UINT        _iScore;
Sprite*     _pChickenSprite;
