#include "Wanderer.h"

BOOL GameInitialize( HINSTANCE hInstance )
{
   g_pGame = new GameEngine( hInstance, TEXT( "Wanderer" ), TEXT( "Wanderer a" ),
                             IDI_WANDERER, IDI_WANDERER_SM, 256, 256 );

   if ( g_pGame == NULL )
   {
      return FALSE;
   }

   g_pGame->SetFrameRate( 30 );

   g_pGame->InitJoystick( );

   g_hInstance = hInstance;

   return TRUE;
}

void GameStart( HWND hWindow )
{
   g_hOffscreenDC     = CreateCompatibleDC( GetDC( hWindow ) );
   g_hOffscreenBitmap = CreateCompatibleBitmap( GetDC( hWindow ),
                                                g_pGame->GetWidth( ), g_pGame->GetHeight( ) );

   SelectObject( g_hOffscreenDC, g_hOffscreenBitmap );

   HDC hDC = GetDC( hWindow );

   g_pBackground       = new ScrollingBackground( 256, 256 );
   g_pBGLandscapeLayer = new BackgroundLayer( hDC, IDB_BG_LANDSCAPE, g_hInstance );

   RECT rcViewport = { 352, 352, 608, 608 };
   g_pBGLandscapeLayer->SetViewport( rcViewport );
   g_pBackground->AddLayer( g_pBGLandscapeLayer );

   g_pForeground    = new ScrollingBackground( 256, 256 );
   g_pFGCloudsLayer = new BackgroundLayer( hDC, IDB_BG_CLOUDS, g_hInstance );
   rcViewport.left  = rcViewport.top = 64;
   rcViewport.right = rcViewport.bottom = 320;
   g_pFGCloudsLayer->SetViewport( rcViewport );
   g_pForeground->AddLayer( g_pFGCloudsLayer );

   g_pPersonBitmap = new Bitmap( hDC, IDB_PERSON, g_hInstance );

   RECT rcBounds = { 115, 112, 26, 32 };

   g_pPersonSprite = new PersonSprite( g_pPersonBitmap, rcBounds, BA_STOP );
   g_pPersonSprite->SetNumFrames( 2 );
   g_pPersonSprite->SetPosition( 115, 112 );
   g_pGame->AddSprite( g_pPersonSprite );

   g_pGame->PlayMIDISong( TEXT( "Music.mid" ) );
}

void GameEnd( )
{
   g_pGame->CloseMIDIPlayer( );

   DeleteObject( g_hOffscreenBitmap );
   DeleteDC( g_hOffscreenDC );

   delete g_pPersonBitmap;

   delete g_pBackground;
   delete g_pBGLandscapeLayer;

   delete g_pForeground;
   delete g_pFGCloudsLayer;

   g_pGame->CleanupSprites( );

   delete g_pGame;
}

void GameActivate( HWND hWindow )
{
   g_pGame->CaptureJoystick( );

   g_pGame->PlayMIDISong( TEXT( "" ), FALSE );
}

void GameDeactivate( HWND hWindow )
{
   g_pGame->ReleaseJoystick( );

   g_pGame->PauseMIDISong( );
}

void GamePaint( HDC hDC )
{
   g_pBackground->Draw( hDC );

   g_pGame->DrawSprites( hDC );

   g_pForeground->Draw( hDC, TRUE );
}

void GameCycle( )
{
   g_pGame->UpdateSprites( );

   HWND hWindow = g_pGame-> GetWindow( );
   HDC  hDC     = GetDC( hWindow );

   GamePaint( g_hOffscreenDC );

   BitBlt( hDC, 0, 0, g_pGame->GetWidth( ), g_pGame->GetHeight( ),
           g_hOffscreenDC, 0, 0, SRCCOPY );

   ReleaseDC( hWindow, hDC );
}

