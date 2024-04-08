#include "GameEngine.hpp"

GameEngine* GameEngine::m_gameEngine = NULL;

int WINAPI wWinMain( _In_     HINSTANCE inst,
                     _In_opt_ HINSTANCE prevInst,
                     _In_     PWSTR     cmdLine,
                     _In_     int       cmdShow )
{
   if ( GameInitialize( inst ) )
   {
      if ( !GameEngine::GetEngine( )->Initialize( cmdShow ) )
      {
         return FALSE;
      }

      MSG msg;

      while ( TRUE )
      {
         if ( PeekMessageW( &msg, NULL, 0, 0, PM_REMOVE ) )
         {
            if ( msg.message == WM_QUIT )
            {
               break;
            }

            TranslateMessage( &msg );
            DispatchMessageW( &msg );
         }
         else
         {
            if ( !GameEngine::GetEngine( )->GetSleep( ) )
            {
               static ULONGLONG tickTrigger = 0;
               ULONGLONG        tickCount   = GetTickCount64( );

               if ( tickCount > tickTrigger )
               {
                  tickTrigger = tickCount + GameEngine::GetEngine( )->GetFrameDelay( );
                  GameCycle( );
               }
            }
         }
      }
      return ( int ) msg.wParam;
   }

   GameEnd( );

   return TRUE;
}

LRESULT CALLBACK WndProc( HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
   return GameEngine::GetEngine( )->HandleEvent( wnd, msg, wParam, lParam );
}

GameEngine::GameEngine( HINSTANCE inst, PCWSTR wndClass, PCWSTR title,
                        WORD icon, WORD smallIcon, int width, int height )
{
   m_gameEngine = this;
   m_inst       = inst;
   m_wnd        = NULL;
   m_wndClass   = wndClass;
   m_title      = title;
   m_icon       = icon;
   m_smallIcon  = smallIcon;
   m_width      = width;
   m_height     = height;
   m_frameDelay = 50;
   m_asleep     = TRUE;
}

GameEngine::~GameEngine( )
{ }

BOOL GameEngine::Initialize( int cmdShow )
{
   WNDCLASSEXW wc;

   wc.cbSize        = sizeof( wc );
   wc.style         = CS_HREDRAW | CS_VREDRAW;
   wc.lpfnWndProc   = WndProc;
   wc.cbClsExtra    = 0;
   wc.cbWndExtra    = 0;
   wc.hInstance     = m_inst;
   wc.hIcon         = ( HICON ) LoadImageW( m_inst, MAKEINTRESOURCEW( IDI_ICON ), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR );
   wc.hIconSm       = ( HICON ) LoadImageW( m_inst, MAKEINTRESOURCEW( IDI_ICON_SM ), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR ); ;
   wc.hCursor       = ( HCURSOR ) LoadImageW( NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED );
   wc.hbrBackground = ( HBRUSH ) ( COLOR_WINDOW + 1 );
   wc.lpszMenuName  = NULL;
   wc.lpszClassName = m_wndClass;

   if ( !RegisterClassExW( &wc ) )
   {
      return FALSE;
   }

   int windowWidth  = m_width  + GetSystemMetrics( SM_CXFIXEDFRAME ) * 2 + 10;
   int windowHeight = m_height + GetSystemMetrics( SM_CYFIXEDFRAME ) * 2 + GetSystemMetrics( SM_CYCAPTION ) + 10;

   if ( wc.lpszMenuName != NULL )
   {
      windowHeight += GetSystemMetrics( SM_CYMENU );
   }

   int xWindowPos = ( GetSystemMetrics( SM_CXSCREEN ) - windowWidth ) / 2;
   int yWindowPos = ( GetSystemMetrics( SM_CYSCREEN ) - windowHeight ) / 2;

   m_wnd = CreateWindowW( m_wndClass, m_title,
                          WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX,
                          xWindowPos, yWindowPos, windowWidth, windowHeight,
                          NULL, NULL, m_inst, NULL );

   if ( !m_wnd )
   {
      return FALSE;
   }

   ShowWindow( m_wnd, cmdShow );
   UpdateWindow( m_wnd );

   return TRUE;
}

LRESULT GameEngine::HandleEvent( HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
   switch ( msg )
   {
   case WM_CREATE:
      SetWindow( wnd );
      GameStart( wnd );
      return 0;

   case WM_SETFOCUS:
      GameActivate( wnd );
      SetSleep( FALSE );
      return 0;

   case WM_KILLFOCUS:
      GameDeactivate( wnd );
      SetSleep( TRUE );
      return 0;

   case WM_PAINT:
   {
      PAINTSTRUCT ps;
      HDC         dc = BeginPaint( wnd, &ps );

      GamePaint( dc );

      EndPaint( wnd, &ps );
   }
   return 0;

   case WM_DESTROY:
      GameEnd( );
      PostQuitMessage( 0 );
      return 0;
   }
   return DefWindowProcW( wnd, msg, wParam, lParam );
}
