#include "winmsg.hpp"

// handles WM_LBUTTONDOWN messages
void OnLButtonDown(HWND wnd)
{
   static const WCHAR aboutLeft[ ] = L"This is a modular WinAPI program.\nYou pressed the left mouse button!";

   MessageBeep(MB_ICONEXCLAMATION);
   MessageBoxW(wnd, aboutLeft, L"About", MB_OK | MB_ICONINFORMATION);
}

// handles WM_RBUTTONDOWN messages
void OnRButtonDown(HWND wnd)
{
   static const WCHAR aboutRight[ ] = L"This is a modular WinAPI program.\nYou pressed the right mouse button!";

   MessageBeep(MB_ICONASTERISK);
   MessageBoxW(wnd, aboutRight, L"About", MB_OK | MB_ICONINFORMATION);
}

// handles WM_DESTROY messages
void OnDestroy(HWND wnd)
{
   PostQuitMessage(0);
}