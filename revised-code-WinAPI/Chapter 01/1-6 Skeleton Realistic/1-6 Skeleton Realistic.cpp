#include "1-6 Skeleton Realistic.hpp"

int WINAPI wWinMain( _In_ HINSTANCE inst, _In_opt_ HINSTANCE prevInst, _In_ PWSTR cmdLine, _In_ int cmdShow )
{
   PCWSTR appName { L"Realistic Windows Skeleton" };

   WNDCLASSEXW wc { };

   wc.cbSize        = sizeof( WNDCLASSEX );
   wc.style         = CS_HREDRAW | CS_VREDRAW;
   wc.lpfnWndProc   = WndProc;
   wc.cbClsExtra    = 0;
   wc.cbWndExtra    = 0;
   wc.hInstance     = inst;
   wc.hIcon         = ( HICON ) LoadImageW( inst, MAKEINTRESOURCEW( IDI_ICON ), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR );
   wc.hIconSm       = ( HICON ) LoadImageW( inst, MAKEINTRESOURCEW( IDI_ICON_SM ), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR );
   wc.hCursor       = ( HCURSOR ) LoadImageW( inst, MAKEINTRESOURCEW( IDC_CURSOR ), IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR );
   wc.hbrBackground = ( HBRUSH ) ( COLOR_WINDOW + 1 );
   wc.lpszMenuName  = MAKEINTRESOURCEW( IDR_MENU );
   wc.lpszClassName = appName;

   if ( FAILED( RegisterClassExW( &wc ) ) )
   {
      MessageBoxW( NULL, L"Can't Register the Window Class!", appName, MB_OK | MB_ICONERROR );
      return E_FAIL;
   }

   PCWSTR appTitle { L"A Realistic Windows Skeleton Application" };

   HWND wnd = CreateWindowW( appName, appTitle,
                             WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             NULL, NULL, inst, NULL );

   if ( NULL == wnd )
   {
      MessageBoxW( NULL, L"Unable to Create the Main Window!", appName, MB_OK | MB_ICONERROR );
      return E_FAIL;
   }

   ShowWindow( wnd, cmdShow );
   UpdateWindow( wnd );

   // load the accelerators
   HACCEL accel = LoadAcceleratorsW( inst, MAKEINTRESOURCEW( IDR_ACCEL ) );

   // check accelerators are loaded
   // the app can still run without accelerators
   if ( NULL == accel )
   {
      MessageBoxW( NULL, L"Unable to Load the Accelerators!", appName, MB_OK | MB_ICONERROR );
   }

   // enter the main message loop with a bit of sanity checking
   static BOOL ret;
   static MSG  msg;

   while ( ( ret = GetMessageW( &msg, NULL, 0, 0 ) ) != 0 ) // 0 = WM_QUIT
   {
      // check for error
      if ( -1 == ret )
      {
         // handle the error and possibly exit

         // for this app simply report error and exit
         MessageBoxW( NULL, L"Unable to Continue!", appName, MB_OK | MB_ICONERROR );
         return E_FAIL;
      }
      else
      {
         // check for accelerator keystrokes
         if ( 0 == TranslateAcceleratorW( wnd, accel, &msg ) )
         {
            // no accelerator used so process the message
            TranslateMessage( &msg );
            DispatchMessageW( &msg );
         }
      }
   }

   return ( int ) msg.wParam;
}

LRESULT CALLBACK WndProc( HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
   switch ( msg )
   {
      // process menu items
   case WM_COMMAND:
      switch ( LOWORD( wParam ) )
      {
      case IDM_APP_EXIT:
         PostQuitMessage( 0 );

         return S_OK;

      case IDM_HELP_ABOUT:
         // the About... menu item was selected, display the app's pre-defined dialog box
         DialogBoxW( ( HINSTANCE ) GetWindowLongPtrW( wnd, GWLP_HINSTANCE ), MAKEINTRESOURCEW( IDD_ABOUT ), wnd, ( DLGPROC ) DlgProc );

         return S_OK;
      }
      break;

   case WM_CREATE:
      // set the custom cursor as the class cursor
      SetClassLongPtrW( wnd, GCLP_HCURSOR,
                        ( LONG64 ) LoadImageW( GetModuleHandleW( NULL ), MAKEINTRESOURCEW( IDC_CURSOR ),
                        IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR ) );
      return S_OK;

   case WM_PAINT:
   {
      PAINTSTRUCT ps;
      HDC         dc { BeginPaint( wnd, &ps ) };

      RECT rect;
      GetClientRect( wnd, &rect );

      DrawTextW( dc, L"This is a realistic skeletal Windows application!", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER );

      EndPaint( wnd, &ps );
      return S_OK;
   }

   case WM_DESTROY:
      PostQuitMessage( 0 );
      return S_OK;
   }

   return DefWindowProcW( wnd, msg, wParam, lParam );
}

// handle dialog box messages
BOOL CALLBACK DlgProc( HWND dlg, UINT msg, WPARAM wParam, LPARAM lParam )
{
   // process dialog box messages
   switch ( msg )
   {
      // choose the message(s) to process
   case WM_COMMAND:
      switch ( LOWORD( wParam ) )
      {
      case IDOK:
         // the OK button was pushed, exit the dialog box
         EndDialog( dlg, 0 );
         return TRUE;
      }
   }

   // no dialog box message processed
   return FALSE;
}
