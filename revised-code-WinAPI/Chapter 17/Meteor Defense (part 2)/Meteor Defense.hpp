#pragma once

#include <windows.h>
#include <memory>
#include "resource.h"
#include "random_toolkit_v131.hpp"
#include "GameEngine.hpp"
#include "Bitmap.hpp"
#include "Sprite.hpp"
#include "Background.hpp"

HDC     g_offscreenDC;
HBITMAP g_offscreenBitmap;

std::unique_ptr<GameEngine>       g_game;
std::unique_ptr<Bitmap>           g_groundBitmap;
std::unique_ptr<Bitmap>           g_cityBitmap;
std::unique_ptr<Bitmap>           g_meteorBitmap;
std::unique_ptr<Bitmap>           g_missileBitmap;
std::unique_ptr<Bitmap>           g_explosionBitmap;
std::unique_ptr<Bitmap>           g_gameOverBitmap;
std::unique_ptr<StarryBackground> g_background;

int  g_numCities;
int  g_score;
int  g_difficulty;
BOOL g_gameOver;

void AddMeteor();
