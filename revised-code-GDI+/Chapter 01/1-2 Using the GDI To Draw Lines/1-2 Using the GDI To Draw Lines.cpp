// 1-2 Using the GDI To Draw Lines

#include <windows.h>
#include <windowsx.h>

LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

void OnDestroy( HWND );
void OnPaint( HWND wnd )
{
   PAINTSTRUCT ps;

   HDC dc { BeginPaint( wnd, &ps ) };

   HPEN redPen    { CreatePen( PS_SOLID, 1, RGB( 255, 0, 0 ) ) };
   HPEN greenPen  { CreatePen( PS_SOLID, 2, RGB( 0, 255, 0 ) ) };
   HPEN bluePen   { CreatePen( PS_SOLID, 3, RGB( 0, 0, 255 ) ) };
   HPEN yellowPen { CreatePen( PS_SOLID, 4, RGB( 255, 255, 0 ) ) };

   LineTo( dc, 100, 50 );
   MoveToEx( dc, 100, 50, NULL );

   HPEN oldPen = ( HPEN ) SelectObject( dc, greenPen );
   LineTo( dc, 200, 100 );

   SelectObject( dc, yellowPen );
   LineTo( dc, 0, 200 );

   SelectObject( dc, bluePen );
   LineTo( dc, 200, 200 );

   SelectObject( dc, redPen );
   LineTo( dc, 0, 0 );

   SelectObject( dc, oldPen );

   EndPaint( wnd, &ps );
}

int WINAPI wWinMain( _In_ HINSTANCE inst, _In_opt_ HINSTANCE prevInst,
                     _In_ PWSTR     cmdLine, _In_     int       winMode )
{
   static PCWSTR appName { L"GDIDrawLines" };

   WNDCLASSEXW wc { };

   wc.cbSize        = sizeof( WNDCLASSEX );
   wc.style         = CS_HREDRAW | CS_VREDRAW;
   wc.lpfnWndProc   = WndProc;
   wc.cbClsExtra    = 0;
   wc.cbWndExtra    = 0;
   wc.hInstance     = inst;
   wc.hIcon         = ( HICON ) LoadImageW( NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR );
   wc.hIconSm       = ( HICON ) LoadImageW( NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR );
   wc.hCursor       = ( HCURSOR ) LoadImageW( NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED );
   wc.hbrBackground = ( HBRUSH ) ( COLOR_WINDOW + 1 );
   wc.lpszMenuName  = NULL;
   wc.lpszClassName = appName;

   if ( 0 == RegisterClassExW( &wc ) )
   {
      MessageBoxW( NULL, L"Can't Register the Window Class!", appName, MB_OK | MB_ICONERROR );
      return E_FAIL;
   }

   static PCWSTR appTitle { L"Using the GDI to Draw Lines" };

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
