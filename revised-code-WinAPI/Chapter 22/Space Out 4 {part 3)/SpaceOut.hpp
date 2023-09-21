#pragma once

#include <windows.h>
#include "resource.h"
#include "random_toolkit.hpp"
#include <memory>
#include <vector>
#include <fstream>
#include <string>
#include <format>
#include "GameEngine.hpp"
#include "Bitmap.hpp"
#include "Sprite.hpp"
#include "Background.hpp"
#include "AlienSprite.hpp"

HINSTANCE                         g_inst;
std::unique_ptr<GameEngine>       g_game;
HDC                               g_hOffscreenDC;
HBITMAP                           g_hOffscreenBitmap;
std::unique_ptr<Bitmap>           g_splashBitmap;
std::unique_ptr<Bitmap>           g_desertBitmap;
std::unique_ptr<Bitmap>           g_carBitmap;
std::unique_ptr<Bitmap>           g_smallCarBitmap;
std::unique_ptr<Bitmap>           g_missileBitmap;
std::unique_ptr<Bitmap>           g_smallExplosionBitmap;
std::unique_ptr<Bitmap>           g_largeExplosionBitmap;
std::unique_ptr<Bitmap>           g_gameOverBitmap;
std::unique_ptr<StarryBackground> g_background;
Sprite*                           g_carSprite;
int                               g_fireInputDelay;
int                               g_numLives;
int                               g_score;
BOOL                              g_gameOver;
int                               g_gameOverDelay;
BOOL                              g_demo;
std::vector<int>                  g_hiScores(5);

void AddAlien( );
void UpdateHiScores( );
BOOL ReadHiScores( );
BOOL WriteHiScores( );

extern std::unique_ptr<Bitmap> g_BlobboBitmap;
extern std::unique_ptr<Bitmap> g_BMissileBitmap;
extern std::unique_ptr<Bitmap> g_JellyBitmap;
extern std::unique_ptr<Bitmap> g_JMissileBitmap;
extern std::unique_ptr<Bitmap> g_TimmyBitmap;
extern std::unique_ptr<Bitmap> g_TMissileBitmap;
extern int                     g_difficulty;

