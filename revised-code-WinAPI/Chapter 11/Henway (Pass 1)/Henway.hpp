// "Talk to me like I'm a 3 year old!" Programming Lessons - Windows Games

// Henway.cpp - Henway Source

#include <windows.h>
#include <vector>
#include "resource.h"
#include "Bitmap.hpp"
#include "Sprite.hpp"
#include "GameEngine.hpp"
#include "random_toolkit.hpp"

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


// HELPER GAME FUNCTION DECLARATION ============================================
void MoveChicken(int iXDistance, int iYDistance);
