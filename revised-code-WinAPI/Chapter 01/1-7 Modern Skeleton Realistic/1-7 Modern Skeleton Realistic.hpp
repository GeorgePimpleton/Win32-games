#pragma once

#include <windows.h>
#include <windowsx.h>
#include <string>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL    CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
