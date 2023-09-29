#include <windows.h>
#include <windowsx.h>

#include "winmsg.hpp"

void OnLButtonDown(HWND wnd, BOOL dblClick, int x, int y, UINT flags)
{
   static PCWSTR aboutLeft = L"This is a modular WinAPI program.\nYou pressed the left mouse button!";

   MessageBoxW(wnd, aboutLeft, L"About", MB_OK | MB_ICONEXCLAMATION);
}

void OnRButtonDown(HWND wnd, BOOL dblClick, int x, int y, UINT flags)
{
   static PCWSTR aboutRight = L"This is a modular WinAPI program.\nYou pressed the right mouse button!";

   MessageBoxW(wnd, aboutRight, L"About", MB_OK | MB_ICONINFORMATION);
}

void OnDestroy(HWND hwnd)
{
   PostQuitMessage(0);
}