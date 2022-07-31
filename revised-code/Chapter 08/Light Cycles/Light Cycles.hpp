#pragma once

#include <windows.h>
#include "resource.h"
#include "GameEngine.hpp"
#include "Bitmap.hpp"

std::unique_ptr<GameEngine> g_pGame;
std::unique_ptr<Bitmap>     g_pBackground;
std::unique_ptr<Bitmap>     g_pCycle[2][4];
POINT                       g_ptCyclePos[2];
POINT                       g_ptCycleSpeed[2];
POINT                       g_ptCycleTrail[2][100];
int                         g_iTrailLen[2];
const int                   g_iSPEED { 4 };
BOOL                        g_bGameOver;

void NewGame();
void UpdateCycles();
void SteerCycle(int iCycle, int iDirection);
void EndGame(int iCycle);
