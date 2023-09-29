#include "winmsg.hpp"

// handles WM_LBUTTONDOWN messages
void OnLButtonDown(HWND wnd)
{
   static PCWSTR aboutLeft = L"This is a modular WinAPI program.\nYou pressed the left mouse button!";

   MessageBoxW(wnd, aboutLeft, L"About", MB_OK | MB_ICONEXCLAMATION);
}

// handles WM_RBUTTONDOWN messages
void OnRButtonDown(HWND wnd)
{
   static PCWSTR aboutRight = L"This is a modular WinAPI program.\nYou pressed the right mouse button!";

   MessageBoxW(wnd, aboutRight, L"About", MB_OK | MB_ICONASTERISK);
}

// handles WM_DESTROY messages
void OnDestroy(HWND wnd)
{
   PostQuitMessage(0);
}