#pragma once

#include <windows.h>
#include <memory>
#include "resource.h"
#include "GameEngine.hpp"
#include "Bitmap.hpp"

constexpr LONG g_MAXSPEED = 8;

std::unique_ptr<GameEngine> g_game;
std::unique_ptr<Bitmap>     g_background;
std::unique_ptr<Bitmap>     g_saucer;
int                         g_saucerX;
int                         g_saucerY;
int                         g_speedX;
int                         g_speedY;
