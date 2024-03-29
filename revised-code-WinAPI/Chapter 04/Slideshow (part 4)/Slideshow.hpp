#pragma once

#include <windows.h>
#include "resource.h"
#include "GameEngine.hpp"
#include "Bitmap.hpp"
#include <vector>
#include <memory>

const int   g_NUMSLIDES = 7;

std::unique_ptr<GameEngine>          g_game;
std::vector<std::unique_ptr<Bitmap>> g_slides(g_NUMSLIDES);
int                                  g_currentSlide;
