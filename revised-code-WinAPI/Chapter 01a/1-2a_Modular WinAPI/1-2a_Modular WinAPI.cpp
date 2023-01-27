// a modularized Win API application skeleton, version 1

#include	<windows.h>
#include <winerror.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HRESULT          InitApplication(HINSTANCE);
HRESULT          InitInstance(HINSTANCE, int);
int              MessageLoop();

static const WCHAR szWinName[]  = L"ModWin1";
static const WCHAR szAppTitle[] = L"Modular Win32 API Application, Version 1";

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
                    _In_ PWSTR szCmdLine, _In_ int nWinMode)
{
   if ( FAILED(InitApplication(hInstance)) )
   {
      return 0;
   }

   if ( FAILED(InitInstance(hInstance, nWinMode)) )
   {
      return 0;
   }

   return MessageLoop();
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   static const WCHAR szAboutLeft[]  = L"This is a modular WinAPI program.\nYou pressed the left mouse button!";
   static const WCHAR szAboutRight[] = L"This is a modular WinAPI program.\nYou pressed the right mouse button!";

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

HRESULT InitApplication(HINSTANCE hInstance)
{
   WNDCLASS wc { };

   wc.style         = CS_HREDRAW | CS_VREDRAW;
   wc.lpfnWndProc   = WndProc;
   wc.cbClsExtra    = 0;
   wc.cbWndExtra    = 0;
   wc.hInstance     = hInstance;
   wc.hIcon         = (HICON)   LoadImageW(NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_SHARED);
   wc.hCursor       = (HCURSOR) LoadImageW(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
   wc.hbrBackground = (HBRUSH)  (COLOR_WINDOW + 1);
   wc.lpszMenuName  = NULL;
   wc.lpszClassName = szWinName;

   if ( 0 == RegisterClassW(&wc) )
   {
      MessageBoxW(NULL, L"Can't Register the Window Class!", szWinName, MB_OK | MB_ICONERROR);
      return E_FAIL;
   }
   else return S_OK;
}

// initializes the instance data and creates a new window for each instance
HRESULT InitInstance(HINSTANCE hInstance, int nWinMode)
{
   HWND hwnd = CreateWindowW(szWinName, szAppTitle,
                             WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             NULL, NULL, hInstance, NULL);

   if ( NULL == hwnd )
   {
      MessageBoxW(NULL, L"Can't Create the Main Window!", szWinName, MB_OK | MB_ICONERROR);
      return E_FAIL;
   }

   ShowWindow(hwnd, nWinMode);
   UpdateWindow(hwnd);

   return S_OK;
}

// starts and runs the application's message loop
int MessageLoop()
{
   MSG msg;

   while ( GetMessageW(&msg, NULL, 0, 0) )
   {
      TranslateMessage(&msg);
      DispatchMessageW(&msg);
   }

   return (int) msg.wParam;
}