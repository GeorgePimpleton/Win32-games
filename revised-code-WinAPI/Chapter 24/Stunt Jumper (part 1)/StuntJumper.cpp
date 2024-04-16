#include "StuntJumper.h"

BOOL GameInitialize( HINSTANCE hInstance )
{
   g_pGame = new GameEngine( hInstance, TEXT( "Stunt Jumper" ), TEXT( "Example Game: Stunt Jumper a" ),
                             IDI_STUNTJUMPER, IDI_STUNTJUMPER_SM, 750, 250 );

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
   srand( GetTickCount( ) );

   g_hOffscreenDC = CreateCompatibleDC( GetDC( hWindow ) );
   g_hOffscreenBitmap = CreateCompatibleBitmap( GetDC( hWindow ),
                                                g_pGame->GetWidth( ), g_pGame->GetHeight( ) );

   SelectObject( g_hOffscreenDC, g_hOffscreenBitmap );

   HDC hDC = GetDC( hWindow );

   g_pSplashBitmap = new Bitmap( hDC, IDB_SPLASH, g_hInstance );
   g_pJumperBitmap = new Bitmap( hDC, IDB_JUMPER, g_hInstance );
   g_pBusBitmap = new Bitmap( hDC, IDB_BUS, g_hInstance );
   g_pRampBitmap[0] = new Bitmap( hDC, IDB_RAMPLEFT, g_hInstance );
   g_pRampBitmap[1] = new Bitmap( hDC, IDB_RAMPRIGHT, g_hInstance );
   g_pGameOverBitmap = new Bitmap( hDC, IDB_GAMEOVER, g_hInstance );

   g_pBackground = new ScrollingBackground( 750, 250 );
   g_pBGSkyLayer = new BackgroundLayer( hDC, IDB_BG_SKY, g_hInstance, 1, SD_LEFT );
   g_pBackground->AddLayer( g_pBGSkyLayer );
   g_pBGMountainsLayer = new BackgroundLayer( hDC, IDB_BG_MOUNTAINS, g_hInstance, 2, SD_LEFT );
   g_pBackground->AddLayer( g_pBGMountainsLayer );
   g_pBGTreesLayer = new BackgroundLayer( hDC, IDB_BG_TREES, g_hInstance, 3, SD_LEFT );
   g_pBackground->AddLayer( g_pBGTreesLayer );
   g_pBGRoadLayer = new BackgroundLayer( hDC, IDB_BG_ROAD, g_hInstance );
   g_pBackground->AddLayer( g_pBGRoadLayer );

   g_bSplash = TRUE;
   g_bGameOver = TRUE;
}

void GameEnd( )
{
   g_pGame->CloseMIDIPlayer( );

   DeleteObject( g_hOffscreenBitmap );
   DeleteDC( g_hOffscreenDC );

   delete g_pSplashBitmap;
   delete g_pJumperBitmap;
   delete g_pBusBitmap;
   delete g_pRampBitmap[0];
   delete g_pRampBitmap[1];
   delete g_pGameOverBitmap;

   delete g_pBackground;
   delete g_pBGSkyLayer;
   delete g_pBGMountainsLayer;
   delete g_pBGTreesLayer;
   delete g_pBGRoadLayer;

   g_pGame->CleanupSprites( );

   delete g_pGame;
}

void GameActivate( HWND hWindow )
{
   g_pGame->CaptureJoystick( );

   if ( !g_bSplash )
   {
      g_pGame->PlayMIDISong( TEXT( "" ), FALSE );
   }
}

void GameDeactivate( HWND hWindow )
{
   g_pGame->ReleaseJoystick( );

   if ( !g_bSplash )
   {
      g_pGame->PauseMIDISong( );
   }
}

void GamePaint( HDC hDC )
{
   g_pBackground->Draw( hDC, TRUE );

   if ( g_bSplash )
   {
      g_pSplashBitmap->Draw( hDC, 175, 15, TRUE );
   }
   else
   {
      g_pGame->DrawSprites( hDC );

      if ( g_bGameOver )
      {
         g_pGameOverBitmap->Draw( hDC, 175, 15, FALSE );
      }
   }
}

void GameCycle( )
{
   if ( !g_bGameOver )
   {
      g_pBackground->Update( );

      g_pGame->UpdateSprites( );

      HWND  hWindow = g_pGame->GetWindow( );
      HDC   hDC = GetDC( hWindow );

      GamePaint( g_hOffscreenDC );

      BitBlt( hDC, 0, 0, g_pGame->GetWidth( ), g_pGame->GetHeight( ),
              g_hOffscreenDC, 0, 0, SRCCOPY );

      ReleaseDC( hWindow, hDC );

      RECT& rc = g_pJumperSprite->GetPosition( );

      if ( rc.right > g_pGame->GetWidth( ) )
      {
         NewJump( rand( ) % 7 + 1 );
      }
   }
}

