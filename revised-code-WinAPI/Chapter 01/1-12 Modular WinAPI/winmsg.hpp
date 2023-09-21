#include <windows.h>
#include <windowsx.h>

#pragma once

// handles left mouse button presses
void OnLButtonDown(HWND hwnd, BOOL dlbClick, int x, int y, UINT flags);

// handles right mouse button presses
void OnRButtonDown(HWND hwnd, BOOL dblClick, int x, int y, UINT flags);

// handles WM_DESTROY messages
void OnDestroy(HWND hwnd);
