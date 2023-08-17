#include	<windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
                    _In_ PWSTR     szCmdLine, _In_ int           nWinMode)
{
   static const WCHAR szWinName[ ]  = L"MinWin";
   static const WCHAR szAppTitle[ ] = L"WinAPI Minimal Application";

   WNDCLASS wc { };

   wc.hInstance     = hInstance;
   wc.lpszClassName = szWinName;
   wc.lpfnWndProc   = WndProc;
   wc.style         = 0;
   wc.hIcon         = (HICON)   LoadImageW(NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_SHARED);
   wc.hCursor       = (HCURSOR) LoadImageW(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
   wc.lpszMenuName  = NULL;
   wc.cbClsExtra    = 0;
   wc.cbWndExtra    = 0;
   wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);

   if ( RegisterClassW(&wc) == 0 )
   {
      MessageBoxW(NULL, L"Couldn't Register the Window Class!", L"ERROR", MB_OK | MB_ICONERROR);
      return 0;
   }

   HWND hwnd = CreateWindowW(szWinName, szAppTitle,
                             WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             NULL, NULL, hInstance, NULL);

   if ( hwnd == NULL )
   {
      MessageBoxW(NULL, L"Couldn't Create the Main Window!", L"ERROR", MB_OK | MB_ICONERROR);
      return 0;
   }

   ShowWindow(hwnd, nWinMode);
   UpdateWindow(hwnd);

   MSG msg;

   while ( GetMessageW(&msg, NULL, 0, 0) )
   {
      TranslateMessage(&msg);
      DispatchMessageW(&msg);
   }

   return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   static const WCHAR szAboutLeft[ ]  = L"This is a minimal WinAPI program.\nYou pressed the left mouse button!";
   static const WCHAR szAboutRight[ ] = L"This is a minimal WinAPI program.\nYou pressed the right mouse button!";

   switch ( message )
   {
   case WM_LBUTTONDOWN:
      MessageBeep(MB_ICONEXCLAMATION);
      MessageBoxW(hwnd, szAboutLeft, L"About", MB_OK | MB_ICONINFORMATION);
      return 0;

   case WM_RBUTTONDOWN:
      MessageBeep(MB_ICONASTERISK);
      MessageBoxW(hwnd, szAboutRight, L"About", MB_OK | MB_ICONINFORMATION);
      return 0;

   case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
   }

   return DefWindowProcW(hwnd, message, wParam, lParam);
}