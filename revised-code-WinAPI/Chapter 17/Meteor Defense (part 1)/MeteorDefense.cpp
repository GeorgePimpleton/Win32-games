#include "MeteorDefense.h"

BOOL GameInitialize( HINSTANCE hInstance )
{
   g_pGame = new GameEngine( hInstance, TEXT( "Meteor Defense" ), TEXT( "Meteor Defense a" ),
                             IDI_METEORDEFENSE, IDI_METEORDEFENSE_SM, 600, 450 );

   if ( g_pGame == NULL )
   {
      return FALSE;
   }

   g_pGame->SetFrameRate( 30 );

   g_hInstance = hInstance;

   return TRUE;
}

void GameStart( HWND hWindow )
{
   srand( GetTickCount( ) );

   g_hOffscreenDC     = CreateCompatibleDC( GetDC( hWindow ) );
   g_hOffscreenBitmap = CreateCompatibleBitmap( GetDC( hWindow ),
                                                g_pGame->GetWidth( ), g_pGame->GetHeight( ) );

   SelectObject( g_hOffscreenDC, g_hOffscreenBitmap );

   HDC hDC = GetDC( hWindow );

   g_pGroundBitmap    = new Bitmap( hDC, IDB_GROUND, g_hInstance );
   g_pTargetBitmap    = new Bitmap( hDC, IDB_TARGET, g_hInstance );
   g_pCityBitmap      = new Bitmap( hDC, IDB_CITY, g_hInstance );
   g_pMeteorBitmap    = new Bitmap( hDC, IDB_METEOR, g_hInstance );
   g_pMissileBitmap   = new Bitmap( hDC, IDB_MISSILE, g_hInstance );
   g_pExplosionBitmap = new Bitmap( hDC, IDB_EXPLOSION, g_hInstance );
   g_pGameOverBitmap  = new Bitmap( hDC, IDB_GAMEOVER, g_hInstance );

   g_pBackground = new StarryBackground( 600, 450 );

   g_pGame->PlayMIDISong( TEXT( "Music.mid" ) );

   NewGame( );
}

void GameEnd( )
{
   g_pGame->CloseMIDIPlayer( );

   DeleteObject( g_hOffscreenBitmap );
   DeleteDC( g_hOffscreenDC );

   delete g_pGroundBitmap;
   delete g_pTargetBitmap;
   delete g_pCityBitmap;
   delete g_pMeteorBitmap;
   delete g_pMissileBitmap;
   delete g_pExplosionBitmap;
   delete g_pGameOverBitmap;

   delete g_pBackground;

   g_pGame->CleanupSprites( );

   delete g_pGame;
}

void GameActivate( HWND hWindow )
{
   g_pGame->PlayMIDISong( TEXT( "" ), FALSE );
}

void GameDeactivate( HWND hWindow )
{
   g_pGame->PauseMIDISong( );
}

void GamePaint( HDC hDC )
{
   g_pBackground->Draw( hDC );

   g_pGroundBitmap->Draw( hDC, 0, 398, TRUE );

   g_pGame->DrawSprites( hDC );

   TCHAR szText[64];
   RECT  rect = { 275, 0, 325, 50 };

   wsprintf( szText, TEXT( "%d" ), g_iScore );

   SetBkMode( hDC, TRANSPARENT );
   SetTextColor( hDC, RGB( 255, 255, 255 ) );

   DrawText( hDC, szText, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER );

   if ( g_bGameOver )
   {
      g_pGameOverBitmap->Draw( hDC, 170, 150, TRUE );
   }
}

void GameCycle( )
{
   if ( !g_bGameOver )
   {
      if ( ( rand( ) % g_iDifficulty ) == 0 )
      {
         AddMeteor( );
      }

      g_pBackground->Update( );

      g_pGame->UpdateSprites( );

      HWND hWindow = g_pGame->GetWindow( );
      HDC  hDC     = GetDC( hWindow );

      GamePaint( g_hOffscreenDC );

      BitBlt( hDC, 0, 0, g_pGame->GetWidth( ), g_pGame->GetHeight( ),
              g_hOffscreenDC, 0, 0, SRCCOPY );

      ReleaseDC( hWindow, hDC );
   }
}

void HandleKeys( )
{ }

void MouseButtonDown( int x, int y, BOOL bLeft )
{
   if ( !g_bGameOver && bLeft )
   {
      RECT rcBounds = { 0, 0, 600, 450 };
      int  iXPos    = ( x < 300 ) ? 144 : 449;

      Sprite* pSprite = new Sprite( g_pMissileBitmap, rcBounds, BA_DIE );

      pSprite->SetPosition( iXPos, 365 );

      int iXVel;
      int iYVel = -6;

      y     = min( y, 300 );
      iXVel = ( iYVel * ( ( iXPos + 8 ) - x ) ) / ( 365 - y );

      pSprite->SetVelocity( iXVel, iYVel );

      g_pGame->AddSprite( pSprite );

      PlaySound( ( PCTSTR ) IDW_FIRE, g_hInstance, SND_ASYNC | SND_RESOURCE | SND_NOSTOP );

      g_iScore = max( --g_iScore, 0 );
   }
   else if ( g_bGameOver && !bLeft )
      NewGame( );
}

