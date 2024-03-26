// 1-7 Using GDI+ to Load and Draw Images

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

   Graphics graphics( hdc );
   Image    image( L"climber.jpg" );

   graphics.DrawImage( &image, 10, 10, image.GetWidth( ), image.GetHeight( ) );

   EndPaint( hwnd, &ps );
}

int WINAPI wWinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
                     _In_ PWSTR     szCmdLine, _In_     int       nWinMode )
{
   static const WCHAR szAppName[ ] { L"GDI+DrawImages" };

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

   static const WCHAR szAppTitle[ ] { L"Using GDI+ To Draw Images" };

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
