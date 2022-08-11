// UFO.hpp - Game Header

#pragma once

#include "Bitmap.hpp"
#include "GameEngine.hpp"

const LONG                  g_iMAXSPEED { 8 };

std::unique_ptr<GameEngine> g_pGame;
std::unique_ptr<Bitmap>     g_pBackground;
std::unique_ptr<Bitmap>     g_pSaucer;
LONG                        g_iSaucerX;
LONG                        g_iSaucerY;
LONG                        g_iSpeedX;
LONG                        g_iSpeedY;
