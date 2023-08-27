#pragma once

#include <windows.h>
#include "resource.h"
#include "GameEngine.hpp"
#include "Bitmap.hpp"

std::unique_ptr<GameEngine> g_game;
std::unique_ptr<Bitmap>     g_background;
std::unique_ptr<Bitmap>     g_cycle[ 2 ][ 4 ];
POINT                       g_cyclePos[ 2 ];
POINT                       g_cycleSpeed[ 2 ];
POINT                       g_cycleTrail[ 2 ][ 100 ];
int                         g_trailLength[ 2 ];
const int                   g_SPEED { 4 };
BOOL                        g_gameOver;

// game specific functions
void NewGame( );
void UpdateCycles( );
void SteerCycle(int cycle, int direction);
void EndGame(int cycle);
