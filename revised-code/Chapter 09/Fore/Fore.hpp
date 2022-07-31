#pragma once

#include <windows.h>
#include "resource.h"
#include <memory>
#include <vector>
#include "Bitmap.hpp"
#include "Sprite.hpp"
#include "GameEngine.hpp"

std::unique_ptr<GameEngine>          g_pGame;
std::unique_ptr<Bitmap>              g_pForestBitmap;
std::unique_ptr<Bitmap>              g_pGolfBallBitmap;
BOOL                                 g_bDragging;
int                                  g_iDragBall;
std::vector<std::unique_ptr<Sprite>> g_pGolfBallSprite(3);
