#pragma once

#include <windows.h>
#include <vector>
#include <string>
#include <format>
#include "resource.h"
#include "Bitmap.hpp"
#include "GameEngine.hpp"
#include "random_toolkit.hpp"

std::unique_ptr<GameEngine>          g_game;
std::vector<std::unique_ptr<Bitmap>> g_tiles(9);
std::vector<std::vector<BOOL>>       g_tileState(4, std::vector<int>(4));
std::vector<std::vector<INT>>        g_tile(4, std::vector<int>(4)); ;
UINT                                 g_matches;
UINT                                 g_tries;
POINT                                g_tile1;
POINT                                g_tile2;
BOOL                                 g_gameOver;
