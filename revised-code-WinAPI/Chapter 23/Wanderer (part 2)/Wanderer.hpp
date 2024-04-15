#pragma once

#include <windows.h>
#include <memory>
#include "resource.h"
#include "GameEngine.hpp"
#include "Bitmap.hpp"
#include "Sprite.hpp"
#include "ScrollingBackground.hpp"
#include "PersonSprite.hpp"

HINSTANCE                            g_inst;
std::unique_ptr<GameEngine>          g_game;
HDC                                  g_offscreenDC;
HBITMAP                              g_offscreenBitmap;
std::unique_ptr<BackgroundLayer>     g_BGLandscapeLayer;
std::unique_ptr<BackgroundLayer>     g_FGCloudsLayer;
std::unique_ptr<ScrollingBackground> g_background;
std::unique_ptr<ScrollingBackground> g_foreground;
std::unique_ptr<Bitmap>              g_personBitmap;
std::unique_ptr<PersonSprite>        g_personSprite;
int                                  g_inputDelay;

void UpdateView(SCROLLDIR);