#pragma once

#include <windows.h>
#include "resource.h"
#include <memory>
#include <vector>
#include "Bitmap.hpp"
#include "Sprite.hpp"
#include "GameEngine.hpp"
#include "random_toolkit.hpp"

std::unique_ptr<GameEngine>          g_game;
std::unique_ptr<Bitmap>              g_forestBitmap;
std::unique_ptr<Bitmap>              g_golfBallBitmap;
BOOL                                 g_dragging;
int                                  g_dragBall;
std::vector<std::unique_ptr<Sprite>> g_golfBallSprite(3);
