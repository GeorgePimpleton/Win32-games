#pragma once

#include <windows.h>
#include "resource.h"
#include "random_toolkit.hpp"
#include <memory>
#include <vector>
#include "GameEngine.hpp"
#include "Bitmap.hpp"
#include "Sprite.hpp"
#include "Background.hpp"

std::unique_ptr<GameEngine>          g_game;
HDC                                  g_offscreenDC;
HBITMAP                              g_offscreenBitmap;
std::unique_ptr<Bitmap>              g_asteroidBitmap;
std::unique_ptr<Bitmap>              g_saucerBitmap;
std::unique_ptr<StarryBackground>    g_background;
std::vector<std::unique_ptr<Sprite>> g_asteroids(3);
std::unique_ptr<Sprite>              g_saucer;

void UpdateSaucer( );
