// 1-3_Minimal Windows App.cpp - a very minimal Windows API application skeleton

// INCLUDES ====================================================================
#include <windows.h>

// FUNCTION PROTOTYPES =========================================================
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// entry point for a Windows application =======================================
int WINAPI wWinMain(_In_      HINSTANCE hInstance,
                    _In_opt_  HINSTANCE hPrevInstance,
                    _In_      PWSTR szCmdLine,
                    _In_      int nWinMode)
{
   // define the window class name
   static const WCHAR szAppName[]  { L"1-3_MinimalWindowsApp" };

   // create an instance of the window class structure
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

   if (0 == RegisterClassExW(&wc))
   {
      MessageBoxW(NULL, L"Can't Register the Window Class!", szAppName, MB_OK | MB_ICONERROR);
      return E_FAIL;
   }

   // define the application title
   static const WCHAR szAppTitle[] { L"WinAPI Skeletal Application" };

   // create the window
   HWND hwnd { CreateWindowW(szAppName, szAppTitle,
                             WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             NULL, NULL, hInstance, NULL) };

   // check if the window was created, exit if fail
   if (NULL == hwnd)
   {
      MessageBoxW(NULL, L"Unable to Create the Main Window!", szAppName, MB_OK | MB_ICONERROR);
      return E_FAIL;
   }

   // show and update the window
   ShowWindow(hwnd, nWinMode);
   UpdateWindow(hwnd);

   // enter the main message loop
   MSG msg;

   while (GetMessageW(&msg, NULL, 0, 0))
   {
      // process the message
      TranslateMessage(&msg);
      DispatchMessageW(&msg);
   }

   // the app is done, exit normally, return control to Windows
   return (int) msg.wParam;
}

// processes the messages that Windows sends to the application ================
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   // choose which Windows messages you want to use (handle)
   switch(message)
   {
   case WM_PAINT:
      HDC         hdc;
      PAINTSTRUCT ps;

      hdc = BeginPaint(hwnd, &ps);

      // draw some text centered in the client area
      RECT rect;
      GetClientRect(hwnd, &rect);
      DrawTextW(hdc, L"Hello, Windows!", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

      EndPaint(hwnd, &ps);
      return S_OK;

   case WM_DESTROY:
      // exit the application
      PostQuitMessage(0);
      return S_OK;
   }

   // let Windows process any unhandled messages
   return DefWindowProcW(hwnd, message, wParam, lParam);
}