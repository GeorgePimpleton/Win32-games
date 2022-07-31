// 1-6_Skeleton Realistic.hpp - realistic Windows skeleton header

#pragma once

// includes
#include <windows.h>
#include "resource.h"

// Windows function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL    CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
