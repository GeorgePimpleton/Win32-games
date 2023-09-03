#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(_In_ HINSTANCE inst, _In_opt_ HINSTANCE prevInst, _In_ PWSTR cmdLine, _In_ int winMode)
{
   static const WCHAR appName[ ] { L"Skeleton" };
   WNDCLASSEXW        wc         { };

   wc.cbSize        = sizeof(wc);
   wc.style         = CS_HREDRAW | CS_VREDRAW;
   wc.lpfnWndProc   = WndProc;
   wc.cbClsExtra    = 0;
   wc.cbWndExtra    = 0;
   wc.hInstance     = inst;
   wc.hIcon         = (HICON)   LoadImageW(inst, MAKEINTRESOURCEW(IDI_SKELETON), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
   wc.hIconSm       = (HICON)   LoadImageW(inst, MAKEINTRESOURCEW(IDI_SKELETON_SM), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
   wc.hCursor       = (HCURSOR) LoadImageW(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
   wc.hbrBackground = (HBRUSH)  (COLOR_WINDOW + 1);
   wc.lpszMenuName  = NULL;
   wc.lpszClassName = appName;

   // another way to test for failure registering the window class
   // I personally prefer using the macros, they are understandable what is happening
   if ( !RegisterClassExW(&wc) )
   {
      MessageBoxW(NULL, L"Can't Register the Window Class!", appName, MB_OK | MB_ICONERROR);
      return E_FAIL;
   }

   HWND wnd { CreateWindowW(appName, L"Minimal Windows Skeleton Application",
                            WS_OVERLAPPEDWINDOW,
                            CW_USEDEFAULT, CW_USEDEFAULT,
                            CW_USEDEFAULT, CW_USEDEFAULT,
                            NULL, NULL, inst, NULL) };

   if ( NULL == wnd )
   {
      MessageBoxW(NULL, L"Unable to Create the Main Window!", appName, MB_OK | MB_ICONERROR);
      return E_FAIL;
   }

   ShowWindow(wnd, winMode);
   UpdateWindow(wnd);

   MSG msg;

   while ( GetMessageW(&msg, NULL, 0, 0) )
   {
      TranslateMessage(&msg);
      DispatchMessageW(&msg);
   }

   return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
   switch ( msg )
   {
   case WM_PAINT:
   {
      PAINTSTRUCT ps;
      HDC         dc = BeginPaint(wnd, &ps);

      RECT rect;

      GetClientRect(wnd, &rect);

      DrawTextW(dc, L"This is a minimal Windows© skeleton application!", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

      EndPaint(wnd, &ps);
      return S_OK;
   }

   case WM_DESTROY:
      PostQuitMessage(0);
      return S_OK;
   }

   return DefWindowProcW(wnd, msg, wParam, lParam);
}