#include <windows.h>
#include <windowsx.h>

#include "winmsg.hpp"

void OnLButtonDown( HWND wnd, BOOL dblClick, int x, int y, UINT flags )
{
   static PCWSTR aboutLeft = L"This is a modular WinAPI program.\nYou pressed the left mouse button!";

   MessageBoxW( wnd, aboutLeft, L"About", MB_OK | MB_ICONEXCLAMATION );
}

void OnRButtonDown( HWND wnd, BOOL dblClick, int x, int y, UINT flags )
{
   static PCWSTR aboutRight = L"This is a modular WinAPI program.\nYou pressed the right mouse button!";

   MessageBoxW( wnd, aboutRight, L"About", MB_OK | MB_ICONINFORMATION );
}

void OnPaint( HWND wnd )
{
   PAINTSTRUCT ps;
   HDC         dc { BeginPaint( wnd, &ps ) };
   RECT        rect;

   GetClientRect( wnd, &rect );
   DrawTextW( dc, L"This WinAPI app uses idle-time background processing.", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER );
   EndPaint( wnd, &ps );
}

void OnDestroy( HWND hwnd )
{
   PostQuitMessage( 0 );
}
