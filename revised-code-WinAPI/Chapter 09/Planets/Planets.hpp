#pragma once

#include <windows.h>
#include <memory>
#include <vector>
#include "random_toolkit.hpp"
#include "resource.h"
#include "GameEngine.hpp"
#include "Bitmap.hpp"
#include "Sprite.hpp"

std::unique_ptr<GameEngine>          g_game;
std::unique_ptr<Bitmap>              g_galaxyBitmap;
std::vector<std::unique_ptr<Bitmap>> g_planetBitmap(3);
std::vector<std::unique_ptr<Sprite>> g_panetSprite(3);
BOOL                                 g_dragging;
LONG                                 g_dragPlanet;
