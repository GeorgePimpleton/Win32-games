#pragma comment(lib, "winmm.lib")

#include "GameEngine.hpp"

GameEngine* GameEngine::m_gameEngine = NULL;

int WINAPI wWinMain( _In_ HINSTANCE inst, _In_opt_ HINSTANCE prevInst, _In_ PWSTR cmdLine, _In_ int cmdShow )
{
   if ( SUCCEEDED( GameInitialize( inst ) ) )
   {
      if ( FAILED( GameEngine::GetEngine( )->Initialize( cmdShow ) ) )
      {
         return E_FAIL;
      }

      HACCEL accel = LoadAcceleratorsW( inst, MAKEINTRESOURCEW( IDR_ACCELERATORS ) );

      if ( NULL == accel )
      {
         MessageBoxW( NULL, L"Unable to Load the Accelerators!", GameEngine::GetEngine( )->GetTitle( ), MB_OK | MB_ICONERROR );
         return E_FAIL;
      }

      MSG msg;

      while ( TRUE )
      {
         if ( PeekMessageW( &msg, NULL, 0, 0, PM_REMOVE ) != 0 )
         {
            if ( WM_QUIT == msg.message )
            {
               break;
            }

            if ( 0 == TranslateAcceleratorW( GameEngine::GetEngine( )->GetWindow( ), accel, &msg ) )
            {
               TranslateMessage( &msg );
               DispatchMessageW( &msg );
            }
         }
         else
         {
            if ( GameEngine::GetEngine( )->GetSleep( ) == FALSE )
            {
               static ULONGLONG tickTrigger = 0;
               ULONGLONG        tickCount   = GetTickCount64( );

               if ( tickCount > tickTrigger )
               {
                  tickTrigger = tickCount + GameEngine::GetEngine( )->GetFrameDelay( );

                  HandleKeys( );
                  GameEngine::GetEngine( )->CheckJoystick( );
                  GameCycle( );
               }
            }
         }
      }
      return ( int ) msg.wParam;
   }

   GameEnd( );
   return S_OK;
}

LRESULT CALLBACK WndProc( HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
   return GameEngine::GetEngine( )->HandleEvent( wnd, msg, wParam, lParam );
}

BOOL CALLBACK DlgProc( HWND dlg, UINT msg, WPARAM wParam, LPARAM lParam )
{
   switch ( msg )
   {
   case WM_COMMAND:
      switch ( LOWORD( wParam ) )
      {
      case IDOK:
         EndDialog( dlg, 0 );
         return TRUE;
      }
   }

   return FALSE;
}

GameEngine::GameEngine( HINSTANCE inst, PCWSTR wndClass, PCWSTR title,
                        WORD icon, WORD smallIcon, UINT width, UINT height )
{
   m_gameEngine = this;
   m_inst       = inst;
   m_wnd        = NULL;
   m_icon       = icon;
   m_smallIcon  = smallIcon;
   m_width      = width;
   m_height     = height;
   m_frameDelay = 50;
   m_asleep     = TRUE;
   m_joyID      = 0;
   m_joyTrip    = { };
   m_wndClass   = wndClass;
   m_title      = title;

   m_sprites.reserve( 50 );
}

GameEngine::~GameEngine( )
{ }

