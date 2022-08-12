#include <windows.h>

#pragma once

// handles left mouse button presses
void OnLButtonDown(HWND hwnd);

// handles right mouse button presses
void OnRButtonDown(HWND hwnd);

// handles WM_DESTROY messages
void OnDestroy(HWND hwnd);
