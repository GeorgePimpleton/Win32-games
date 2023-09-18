#include <windows.h>
#include <memory>
#include <vector>
#include "resource.h"
#include "Bitmap.hpp"
#include "Sprite.hpp"
#include "GameEngine.hpp"
#include "random_toolkit.hpp"

std::unique_ptr<GameEngine>          g_game;
std::unique_ptr<Bitmap>              g_highwayBitmap;
std::unique_ptr<Bitmap>              g_chickenBitmap;
std::unique_ptr<Bitmap>              g_chickenHeadBitmap;
std::vector<std::unique_ptr<Bitmap>> g_carBitmaps(4);
BOOL                                 g_gameOver;
HDC                                  g_offscreenDC;
HBITMAP                              g_offscreenBitmap;
UINT                                 g_inputDelay;
UINT                                 g_numLives;
UINT                                 g_score;
Sprite*                              g_chickenSprite;

void MoveChicken(int xDistance, int yDistance);
