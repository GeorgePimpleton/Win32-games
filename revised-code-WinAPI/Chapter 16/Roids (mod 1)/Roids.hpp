// "Talk to me like I'm a 3 year old!" Programming Lessons - Windows Games

// Roids.hpp - Game Header

#pragma once

#include <windows.h>
#include <vector>
#include "resource.h"
#include "Bitmap.hpp"
#include "Sprite.hpp"
#include "GameEngine.hpp"
#include "Background.hpp"

GameEngine*       _pGame;
HDC               _hOffscreenDC;
HBITMAP           _hOffscreenBitmap;
Bitmap*           _pAsteroidBitmap;
StarryBackground* _pBackground;
