#include "winmsg.hpp"
#include "winfuncs.hpp"

static const WCHAR szWinName[ ]  = L"IdleTime";
static const WCHAR szAppTitle[ ] = L"Idle Time Processing";

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

   if ( RegisterClassW(&wc) == 0 )
   {
      MessageBoxW(NULL, L"Can't Register the Window Class!", szWinName, MB_OK | MB_ICONERROR);
      return E_FAIL;
   }
   else return S_OK;
}

HRESULT InitInstance(HINSTANCE hInstance, int nWinMode)
{
   HWND hwnd = CreateWindowW(szWinName, szAppTitle,
                             WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             NULL, NULL, hInstance, NULL);

   if ( hwnd == NULL )
   {
      MessageBoxW(NULL, L"Can't Create the Main Window!", szWinName, MB_OK | MB_ICONERROR);
      return E_FAIL;
   }

   ShowWindow(hwnd, nWinMode);
   UpdateWindow(hwnd);

   return S_OK;
}

int MessageLoop( )
{
   MSG msg;

   ZeroMemory(&msg, sizeof(MSG));

   while ( msg.message != WM_QUIT )
   {
      if ( PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE) )
      {
         TranslateMessage(&msg);
         DispatchMessageW(&msg);
      }
      else
      {
         // idle-time processing
         // best if a function call is used
      }
   }

   return (int) msg.wParam;
}