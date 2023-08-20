// 1-4 Skeleton Minimal.cpp - minimal Windows skeleton source

#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR szCmdLine, _In_ int iCmdShow)
{
   static WCHAR szAppName[ ] { L"Skeleton" };
   WNDCLASSEXW  wc           { };

   wc.cbSize        = sizeof(wc);
   wc.style         = CS_HREDRAW | CS_VREDRAW;
   wc.lpfnWndProc   = WndProc;
   wc.cbClsExtra    = 0;
   wc.cbWndExtra    = 0;
   wc.hInstance     = hInstance;
   wc.hIcon         = (HICON)   LoadImageW(hInstance, MAKEINTRESOURCEW(IDI_SKELETON), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
   wc.hIconSm       = (HICON)   LoadImageW(hInstance, MAKEINTRESOURCEW(IDI_SKELETON_SM), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
   wc.hCursor       = (HCURSOR) LoadImageW(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
   wc.hbrBackground = (HBRUSH)  (COLOR_WINDOW + 1);
   wc.lpszMenuName  = NULL;
   wc.lpszClassName = szAppName;

   // another way to test for failure registering the window class
   // I personally prefer using the macros, they are understandable what is happening
   if ( !RegisterClassExW(&wc) )
   {
      MessageBoxW(NULL, L"Can't Register the Window Class!", szAppName, MB_OK | MB_ICONERROR);
      return E_FAIL;
   }

   HWND hwnd { CreateWindowW(szAppName, L"Minimal Windows Skeleton Application",
                             WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             NULL, NULL, hInstance, NULL) };

   if ( NULL == hwnd )
   {
      MessageBoxW(NULL, L"Unable to Create the Main Window!", szAppName, MB_OK | MB_ICONERROR);
      return E_FAIL;
   }

   ShowWindow(hwnd, iCmdShow);
   UpdateWindow(hwnd);

   MSG msg;

   while ( GetMessageW(&msg, NULL, 0, 0) )
   {
      TranslateMessage(&msg);
      DispatchMessageW(&msg);
   }

   return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
   switch ( msg )
   {
   case WM_PAINT:
   {
      PAINTSTRUCT ps;
      HDC         hDC = BeginPaint(hWindow, &ps);

      RECT rect;

      GetClientRect(hWindow, &rect);

      DrawTextW(hDC, L"This is a minimal Windows skeleton application!", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

      EndPaint(hWindow, &ps);
      return S_OK;
   }

   case WM_DESTROY:
      PostQuitMessage(0);
      return S_OK;
   }

   return DefWindowProcW(hWindow, msg, wParam, lParam);
}