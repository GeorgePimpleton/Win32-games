#include <windows.h>
#include <windowsx.h>

#pragma once

// handles left mouse button presses
void OnLButtonDown( HWND wnd, BOOL dlbClick, int x, int y, UINT flags );

// handles right mouse button presses
void OnRButtonDown( HWND wnd, BOOL dblClick, int x, int y, UINT flags );

// handles WM_PAINT messages
void OnPaint( HWND hwnd );

// handles WM_DESTROY messages
void OnDestroy( HWND wnd );