void HandleKeys( )
{
   if ( g_iInputDelay++ > 1 )
   {
      if ( GetAsyncKeyState( VK_LEFT ) < 0 )
      {
         g_pPersonSprite->Walk( );

         g_pBGLandscapeLayer->SetSpeed( 16 );
         g_pBGLandscapeLayer->SetDirection( SD_RIGHT );
         g_pBGLandscapeLayer->Update( );
         g_pBGLandscapeLayer->SetSpeed( 0 );

         g_pFGCloudsLayer->SetSpeed( 4 );
         g_pFGCloudsLayer->SetDirection( SD_RIGHT );
         g_pFGCloudsLayer->Update( );
         g_pFGCloudsLayer->SetSpeed( 0 );
      }
      else if ( GetAsyncKeyState( VK_RIGHT ) < 0 )
      {
         g_pPersonSprite->Walk( );

         g_pBGLandscapeLayer->SetSpeed( 16 );
         g_pBGLandscapeLayer->SetDirection( SD_LEFT );
         g_pBGLandscapeLayer->Update( );
         g_pBGLandscapeLayer->SetSpeed( 0 );

         g_pFGCloudsLayer->SetSpeed( 4 );
         g_pFGCloudsLayer->SetDirection( SD_LEFT );
         g_pFGCloudsLayer->Update( );
         g_pFGCloudsLayer->SetSpeed( 0 );
      }
      else if ( GetAsyncKeyState( VK_UP ) < 0 )
      {
         g_pPersonSprite->Walk( );

         g_pBGLandscapeLayer->SetSpeed( 16 );
         g_pBGLandscapeLayer->SetDirection( SD_DOWN );
         g_pBGLandscapeLayer->Update( );
         g_pBGLandscapeLayer->SetSpeed( 0 );

         g_pFGCloudsLayer->SetSpeed( 4 );
         g_pFGCloudsLayer->SetDirection( SD_DOWN );
         g_pFGCloudsLayer->Update( );
         g_pFGCloudsLayer->SetSpeed( 0 );
      }
      else if ( GetAsyncKeyState( VK_DOWN ) < 0 )
      {
         g_pPersonSprite->Walk( );

         g_pBGLandscapeLayer->SetSpeed( 16 );
         g_pBGLandscapeLayer->SetDirection( SD_UP );
         g_pBGLandscapeLayer->Update( );
         g_pBGLandscapeLayer->SetSpeed( 0 );

         g_pFGCloudsLayer->SetSpeed( 4 );
         g_pFGCloudsLayer->SetDirection( SD_UP );
         g_pFGCloudsLayer->Update( );
         g_pFGCloudsLayer->SetSpeed( 0 );
      }

      g_iInputDelay = 0;
   }
}

void MouseButtonDown( int x, int y, BOOL bLeft )
{ }

void MouseButtonUp( int x, int y, BOOL bLeft )
{ }

void MouseMove( int x, int y )
{ }

void HandleJoystick( JOYSTATE jsJoystickState )
{
   if ( ++g_iInputDelay > 2 )
   {
      if ( jsJoystickState & JOY_LEFT )
      {
         g_pPersonSprite->Walk( );

         g_pBGLandscapeLayer->SetSpeed( 16 );
         g_pBGLandscapeLayer->SetDirection( SD_RIGHT );
         g_pBGLandscapeLayer->Update( );
         g_pBGLandscapeLayer->SetSpeed( 0 );

         g_pFGCloudsLayer->SetSpeed( 4 );
         g_pFGCloudsLayer->SetDirection( SD_RIGHT );
         g_pFGCloudsLayer->Update( );
         g_pFGCloudsLayer->SetSpeed( 0 );
      }
      else if ( jsJoystickState & JOY_RIGHT )
      {
         g_pPersonSprite->Walk( );

         g_pBGLandscapeLayer->SetSpeed( 16 );
         g_pBGLandscapeLayer->SetDirection( SD_LEFT );
         g_pBGLandscapeLayer->Update( );
         g_pBGLandscapeLayer->SetSpeed( 0 );

         g_pFGCloudsLayer->SetSpeed( 4 );
         g_pFGCloudsLayer->SetDirection( SD_LEFT );
         g_pFGCloudsLayer->Update( );
         g_pFGCloudsLayer->SetSpeed( 0 );
      }
      else if ( jsJoystickState & JOY_UP )
      {
         g_pPersonSprite->Walk( );

         g_pBGLandscapeLayer->SetSpeed( 16 );
         g_pBGLandscapeLayer->SetDirection( SD_DOWN );
         g_pBGLandscapeLayer->Update( );
         g_pBGLandscapeLayer->SetSpeed( 0 );

         g_pFGCloudsLayer->SetSpeed( 4 );
         g_pFGCloudsLayer->SetDirection( SD_DOWN );
         g_pFGCloudsLayer->Update( );
         g_pFGCloudsLayer->SetSpeed( 0 );
      }
      else if ( jsJoystickState & JOY_DOWN )
      {
         g_pPersonSprite->Walk( );

         g_pBGLandscapeLayer->SetSpeed( 16 );
         g_pBGLandscapeLayer->SetDirection( SD_UP );
         g_pBGLandscapeLayer->Update( );
         g_pBGLandscapeLayer->SetSpeed( 0 );

         g_pFGCloudsLayer->SetSpeed( 4 );
         g_pFGCloudsLayer->SetDirection( SD_UP );
         g_pFGCloudsLayer->Update( );
         g_pFGCloudsLayer->SetSpeed( 0 );
      }

      g_iInputDelay = 0;
   }
}

BOOL SpriteCollision( Sprite* pSpriteHitter, Sprite* pSpriteHittee )
{
   return FALSE;
}

void SpriteDying( Sprite* pSpriteDying )
{ }
