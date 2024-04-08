#pragma once

#include <windows.h>
#include "resource.h"
#include "Bitmap.hpp"
#include "GameEngine.hpp"

constexpr UINT g_NUMSLIDES = 7;

HINSTANCE   g_inst;
GameEngine* g_game;
Bitmap*     g_slides[g_NUMSLIDES];
UINT        g_currentSlide;
