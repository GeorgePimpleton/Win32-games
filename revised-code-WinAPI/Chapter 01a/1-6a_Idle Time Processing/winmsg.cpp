#include <windows.h>
#include <windowsx.h>

#include "winmsg.hpp"

void OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
   static const WCHAR szAboutLeft[] = L"This is a modular Win API program.\nYou pressed the left mouse button!";

   MessageBeep(MB_ICONEXCLAMATION);
   MessageBoxW(hwnd, szAboutLeft, L"About", MB_OK | MB_ICONINFORMATION);
}

void OnRButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
   static const WCHAR szAboutRight[] = L"This is a modular Win API program.\nYou pressed the right mouse button!";

   MessageBeep(MB_ICONASTERISK);
   MessageBoxW(hwnd, szAboutRight, L"About", MB_OK | MB_ICONINFORMATION);
}

void OnDestroy(HWND hwnd)
{
   PostQuitMessage(0);
}