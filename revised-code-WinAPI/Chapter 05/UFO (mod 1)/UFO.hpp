#pragma once

#include "random_toolkit.hpp"
#include "Bitmap.hpp"
#include "GameEngine.hpp"

const LONG                  g_MAXSPEED = 8;

std::unique_ptr<GameEngine> g_game;
std::unique_ptr<Bitmap>     g_background;
std::unique_ptr<Bitmap>     g_saucer;
LONG                        g_saucerX;
LONG                        g_saucerY;
LONG                        g_speedX;
LONG                        g_speedY;
