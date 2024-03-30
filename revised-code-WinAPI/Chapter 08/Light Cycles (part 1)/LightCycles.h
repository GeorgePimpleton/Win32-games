#pragma once

#include <windows.h>
#include "Resource.h"
#include "GameEngine.h"
#include "Bitmap.h"

HINSTANCE   g_hInstance;
GameEngine* g_pGame;
Bitmap*     g_pBackground;
Bitmap*     g_pCycle[2][4];
POINT       g_ptCyclePos[2];
POINT       g_ptCycleSpeed[2];
POINT       g_ptCycleTrail[2][100];
int         g_iTrailLen[2];
const int   g_iSPEED = 4;
BOOL        g_bGameOver;

void NewGame( );
void UpdateCycles( );
void SteerCycle( int iCycle, int iDirection );
void EndGame( int iCycle );
