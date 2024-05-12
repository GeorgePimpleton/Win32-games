#pragma once

#include <windows.h>
#include "resource.h"
#include "GameEngine.hpp"

#include <memory> // for smart pointers

std::unique_ptr<GameEngine> g_game;
