#pragma once

#include <windows.h>
#include <memory>
#include <array>
#include "resource.h"
#include "GameEngine.hpp"
#include "Bitmap.hpp"

const LONG g_SPEED = 4;

std::unique_ptr<GameEngine>                           g_game;
std::unique_ptr<Bitmap>                               g_background;
std::array<std::array<std::unique_ptr<Bitmap>, 4>, 2> g_cycle;
std::array<POINT, 2>                                  g_cyclePos;
std::array<POINT, 2>                                  g_cycleSpeed;
std::array<std::array<POINT, 100>, 2>                 g_cycleTrail;
std::array<LONG, 2>                                   g_trailLength;
BOOL                                                  g_gameOver;

// game specific functions
void NewGame( );
void UpdateCycles( );
void SteerCycle(LONG cycle, LONG direction);
void EndGame(LONG cycle);
