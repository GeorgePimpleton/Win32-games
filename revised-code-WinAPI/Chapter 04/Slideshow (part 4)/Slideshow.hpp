#pragma once

#include <windows.h>
#include <array>
#include <memory>
#include "resource.h"
#include "Bitmap.hpp"
#include "GameEngine.hpp"

constexpr UINT g_NUMSLIDES = 7;

HINSTANCE                                        g_inst;
std::unique_ptr<GameEngine>                      g_game;
std::array<std::unique_ptr<Bitmap>, g_NUMSLIDES> g_slides;
UINT                                             g_currentSlide;
