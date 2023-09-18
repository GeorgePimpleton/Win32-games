#pragma once

#include <windows.h>
#include "Resource.h"
#include "GameEngine.h"
#include "Bitmap.h"

HINSTANCE   g_hInstance;
GameEngine* g_pGame;
const int   g_iMAXSPEED = 8;
Bitmap*     g_pBackground;
Bitmap*     g_pSaucer[ 2 ];
int         g_iSaucerX;
int         g_iSaucerY;
int         g_iSpeedX;
int         g_iSpeedY;
BOOL        g_bSaucerFlame;
