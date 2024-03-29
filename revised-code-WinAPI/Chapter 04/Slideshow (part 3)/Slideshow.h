#pragma once

#include <windows.h>
#include "Resource.h"
#include "GameEngine.h"
#include "Bitmap.h"

HINSTANCE   g_hInstance;
GameEngine* g_pGame;
const int   g_iNUMSLIDES = 7;
Bitmap*     g_pSlides[g_iNUMSLIDES];
int         g_iCurSlide;