void HandleKeys( )
{
   if ( !g_bGameOver )
   {
      POINT ptVelocity = g_pJumperSprite->GetVelocity( );

      if ( g_iInputDelay++ > 1 )
      {
         if ( GetAsyncKeyState( VK_LEFT ) < 0 )
         {
            PlaySound( ( PCTSTR ) IDW_BRAKES, g_hInstance, SND_ASYNC |
                       SND_RESOURCE );

            g_pJumperSprite->DecreaseSpeed( );

            g_iInputDelay = 0;
         }
         else if ( GetAsyncKeyState( VK_RIGHT ) < 0 )
         {
            PlaySound( ( PCTSTR ) IDW_ENGINE, g_hInstance, SND_ASYNC |
                       SND_RESOURCE );

            g_pJumperSprite->IncreaseSpeed( );

            g_iInputDelay = 0;
         }
      }
   }

   if ( GetAsyncKeyState( VK_RETURN ) < 0 )
   {
      if ( g_bSplash )
      {
         g_bSplash = FALSE;
         NewGame( );
      }
      else if ( g_bGameOver )
      {
         NewGame( );
      }
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
   if ( !g_bGameOver )
   {
      POINT ptVelocity = g_pJumperSprite->GetVelocity( );

      if ( g_iInputDelay++ > 1 )
      {
         if ( jsJoystickState & JOY_LEFT )
         {
            PlaySound( ( PCTSTR ) IDW_BRAKES, g_hInstance, SND_ASYNC |
                       SND_RESOURCE );

            g_pJumperSprite->DecreaseSpeed( );

            g_iInputDelay = 0;
         }
         else if ( jsJoystickState & JOY_RIGHT )
         {
            PlaySound( ( PCTSTR ) IDW_ENGINE, g_hInstance, SND_ASYNC |
                       SND_RESOURCE );

            g_pJumperSprite->IncreaseSpeed( );

            g_iInputDelay = 0;
         }
      }
   }

   if ( jsJoystickState & JOY_FIRE1 )
      if ( g_bSplash )
      {
         g_bSplash = FALSE;
         NewGame( );
      }
      else if ( g_bGameOver )
      {
         NewGame( );
      }
}

BOOL SpriteCollision( Sprite* pSpriteHitter, Sprite* pSpriteHittee )
{
   Bitmap* pHitter = pSpriteHitter->GetBitmap( );
   Bitmap* pHittee = pSpriteHittee->GetBitmap( );

   if ( !pSpriteHitter->IsHidden( ) && !pSpriteHittee->IsHidden( ) )
   {
      if ( ( pHitter == g_pJumperBitmap ) && ( pHittee == g_pRampBitmap[0] ) )
      {
         g_pJumperSprite->StartJumping( );
      }
      else if ( ( pHitter == g_pJumperBitmap ) && ( pHittee == g_pRampBitmap[1] ) )
      {
         if ( !g_pJumperSprite->HasLandedSafely( ) )
         {
            PlaySound( ( PCTSTR ) IDW_CELEBRATION, g_hInstance, SND_ASYNC |
                       SND_RESOURCE );

            g_pJumperSprite->LandedSafely( );
         }
      }
      else if ( ( pHitter == g_pJumperBitmap ) && ( pHittee == g_pBusBitmap ) )
      {
         PlaySound( ( PCTSTR ) IDW_CRASH, g_hInstance, SND_ASYNC |
                    SND_RESOURCE );

         g_bGameOver = TRUE;
      }
   }

   return FALSE;
}

void SpriteDying( Sprite* pSpriteDying )
{ }

void NewGame( )
{
   g_pGame->CleanupSprites( );

   g_iInputDelay = 0;
   g_bGameOver = FALSE;

   RECT rcBounds = { 0, 0, 750, 250 };

   g_pLaunchRampSprite = new Sprite( g_pRampBitmap[0], rcBounds );
   g_pGame->AddSprite( g_pLaunchRampSprite );
   g_pLandingRampSprite = new Sprite( g_pRampBitmap[1], rcBounds );
   g_pGame->AddSprite( g_pLandingRampSprite );

   for ( int i = 0; i < 7; i++ )
   {
      g_pBusSprite[i] = new Sprite( g_pBusBitmap, rcBounds );
      g_pGame->AddSprite( g_pBusSprite[i] );
   }

   g_pJumperSprite = new MotorcycleSprite( g_pJumperBitmap, rcBounds, BA_WRAP );
   g_pJumperSprite->SetNumFrames( 13 );
   g_pJumperSprite->SetVelocity( 4, 0 );
   g_pJumperSprite->SetPosition( 0, 200 );
   g_pGame->AddSprite( g_pJumperSprite );

   NewJump( rand( ) % 3 + 1 );

   g_pGame->PlayMIDISong( TEXT( "Music.mid" ) );
}

void NewJump( int iNumBuses )
{
   int iXStart = ( g_pGame->GetWidth( ) / 2 ) - ( iNumBuses * 40 );

   g_pLaunchRampSprite->SetPosition( iXStart, 215 );

   for ( int i = 0; i < 7; i++ )
   {
      if ( i < iNumBuses )
      {
         g_pBusSprite[i]->SetPosition( iXStart + g_pRampBitmap[0]->GetWidth( ) +
                                       5 + i * g_pBusBitmap->GetWidth( ), 200 );
         g_pBusSprite[i]->SetHidden( FALSE );
      }
      else
      {
         g_pBusSprite[i]->SetPosition( 0, 0 );
         g_pBusSprite[i]->SetHidden( TRUE );
      }
   }

   g_pLandingRampSprite->SetPosition( iXStart + g_pRampBitmap[0]->GetWidth( ) +
                                      5 + iNumBuses * g_pBusBitmap->GetWidth( ) + 5, 215 );
}
