#pragma once

#include <windows.h>
#include "resource.h"
#include <memory>
#include "GameEngine.hpp"
#include "Bitmap.hpp"
#include "Sprite.hpp"
#include "Background.hpp"
#include "AlienSprite.hpp"

HINSTANCE                   g_inst;
std::unique_ptr<GameEngine> g_game;
HDC                         g_hOffscreenDC;
HBITMAP                     g_hOffscreenBitmap;

std::unique_ptr<Bitmap>           g_splashBitmap;
std::unique_ptr<Bitmap>           g_desertBitmap;
std::unique_ptr<Bitmap>           g_carBitmap;
std::unique_ptr<Bitmap>           g_smallCarBitmap;
std::unique_ptr<Bitmap>           g_missileBitmap;
std::unique_ptr<Bitmap>           g_BlobboBitmap;
std::unique_ptr<Bitmap>           g_BMissileBitmap;
std::unique_ptr<Bitmap>           g_JellyBitmap;
std::unique_ptr<Bitmap>           g_JMissileBitmap;
std::unique_ptr<Bitmap>           g_TimmyBitmap;
std::unique_ptr<Bitmap>           g_TMissileBitmap;
std::unique_ptr<Bitmap>           g_smallExplosionBitmap;
std::unique_ptr<Bitmap>           g_largeExplosionBitmap;
std::unique_ptr<Bitmap>           g_gameOverBitmap;
std::unique_ptr<StarryBackground> g_background;
std::unique_ptr<Sprite>           g_carSprite;

UINT g_fireInputDelay;
UINT g_numLives;
UINT g_score;
UINT g_difficulty;
BOOL g_gameOver;
BOOL g_splash;

void AddAlien( );
