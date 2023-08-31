#pragma once

#include "random_toolkit.hpp"
#include <vector>
#include "Bitmap.hpp"
#include "GameEngine.hpp"

const UINT g_NUMSLIDES = 7;

std::unique_ptr<GameEngine>          g_game;
std::vector<std::unique_ptr<Bitmap>> g_slides(g_NUMSLIDES);
UINT                                 g_currentSlide;
