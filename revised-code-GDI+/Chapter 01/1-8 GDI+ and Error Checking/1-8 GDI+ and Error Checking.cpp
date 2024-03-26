// 1-8 GDI+ and Error Checking

#include <windows.h>
#include <windowsx.h>
#include <GDIPlus.h>

using namespace Gdiplus;

#pragma comment(lib, "Gdiplus.lib" )

LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

void OnDestroy( HWND );
void OnPaint( HWND hwnd )
{
   PAINTSTRUCT ps;

   HDC hdc = BeginPaint( hwnd, &ps );

   Status   status;

   // construct an object, and its status is checked for error
   Graphics graphics( hdc );
   status = graphics.GetLastStatus( );
   if ( Ok != status )
   {
      // an error occured, so clean up and return
      EndPaint( hwnd, &ps );
      return;
   }

   Pen      blackPen( Color( 255, 0, 0, 0 ) );
   status = blackPen.GetLastStatus( );
   if ( Ok != status )
   {
      EndPaint( hwnd, &ps );
      return;
   }

   Pen      redPen( Color( 255, 255, 0, 0 ), 1 );
   status = redPen.GetLastStatus( );
   if ( Ok != status )
   {
      EndPaint( hwnd, &ps );
      return;
   }

   Pen      greenPen( Color( 255, 0, 255, 0 ), 2 );
   status = greenPen.GetLastStatus( );
   if ( Ok != status )
   {
      EndPaint( hwnd, &ps );
      return;
   }

   Pen      bluePen( Color( 255, 0, 0, 255 ), 3 );
   status = bluePen.GetLastStatus( );
   if ( Ok != status )
   {
      EndPaint( hwnd, &ps );
      return;
   }

   Pen      yellowPen( Color( 255, 255, 0 ), 4 );
   status = yellowPen.GetLastStatus( );
   if ( Ok != status )
   {
      EndPaint( hwnd, &ps );
      return;
   }

   // graphical method is called on an existing object
   status = graphics.DrawLine( &blackPen, 0, 0, 100, 50 );

   // if the method succeeded, go onto the next graphical method
   if ( Ok == status )
   {
      status = graphics.DrawLine( &greenPen, 100, 50, 200, 100 );
   }

   if ( Ok == status )
   {
      status = graphics.DrawLine( &yellowPen, 200, 100, 0, 200 );
   }

   if ( Ok == status )
   {
      status = graphics.DrawLine( &bluePen, 0, 200, 200, 200 );
   }

   if ( Ok == status )
   {
      status = graphics.DrawLine( &redPen, 200, 200, 0, 0 );
   }

   EndPaint( hwnd, &ps );
}

int WINAPI wWinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
                     _In_ PWSTR     szCmdLine, _In_     int       nWinMode )
{
   static const WCHAR szAppName[ ] { L"GDI+ErrorChecking" };

   WNDCLASSEXW wc { };

   GdiplusStartupInput gdiplusStartupInput;
   ULONG_PTR           gdiplusToken;
   Status              status = GdiplusStartup( &gdiplusToken, &gdiplusStartupInput, NULL );

   if ( Ok != status )
   {
      MessageBoxW( NULL, L"Unable to Initialize the GDI+ System!", L"ERROR",
                   MB_OK | MB_ICONERROR );
      return 0;
   }

   wc.cbSize        = sizeof( WNDCLASSEX );
   wc.style         = CS_HREDRAW | CS_VREDRAW;
   wc.lpfnWndProc   = WndProc;
   wc.cbClsExtra    = 0;
   wc.cbWndExtra    = 0;
   wc.hInstance     = hInstance;
   wc.hIcon         = ( HICON )   LoadImageW( NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR );
   wc.hIconSm       = ( HICON )   LoadImageW( NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR );
   wc.hCursor       = ( HCURSOR ) LoadImageW( NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED );
   wc.hbrBackground = ( HBRUSH ) ( COLOR_WINDOW + 1 );
   wc.lpszMenuName  = NULL;
   wc.lpszClassName = szAppName;

   if ( FAILED( RegisterClassExW( &wc ) ) )
   {
      MessageBoxW( NULL, L"Can't Register the Window Class!", szAppName, MB_OK | MB_ICONERROR );
      return E_FAIL;
   }

   static const WCHAR szAppTitle[ ] { L"GDI+ and Error Checking" };

   HWND hwnd { CreateWindowW( szAppName, szAppTitle,
                              WS_OVERLAPPEDWINDOW,
                              CW_USEDEFAULT, CW_USEDEFAULT,
                              CW_USEDEFAULT, CW_USEDEFAULT,
                              NULL, NULL, hInstance, NULL ) };

   if ( NULL == hwnd )
   {
      MessageBoxW( NULL, L"Unable to Create the Main Window!", szAppName, MB_OK | MB_ICONERROR );
      return E_FAIL;
   }

   ShowWindow( hwnd, nWinMode );
   UpdateWindow( hwnd );

   MSG msg;

   while ( GetMessageW( &msg, NULL, 0, 0 ) )
   {
      TranslateMessage( &msg );
      DispatchMessageW( &msg );
   }

   GdiplusShutdown( gdiplusToken );

   return ( int ) msg.wParam;
}

LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
   switch ( message )
   {
      HANDLE_MSG( hwnd, WM_PAINT, OnPaint );
      HANDLE_MSG( hwnd, WM_DESTROY, OnDestroy );
   }

   return DefWindowProcW( hwnd, message, wParam, lParam );
}

void OnDestroy( HWND hwnd )
{
   PostQuitMessage( 0 );
}
