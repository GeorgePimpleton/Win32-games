// "Talk to me like I'm a 3 year old!" Programming Lessons - Windows Games

// Battle Office 2.hpp - Game Header

#pragma once

#include <windows.h>
#include "random_toolkit.hpp"
#include "resource.h"
#include "Bitmap.hpp"
#include "Sprite.hpp"
#include "GameEngine.hpp"

GameEngine* _pGame;
Bitmap*     _pOfficeBitmap;
Bitmap*     _pPowBitmap;
Bitmap*     _pGuyBitmaps[5];
Bitmap*     _pSmallGuyBitmap;
Bitmap*     _pGameOverBitmap;
BOOL        _bGameOver;
HDC         _hOffscreenDC;
HBITMAP     _hOffscreenBitmap;
int         _iGuyDelay[5];
int         _iGuyMasterDelay;
int         _iHits;
int         _iMisses;
Sprite*     _pPowSprite;
Sprite*     _pGuySprites[5];
