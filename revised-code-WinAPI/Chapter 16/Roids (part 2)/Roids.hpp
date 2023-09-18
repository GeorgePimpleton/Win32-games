#pragma once

#include <windows.h>
#include <memory>
#include <vector>
#include "resource.h"
#include "Bitmap.hpp"
#include "Sprite.hpp"
#include "GameEngine.hpp"
#include "Background.hpp"

std::unique_ptr<GameEngine>       g_game;
HDC                               g_offscreenDC;
HBITMAP                           g_offscreenBitmap;
std::unique_ptr<Bitmap>           g_asteroidBitmap;
std::unique_ptr<StarryBackground> g_background;
