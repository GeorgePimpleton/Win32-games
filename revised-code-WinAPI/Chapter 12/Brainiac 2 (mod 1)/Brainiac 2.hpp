#pragma once

#include <windows.h>
#include "random_toolkit.hpp"
#include "resource.h"
#include "Bitmap.hpp"
#include "Sprite.hpp"
#include "GameEngine.hpp"

GameEngine* g_game;
Bitmap*     g_tile[9];
BOOL        g_tileStates[4][4];
HINSTANCE   g_inst;
int         g_tiles[4][4];
UINT        g_matches;
UINT        g_tries;
POINT       g_tile1;
POINT       g_tile2;
