#include "SpaceOut.h"

BOOL GameInitialize( HINSTANCE hInstance )
{
   g_pGame = new GameEngine( hInstance, TEXT( "Space Out 4" ), TEXT( "Space Out 4 a: Keeping Track of High Scores" ),
                             IDI_SPACEOUT, IDI_SPACEOUT_SM, 600, 450 );

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
   ReadHiScores( );

   srand( GetTickCount( ) );

   g_hOffscreenDC     = CreateCompatibleDC( GetDC( hWindow ) );
   g_hOffscreenBitmap = CreateCompatibleBitmap( GetDC( hWindow ),
                                                g_pGame->GetWidth( ), g_pGame->GetHeight( ) );

   SelectObject( g_hOffscreenDC, g_hOffscreenBitmap );

   HDC hDC = GetDC( hWindow );

   g_pSplashBitmap      = new Bitmap( hDC, IDB_SPLASH, g_hInstance );
   g_pDesertBitmap      = new Bitmap( hDC, IDB_DESERT, g_hInstance );
   g_pCarBitmap         = new Bitmap( hDC, IDB_CAR, g_hInstance );
   g_pSmCarBitmap       = new Bitmap( hDC, IDB_SMCAR, g_hInstance );
   g_pMissileBitmap     = new Bitmap( hDC, IDB_MISSILE, g_hInstance );
   g_pBlobboBitmap      = new Bitmap( hDC, IDB_BLOBBO, g_hInstance );
   g_pBMissileBitmap    = new Bitmap( hDC, IDB_BMISSILE, g_hInstance );
   g_pJellyBitmap       = new Bitmap( hDC, IDB_JELLY, g_hInstance );
   g_pJMissileBitmap    = new Bitmap( hDC, IDB_JMISSILE, g_hInstance );
   g_pTimmyBitmap       = new Bitmap( hDC, IDB_TIMMY, g_hInstance );
   g_pTMissileBitmap    = new Bitmap( hDC, IDB_TMISSILE, g_hInstance );
   g_pSmExplosionBitmap = new Bitmap( hDC, IDB_SMEXPLOSION, g_hInstance );
   g_pLgExplosionBitmap = new Bitmap( hDC, IDB_LGEXPLOSION, g_hInstance );
   g_pGameOverBitmap    = new Bitmap( hDC, IDB_GAMEOVER, g_hInstance );

   g_pBackground = new StarryBackground( 600, 450 );

   g_bDemo = TRUE;
   NewGame( );
}

void GameEnd( )
{
   g_pGame->CloseMIDIPlayer( );

   DeleteObject( g_hOffscreenBitmap );
   DeleteDC( g_hOffscreenDC );

   delete g_pSplashBitmap;
   delete g_pDesertBitmap;
   delete g_pCarBitmap;
   delete g_pSmCarBitmap;
   delete g_pMissileBitmap;
   delete g_pBlobboBitmap;
   delete g_pBMissileBitmap;
   delete g_pJellyBitmap;
   delete g_pJMissileBitmap;
   delete g_pTimmyBitmap;
   delete g_pTMissileBitmap;
   delete g_pSmExplosionBitmap;
   delete g_pLgExplosionBitmap;
   delete g_pGameOverBitmap;

   delete g_pBackground;

   g_pGame->CleanupSprites( );

   delete g_pGame;

   WriteHiScores( );
}

void GameActivate( HWND hWindow )
{
   if ( !g_bDemo )
   {
      g_pGame->PlayMIDISong( TEXT( "" ), FALSE );
   }
}

void GameDeactivate( HWND hWindow )
{
   if ( !g_bDemo )
   {
      g_pGame->PauseMIDISong( );
   }
}

