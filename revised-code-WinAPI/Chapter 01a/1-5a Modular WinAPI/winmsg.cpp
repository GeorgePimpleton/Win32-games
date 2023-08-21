#include <windows.h>
#include <windowsx.h>

#include "winmsg.hpp"

void OnLButtonDown(HWND hwnd, BOOL dblClick, int x, int y, UINT flags)
{
   static const WCHAR aboutLeft[ ] = L"This is a modular WinAPI program.\nYou pressed the left mouse button!";

   MessageBeep(MB_ICONEXCLAMATION);
   MessageBoxW(hwnd, aboutLeft, L"About", MB_OK | MB_ICONINFORMATION);
}

void OnRButtonDown(HWND hwnd, BOOL dblClick, int x, int y, UINT flags)
{
   static const WCHAR aboutRight[ ] = L"This is a modular WinAPI program.\nYou pressed the right mouse button!";

   MessageBeep(MB_ICONASTERISK);
   MessageBoxW(hwnd, aboutRight, L"About", MB_OK | MB_ICONINFORMATION);
}

void OnDestroy(HWND hwnd)
{
   PostQuitMessage(0);
}