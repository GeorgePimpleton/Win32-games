#pragma once

#include <windows.h>
#include <windowsx.h>

#pragma once

// handles left mouse button presses
void OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);

// handles right mouse button presses
void OnRButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);

// handles WM_DESTROY messages
void OnDestroy(HWND hwnd);
