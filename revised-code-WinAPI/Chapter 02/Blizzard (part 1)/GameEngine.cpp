#include "GameEngine.h"

// static variable initialization
GameEngine* GameEngine::m_pGameEngine = NULL;

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow )
{
   MSG        msg;
   static int iTickTrigger = 0;
   int        iTickCount;

   if ( GameInitialize( hInstance ) )
   {
      // initialize the game engine
      if ( !GameEngine::GetEngine( )->Initialize( iCmdShow ) )
      {
         return FALSE;
      }

      // enter the main message loop
      while ( TRUE )
      {
         if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
         {
            // process the message
            if ( msg.message == WM_QUIT )
            {
               break;
            }

            TranslateMessage( &msg );
            DispatchMessage( &msg );
         }
         else
         {
            // make sure the game engine isn't sleeping
            if ( !GameEngine::GetEngine( )->GetSleep( ) )
            {
               // check the tick count to see if a game cycle has elapsed
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

   // end the game
   GameEnd( );

   return TRUE;
}

LRESULT CALLBACK WndProc( HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam )
{
   // route all windows messages to the game engine
   return GameEngine::GetEngine( )->HandleEvent( hWindow, msg, wParam, lParam );
}

// GameEngine constructor(s)/destructor
GameEngine::GameEngine( HINSTANCE hInstance, PCTSTR szWindowClass, PCTSTR szTitle,
                        WORD wIcon, WORD wSmallIcon, int iWidth, int iHeight )
{
   // set the data members for the game engine
   m_pGameEngine = this;
   m_hInstance   = hInstance;
   m_hWindow     = NULL;

   if ( lstrlen( szWindowClass ) > 0 ) { lstrcpy( m_szWindowClass, szWindowClass ); }
   if ( lstrlen( szTitle ) > 0 )       { lstrcpy( m_szTitle, szTitle ); }

   m_wIcon       = wIcon;
   m_wSmallIcon  = wSmallIcon;
   m_iWidth      = iWidth;
   m_iHeight     = iHeight;
   m_iFrameDelay = 50;   // 20 FPS default
   m_bSleep      = TRUE;
}

GameEngine::~GameEngine( )
{ }

BOOL GameEngine::Initialize( int iCmdShow )
{
   WNDCLASSEX wndclass;

   // create the window class for the main window
   wndclass.cbSize        = sizeof( wndclass );
   wndclass.style         = CS_HREDRAW | CS_VREDRAW;
   wndclass.lpfnWndProc   = WndProc;
   wndclass.cbClsExtra    = 0;
   wndclass.cbWndExtra    = 0;
   wndclass.hInstance     = m_hInstance;
   wndclass.hIcon         = LoadIcon( m_hInstance, MAKEINTRESOURCE( GetIcon( ) ) );
   wndclass.hIconSm       = LoadIcon( m_hInstance, MAKEINTRESOURCE( GetSmallIcon( ) ) );
   wndclass.hCursor       = LoadCursor( NULL, IDC_ARROW );
   wndclass.hbrBackground = ( HBRUSH ) ( COLOR_WINDOW + 1 );
   wndclass.lpszMenuName  = NULL;
   wndclass.lpszClassName = m_szWindowClass;

   // Register the window class
   if ( !RegisterClassEx( &wndclass ) )
   {
      return FALSE;
   }

   // calculate the window size and position based upon the game size
   int iWindowWidth  = m_iWidth  + GetSystemMetrics( SM_CXFIXEDFRAME ) * 2;
   int iWindowHeight = m_iHeight + GetSystemMetrics( SM_CYFIXEDFRAME ) * 2 + GetSystemMetrics( SM_CYCAPTION );

   // Windows 10 needs a little extra room
   iWindowWidth  += 10;
   iWindowHeight += 10;

   if ( wndclass.lpszMenuName != NULL )
   {
      iWindowHeight += GetSystemMetrics( SM_CYMENU );
   }

   int iXWindowPos = ( GetSystemMetrics( SM_CXSCREEN ) - iWindowWidth ) / 2;
   int iYWindowPos = ( GetSystemMetrics( SM_CYSCREEN ) - iWindowHeight ) / 2;

   // create the window`
   m_hWindow = CreateWindow( m_szWindowClass, m_szTitle, WS_POPUPWINDOW |
                             WS_CAPTION | WS_MINIMIZEBOX, iXWindowPos, iYWindowPos, iWindowWidth,
                             iWindowHeight, NULL, NULL, m_hInstance, NULL );
   if ( !m_hWindow )
   {
      return FALSE;
   }

   // show and update the window
   ShowWindow( m_hWindow, iCmdShow );
   UpdateWindow( m_hWindow );

   return TRUE;
}

LRESULT GameEngine::HandleEvent( HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam )
{
   // route Windows messages to game engine member functions
   switch ( msg )
   {
   case WM_CREATE:
      // set the game window and start the game
      SetWindow( hWindow );
      GameStart( hWindow );
      return 0;

   case WM_SETFOCUS:
      // activate the game and update the sleep status
      GameActivate( hWindow );
      SetSleep( FALSE );
      return 0;

   case WM_KILLFOCUS:
      // deactivate the game and update the sleep status
      GameDeactivate( hWindow );
      SetSleep( TRUE );
      return 0;

   case WM_PAINT:
      HDC         hDC;
      PAINTSTRUCT ps;
      hDC = BeginPaint( hWindow, &ps );

      // paint the game in the client area
      GamePaint( hDC );

      EndPaint( hWindow, &ps );
      return 0;

   case WM_DESTROY:
      // end the game and exit the application
      GameEnd( );
      PostQuitMessage( 0 );
      return 0;
   }

   return DefWindowProc( hWindow, msg, wParam, lParam );
}
