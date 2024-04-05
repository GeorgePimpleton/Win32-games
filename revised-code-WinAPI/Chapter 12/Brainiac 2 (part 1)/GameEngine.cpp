#pragma comment(lib, "winmm.lib")

#include "GameEngine.h"

GameEngine* GameEngine::m_pGameEngine = NULL;

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow )
{
   MSG        msg;
   static int iTickTrigger = 0;
   int        iTickCount;

   if ( GameInitialize( hInstance ) )
   {
      if ( !GameEngine::GetEngine( )->Initialize( iCmdShow ) )
         return FALSE;

      while ( TRUE )
      {
         if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
         {
            if ( msg.message == WM_QUIT )
               break;
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
                  iTickTrigger = iTickCount +
                     GameEngine::GetEngine( )->GetFrameDelay( );
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

   return TRUE;
}

LRESULT CALLBACK WndProc( HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam )
{
   return GameEngine::GetEngine( )->HandleEvent( hWindow, msg, wParam, lParam );
}

BOOL GameEngine::CheckSpriteCollision( Sprite* pTestSprite )
{
   vector<Sprite*>::iterator siSprite;
   for ( siSprite = m_vSprites.begin( ); siSprite != m_vSprites.end( ); siSprite++ )
   {
      if ( pTestSprite == ( *siSprite ) )
         continue;

      if ( pTestSprite->TestCollision( *siSprite ) )
         // Collision detected
         return SpriteCollision( ( *siSprite ), pTestSprite );
   }

   return FALSE;
}

GameEngine::GameEngine( HINSTANCE hInstance, PCTSTR szWindowClass, PCTSTR szTitle,
                        WORD wIcon, WORD wSmallIcon, int iWidth, int iHeight )
{
   m_pGameEngine = this;
   m_hInstance = hInstance;
   m_hWindow = NULL;
   if ( lstrlen( szWindowClass ) > 0 )
      lstrcpy( m_szWindowClass, szWindowClass );
   if ( lstrlen( szTitle ) > 0 )
      lstrcpy( m_szTitle, szTitle );
   m_wIcon = wIcon;
   m_wSmallIcon = wSmallIcon;
   m_iWidth = iWidth;
   m_iHeight = iHeight;
   m_iFrameDelay = 50;   // 20 FPS default
   m_bSleep = TRUE;
   m_uiJoystickID = 0;
   m_vSprites.reserve( 50 );
}

GameEngine::~GameEngine( )
{ }

BOOL GameEngine::Initialize( int iCmdShow )
{
   WNDCLASSEX    wndclass;

   wndclass.cbSize = sizeof( wndclass );
   wndclass.style = CS_HREDRAW | CS_VREDRAW;
   wndclass.lpfnWndProc = WndProc;
   wndclass.cbClsExtra = 0;
   wndclass.cbWndExtra = 0;
   wndclass.hInstance = m_hInstance;
   wndclass.hIcon = LoadIcon( m_hInstance,
                              MAKEINTRESOURCE( GetIcon( ) ) );
   wndclass.hIconSm = LoadIcon( m_hInstance,
                                MAKEINTRESOURCE( GetSmallIcon( ) ) );
   wndclass.hCursor = LoadCursor( NULL, IDC_ARROW );
   wndclass.hbrBackground = ( HBRUSH ) ( COLOR_WINDOW + 1 );
   wndclass.lpszMenuName = NULL;
   wndclass.lpszClassName = m_szWindowClass;

   if ( !RegisterClassEx( &wndclass ) )
      return FALSE;

   int iWindowWidth = m_iWidth + GetSystemMetrics( SM_CXFIXEDFRAME ) * 2 + 10,
      iWindowHeight = m_iHeight + GetSystemMetrics( SM_CYFIXEDFRAME ) * 2 +
      GetSystemMetrics( SM_CYCAPTION ) + 10;
   if ( wndclass.lpszMenuName != NULL )
      iWindowHeight += GetSystemMetrics( SM_CYMENU );
   int iXWindowPos = ( GetSystemMetrics( SM_CXSCREEN ) - iWindowWidth ) / 2,
      iYWindowPos = ( GetSystemMetrics( SM_CYSCREEN ) - iWindowHeight ) / 2;

   m_hWindow = CreateWindow( m_szWindowClass, m_szTitle, WS_POPUPWINDOW |
                             WS_CAPTION | WS_MINIMIZEBOX, iXWindowPos, iYWindowPos, iWindowWidth,
                             iWindowHeight, NULL, NULL, m_hInstance, NULL );
   if ( !m_hWindow )
      return FALSE;

   ShowWindow( m_hWindow, iCmdShow );
   UpdateWindow( m_hWindow );

   return TRUE;
}

LRESULT GameEngine::HandleEvent( HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam )
{
   switch ( msg )
   {
   case WM_CREATE:
      SetWindow( hWindow );
      GameStart( hWindow );
      return 0;

   case WM_ACTIVATE:
      if ( wParam != WA_INACTIVE )
      {
         GameActivate( hWindow );
         SetSleep( FALSE );
      }
      else
      {
         GameDeactivate( hWindow );
         SetSleep( TRUE );
      }
      return 0;

   case WM_PAINT:
      HDC         hDC;
      PAINTSTRUCT ps;
      hDC = BeginPaint( hWindow, &ps );

      GamePaint( hDC );

      EndPaint( hWindow, &ps );
      return 0;

   case WM_LBUTTONDOWN:
      MouseButtonDown( LOWORD( lParam ), HIWORD( lParam ), TRUE );
      return 0;

   case WM_LBUTTONUP:
      MouseButtonUp( LOWORD( lParam ), HIWORD( lParam ), TRUE );
      return 0;

   case WM_RBUTTONDOWN:
      MouseButtonDown( LOWORD( lParam ), HIWORD( lParam ), FALSE );
      return 0;

   case WM_RBUTTONUP:
      MouseButtonUp( LOWORD( lParam ), HIWORD( lParam ), FALSE );
      return 0;

   case WM_MOUSEMOVE:
      MouseMove( LOWORD( lParam ), HIWORD( lParam ) );
      return 0;

   case WM_DESTROY:
      GameEnd( );
      PostQuitMessage( 0 );
      return 0;
   }
   return DefWindowProc( hWindow, msg, wParam, lParam );
}

BOOL GameEngine::InitJoystick( )
{
   UINT uiNumJoysticks;
   if ( ( uiNumJoysticks = joyGetNumDevs( ) ) == 0 )
      return FALSE;

   JOYINFO jiInfo;
   if ( joyGetPos( JOYSTICKID1, &jiInfo ) != JOYERR_UNPLUGGED )
      m_uiJoystickID = JOYSTICKID1;
   else
      return FALSE;

   JOYCAPS jcCaps;
   joyGetDevCaps( m_uiJoystickID, &jcCaps, sizeof( JOYCAPS ) );
   DWORD dwXCenter = ( ( DWORD ) jcCaps.wXmin + jcCaps.wXmax ) / 2;
   DWORD dwYCenter = ( ( DWORD ) jcCaps.wYmin + jcCaps.wYmax ) / 2;
   m_rcJoystickTrip.left = ( jcCaps.wXmin + ( WORD ) dwXCenter ) / 2;
   m_rcJoystickTrip.right = ( jcCaps.wXmax + ( WORD ) dwXCenter ) / 2;
   m_rcJoystickTrip.top = ( jcCaps.wYmin + ( WORD ) dwYCenter ) / 2;
   m_rcJoystickTrip.bottom = ( jcCaps.wYmax + ( WORD ) dwYCenter ) / 2;

   return TRUE;
}

void GameEngine::CaptureJoystick( )
{
   if ( m_uiJoystickID == JOYSTICKID1 )
      joySetCapture( m_hWindow, m_uiJoystickID, NULL, TRUE );
}

void GameEngine::ReleaseJoystick( )
{
   if ( m_uiJoystickID == JOYSTICKID1 )
      joyReleaseCapture( m_uiJoystickID );
}

void GameEngine::CheckJoystick( )
{
   if ( m_uiJoystickID == JOYSTICKID1 )
   {
      JOYINFO jiInfo;
      JOYSTATE jsJoystickState = 0;
      if ( joyGetPos( m_uiJoystickID, &jiInfo ) == JOYERR_NOERROR )
      {
         if ( jiInfo.wXpos < ( WORD ) m_rcJoystickTrip.left )
            jsJoystickState |= JOY_LEFT;
         else if ( jiInfo.wXpos > ( WORD )m_rcJoystickTrip.right )
            jsJoystickState |= JOY_RIGHT;

         if ( jiInfo.wYpos < ( WORD ) m_rcJoystickTrip.top )
            jsJoystickState |= JOY_UP;
         else if ( jiInfo.wYpos > ( WORD )m_rcJoystickTrip.bottom )
            jsJoystickState |= JOY_DOWN;

         if ( jiInfo.wButtons & JOY_BUTTON1 )
            jsJoystickState |= JOY_FIRE1;
         if ( jiInfo.wButtons & JOY_BUTTON2 )
            jsJoystickState |= JOY_FIRE2;
      }

      HandleJoystick( jsJoystickState );
   }
}

void GameEngine::AddSprite( Sprite* pSprite )
{
   if ( pSprite != NULL )
   {
      if ( m_vSprites.size( ) > 0 )
      {
         vector<Sprite*>::iterator siSprite;
         for ( siSprite = m_vSprites.begin( ); siSprite != m_vSprites.end( ); siSprite++ )
            if ( pSprite->GetZOrder( ) < ( *siSprite )->GetZOrder( ) )
            {
               m_vSprites.insert( siSprite, pSprite );
               return;
            }
      }

      m_vSprites.push_back( pSprite );
   }
}

void GameEngine::DrawSprites( HDC hDC )
{
   vector<Sprite*>::iterator siSprite;
   for ( siSprite = m_vSprites.begin( ); siSprite != m_vSprites.end( ); siSprite++ )
      ( *siSprite )->Draw( hDC );
}

void GameEngine::UpdateSprites( )
{
   RECT          rcOldSpritePos;
   SPRITEACTION  saSpriteAction;
   vector<Sprite*>::iterator siSprite;
   for ( siSprite = m_vSprites.begin( ); siSprite != m_vSprites.end( ); siSprite++ )
   {
      rcOldSpritePos = ( *siSprite )->GetPosition( );

      saSpriteAction = ( *siSprite )->Update( );

      if ( saSpriteAction & SA_KILL )
      {
         delete ( *siSprite );
         m_vSprites.erase( siSprite );
         siSprite--;
         continue;
      }

      if ( CheckSpriteCollision( *siSprite ) )
         ( *siSprite )->SetPosition( rcOldSpritePos );
   }
}

void GameEngine::CleanupSprites( )
{
   vector<Sprite*>::iterator siSprite;
   for ( siSprite = m_vSprites.begin( ); siSprite != m_vSprites.end( ); siSprite++ )
   {
      delete ( *siSprite );
      m_vSprites.erase( siSprite );
      siSprite--;
   }
}

Sprite* GameEngine::IsPointInSprite( int x, int y )
{
   vector<Sprite*>::reverse_iterator siSprite;
   for ( siSprite = m_vSprites.rbegin( ); siSprite != m_vSprites.rend( ); siSprite++ )
      if ( !( *siSprite )->IsHidden( ) && ( *siSprite )->IsPointInside( x, y ) )
         return ( *siSprite );

   return NULL;
}