void MouseButtonUp( int x, int y, BOOL bLeft )
{ }

void MouseMove( int x, int y )
{
   g_pTargetSprite->SetPosition( x - ( g_pTargetSprite->GetWidth( ) / 2 ),
                                 y - ( g_pTargetSprite->GetHeight( ) / 2 ) );
}

void HandleJoystick( JOYSTATE jsJoystickState )
{ }

BOOL SpriteCollision( Sprite* pSpriteHitter, Sprite* pSpriteHittee )
{
   if ( ( pSpriteHitter->GetBitmap( ) == g_pMissileBitmap &&
        pSpriteHittee->GetBitmap( ) == g_pMeteorBitmap ) ||
        ( pSpriteHitter->GetBitmap( ) == g_pMeteorBitmap &&
        pSpriteHittee->GetBitmap( ) == g_pMissileBitmap ) )
   {
      pSpriteHitter->Kill( );
      pSpriteHittee->Kill( );

      g_iScore      += 6;
      g_iDifficulty  = max( 50 - ( g_iScore / 10 ), 5 );
   }

   if ( pSpriteHitter->GetBitmap( ) == g_pMeteorBitmap &&
        pSpriteHittee->GetBitmap( ) == g_pCityBitmap )
   {
      PlaySound( ( PCTSTR ) IDW_BIGEXPLODE, g_hInstance, SND_ASYNC | SND_RESOURCE );

      pSpriteHitter->Kill( );
      pSpriteHittee->Kill( );

      if ( --g_iNumCities == 0 )
      {
         g_bGameOver = TRUE;
      }
   }

   return FALSE;
}

void SpriteDying( Sprite* pSpriteDying )
{
   if ( pSpriteDying->GetBitmap( ) == g_pMeteorBitmap )
   {
      PlaySound( ( PCTSTR ) IDW_EXPLODE, g_hInstance, SND_ASYNC | SND_RESOURCE | SND_NOSTOP );

      RECT rcBounds = { 0, 0, 600, 450 };
      RECT rcPos    = pSpriteDying->GetPosition( );

      Sprite* pSprite = new Sprite( g_pExplosionBitmap, rcBounds );

      pSprite->SetNumFrames( 12, TRUE );
      pSprite->SetPosition( rcPos.left, rcPos.top );
      g_pGame->AddSprite( pSprite );
   }
}

void NewGame( )
{
   g_pGame->CleanupSprites( );

   RECT rcBounds = { 0, 0, 600, 450 };
   g_pTargetSprite = new Sprite( g_pTargetBitmap, rcBounds, BA_STOP );
   g_pTargetSprite->SetZOrder( 10 );
   g_pGame->AddSprite( g_pTargetSprite );

   Sprite* pSprite = new Sprite( g_pCityBitmap, rcBounds );
   pSprite->SetPosition( 2, 370 );
   g_pGame->AddSprite( pSprite );
   pSprite = new Sprite( g_pCityBitmap, rcBounds );
   pSprite->SetPosition( 186, 370 );
   g_pGame->AddSprite( pSprite );
   pSprite = new Sprite( g_pCityBitmap, rcBounds );
   pSprite->SetPosition( 302, 370 );
   g_pGame->AddSprite( pSprite );
   pSprite = new Sprite( g_pCityBitmap, rcBounds );
   pSprite->SetPosition( 490, 370 );
   g_pGame->AddSprite( pSprite );

   g_iScore      = 0;
   g_iNumCities  = 4;
   g_iDifficulty = 50;
   g_bGameOver   = FALSE;

   g_pGame->PlayMIDISong( );
}

void AddMeteor( )
{
   RECT    rcBounds = { 0, 0, 600, 390 };
   int     iXPos = rand( ) % 600;
   Sprite* pSprite = new Sprite( g_pMeteorBitmap, rcBounds, BA_DIE );
   pSprite->SetNumFrames( 14 );
   pSprite->SetPosition( iXPos, 0 );

   int iXVel;
   int iYVel = ( rand( ) % 4 ) + 3;

   switch ( rand( ) % 4 )
   {
   case 0:
      iXVel = ( iYVel * ( 56 - ( iXPos + 50 ) ) ) / 400;
      break;

   case 1:
      iXVel = ( iYVel * ( 240 - ( iXPos + 50 ) ) ) / 400;
      break;

   case 2:
      iXVel = ( iYVel * ( 360 - ( iXPos + 50 ) ) ) / 400;
      break;

   case 3:
      iXVel = ( iYVel * ( 546 - ( iXPos + 50 ) ) ) / 400;
      break;
   }

   pSprite->SetVelocity( iXVel, iYVel );

   g_pGame->AddSprite( pSprite );
}
