#pragma once

#include <windows.h>
#include <memory>
#include <vector>
#include "resource.h"
#include "GameEngine.hpp"
#include "Bitmap.hpp"
#include "random_toolkit.hpp"

const LONG                      g_SPEED = 4;

std::unique_ptr<GameEngine>     g_game;
std::unique_ptr<Bitmap>         g_background;
std::unique_ptr<Bitmap>         g_cycle[ 2 ][ 4 ];
std::vector<POINT>              g_cyclePos(2);
std::vector<POINT>              g_cycleSpeed(2);
std::vector<std::vector<POINT>> g_cycleTrail(2, std::vector<POINT>(100));
std::vector<LONG>               g_trailLength(2);
BOOL                            g_gameOver;

// game specific functions
void NewGame( );
void UpdateCycles( );
void SteerCycle(LONG cycle, LONG direction);
void EndGame(LONG cycle);
