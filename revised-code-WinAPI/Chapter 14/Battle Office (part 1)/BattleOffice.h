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
Bitmap*     _pOfficeBitmap;
Bitmap*     _pTargetBitmap;
Bitmap*     _pPowBitmap;
Bitmap*     _pGuyBitmaps[5];
Bitmap*     _pSmallGuyBitmap;
Bitmap*     _pGameOverBitmap;
Sprite*     _pTargetSprite;
Sprite*     _pPowSprite;
Sprite*     _pGuySprites[5];
int         _iGuyDelay[5];
int         _iGuyMasterDelay;
int         _iHits;
int         _iMisses;
BOOL        _bGameOver;
