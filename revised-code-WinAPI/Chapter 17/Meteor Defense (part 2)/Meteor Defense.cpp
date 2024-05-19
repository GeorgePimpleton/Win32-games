#include "Meteor Defense.hpp"

HRESULT GameInitialize( HINSTANCE inst )
{
   g_game = std::make_unique<GameEngine>( inst, L"Meteor Defense", L"Example Game: Meteor Defense b",
                                          IDI_ICON, IDI_ICON_SM, 600, 450 );

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

   SetClassLongPtrW( wnd, GCLP_HCURSOR,
                     ( LONG64 ) LoadImageW( g_game->GetInstance( ), MAKEINTRESOURCEW( IDC_CURSOR ),
                     IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR ) );

   g_offscreenDC     = CreateCompatibleDC( GetDC( wnd ) );
   g_offscreenBitmap = CreateCompatibleBitmap( GetDC( wnd ), g_game->GetWidth( ), g_game->GetHeight( ) );

   SelectObject( g_offscreenDC, g_offscreenBitmap );

   g_groundBitmap    = std::make_unique<Bitmap>( IDB_GROUND );
   g_cityBitmap      = std::make_unique<Bitmap>( IDB_CITY );
   g_meteorBitmap    = std::make_unique<Bitmap>( IDB_METEOR );
   g_missileBitmap   = std::make_unique<Bitmap>( IDB_MISSILE );
   g_explosionBitmap = std::make_unique<Bitmap>( IDB_EXPLOSION );
   g_gameOverBitmap  = std::make_unique<Bitmap>( IDB_GAMEOVER );

   g_background = std::make_unique<StarryBackground>( 600, 450 );

   g_game->PlayMIDISong( L"Music.mid" );

   GameNew( );
}

void GameNew( )
{
   g_game->CleanupSprites( );

   RECT bounds = { 0, 0, 600, 450 };

   Sprite* sprite = new Sprite( g_cityBitmap.get( ), bounds );
   sprite->SetPosition( 2, 370 );
   g_game->AddSprite( sprite );

   sprite = new Sprite( g_cityBitmap.get( ), bounds );
   sprite->SetPosition( 186, 370 );
   g_game->AddSprite( sprite );

   sprite = new Sprite( g_cityBitmap.get( ), bounds );
   sprite->SetPosition( 302, 370 );
   g_game->AddSprite( sprite );

   sprite = new Sprite( g_cityBitmap.get( ), bounds );
   sprite->SetPosition( 490, 370 );
   g_game->AddSprite( sprite );

   g_score      = 0;
   g_numCities  = 4;
   g_difficulty = 50;
   g_gameOver   = FALSE;

   EnableMenuItem( GetMenu( g_game->GetWindow( ) ),
                   ( UINT ) ( ULONGLONG ) MAKEINTRESOURCEW( IDM_GAME_NEW ), MF_GRAYED );

   g_game->PlayMIDISong( );
}

void GameEnd( )
{
   g_game->CloseMIDIPlayer( );

   DeleteObject( g_offscreenBitmap );
   DeleteDC( g_offscreenDC );
}

void GameActivate( HWND )
{
   g_game->PlayMIDISong( L"", FALSE );
}

void GameDeactivate( HWND )
{
   g_game->PauseMIDISong( );
}

void GamePaint( HDC dc )
{
   g_background->Draw( dc );

   g_groundBitmap->Draw( dc, 0, 398, TRUE );

   g_game->DrawSprites( dc );

   WCHAR text[64];
   RECT  rect = { 275, 0, 325, 50 };

   wsprintfW( text, L"%d", g_score );

   SetBkMode( dc, TRANSPARENT );
   SetTextColor( dc, RGB( 255, 255, 255 ) );

   DrawTextW( dc, text, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER );

   if ( g_gameOver )
   {
      g_gameOverBitmap->Draw( dc, 170, 150, TRUE );
   }
}

void GameCycle( )
{
   if ( !g_gameOver )
   {
      if ( 0 == ( rtk::rand( 0, g_difficulty ) ) )
      {
         AddMeteor( );
      }

      g_background->Update( );

      g_game->UpdateSprites( );

      HWND wnd = g_game->GetWindow( );
      HDC  dc  = GetDC( wnd );

      GamePaint( g_offscreenDC );

      BitBlt( dc, 0, 0, g_game->GetWidth( ), g_game->GetHeight( ),
              g_offscreenDC, 0, 0, SRCCOPY );

      ReleaseDC( wnd, dc );
   }
}

