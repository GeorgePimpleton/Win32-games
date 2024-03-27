// 1-5 Using GDI+ To Draw Strings

#include <windows.h>
#include <windowsx.h>
#include <GDIPlus.h>

using namespace Gdiplus;

#pragma comment(lib, "Gdiplus.lib" )

LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

void OnDestroy( HWND );
void OnPaint( HWND wnd )
{
   PAINTSTRUCT ps;

   HDC dc { BeginPaint( wnd, &ps ) };

   Graphics   graphics( dc );
   SolidBrush brush( Color( 255, 0, 0, 255 ) );
   FontFamily fontFamily( L"Times New Roman" );
   Font       font( &fontFamily, 24, FontStyleRegular, UnitPixel );
   PointF     pointF( 10.0f, 20.0f );

   graphics.DrawString( L"Hello World!", -1, &font, pointF, &brush );

   EndPaint( wnd, &ps );
}

int WINAPI wWinMain( _In_ HINSTANCE inst,    _In_opt_ HINSTANCE prevInst,
                     _In_ PWSTR     cmdLine, _In_     int       winMode )
{
   static PCWSTR appName { L"GDI+DrawStrings" };

   WNDCLASSEXW wc { };

   GdiplusStartupInput  gdiplusStartupInput;
   ULONG_PTR            gdiplusToken;
   Status               status { GdiplusStartup( &gdiplusToken, &gdiplusStartupInput, NULL ) };

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
   wc.hInstance     = inst;
   wc.hIcon         = ( HICON )   LoadImageW( NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR );
   wc.hIconSm       = ( HICON )   LoadImageW( NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR );
   wc.hCursor       = ( HCURSOR ) LoadImageW( NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED );
   wc.hbrBackground = ( HBRUSH ) ( COLOR_WINDOW + 1 );
   wc.lpszMenuName  = NULL;
   wc.lpszClassName = appName;

   if ( FAILED( RegisterClassExW( &wc ) ) )
   {
      MessageBoxW( NULL, L"Can't Register the Window Class!", appName, MB_OK | MB_ICONERROR );
      return E_FAIL;
   }

   static PCWSTR appTitle { L"Using GDI+ To Draw Strings" };

   HWND wnd { CreateWindowW( appName, appTitle,
                             WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             NULL, NULL, inst, NULL ) };

   if ( NULL == wnd )
   {
      MessageBoxW( NULL, L"Unable to Create the Main Window!", appName, MB_OK | MB_ICONERROR );
      return E_FAIL;
   }

   ShowWindow( wnd, winMode );
   UpdateWindow( wnd );

   MSG msg;

   while ( GetMessageW( &msg, NULL, 0, 0 ) )
   {
      TranslateMessage( &msg );
      DispatchMessageW( &msg );
   }

   GdiplusShutdown( gdiplusToken );

   return ( int ) msg.wParam;
}

LRESULT CALLBACK WndProc( HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
   switch ( msg )
   {
      HANDLE_MSG( wnd, WM_PAINT, OnPaint );
      HANDLE_MSG( wnd, WM_DESTROY, OnDestroy );
   }

   return DefWindowProcW( wnd, msg, wParam, lParam );
}

void OnDestroy( HWND wnd )
{
   PostQuitMessage( 0 );
}
