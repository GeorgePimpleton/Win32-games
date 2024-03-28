#pragma once

// stop the WinAPI MIN/MAX macros from being used
// instead of C++ std::/min/std::max
#define NOMINMAX

#include <windows.h>
#include "resource.h"
#include "GameEngine.hpp"
#include "random_toolkit_v1312.hpp"

GameEngine* g_pGame;
