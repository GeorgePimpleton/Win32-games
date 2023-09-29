#include "winfuncs.hpp"

PCWSTR winName  = L"ModWin2";
PCWSTR appTitle = L"Modular WinAPI Application, Version 2";

LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
   static PCWSTR aboutLeft  = L"This is a modular WinAPI program.\nYou pressed the left mouse button!";
   static PCWSTR aboutRight = L"This is a modular WinAPI program.\nYou pressed the right mouse button!";

   switch ( msg )
   {
   case WM_LBUTTONDOWN:
      MessageBoxW(wnd, aboutLeft, L"About", MB_OK | MB_ICONEXCLAMATION);
      return S_OK;

   case WM_RBUTTONDOWN:
      MessageBoxW(wnd, aboutRight, L"About", MB_OK | MB_ICONINFORMATION);
      return S_OK;

   case WM_DESTROY:
      PostQuitMessage(0);
      return S_OK;
   }

   return DefWindowProcW(wnd, msg, wParam, lParam);
}

HRESULT InitApplication(HINSTANCE inst)
{
   WNDCLASSW wc = { };

   wc.style         = CS_HREDRAW | CS_VREDRAW;
   wc.lpfnWndProc   = WndProc;
   wc.cbClsExtra    = 0;
   wc.cbWndExtra    = 0;
   wc.hInstance     = inst;
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

HRESULT InitInstance(HINSTANCE inst, int winMode)
{
   HWND wnd = CreateWindowW(winName, appTitle,
                            WS_OVERLAPPEDWINDOW,
                            CW_USEDEFAULT, CW_USEDEFAULT,
                            CW_USEDEFAULT, CW_USEDEFAULT,
                            NULL, NULL, inst, NULL);

   if ( wnd == NULL )
   {
      MessageBoxW(NULL, L"Can't Create the Main Window!", winName, MB_OK | MB_ICONERROR);
      return E_FAIL;
   }

   ShowWindow(wnd, winMode);
   UpdateWindow(wnd);

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