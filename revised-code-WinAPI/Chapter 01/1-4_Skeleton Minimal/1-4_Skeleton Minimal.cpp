// 1-4_Skeleton Minimal.cpp - minimal Windows skeleton source

#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(_In_     HINSTANCE hInstance,
                    _In_opt_ HINSTANCE hPrevInstance,
                    _In_     PWSTR szCmdLine,
                    _In_     int iCmdShow)
{
   static WCHAR szAppName[ ] { L"Skeleton" };
   WNDCLASSEXW  wndclass     { };

   wndclass.cbSize        = sizeof(wndclass);
   wndclass.style         = CS_HREDRAW | CS_VREDRAW;
   wndclass.lpfnWndProc   = WndProc;
   wndclass.cbClsExtra    = 0;
   wndclass.cbWndExtra    = 0;
   wndclass.hInstance     = hInstance;
   wndclass.hIcon         = (HICON)   LoadImageW(hInstance, MAKEINTRESOURCEW(IDI_SKELETON), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
   wndclass.hIconSm       = (HICON)   LoadImageW(hInstance, MAKEINTRESOURCEW(IDI_SKELETON_SM), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
   wndclass.hCursor       = (HCURSOR) LoadImageW(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
   wndclass.hbrBackground = (HBRUSH)  (COLOR_WINDOW + 1);
   wndclass.lpszMenuName  = NULL;
   wndclass.lpszClassName = szAppName;

   if (!RegisterClassExW(&wndclass))
   {
      return 0;
   }

   HWND hWindow { CreateWindowW(szAppName, L"Minimal Windows Skeleton Application",
                                WS_OVERLAPPEDWINDOW,
                                CW_USEDEFAULT, CW_USEDEFAULT,
                                CW_USEDEFAULT, CW_USEDEFAULT,
                                NULL, NULL, hInstance, NULL) };

   ShowWindow(hWindow, iCmdShow);
   UpdateWindow(hWindow);

   MSG msg;

   while (GetMessageW(&msg, NULL, 0, 0))
   {
      TranslateMessage(&msg);
      DispatchMessageW(&msg);
   }

   return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
   switch (msg)
   {
   case WM_PAINT:
      HDC         hDC;
      PAINTSTRUCT ps;

      hDC = BeginPaint(hWindow, &ps);

      RECT rect;

      GetClientRect(hWindow, &rect);

      DrawTextW(hDC, L"This is a minimal Windows skeleton application!", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

      EndPaint(hWindow, &ps);
      return 0;

   case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
   }

   return DefWindowProcW(hWindow, msg, wParam, lParam);
}