#include "winmsg.hpp"

// handles WM_LBUTTONDOWN messages
void OnLButtonDown(HWND hwnd)
{
   static const WCHAR aboutLeft[ ] = L"This is a modular WinAPI program.\nYou pressed the left mouse button!";

   MessageBeep(MB_ICONEXCLAMATION);
   MessageBoxW(hwnd, aboutLeft, L"About", MB_OK | MB_ICONINFORMATION);
}

// handles WM_RBUTTONDOWN messages
void OnRButtonDown(HWND hwnd)
{
   static const WCHAR aboutRight[ ] = L"This is a modular WinAPI program.\nYou pressed the right mouse button!";

   MessageBeep(MB_ICONASTERISK);
   MessageBoxW(hwnd, aboutRight, L"About", MB_OK | MB_ICONINFORMATION);
}

// handles WM_DESTROY messages
void OnDestroy(HWND hwnd)
{
   PostQuitMessage(0);
}