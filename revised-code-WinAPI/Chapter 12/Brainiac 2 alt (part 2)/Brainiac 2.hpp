#pragma once

#include <windows.h>
#include <memory>
#include <array>
#include "random_toolkit.hpp"
#include "resource.h"
#include "Bitmap.hpp"
#include "Sprite.hpp"
#include "GameEngine.hpp"

std::unique_ptr<GameEngine>            g_game;
std::array<std::unique_ptr<Bitmap>, 9> g_tile;
std::array<std::array<BOOL, 4>, 4>     g_tileStates;
std::array<std::array<INT, 4>, 4>      g_tiles;
UINT                                   g_matches;
UINT                                   g_tries;
POINT                                  g_tile1;
POINT                                  g_tile2;
