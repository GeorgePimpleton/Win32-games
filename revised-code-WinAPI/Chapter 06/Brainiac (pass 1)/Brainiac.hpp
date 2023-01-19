#pragma once

#include <windows.h>
#include <vector>
#include "resource.h"
#include "Bitmap.hpp"
#include "GameEngine.hpp"

std::unique_ptr<GameEngine>          g_pGame;
std::vector<std::unique_ptr<Bitmap>> g_pTiles(9);
BOOL                                 g_bTileStates[ 4 ][ 4 ];
INT                                  g_iTiles[ 4 ][ 4 ];
UINT                                 g_iMatches;
UINT                                 g_iTries;
POINT                                g_ptTile1;
POINT                                g_ptTile2;