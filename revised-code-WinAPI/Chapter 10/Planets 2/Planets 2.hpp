#pragma once

#include <windows.h>
#include <memory>
#include <array>
#include "resource.h"
#include "Bitmap.hpp"
#include "Sprite.hpp"
#include "GameEngine.hpp"

std::unique_ptr<GameEngine> g_game;
std::unique_ptr<Bitmap>                g_galaxyBitmap;
std::array<std::unique_ptr<Bitmap>, 3> g_planetBitmap;
HDC                                    g_offscreenDC;
HBITMAP                                g_offscreenBitmap;
Sprite*                                g_dragSprite;
