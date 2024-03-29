#pragma once

#include <windows.h>
#include "resource.h"
#include "GameEngine.hpp"
#include "Bitmap.hpp"
#include <array>
#include <memory>

constexpr UINT g_NUMSLIDES = 7;

std::unique_ptr<GameEngine>                      g_game;
std::array<std::unique_ptr<Bitmap>, g_NUMSLIDES> g_slides;
int                                              g_currentSlide;
