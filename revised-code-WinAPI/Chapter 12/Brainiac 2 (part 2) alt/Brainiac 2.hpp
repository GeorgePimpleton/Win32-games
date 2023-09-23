#pragma once

#include <windows.h>
#include <memory>
#include <vector>
#include "random_toolkit.hpp"
#include "resource.h"
#include "Bitmap.hpp"
#include "Sprite.hpp"
#include "GameEngine.hpp"

std::unique_ptr<GameEngine>          g_game;
std::vector<std::unique_ptr<Bitmap>> g_tile(9);
std::vector<std::vector<BOOL>>       g_tileStates(4, std::vector<BOOL>(4));
std::vector<std::vector<INT>>        g_tiles(4, std::vector<INT>(4)); ;
UINT                                 g_matches;
UINT                                 g_tries;
POINT                                g_tile1;
POINT                                g_tile2;
