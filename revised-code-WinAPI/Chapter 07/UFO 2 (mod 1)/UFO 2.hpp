#pragma once

#include "Bitmap.hpp"
#include "GameEngine.hpp"

std::unique_ptr<GameEngine> g_game;
std::unique_ptr<Bitmap>     g_background;
std::unique_ptr<Bitmap>     g_saucer[2];
BOOL                        g_saucerFlame;
const LONG                  g_MAXSPEED { 8 };
LONG                        g_saucerX;
LONG                        g_saucerY;
LONG                        g_speedX;
LONG                        g_speedY;
