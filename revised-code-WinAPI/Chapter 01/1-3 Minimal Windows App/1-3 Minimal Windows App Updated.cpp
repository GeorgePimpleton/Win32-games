// 1-3 Minimal Windows App Updated.cpp - a very minimal Windows API application skeleton

#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(_In_ HINSTANCE hInst,   _In_opt_ HINSTANCE hPrevInst,
                    _In_ PWSTR     cmdLine, _In_     int       winMode)
{
   static const WCHAR appName[ ] { L"MinWinApp" };

   WNDCLASSW wc { };

   wc.style         = CS_HREDRAW | CS_VREDRAW;
   wc.lpfnWndProc   = WndProc;
   wc.cbClsExtra    = 0;
   wc.cbWndExtra    = 0;
   wc.hInstance     = hInst;
   wc.hIcon         = (HICON)   LoadImageW(NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
   wc.hCursor       = (HCURSOR) LoadImageW(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
   wc.hbrBackground = (HBRUSH)  (COLOR_WINDOW + 1);
   wc.lpszMenuName  = NULL;
   wc.lpszClassName = appName;

   if ( FAILED(RegisterClassW(&wc)) )
   {
      MessageBoxW(NULL, L"Can't Register the Window Class!", appName, MB_OK | MB_ICONERROR);
      return E_FAIL;
   }

   // Win32 is now known as the Windows API (WinAPI) since x64 Windows
   static const WCHAR appTitle[ ] { L"Windows API Skeletal Application" };

   HWND hwnd { CreateWindowW(appName, appTitle,
                             WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             NULL, NULL, hInst, NULL) };

   if ( NULL == hwnd )
   {
      MessageBoxW(NULL, L"Unable to Create the Main Window!", appName, MB_OK | MB_ICONERROR);
      return E_FAIL;
   }

   ShowWindow(hwnd, winMode);
   UpdateWindow(hwnd);

   MSG msg;

   while ( GetMessageW(&msg, NULL, 0, 0) )
   {
      TranslateMessage(&msg);
      DispatchMessageW(&msg);
   }

   // the cast is done for x64 Windows, otherwise VS whinges about possible loss of data
   return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
   switch ( msg )
   {
   case WM_PAINT:
   {
      PAINTSTRUCT ps;
      HDC         hdc = BeginPaint(hwnd, &ps);

      RECT rect;
      GetClientRect(hwnd, &rect);
      DrawTextW(hdc, L"Hello, Windows!", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

      EndPaint(hwnd, &ps);
      return S_OK;
   }

   case WM_DESTROY:
      PostQuitMessage(0);
      return S_OK;
   }

   return DefWindowProcW(hwnd, msg, wParam, lParam);
}