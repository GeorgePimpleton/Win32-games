#include "Roids.hpp"

HRESULT GameInitialize( HINSTANCE inst )
{
   g_game = std::make_unique<GameEngine>( inst, L"Roids", L"Roids b: Creating Backgrounds for Your Sprites",
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
   g_offscreenBitmap = CreateCompatibleBitmap( GetDC( wnd ), g_game->GetWidth( ), g_game->GetHeight( ) );

   SelectObject( g_offscreenDC, g_offscreenBitmap );

   g_asteroidBitmap = std::make_unique<Bitmap>( IDB_ASTEROID );

   g_background = std::make_unique<StarryBackground>( 500, 400 );

   RECT    bounds = { 0, 0, 500, 400 };
   Sprite* sprite;

   sprite = new Sprite( g_asteroidBitmap.get( ), bounds, BA_WRAP );
   sprite->SetNumFrames( 14 );
   sprite->SetFrameDelay( 1 );
   sprite->SetPosition( 250, 200 );
   sprite->SetVelocity( -3, 1 );
   g_game->AddSprite( sprite );

   sprite = new Sprite( g_asteroidBitmap.get( ), bounds, BA_WRAP );
   sprite->SetNumFrames( 14 );
   sprite->SetFrameDelay( 2 );
   sprite->SetPosition( 250, 200 );
   sprite->SetVelocity( 3, -2 );
   g_game->AddSprite( sprite );

   sprite = new Sprite( g_asteroidBitmap.get( ), bounds, BA_WRAP );
   sprite->SetNumFrames( 14 );
   sprite->SetFrameDelay( 3 );
   sprite->SetPosition( 250, 200 );
   sprite->SetVelocity( -2, -4 );
   g_game->AddSprite( sprite );

   sprite = new Sprite( g_asteroidBitmap.get( ), bounds, BA_WRAP );
   sprite->SetNumFrames( 14 );
   sprite->SetFrameDelay( 2 );
   sprite->SetPosition( 150, 200 );
   sprite->SetVelocity( 3, 4 );
   g_game->AddSprite( sprite );
}

void GameNew( )
{ }

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

   HWND wnd = g_game->GetWindow( );
   HDC  dc  = GetDC( wnd );

   GamePaint( g_offscreenDC );

   BitBlt( dc, 0, 0, g_game->GetWidth( ), g_game->GetHeight( ), g_offscreenDC, 0, 0, SRCCOPY );

   ReleaseDC( wnd, dc );
}

void GameMenu( WPARAM wParam )
{
   switch ( LOWORD( wParam ) )
   {
   case IDM_GAME_NEW:
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

void MouseButtonDown( LONG, LONG, BOOL )
{ }

void MouseButtonUp( LONG, LONG, BOOL )
{ }

void MouseMove( LONG, LONG )
{ }

void HandleJoystick( JOYSTATE )
{ }

BOOL SpriteCollision( Sprite*, Sprite* )
{
   return FALSE;
}