HRESULT GameEngine::Initialize( int cmdShow )
{
   WNDCLASSEXW wc = { };

   wc.cbSize        = sizeof( WNDCLASSEXW );
   wc.style         = CS_HREDRAW | CS_VREDRAW;
   wc.lpfnWndProc   = WndProc;
   wc.cbClsExtra    = 0;
   wc.cbWndExtra    = 0;
   wc.hInstance     = m_inst;
   wc.hIcon         = ( HICON ) LoadImageW( m_inst, MAKEINTRESOURCEW( IDI_ICON ), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR );
   wc.hIconSm       = ( HICON ) LoadImageW( m_inst, MAKEINTRESOURCEW( IDI_ICON_SM ), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR ); ;
   wc.hCursor       = ( HCURSOR ) LoadImageW( NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED );
   wc.hbrBackground = ( HBRUSH ) ( COLOR_WINDOW + 1 );
   wc.lpszMenuName  = MAKEINTRESOURCEW( IDR_MENU );
   wc.lpszClassName = m_wndClass;

   if ( FAILED( RegisterClassEx( &wc ) ) )
   {
      MessageBoxW( NULL, L"Unable to initialize Main Window!", L"ERROR", MB_ICONERROR | MB_OK );
      return E_FAIL;
   }

   UINT windowWidth  = m_width  + GetSystemMetrics( SM_CXFIXEDFRAME ) * 2 + 10;
   UINT windowHeight = m_height + GetSystemMetrics( SM_CYFIXEDFRAME ) * 2 + GetSystemMetrics( SM_CYCAPTION ) + 10;

   if ( NULL != wc.lpszMenuName )
   {
      windowHeight += GetSystemMetrics( SM_CYMENU );
   }

   UINT windowPosX = ( GetSystemMetrics( SM_CXSCREEN ) - windowWidth  ) / 2;
   UINT windowPosY = ( GetSystemMetrics( SM_CYSCREEN ) - windowHeight ) / 2;

   m_wnd = CreateWindowW( m_wndClass, m_title,
                          WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX,
                          windowPosX, windowPosY,
                          windowWidth, windowHeight,
                          NULL, NULL, m_inst, NULL );

   if ( NULL == m_wnd )
   {
      MessageBoxW( NULL, L"Unable to create Main Window!", L"ERROR", MB_ICONERROR | MB_OK );
      return E_FAIL;
   }

   ShowWindow( m_wnd, cmdShow );
   UpdateWindow( m_wnd );

   return S_OK;
}

LRESULT GameEngine::HandleEvent( HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
   switch ( msg )
   {
   case WM_CREATE:
      SetWindow( wnd );
      GameStart( wnd );
      return S_OK;

   case WM_ACTIVATE:
      if ( WA_INACTIVE != wParam )
      {
         GameActivate( wnd );
         SetSleep( FALSE );
      }
      else
      {
         GameDeactivate( wnd );
         SetSleep( TRUE );
      }
      return S_OK;

   case WM_COMMAND:
      GameMenu( wParam );
      return S_OK;

   case WM_PAINT:
   {
      PAINTSTRUCT ps;
      HDC         dc = BeginPaint( wnd, &ps );

      GamePaint( dc );

      EndPaint( wnd, &ps );
   }
   return S_OK;

   case WM_LBUTTONDOWN:
      MouseButtonDown( LOWORD( lParam ), HIWORD( lParam ), TRUE );
      return S_OK;

   case WM_LBUTTONUP:
      MouseButtonUp( LOWORD( lParam ), HIWORD( lParam ), TRUE );
      return S_OK;

   case WM_RBUTTONDOWN:
      MouseButtonDown( LOWORD( lParam ), HIWORD( lParam ), FALSE );
      return S_OK;

   case WM_RBUTTONUP:
      MouseButtonUp( LOWORD( lParam ), HIWORD( lParam ), FALSE );
      return S_OK;

   case WM_MOUSEMOVE:
      MouseMove( LOWORD( lParam ), HIWORD( lParam ) );
      return S_OK;

   case WM_DESTROY:
      GameEnd( );
      PostQuitMessage( 0 );
      return S_OK;
   }

   return DefWindowProcW( wnd, msg, wParam, lParam );
}

HRESULT GameEngine::InitJoystick( )
{
   UINT numJoysticks = { };

   if ( 0 == ( numJoysticks = joyGetNumDevs( ) ) )
   {
      return E_FAIL;
   }

   JOYINFO joyInfo = { };

   if ( JOYERR_UNPLUGGED != joyGetPos( JOYSTICKID1, &joyInfo ) )
   {
      m_joyID = JOYSTICKID1;
   }
   else
   {
      return E_FAIL;
   }

   JOYCAPSW joyCaps = { };

   joyGetDevCapsW( m_joyID, &joyCaps, sizeof( JOYCAPSW ) );

   WORD xCenter = ( ( WORD ) joyCaps.wXmin + joyCaps.wXmax ) / 2;
   WORD yCenter = ( ( WORD ) joyCaps.wYmin + joyCaps.wYmax ) / 2;

   m_joyTrip.left   = ( joyCaps.wXmin + ( WORD ) xCenter ) / 2;
   m_joyTrip.right  = ( joyCaps.wXmax + ( WORD ) xCenter ) / 2;
   m_joyTrip.top    = ( joyCaps.wYmin + ( WORD ) yCenter ) / 2;
   m_joyTrip.bottom = ( joyCaps.wYmax + ( WORD ) yCenter ) / 2;

   return S_OK;
}

