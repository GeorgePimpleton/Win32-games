// "Talk to me like I'm a 3 year old!" Programming Lessons - Windows Games

// Slideshow.hpp - game header

#pragma once

#include <vector>
#include "Bitmap.hpp"
#include "GameEngine.hpp"

const UINT g_iNUMSLIDES { 7 };

std::unique_ptr<GameEngine>          g_pGame;
std::vector<std::unique_ptr<Bitmap>> g_pSlides(g_iNUMSLIDES);
UINT                                 g_iCurSlide;
