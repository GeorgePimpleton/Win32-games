// Windows function declarations

#include <windows.h>
#include <winerror.h>

#pragma once

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HRESULT          InitApplication(HINSTANCE);
HRESULT          InitInstance(HINSTANCE, int);
int              MessageLoop();
