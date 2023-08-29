#include <windows.h>
#include <vector>
#include "resource.h"
#include "Bitmap.hpp"
#include "Sprite.hpp"
#include "GameEngine.hpp"
#include "random_toolkit.hpp"

GameEngine* g_game;
Bitmap*     g_highwayBitmap;
Bitmap*     g_chickenBitmap;
Bitmap*     g_carBitmaps[4];
Bitmap*     g_chickenHeadBitmap;
BOOL        g_gameOver;
HDC         g_offscreenDC;
HBITMAP     g_offscreenBitmap;
UINT        g_inputDelay;
UINT        g_numLives;
UINT        g_score;
Sprite*     g_chickenSprite;

// HELPER GAME FUNCTION DECLARATION ============================================
void MoveChicken(int xDistance, int yDistance);
