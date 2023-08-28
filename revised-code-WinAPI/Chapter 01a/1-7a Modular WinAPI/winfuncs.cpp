#include "winmsg.hpp"
#include "winfuncs.hpp"

static const WCHAR winName[ ]  = L"ModWin4";
static const WCHAR appTitle[ ] = L"Modular WinAPI Application, Version 4";

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   switch ( message )
   {
      HANDLE_MSG(hwnd, WM_LBUTTONDOWN, OnLButtonDown);
      HANDLE_MSG(hwnd, WM_RBUTTONDOWN, OnRButtonDown);
      HANDLE_MSG(hwnd, WM_DESTROY, OnDestroy);
   }

   return DefWindowProcW(hwnd, message, wParam, lParam);
}

HRESULT InitApplication(HINSTANCE hInst)
{
   WNDCLASS wc { };

   wc.style         = CS_HREDRAW | CS_VREDRAW;
   wc.lpfnWndProc   = WndProc;
   wc.cbClsExtra    = 0;
   wc.cbWndExtra    = 0;
   wc.hInstance     = hInst;
   wc.hIcon         = (HICON)   LoadImageW(NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_SHARED);
   wc.hCursor       = (HCURSOR) LoadImageW(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
   wc.hbrBackground = (HBRUSH)  (COLOR_WINDOW + 1);
   wc.lpszMenuName  = NULL;
   wc.lpszClassName = winName;

   if ( FAILED(RegisterClassW(&wc)) )
   {
      MessageBoxW(NULL, L"Can't Register the Window Class!", winName, MB_OK | MB_ICONERROR);
      return E_FAIL;
   }
   else return S_OK;
}

HRESULT InitInstance(HINSTANCE hInst, int winMode)
{
   HWND hwnd = CreateWindowW(winName, appTitle,
                             WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             NULL, NULL, hInst, NULL);

   if ( NULL == hwnd )
   {
      MessageBoxW(NULL, L"Can't Create the Main Window!", winName, MB_OK | MB_ICONERROR);
      return E_FAIL;
   }

   ShowWindow(hwnd, winMode);
   UpdateWindow(hwnd);

   return S_OK;
}

int MessageLoop( )
{
   MSG msg;

   while ( GetMessageW(&msg, NULL, 0, 0) )
   {
      TranslateMessage(&msg);
      DispatchMessageW(&msg);
   }

   return (int) msg.wParam;
}