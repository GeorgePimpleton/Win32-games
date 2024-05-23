#include "Wanderer.hpp"

HRESULT GameInitialize( HINSTANCE inst )
{
   g_game = std::make_unique<GameEngine>( inst, L"Wanderer", L"Wanderer",
                                          IDI_ICON, IDI_ICON_SM, 256, 256 );

   if ( NULL == g_game )
   {
      return E_FAIL;
   }

   g_game->SetFrameRate( 30 );

   g_game->InitJoystick( );

   return S_OK;
}

void GameStart( HWND wnd )
{
   g_offscreenDC     = CreateCompatibleDC( GetDC( wnd ) );
   g_offscreenBitmap = CreateCompatibleBitmap( GetDC( wnd ),
                                               g_game->GetWidth( ), g_game->GetHeight( ) );

   SelectObject( g_offscreenDC, g_offscreenBitmap );

   HDC dc = GetDC( wnd );

   g_background       = std::make_unique<ScrollingBackground>( 256, 256 );
   g_BGLandscapeLayer = std::make_unique<BackgroundLayer>( IDB_BG_LANDSCAPE );

   RECT viewport = { 352, 352, 608, 608 };
   g_BGLandscapeLayer->SetViewport( viewport );
   g_background->AddLayer( g_BGLandscapeLayer.get( ) );

   g_foreground    = std::make_unique<ScrollingBackground>( 256, 256 );
   g_FGCloudsLayer = std::make_unique<BackgroundLayer>( IDB_BG_CLOUDS );
   viewport.left   = viewport.top = 64;
   viewport.right  = viewport.bottom = 320;
   g_FGCloudsLayer->SetViewport( viewport );
   g_foreground->AddLayer( g_FGCloudsLayer.get( ) );

   g_personBitmap = std::make_unique<Bitmap>( IDB_PERSON );

   RECT bounds = { 115, 112, 26, 32 };
   g_personSprite = std::make_unique<PersonSprite>( g_personBitmap.get( ), bounds, BA_STOP );
   g_personSprite->SetNumFrames( 2 );
   g_personSprite->SetPosition( 115, 112 );
   g_game->AddSprite( g_personSprite.get( ) );

   g_game->PlayMIDISong( L"Music.mid" );
}

void GameNew( )
{ }

void GameEnd( )
{
   g_game->CloseMIDIPlayer( );

   DeleteObject( g_offscreenBitmap );
   DeleteDC( g_offscreenDC );
}

void GameActivate( HWND wnd )
{
   g_game->CaptureJoystick( );

   g_game->PlayMIDISong( L"", FALSE );
}

void GameDeactivate( HWND wnd )
{
   g_game->ReleaseJoystick( );

   g_game->PauseMIDISong( );
}

void GamePaint( HDC dc )
{
   g_background->Draw( dc );

   g_game->DrawSprites( dc );

   g_foreground->Draw( dc, TRUE );
}

void GameCycle( )
{
   g_game->UpdateSprites( );

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
      GameEnd( );
      PostQuitMessage( 0 );
      return;

   case IDM_HELP_ABOUT:
      DialogBoxParamW( g_game->GetInstance( ), MAKEINTRESOURCEW( IDD_ABOUT ),
                       g_game->GetWindow( ), ( DLGPROC ) DlgProc, 0L );
      return;
   }
}

void HandleKeys( )
{
   if ( g_inputDelay++ > 1 )
   {
      if ( GetAsyncKeyState( VK_LEFT ) < 0 )
      {
         UpdateView( SD_RIGHT );
      }
      else if ( GetAsyncKeyState( VK_RIGHT ) < 0 )
      {
         UpdateView( SD_LEFT );
      }
      else if ( GetAsyncKeyState( VK_UP ) < 0 )
      {
         UpdateView( SD_DOWN );
      }
      else if ( GetAsyncKeyState( VK_DOWN ) < 0 )
      {
         UpdateView( SD_UP );
      }

      g_inputDelay = 0;
   }
}

void MouseButtonDown( int, int, BOOL )
{ }

void MouseButtonUp( int, int, BOOL )
{ }

void MouseMove( int, int )
{ }

void HandleJoystick( JOYSTATE joyState )
{
   if ( ++g_inputDelay > 2 )
   {
      if ( joyState & JOY_LEFT )
      {
         UpdateView( SD_RIGHT );
      }
      else if ( joyState & JOY_RIGHT )
      {
         UpdateView( SD_LEFT );
      }
      else if ( joyState & JOY_UP )
      {
         UpdateView( SD_DOWN );
      }
      else if ( joyState & JOY_DOWN )
      {
         UpdateView( SD_UP );
      }

      g_inputDelay = 0;
   }
}

void UpdateView( SCROLLDIR sd )
{
   g_personSprite->Walk( );

   g_BGLandscapeLayer->SetSpeed( 16 );
   g_BGLandscapeLayer->SetDirection( sd );
   g_BGLandscapeLayer->Update( );
   g_BGLandscapeLayer->SetSpeed( 0 );

   g_FGCloudsLayer->SetSpeed( 4 );
   g_FGCloudsLayer->SetDirection( sd );
   g_FGCloudsLayer->Update( );
   g_FGCloudsLayer->SetSpeed( 0 );
}

BOOL SpriteCollision( Sprite*, Sprite* )
{
   return FALSE;
}

void SpriteDying( Sprite* )
{ }
