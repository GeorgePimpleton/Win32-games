#pragma once

#include <windows.h>
#include <memory>
#include <vector>
#include "Bitmap.hpp"
#include "GameEngine.hpp"
#include "random_toolkit.hpp"

const LONG                           g_MAXSPEED = 8;

std::unique_ptr<GameEngine>          g_game;
std::unique_ptr<Bitmap>              g_background;
std::vector<std::unique_ptr<Bitmap>> g_saucer(2);
BOOL                                 g_saucerFlame;
LONG                                 g_saucerX;
LONG                                 g_saucerY;
LONG                                 g_speedX;
LONG                                 g_speedY;
