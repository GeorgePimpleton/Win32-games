#pragma once

#include <windows.h>
#include "resource.h"
#include "random_toolkit.hpp"
#include <memory>
#include <array>
#include "GameEngine.hpp"
#include "Bitmap.hpp"
#include "Sprite.hpp"
#include "ScrollingBackground.hpp"
#include "MotorcycleSprite.hpp"

HINSTANCE                            g_inst;
std::unique_ptr<GameEngine>          g_game;
HDC                                  g_offscreenDC;
HBITMAP                              g_offscreenBitmap;
std::unique_ptr<Bitmap>              g_splashBitmap;
std::unique_ptr<BackgroundLayer>     g_BGRoadLayer;
std::unique_ptr<BackgroundLayer>     g_BGTreesLayer;
std::unique_ptr<BackgroundLayer>     g_BGMountainsLayer;
std::unique_ptr<BackgroundLayer>     g_BGSkyLayer;
std::unique_ptr<Bitmap>              g_pJumperBitmap;
std::unique_ptr<Bitmap>              g_busBitmap;
std::unique_ptr<Bitmap>              g_rampBitmap[2];
std::unique_ptr<Bitmap>              g_gameOverBitmap;
std::unique_ptr<ScrollingBackground> g_background;
std::unique_ptr<MotorcycleSprite>    g_jumperSprite;
std::unique_ptr<Sprite>              g_launchRampSprite;
std::unique_ptr<Sprite>              g_landingRampSprite;
std::array<std::unique_ptr<Sprite>, 7>              g_busSprite;
int                                  g_inputDelay;
BOOL                                 g_gameOver;
BOOL                                 g_splash;

void GameNew();
void NewJump(int numBuses);
