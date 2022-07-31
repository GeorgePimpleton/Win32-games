#pragma once

//-----------------------------------------------------------------
// include files
//-----------------------------------------------------------------
#include <windows.h>
#include <memory>
#include "random_toolkit.hpp"
#include "resource.h"
#include "GameEngine.hpp"

//-----------------------------------------------------------------
// global variables
//-----------------------------------------------------------------
std::unique_ptr<GameEngine> g_pGame;
