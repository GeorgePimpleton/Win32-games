#pragma once

#include <windows.h>
#include <string>
#include "resource.h"

// Windows function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL    CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
