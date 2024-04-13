#include "Roids 2.hpp"

HRESULT GameInitialize( HINSTANCE inst )
{
   g_game = std::make_unique<GameEngine>( inst, L"Roids 2", L"Roids 2 b: Teaching Games to Think",
                                          IDI_ICON, IDI_ICON_SM, 500, 400 );

   if ( NULL == g_game )
   {
      return E_FAIL;
   }

   g_game->SetFrameRate( 30 );

   return S_OK;
}

void GameStart( HWND wnd )
{
   rtk::srand( );

   g_offscreenDC     = CreateCompatibleDC( GetDC( wnd ) );
   g_offscreenBitmap = CreateCompatibleBitmap( GetDC( wnd ),
                                               g_game->GetWidth( ), g_game->GetHeight( ) );

   SelectObject( g_offscreenDC, g_offscreenBitmap );

   g_asteroidBitmap = std::make_unique<Bitmap>( IDB_ASTEROID );
   g_saucerBitmap   = std::make_unique<Bitmap>( IDB_SAUCER );

   g_background = std::make_unique<StarryBackground>( 500, 400 );

   GameNew( );
}

void GameNew( )
{
   RECT bounds = { 0, 0, 500, 400 };

   g_asteroids[0] = std::make_unique<Sprite>( g_asteroidBitmap.get( ), bounds, BA_WRAP );
   g_asteroids[0]->SetNumFrames( 14 );
   g_asteroids[0]->SetFrameDelay( 1 );
   g_asteroids[0]->SetPosition( 250, 200 );
   g_asteroids[0]->SetVelocity( -3, 1 );
   g_game->AddSprite( g_asteroids[0].get( ) );

   g_asteroids[1] = std::make_unique<Sprite>( g_asteroidBitmap.get( ), bounds, BA_WRAP );
   g_asteroids[1]->SetNumFrames( 14 );
   g_asteroids[1]->SetFrameDelay( 2 );
   g_asteroids[1]->SetPosition( 250, 200 );
   g_asteroids[1]->SetVelocity( 3, -2 );
   g_game->AddSprite( g_asteroids[1].get( ) );

   g_asteroids[2] = std::make_unique<Sprite>( g_asteroidBitmap.get( ), bounds, BA_WRAP );
   g_asteroids[2]->SetNumFrames( 14 );
   g_asteroids[2]->SetFrameDelay( 3 );
   g_asteroids[2]->SetPosition( 250, 200 );
   g_asteroids[2]->SetVelocity( -2, -4 );
   g_game->AddSprite( g_asteroids[2].get( ) );

   g_saucer = std::make_unique<Sprite>( g_saucerBitmap.get( ), bounds, BA_WRAP );
   g_saucer->SetPosition( 0, 0 );
   g_saucer->SetVelocity( 3, 1 );
   g_game->AddSprite( g_saucer.get( ) );
}

void GameEnd( )
{
   DeleteObject( g_offscreenBitmap );
   DeleteDC( g_offscreenDC );
}

void GameActivate( HWND )
{ }

void GameDeactivate( HWND )
{ }

void GamePaint( HDC dc )
{
   g_background->Draw( dc );

   g_game->DrawSprites( dc );
}

void GameCycle( )
{
   g_background->Update( );

   g_game->UpdateSprites( );

   UpdateSaucer( );

   HWND wnd = g_game->GetWindow( );
   HDC  dc  = GetDC( wnd );

   GamePaint( g_offscreenDC );

   BitBlt( dc, 0, 0, g_game->GetWidth( ), g_game->GetHeight( ),
           g_offscreenDC, 0, 0, SRCCOPY );

   ReleaseDC( wnd, dc );
}

void GameMenu( WPARAM wParam )
{
   switch ( LOWORD( wParam ) )
   {
   case IDM_GAME_NEW:
      GameNew( );
      return;

   case IDM_GAME_EXIT:
      DestroyWindow( g_game->GetWindow( ) );
      return;

   case IDM_HELP_ABOUT:
      DialogBoxParamW( g_game->GetInstance( ), MAKEINTRESOURCEW( IDD_ABOUT ),
                       g_game->GetWindow( ), ( DLGPROC ) DlgProc, 0L );
      return;
   }
}

void HandleKeys( )
{ }

void MouseButtonDown( int, int, BOOL )
{ }

void MouseButtonUp( int, int, BOOL )
{ }

void MouseMove( int, int )
{ }

void HandleJoystick( JOYSTATE )
{ }

BOOL SpriteCollision( Sprite*, Sprite* )
{
   return FALSE;
}

void SpriteDying( Sprite* )
{ }

void UpdateSaucer( )
{
   RECT saucer;
   RECT roid;

   saucer = g_saucer->GetPosition( );

   int xCollision  = 500;
   int yCollision  = 400;
   int xyCollision = 900;

   for ( int i = 0; i < 3; i++ )
   {
      roid = g_asteroids[i]->GetPosition( );

      int xCollisionDist = ( saucer.left +
                             ( saucer.right - saucer.left ) / 2 ) -
                              ( roid.left +
                                ( roid.right - roid.left ) / 2 );

      int yCollisionDist = ( saucer.top +
                             ( saucer.bottom - saucer.top ) / 2 ) -
                              ( roid.top +
                                ( roid.bottom - roid.top ) / 2 );

      if ( ( abs( xCollisionDist ) < abs( xCollision ) ) || ( abs( yCollisionDist ) < abs( yCollision ) ) )
      {
         if ( ( abs( xCollisionDist ) + abs( yCollisionDist ) ) < xyCollision )
         {
            xyCollision = abs( xCollision ) + abs( yCollision );
            xCollision  = xCollisionDist;
            yCollision  = yCollisionDist;
         }
      }
   }

   POINT velocity = g_saucer->GetVelocity( );

   if ( abs( xCollision ) < 60 )
   {
      if ( xCollision < 0 )
      {
         velocity.x = max( velocity.x - 1, -8 );
      }
      else
      {
         velocity.x = min( velocity.x + 1, 8 );
      }
   }

   if ( abs( yCollision ) < 60 )
   {
      if ( yCollision < 0 )
      {
         velocity.y = max( velocity.y - 1, -8 );
      }
      else
      {
         velocity.y = min( velocity.y + 1, 8 );
      }
   }

   g_saucer->SetVelocity( velocity );
}
