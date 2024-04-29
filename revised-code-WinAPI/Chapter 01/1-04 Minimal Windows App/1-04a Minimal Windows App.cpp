// a very minimal Windows API application skeleton

// INCLUDES ====================================================================
#include <windows.h>

// FUNCTION PROTOTYPES =========================================================
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

// entry point for a Windows application =======================================
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int nWinMode )
{
   // define the window class name
   static const TCHAR szAppName[ ] = TEXT( "MinimalWindowsApp" );

   // create an instance of the window class structure
   WNDCLASS wc;

   wc.style         = CS_HREDRAW | CS_VREDRAW;
   wc.lpfnWndProc   = WndProc;
   wc.cbClsExtra    = 0;
   wc.cbWndExtra    = 0;
   wc.hInstance     = hInstance;
   wc.hIcon         = ( HICON ) LoadIcon( NULL, IDI_APPLICATION );
   wc.hCursor       = ( HCURSOR ) LoadCursor( NULL, IDC_ARROW );
   wc.hbrBackground = ( HBRUSH ) ( COLOR_WINDOW + 1 );
   wc.lpszMenuName  = NULL;
   wc.lpszClassName = szAppName;

   if ( 0 == RegisterClass( &wc ) )
   {
      MessageBox( NULL, TEXT( "Can't Register the Window Class!" ), szAppName, MB_OK | MB_ICONERROR );
      return 0;
   }

   // define the application title
   static const TCHAR szAppTitle[ ] = TEXT( "Win32 API Skeletal Application" );

   // create the window
   HWND hwnd = CreateWindow( szAppName, szAppTitle,
                             WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             NULL, NULL, hInstance, NULL );

   // check if the window was created, exit if fail
   if ( NULL == hwnd )
   {
      MessageBox( NULL, TEXT( "Unable to Create the Main Window!" ), szAppName, MB_OK | MB_ICONERROR );
      return 0;
   }

   // show and update the window
   ShowWindow( hwnd, nWinMode );
   UpdateWindow( hwnd );

   // enter the main message loop
   MSG msg;

   while ( GetMessageW( &msg, NULL, 0, 0 ) )
   {
      // process the message
      TranslateMessage( &msg );
      DispatchMessageW( &msg );
   }

   // the app is done, exit normally, return control to Windows©
   return msg.wParam;
}

// processes the messages that Windows sends to the application ================
LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
   HDC         hdc;
   PAINTSTRUCT ps;
   RECT        rect;

   // choose which Windows© messages you want to use
   switch ( message )
   {
   case WM_PAINT:
      hdc = BeginPaint( hwnd, &ps );

      // draw some text centered in the client area
      GetClientRect( hwnd, &rect );
      DrawText( hdc, TEXT( "Hello, Windows!" ), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER );

      EndPaint( hwnd, &ps );
      return 0;

   case WM_DESTROY:
      // exit the application
      PostQuitMessage( 0 );
      return 0;
   }

   // let Windows process any unhandled messages
   return DefWindowProc( hwnd, message, wParam, lParam );
}
