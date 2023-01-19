#include "winmsg.hpp"

// handles WM_LBUTTONDOWN messages
void OnLButtonDown(HWND hwnd)
{
   static const WCHAR szAboutLeft[] = L"This is a modular Win API program.\nYou pressed the left mouse button!";

   MessageBeep(MB_ICONEXCLAMATION);
   MessageBoxW(hwnd, szAboutLeft, L"About", MB_OK | MB_ICONINFORMATION);
}

// handles WM_RBUTTONDOWN messages
void OnRButtonDown(HWND hwnd)
{
   static const WCHAR szAboutRight[] = L"This is a modular Win API program.\nYou pressed the right mouse button!";

   MessageBeep(MB_ICONASTERISK);
   MessageBoxW(hwnd, szAboutRight, L"About", MB_OK | MB_ICONINFORMATION);
}

// handles WM_DESTROY messages
void OnDestroy(HWND hwnd)
{
   PostQuitMessage(0);
}