void GameMenu( WPARAM wParam )
{
   switch ( LOWORD( wParam ) )
   {
   case IDM_GAME_NEW:
      if ( g_gameOver )
      {
         GameNew( );
      }
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

void MouseButtonDown( int x, int y, BOOL bLeft )
{
   if ( !g_gameOver && bLeft )
   {
      RECT bounds = { 0, 0, 600, 450 };
      int  xPos   = ( x < 300 ) ? 144 : 449;

      Sprite* sprite = new Sprite( g_missileBitmap.get( ), bounds, BA_DIE );

      sprite->SetPosition( xPos, 365 );

      int xVel = 0;
      int yVel = -6;

      y    = min( y, 300 );
      xVel = ( yVel * ( ( xPos + 8 ) - x ) ) / ( 365 - y );

      sprite->SetVelocity( xVel, yVel );

      g_game->AddSprite( sprite );

      PlaySoundW( ( PCWSTR ) IDW_FIRE, g_game->GetInstance( ), SND_ASYNC | SND_RESOURCE | SND_NOSTOP );

      g_score = max( --g_score, 0 );
   }
   else if ( g_gameOver && !bLeft )
   {
      GameNew( );
   }
}

void MouseButtonUp( int, int, BOOL )
{ }

void MouseMove( int, int )
{ }

void HandleJoystick( JOYSTATE )
{ }

BOOL SpriteCollision( Sprite* spriteHitter, Sprite* spriteHittee )
{
   if ( ( spriteHitter->GetBitmap( ) == g_missileBitmap.get( ) && spriteHittee->GetBitmap( ) == g_meteorBitmap.get( ) ) ||
        ( spriteHitter->GetBitmap( ) == g_meteorBitmap.get( )  && spriteHittee->GetBitmap( ) == g_missileBitmap.get( ) ) )
   {
      spriteHitter->Kill( );
      spriteHittee->Kill( );

      g_score      += 6;
      g_difficulty  = max( 50 - ( g_score / 10 ), 5 );
   }

   if ( spriteHitter->GetBitmap( ) == g_meteorBitmap.get( ) && spriteHittee->GetBitmap( ) == g_cityBitmap.get( ) )
   {
      PlaySoundW( ( PCWSTR ) IDW_BIGEXPLODE, g_game->GetInstance( ), SND_ASYNC | SND_RESOURCE );

      spriteHitter->Kill( );
      spriteHittee->Kill( );

      if ( 0 == ( --g_numCities ) )
      {
         EnableMenuItem( GetMenu( g_game->GetWindow( ) ),
                         ( UINT ) ( ULONGLONG ) MAKEINTRESOURCEW( IDM_GAME_NEW ), MF_ENABLED );

         g_gameOver = TRUE;
      }
   }

   return FALSE;
}

void SpriteDying( Sprite* spriteDying )
{
   if ( spriteDying->GetBitmap( ) == g_meteorBitmap.get( ) )
   {
      PlaySoundW( ( PCWSTR ) IDW_EXPLODE, g_game->GetInstance( ), SND_ASYNC | SND_RESOURCE | SND_NOSTOP );

      RECT bounds = { 0, 0, 600, 450 };
      RECT pos    = spriteDying->GetPosition( );

      Sprite* sprite = new Sprite( g_explosionBitmap.get( ), bounds );

      sprite->SetNumFrames( 12, TRUE );
      sprite->SetPosition( pos.left, pos.top );
      g_game->AddSprite( sprite );
   }
}

void AddMeteor( )
{
   RECT    bounds = { 0, 0, 600, 390 };
   int     xPos   = rtk::rand( 0, 600 );
   Sprite* sprite = new Sprite( g_meteorBitmap.get( ), bounds, BA_DIE );

   sprite->SetNumFrames( 14 );
   sprite->SetPosition( xPos, 0 );

   int xVel = 0;
   int yVel = rtk::rand( 3, 4 );

   switch ( rtk::rand( 0, 4 ) )
   {
   case 0:
      xVel = ( yVel * ( 56 - ( xPos + 50 ) ) ) / 400;
      break;

   case 1:
      xVel = ( yVel * ( 240 - ( xPos + 50 ) ) ) / 400;
      break;

   case 2:
      xVel = ( yVel * ( 360 - ( xPos + 50 ) ) ) / 400;
      break;

   case 3:
      xVel = ( yVel * ( 546 - ( xPos + 50 ) ) ) / 400;
      break;
   }

   sprite->SetVelocity( xVel, yVel );

   g_game->AddSprite( sprite );
}