void GamePaint( HDC hDC )
{
   g_pBackground->Draw( hDC );

   g_pDesertBitmap->Draw( hDC, 0, 371 );

   g_pGame->DrawSprites( hDC );

   if ( g_bDemo )
   {
      g_pSplashBitmap->Draw( hDC, 142, 20, TRUE );

      TCHAR szText[64];
      RECT  rect = { 275, 250, 325, 270 };

      SetBkMode( hDC, TRANSPARENT );
      SetTextColor( hDC, RGB( 255, 255, 255 ) );

      for ( int i = 0; i < 5; i++ )
      {
         wsprintf( szText, TEXT( "%d" ), g_iHiScores[i] );
         DrawText( hDC, szText, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER );
         rect.top    += 20;
         rect.bottom += 20;
      }
   }
   else
   {
      TCHAR szText[64];
      RECT  rect = { 460, 0, 510, 30 };

      wsprintf( szText, TEXT( "%d" ), g_iScore );

      SetBkMode( hDC, TRANSPARENT );
      SetTextColor( hDC, RGB( 255, 255, 255 ) );

      DrawText( hDC, szText, -1, &rect, DT_SINGLELINE | DT_RIGHT | DT_VCENTER );

      for ( int i = 0; i < g_iNumLives; i++ )
      {
         g_pSmCarBitmap->Draw( hDC, 520 + ( g_pSmCarBitmap->GetWidth( ) * i ),
                               10, TRUE );
      }

      if ( g_bGameOver )
      {
         g_pGameOverBitmap->Draw( hDC, 170, 100, TRUE );
      }
   }
}