void GameEngine::CaptureJoystick( )
{
   if ( JOYSTICKID1 == m_joyID )
   {
      joySetCapture( m_wnd, m_joyID, NULL, TRUE );
   }
}

void GameEngine::ReleaseJoystick( )
{
   if ( JOYSTICKID1 == m_joyID )
   {
      joyReleaseCapture( m_joyID );
   }
}

void GameEngine::CheckJoystick( )
{
   if ( m_joyID == JOYSTICKID1 )
   {
      JOYINFO  joyInfo  = { };
      JOYSTATE joyState = 0L;

      if ( JOYERR_NOERROR == joyGetPos( m_joyID, &joyInfo ) )
      {
         if ( joyInfo.wXpos < ( WORD ) m_joyTrip.left )
         {
            joyState |= JOY_LEFT;
         }
         else if ( joyInfo.wXpos > ( WORD )m_joyTrip.right )
         {
            joyState |= JOY_RIGHT;
         }

         if ( joyInfo.wYpos < ( WORD ) m_joyTrip.top )
         {
            joyState |= JOY_UP;
         }
         else if ( joyInfo.wYpos > ( WORD )m_joyTrip.bottom )
         {
            joyState |= JOY_DOWN;
         }

         if ( joyInfo.wButtons & JOY_BUTTON1 )
         {
            joyState |= JOY_FIRE1;
         }

         if ( joyInfo.wButtons & JOY_BUTTON2 )
         {
            joyState |= JOY_FIRE2;
         }
      }

      HandleJoystick( joyState );
   }
}

void GameEngine::AddSprite( Sprite* sprite )
{
   if ( NULL != sprite )
   {
      if ( m_sprites.size( ) > 0 )
      {
         for ( auto iterSprite = m_sprites.begin( ); iterSprite != m_sprites.end( ); iterSprite++ )
         {
            if ( sprite->GetZOrder( ) < ( *iterSprite )->GetZOrder( ) )
            {
               m_sprites.insert( iterSprite, sprite );
               return;
            }
         }
      }

      m_sprites.push_back( sprite );
   }
}

void GameEngine::DrawSprites( HDC dc ) const
{
   for ( auto iterSprite = m_sprites.begin( ); iterSprite != m_sprites.end( ); iterSprite++ )
   {
      ( *iterSprite )->Draw( dc );
   }
}

void GameEngine::UpdateSprites( )
{
   RECT         oldSpritePos = { };
   SPRITEACTION spriteAction = { };

   for ( auto iterSprite = m_sprites.begin( ); iterSprite != m_sprites.end( ); iterSprite++ )
   {
      oldSpritePos = ( *iterSprite )->GetPosition( );

      spriteAction = ( *iterSprite )->Update( );

      if ( spriteAction & SA_KILL )
      {
         delete ( *iterSprite );
         m_sprites.erase( iterSprite );
         iterSprite--;
         continue;
      }

      if ( CheckSpriteCollision( *iterSprite ) )
      {
         ( *iterSprite )->SetPosition( oldSpritePos );
      }
   }
}

void GameEngine::CleanupSprites( )
{
   for ( auto iterSprite = m_sprites.begin( ); iterSprite != m_sprites.end( ); iterSprite++ )
   {
      delete ( *iterSprite );
      m_sprites.erase( iterSprite );
      iterSprite--;
   }
}

Sprite* GameEngine::IsPointInSprite( LONG x, LONG y )
{
   for ( auto iterSprite = m_sprites.rbegin( ); iterSprite != m_sprites.rend( ); iterSprite++ )
   {
      if ( !( *iterSprite )->IsHidden( ) && ( *iterSprite )->IsPointInside( x, y ) )
      {
         return ( *iterSprite );
      }
   }

   return NULL;
}

BOOL GameEngine::CheckSpriteCollision( Sprite* testSprite )
{
   for ( auto iterSprite = m_sprites.begin( ); iterSprite != m_sprites.end( ); iterSprite++ )
   {
      if ( testSprite == ( *iterSprite ) )
      {
         continue;
      }

      if ( testSprite->TestCollision( *iterSprite ) )
      {
         return SpriteCollision( ( *iterSprite ), testSprite );
      }
   }

   return FALSE;
}
