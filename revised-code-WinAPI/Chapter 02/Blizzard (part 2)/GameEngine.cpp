#include "GameEngine.h"

GameEngine* GameEngine::m_gameEngine = NULL;

int WINAPI WinMain( HINSTANCE inst, HINSTANCE prevInst, PSTR cmdLine, int cmdShow )
{
   MSG        msg;
   static int iTickTrigger = 0;
   int        iTickCount;

   if ( GameInitialize( inst ) )
   {
      if ( !GameEngine::GetEngine( )->Initialize( cmdShow ) )
      {
         return FALSE;
      }

      while ( TRUE )
      {
         if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
         {
            if ( msg.message == WM_QUIT )
            {
               break;
            }

            TranslateMessage( &msg );
            DispatchMessage( &msg );
         }
         else
         {
            if ( !GameEngine::GetEngine( )->GetSleep( ) )
            {
               iTickCount = GetTickCount( );

               if ( iTickCount > iTickTrigger )
               {
                  iTickTrigger = iTickCount + GameEngine::GetEngine( )->GetFrameDelay( );
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

GameEngine::GameEngine( HINSTANCE inst, PCTSTR wndClass, PCTSTR title,
                        WORD icon, WORD smallIcon, int width, int height )
{
   m_gameEngine = this;
   m_inst       = inst;
   m_wnd        = NULL;

   if ( lstrlen( wndClass ) > 0 )
   {
      lstrcpy( m_wndClass, wndClass );
   }

   if ( lstrlen( title ) > 0 )
   {
      lstrcpy( m_title, title );
   }

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
   WNDCLASSEX wc;

   wc.cbSize        = sizeof( wc );
   wc.style         = CS_HREDRAW | CS_VREDRAW;
   wc.lpfnWndProc   = WndProc;
   wc.cbClsExtra    = 0;
   wc.cbWndExtra    = 0;
   wc.hInstance     = m_inst;
   wc.hIcon         = LoadIcon( m_inst, MAKEINTRESOURCE( GetIcon( ) ) );
   wc.hIconSm       = LoadIcon( m_inst, MAKEINTRESOURCE( GetSmallIcon( ) ) );
   wc.hCursor       = LoadCursor( NULL, IDC_ARROW );
   wc.hbrBackground = ( HBRUSH ) ( COLOR_WINDOW + 1 );
   wc.lpszMenuName  = NULL;
   wc.lpszClassName = m_wndClass;

   if ( !RegisterClassEx( &wc ) )
   {
      return FALSE;
   }

   int windowWidth  = m_width  + GetSystemMetrics( SM_CXFIXEDFRAME ) * 2 + 10;
   int windowHeight = m_height + GetSystemMetrics( SM_CYFIXEDFRAME ) * 2 + GetSystemMetrics( SM_CYCAPTION ) + 10;

   if ( wc.lpszMenuName != NULL )
   {
      windowHeight += GetSystemMetrics( SM_CYMENU );
   }

   int xWindowPos = ( GetSystemMetrics( SM_CXSCREEN ) - windowWidth )  / 2;
   int yWindowPos = ( GetSystemMetrics( SM_CYSCREEN ) - windowHeight ) / 2;

   m_wnd = CreateWindow( m_wndClass, m_title, WS_POPUPWINDOW |
                         WS_CAPTION | WS_MINIMIZEBOX, xWindowPos, yWindowPos, windowWidth,
                         windowHeight, NULL, NULL, m_inst, NULL );
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
      HDC         dc;
      PAINTSTRUCT ps;
      dc = BeginPaint( wnd, &ps );

      GamePaint( dc );

      EndPaint( wnd, &ps );
      return 0;

   case WM_DESTROY:
      GameEnd( );
      PostQuitMessage( 0 );
      return 0;
   }
   return DefWindowProc( wnd, msg, wParam, lParam );
}
