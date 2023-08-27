#pragma once

#include <windows.h>
#include <vector>
#include "resource.h"
#include "Bitmap.hpp"
#include "GameEngine.hpp"

std::unique_ptr<GameEngine>          g_game;
std::vector<std::unique_ptr<Bitmap>> g_tiles(9);
BOOL                                 g_tileState[ 4 ][ 4 ];
INT                                  g_tile[ 4 ][ 4 ];
UINT                                 g_matches;
UINT                                 g_tries;
POINT                                g_tile1;
POINT                                g_tile2;