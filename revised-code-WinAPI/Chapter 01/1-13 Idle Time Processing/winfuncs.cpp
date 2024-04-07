#include "winmsg.hpp"
#include "winfuncs.hpp"

static PCWSTR winName  = L"IdleTime";
static PCWSTR appTitle = L"Idle Time Processing";

LRESULT CALLBACK WndProc( HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
   switch ( msg )
   {
      HANDLE_MSG( wnd, WM_LBUTTONDOWN, OnLButtonDown );
      HANDLE_MSG( wnd, WM_RBUTTONDOWN, OnRButtonDown );
      HANDLE_MSG( wnd, WM_PAINT, OnPaint );
      HANDLE_MSG( wnd, WM_DESTROY, OnDestroy );
   }

   return DefWindowProcW( wnd, msg, wParam, lParam );
}

HRESULT InitApplication( HINSTANCE inst )
{
   WNDCLASSW wc = { };

   wc.style         = CS_HREDRAW | CS_VREDRAW;
   wc.lpfnWndProc   = WndProc;
   wc.cbClsExtra    = 0;
   wc.cbWndExtra    = 0;
   wc.hInstance     = inst;
   wc.hIcon         = ( HICON ) LoadImageW( NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_SHARED );
   wc.hCursor       = ( HCURSOR ) LoadImageW( NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED );
   wc.hbrBackground = ( HBRUSH ) ( COLOR_WINDOW + 1 );
   wc.lpszMenuName  = NULL;
   wc.lpszClassName = winName;

   if ( FAILED( RegisterClassW( &wc ) ) )
   {
      MessageBoxW( NULL, L"Can't Register the Window Class!", winName, MB_OK | MB_ICONERROR );
      return E_FAIL;
   }
   else return S_OK;
}

HRESULT InitInstance( HINSTANCE inst, int winMode )
{
   HWND wnd = CreateWindowW( winName, appTitle,
                             WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             800, 600,
                             NULL, NULL, inst, NULL );

   if ( NULL == wnd )
   {
      MessageBoxW( NULL, L"Can't Create the Main Window!", winName, MB_OK | MB_ICONERROR );
      return E_FAIL;
   }

   ShowWindow( wnd, winMode );
   UpdateWindow( wnd );

   return S_OK;
}

int MessageLoop( )
{
   MSG msg;

   ZeroMemory( &msg, sizeof( MSG ) );

   while ( msg.message != WM_QUIT )
   {
      if ( PeekMessageW( &msg, NULL, 0, 0, PM_REMOVE ) )
      {
         TranslateMessage( &msg );
         DispatchMessageW( &msg );
      }
      else
      {
         // idle-time processing
         // best if a function call is used
      }
   }

   return ( int ) msg.wParam;
}