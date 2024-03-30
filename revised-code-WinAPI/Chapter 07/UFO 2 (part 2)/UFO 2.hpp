#pragma once

#include <windows.h>
#include <algorithm>
#include <memory>
#include <array>
#include "Bitmap.hpp"
#include "GameEngine.hpp"
#include "random_toolkit_v131.hpp"

const LONG g_MAXSPEED = 8;

std::unique_ptr<GameEngine>            g_game;
std::unique_ptr<Bitmap>                g_background;
std::array<std::unique_ptr<Bitmap>, 2> g_saucer;
BOOL                                   g_saucerFlame;
LONG                                   g_saucerX;
LONG                                   g_saucerY;
LONG                                   g_speedX;
LONG                                   g_speedY;
