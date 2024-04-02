#pragma once

#include <windows.h>
#include "resource.h"
#include <memory>
#include <array>
#include "Bitmap.hpp"
#include "Sprite.hpp"
#include "GameEngine.hpp"
#include "random_toolkit_v131.hpp"

std::unique_ptr<GameEngine>            g_game;
std::unique_ptr<Bitmap>                g_forestBitmap;
std::unique_ptr<Bitmap>                g_golfBallBitmap;
BOOL                                   g_dragging;
LONG                                   g_dragBall;
std::array<std::unique_ptr<Sprite>, 3> g_golfBallSprite;
