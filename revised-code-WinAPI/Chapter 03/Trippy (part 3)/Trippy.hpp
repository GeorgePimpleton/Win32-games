#pragma once

#include <windows.h>
#include <memory>
#include "resource.h"
#include "GameEngine.hpp"
#include "random_toolkit_v131.hpp"

std::unique_ptr<GameEngine> g_game;
RECT                        g_rect;
