#pragma once

#include <windows.h>
#include "Resource.h"
#include "GameEngine.h"
#include "Bitmap.h"

HINSTANCE   _hInstance;
GameEngine* _pGame;
Bitmap*     _pTiles[ 9 ];
BOOL        _bTileStates[ 4 ][ 4 ];
int         _iTiles[ 4 ][ 4 ];
POINT       _ptTile1, _ptTile2;
int         _iMatches, _iTries;
