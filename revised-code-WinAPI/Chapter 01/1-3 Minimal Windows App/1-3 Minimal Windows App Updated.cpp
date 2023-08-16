// 1-3 Minimal Windows App Updated.cpp - a very minimal Windows API application skeleton

#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(_In_      HINSTANCE hInstance,
                    _In_opt_  HINSTANCE hPrevInstance,
                    _In_      PWSTR szCmdLine,
                    _In_      int nWinMode)
{
   static const WCHAR szAppName[ ] { L"MinimalWindowsApp" };

   WNDCLASSEXW wc { };

   wc.cbSize        = sizeof(WNDCLASSEX);
   wc.style         = CS_HREDRAW | CS_VREDRAW;
   wc.lpfnWndProc   = WndProc;
   wc.cbClsExtra    = 0;
   wc.cbWndExtra    = 0;
   wc.hInstance     = hInstance;
   wc.hIcon         = (HICON)   LoadImageW(NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
   wc.hIconSm       = (HICON)   LoadImageW(NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
   wc.hCursor       = (HCURSOR) LoadImageW(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
   wc.hbrBackground = (HBRUSH)  (COLOR_WINDOW + 1);
   wc.lpszMenuName  = NULL;
   wc.lpszClassName = szAppName;

   if ( 0 == RegisterClassExW(&wc) )
   {
      MessageBoxW(NULL, L"Can't Register the Window Class!", szAppName, MB_OK | MB_ICONERROR);
      return E_FAIL;
   }

   // Win32 is now known as the Windows API since x64 Windows
   static const WCHAR szAppTitle[ ] { L"Windows API Skeletal Application" };

   HWND hwnd { CreateWindowW(szAppName, szAppTitle,
                             WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             NULL, NULL, hInstance, NULL) };

   if ( NULL == hwnd )
   {
      MessageBoxW(NULL, L"Unable to Create the Main Window!", szAppName, MB_OK | MB_ICONERROR);
      return E_FAIL;
   }

   ShowWindow(hwnd, nWinMode);
   UpdateWindow(hwnd);

   MSG msg;

   while ( GetMessageW(&msg, NULL, 0, 0) )
   {
      // process the message
      TranslateMessage(&msg);
      DispatchMessageW(&msg);
   }

   // the cast is done for x64 Windows, otherwise VS whinges about possible loss of data
   return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   switch ( message )
   {
   case WM_PAINT:
      HDC         hdc;
      PAINTSTRUCT ps;

      hdc = BeginPaint(hwnd, &ps);

      RECT rect;
      GetClientRect(hwnd, &rect);
      DrawTextW(hdc, L"Hello, Windows!", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

      EndPaint(hwnd, &ps);
      return S_OK;

   case WM_DESTROY:
      PostQuitMessage(0);
      return S_OK;
   }

   return DefWindowProcW(hwnd, message, wParam, lParam);
}