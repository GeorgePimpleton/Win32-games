#pragma once

#include <windows.h>
#include "Resource.h"
#include "GameEngine.h"
#include "Bitmap.h"

HINSTANCE   g_hInstance;
GameEngine* g_pGame;
const int   g_iMAXSPEED = 8;
Bitmap*     g_pBackground;
Bitmap*     g_pSaucer;
int         g_iSaucerX, g_iSaucerY;
int         g_iSpeedX, g_iSpeedY;
