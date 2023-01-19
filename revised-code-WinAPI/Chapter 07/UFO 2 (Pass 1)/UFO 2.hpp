// "Talk to me like I'm a 3 year old!" Programming Lessons - Windows Games

// UFO 2.cpp - Game Source

#pragma once

#include "Bitmap.hpp"
#include "GameEngine.hpp"

std::unique_ptr<GameEngine> g_pGame;
std::unique_ptr<Bitmap>     g_pBackground;
std::unique_ptr<Bitmap>     g_pSaucer[2];
BOOL                        g_bSaucerFlame;
const LONG                  g_iMAXSPEED { 8 };
LONG                        g_iSaucerX;
LONG                        g_iSaucerY;
LONG                        g_iSpeedX;
LONG                        g_iSpeedY;
