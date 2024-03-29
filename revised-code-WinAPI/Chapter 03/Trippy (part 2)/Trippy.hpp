#pragma once

#define NOMINMAX

#include <windows.h>
#include "resource.h"
#include "GameEngine.hpp"
#include "random_toolkit_v1312.hpp"
#include <memory>

std::unique_ptr<GameEngine> g_game;
RECT                        g_rect;
