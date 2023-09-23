#pragma once

#include <windows.h>
#include <memory>
#include <vector>
#include "resource.h"
#include "Bitmap.hpp"
#include "Sprite.hpp"
#include "GameEngine.hpp"

std::unique_ptr<GameEngine>          g_game;
std::unique_ptr<Bitmap>              g_galaxyBitmap;
std::vector<std::unique_ptr<Bitmap>> g_planetBitmap(3);
HDC                                  g_offscreenDC;
HBITMAP                              g_offscreenBitmap;
Sprite*                              g_dragSprite;
