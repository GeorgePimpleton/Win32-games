#pragma once

#include <windows.h>
#include "resource.h"
#include "random_toolkit_v131.hpp"
#include <memory>
#include <array>
#include "GameEngine.hpp"
#include "Bitmap.hpp"
#include "Sprite.hpp"
#include "Background.hpp"

std::unique_ptr<GameEngine>            g_game;
HDC                                    g_offscreenDC;
HBITMAP                                g_offscreenBitmap;
std::unique_ptr<Bitmap>                g_asteroidBitmap;
std::unique_ptr<Bitmap>                g_saucerBitmap;
std::unique_ptr<StarryBackground>      g_background;
std::array<std::unique_ptr<Sprite>, 3> g_asteroids;
std::unique_ptr<Sprite>                g_saucer;

void UpdateSaucer( );
