// "Talk to me like I'm a 3 year old!" Programming Lessons - Windows Games

// Brainiac 2.hpp - Brainiac 2 Game Header

#pragma once

#include <windows.h>
#include "random_toolkit.hpp"
#include "resource.h"
#include "Bitmap.hpp"
#include "Sprite.hpp"
#include "GameEngine.hpp"

GameEngine* _pGame;
Bitmap*     _pTiles[9];
BOOL        _bTileStates[4][4];
HINSTANCE   _hInstance;
int         _iTiles[4][4];
UINT        _iMatches;
UINT        _iTries;
POINT       _ptTile1;
POINT       _ptTile2;