void GameCycle( )
{
   if ( !g_bGameOver )
   {
      if ( !g_bDemo )
      {
         if ( ( rand( ) % g_iDifficulty ) == 0 )
         {
            AddAlien( );
         }
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
   else
   {
      if ( --g_iGameOverDelay == 0 )
      {
         g_pGame->PauseMIDISong( );
         g_bDemo = TRUE;
         NewGame( );
      }
   }
}

void HandleKeys( )
{
   if ( !g_bGameOver && !g_bDemo )
   {
      POINT ptVelocity = g_pCarSprite->GetVelocity( );

      if ( GetAsyncKeyState( VK_LEFT ) < 0 )
      {
         ptVelocity.x = max( ptVelocity.x - 1, -4 );
         g_pCarSprite->SetVelocity( ptVelocity );
      }
      else if ( GetAsyncKeyState( VK_RIGHT ) < 0 )
      {
         ptVelocity.x = min( ptVelocity.x + 2, 6 );
         g_pCarSprite->SetVelocity( ptVelocity );
      }

      if ( ( ++g_iFireInputDelay > 6 ) && GetAsyncKeyState( VK_SPACE ) < 0 )
      {
         RECT    rcBounds = { 0, 0, 600, 450 };
         RECT    rcPos    = g_pCarSprite->GetPosition( );
         Sprite* pSprite  = new Sprite( g_pMissileBitmap, rcBounds, BA_DIE );
         pSprite->SetPosition( rcPos.left + 15, 400 );
         pSprite->SetVelocity( 0, -7 );
         g_pGame->AddSprite( pSprite );

         PlaySound( ( PCTSTR ) IDW_MISSILE, g_hInstance, SND_ASYNC | SND_RESOURCE | SND_NOSTOP );

         g_iFireInputDelay = 0;
      }
   }

   if ( GetAsyncKeyState( VK_RETURN ) < 0 )
      if ( g_bDemo )
      {
         g_bDemo = FALSE;
         NewGame( );
      }
      else if ( g_bGameOver )
      {
         NewGame( );
      }
}

void MouseButtonDown( int x, int y, BOOL bLeft )
{ }

void MouseButtonUp( int x, int y, BOOL bLeft )
{ }

void MouseMove( int x, int y )
{ }

void HandleJoystick( JOYSTATE jsJoystickState )
{ }

BOOL SpriteCollision( Sprite* pSpriteHitter, Sprite* pSpriteHittee )
{
   Bitmap* pHitter = pSpriteHitter->GetBitmap( );
   Bitmap* pHittee = pSpriteHittee->GetBitmap( );

   if ( ( pHitter == g_pMissileBitmap && ( pHittee == g_pBlobboBitmap || pHittee == g_pJellyBitmap || pHittee == g_pTimmyBitmap ) ) ||
        ( pHittee == g_pMissileBitmap && ( pHitter == g_pBlobboBitmap || pHitter == g_pJellyBitmap || pHitter == g_pTimmyBitmap ) ) )
   {
      PlaySound( ( PCTSTR ) IDW_LGEXPLODE, g_hInstance, SND_ASYNC | SND_RESOURCE );

      pSpriteHitter->Kill( );
      pSpriteHittee->Kill( );

      RECT rcBounds = { 0, 0, 600, 450 };
      RECT rcPos;

      if ( pHitter == g_pMissileBitmap )
      {
         rcPos = pSpriteHittee->GetPosition( );
      }
      else
      {
         rcPos = pSpriteHitter->GetPosition( );
      }

      Sprite* pSprite = new Sprite( g_pLgExplosionBitmap, rcBounds );
      pSprite->SetNumFrames( 8, TRUE );
      pSprite->SetPosition( rcPos.left, rcPos.top );
      g_pGame->AddSprite( pSprite );

      g_iScore      += 25;
      g_iDifficulty  = max( 80 - ( g_iScore / 20 ), 20 );
   }

   if ( ( pHitter == g_pCarBitmap && ( pHittee == g_pBMissileBitmap || pHittee == g_pJMissileBitmap || pHittee == g_pTMissileBitmap ) ) ||
        ( pHittee == g_pCarBitmap && ( pHitter == g_pBMissileBitmap || pHitter == g_pJMissileBitmap || pHitter == g_pTMissileBitmap ) ) )
   {
      PlaySound( ( PCTSTR ) IDW_LGEXPLODE, g_hInstance, SND_ASYNC | SND_RESOURCE );

      if ( pHitter == g_pCarBitmap )
      {
         pSpriteHittee->Kill( );
      }
      else
      {
         pSpriteHitter->Kill( );
      }

      RECT rcBounds = { 0, 0, 600, 480 };
      RECT rcPos;

      if ( pHitter == g_pCarBitmap )
      {
         rcPos = pSpriteHitter->GetPosition( );
      }
      else
      {
         rcPos = pSpriteHittee->GetPosition( );
      }

      Sprite* pSprite = new Sprite( g_pLgExplosionBitmap, rcBounds );
      pSprite->SetNumFrames( 8, TRUE );
      pSprite->SetPosition( rcPos.left, rcPos.top );
      g_pGame->AddSprite( pSprite );

      g_pCarSprite->SetPosition( 300, 405 );

      if ( --g_iNumLives == 0 )
      {
         PlaySound( ( PCTSTR ) IDW_GAMEOVER, g_hInstance, SND_ASYNC | SND_RESOURCE );
         g_bGameOver = TRUE;
         g_iGameOverDelay = 150;

         UpdateHiScores( );
      }
   }

   return FALSE;
}

void SpriteDying( Sprite* pSpriteDying )
{
   if ( pSpriteDying->GetBitmap( ) == g_pBMissileBitmap ||
        pSpriteDying->GetBitmap( ) == g_pJMissileBitmap ||
        pSpriteDying->GetBitmap( ) == g_pTMissileBitmap )
   {
      if ( !g_bDemo )
      {
         PlaySound( ( PCTSTR ) IDW_SMEXPLODE, g_hInstance, SND_ASYNC |
                    SND_RESOURCE | SND_NOSTOP );
      }

      RECT    rcBounds = { 0, 0, 600, 450 };
      RECT    rcPos    = pSpriteDying->GetPosition( );
      Sprite* pSprite  = new Sprite( g_pSmExplosionBitmap, rcBounds );
      pSprite->SetNumFrames( 8, TRUE );
      pSprite->SetPosition( rcPos.left, rcPos.top );
      g_pGame->AddSprite( pSprite );
   }
}

void NewGame( )
{
   g_pGame->CleanupSprites( );

   g_iFireInputDelay = 0;
   g_iScore = 0;
   g_iNumLives = 3;
   g_iDifficulty = 80;
   g_bGameOver = FALSE;

   if ( g_bDemo )
   {
      for ( int i = 0; i < 6; i++ )
      {
         AddAlien( );
      }
   }
   else
   {
      RECT rcBounds = { 0, 0, 600, 450 };
      g_pCarSprite  = new Sprite( g_pCarBitmap, rcBounds, BA_WRAP );
      g_pCarSprite->SetPosition( 300, 405 );
      g_pGame->AddSprite( g_pCarSprite );

      g_pGame->PlayMIDISong( TEXT( "Music.mid" ) );
   }
}

void AddAlien( )
{
   RECT         rcBounds = { 0, 0, 600, 410 };
   AlienSprite* pSprite  = NULL;

   switch ( rand( ) % 3 )
   {
   case 0:
      pSprite = new AlienSprite( g_pBlobboBitmap, rcBounds, BA_BOUNCE );
      pSprite->SetNumFrames( 8 );
      pSprite->SetPosition( ( ( rand( ) % 2 ) == 0 ) ? 0 : 600, rand( ) % 370 );
      pSprite->SetVelocity( ( rand( ) % 7 ) - 2, ( rand( ) % 7 ) - 2 );
      break;

   case 1:
      pSprite = new AlienSprite( g_pJellyBitmap, rcBounds, BA_BOUNCE );
      pSprite->SetNumFrames( 8 );
      pSprite->SetPosition( rand( ) % 600, rand( ) % 370 );
      pSprite->SetVelocity( ( rand( ) % 5 ) - 2, ( rand( ) % 5 ) + 3 );
      break;

   case 2:
      pSprite = new AlienSprite( g_pTimmyBitmap, rcBounds, BA_WRAP );
      pSprite->SetNumFrames( 8 );
      pSprite->SetPosition( rand( ) % 600, rand( ) % 370 );
      pSprite->SetVelocity( ( rand( ) % 7 ) + 3, 0 );
      break;
   }

   g_pGame->AddSprite( pSprite );
}

void UpdateHiScores( )
{
   int i;

   for ( i = 0; i < 5; i++ )
   {
      if ( g_iScore > g_iHiScores[i] )
      {
         break;
      }
   }

   if ( i < 5 )
   {
      for ( int j = 4; j > i; j-- )
      {
         g_iHiScores[j] = g_iHiScores[j - 1];
      }
      g_iHiScores[i] = g_iScore;
   }
}

BOOL ReadHiScores( )
{
   HANDLE hFile = CreateFile( TEXT( "HiScores.dat" ), GENERIC_READ, 0, NULL,
                              OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL );

   if ( hFile == INVALID_HANDLE_VALUE )
   {
      for ( int i = 0; i < 5; i++ )
      {
         g_iHiScores[i] = 0;
      }
      return FALSE;
   }

   for ( int i = 0; i < 5; i++ )
   {
      char  cData[6];
      DWORD dwBytesRead;

      if ( !ReadFile( hFile, &cData, 5, &dwBytesRead, NULL ) )
      {
         CloseHandle( hFile );
         return FALSE;
      }

      g_iHiScores[i] = atoi( cData );
   }

   return CloseHandle( hFile );
}

BOOL WriteHiScores( )
{
   HANDLE hFile = CreateFile( TEXT( "HiScores.dat" ), GENERIC_WRITE, 0, NULL,
                              CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

   if ( hFile == INVALID_HANDLE_VALUE )
   {
      return FALSE;
   }

   for ( int i = 0; i < 5; i++ )
   {
      CHAR cData[6];
      wsprintfA( cData, "%05d", g_iHiScores[i] );

      DWORD dwBytesWritten;

      if ( !WriteFile( hFile, &cData, 5, &dwBytesWritten, NULL ) )
      {
         CloseHandle( hFile );
         return FALSE;
      }
   }

   return CloseHandle( hFile );
}
