#pragma once

#define NOMINMAX

#include <windows.h>
#include <array>
#include <string>
#include <format>
#include "resource.h"
#include "Bitmap.hpp"
#include "GameEngine.hpp"
#include "random_toolkit.hpp"

std::unique_ptr<GameEngine>            g_game;
std::array<std::unique_ptr<Bitmap>, 9> g_tiles;
std::array<std::array<BOOL, 4>, 4>     g_tileState;
std::array<std::array<INT, 4>, 4>      g_tile;
UINT                                   g_matches;
UINT                                   g_tries;
POINT                                  g_tile1;
POINT                                  g_tile2;