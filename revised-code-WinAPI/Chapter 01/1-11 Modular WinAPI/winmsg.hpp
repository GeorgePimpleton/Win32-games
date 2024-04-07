#include <windows.h>

#pragma once

// handles left mouse button presses
void OnLButtonDown( HWND );

// handles right mouse button presses
void OnRButtonDown( HWND );

// handles WM_PAINT messages
void OnPaint( HWND );

// handles WM_DESTROY messages
void OnDestroy( HWND );
