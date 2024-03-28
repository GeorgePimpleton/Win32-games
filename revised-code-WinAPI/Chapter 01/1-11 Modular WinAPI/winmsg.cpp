#include "winmsg.hpp"

// handles WM_LBUTTONDOWN messages
void OnLButtonDown( HWND wnd )
{
   static PCWSTR aboutLeft = L"This is a modular WinAPI program.\nYou pressed the left mouse button!";

   MessageBoxW( wnd, aboutLeft, L"About", MB_OK | MB_ICONEXCLAMATION );
}

// handles WM_RBUTTONDOWN messages
void OnRButtonDown( HWND wnd )
{
   static PCWSTR aboutRight = L"This is a modular WinAPI program.\nYou pressed the right mouse button!";

   MessageBoxW( wnd, aboutRight, L"About", MB_OK | MB_ICONASTERISK );
}

// handles WM_PAINT messages
void OnPaint( HWND wnd )
{
   PAINTSTRUCT ps;
   HDC         dc { BeginPaint( wnd, &ps ) };
   RECT        rect;

   GetClientRect( wnd, &rect );
   DrawTextW( dc, L"This is a modularized WinAPI application!", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER );
   EndPaint( wnd, &ps );
}
// handles WM_DESTROY messages
void OnDestroy( HWND wnd )
{
   PostQuitMessage( 0 );
}