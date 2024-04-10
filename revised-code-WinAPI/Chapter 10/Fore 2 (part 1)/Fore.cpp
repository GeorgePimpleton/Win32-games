#include "Fore.h"

BOOL GameInitialize( HINSTANCE hInstance )
{
   _pGame = new GameEngine( hInstance, TEXT( "Fore 2" ), TEXT( "Fore 2 a: Managing a World of Sprites" ),
                            IDI_FORE, IDI_FORE_SM, 600, 400 );
   if ( _pGame == NULL )
      return FALSE;

   _pGame->SetFrameRate( 30 );

   _hInstance = hInstance;

   return TRUE;
}

void GameStart( HWND hWindow )
{
   srand( GetTickCount( ) );

   _hOffscreenDC     = CreateCompatibleDC( GetDC( hWindow ) );
   _hOffscreenBitmap = CreateCompatibleBitmap( GetDC( hWindow ), _pGame->GetWidth( ), _pGame->GetHeight( ) );
   SelectObject( _hOffscreenDC, _hOffscreenBitmap );

   HDC hDC = GetDC( hWindow );
   _pForestBitmap = new Bitmap( hDC, IDB_FOREST, _hInstance );
   _pGolfBallBitmap = new Bitmap( hDC, IDB_GOLFBALL, _hInstance );

   RECT    rcBounds = { 0, 0, 600, 400 };
   Sprite* pSprite;

   pSprite = new Sprite( _pGolfBallBitmap, rcBounds, BA_WRAP );
   pSprite->SetVelocity( 5, 3 );
   _pGame->AddSprite( pSprite );

   pSprite = new Sprite( _pGolfBallBitmap, rcBounds, BA_WRAP );
   pSprite->SetVelocity( 3, 2 );
   _pGame->AddSprite( pSprite );
   rcBounds.left = 265; rcBounds.right = 500; rcBounds.bottom = 335;

   pSprite = new Sprite( _pGolfBallBitmap, rcBounds, BA_BOUNCE );
   pSprite->SetVelocity( -6, 5 );
   _pGame->AddSprite( pSprite );
   rcBounds.right = 470;

   pSprite = new Sprite( _pGolfBallBitmap, rcBounds, BA_BOUNCE );
   pSprite->SetVelocity( 7, -3 );
   _pGame->AddSprite( pSprite );

   _pDragSprite = NULL;
}

void GameEnd( )
{
   DeleteObject( _hOffscreenBitmap );
   DeleteDC( _hOffscreenDC );

   delete _pForestBitmap;
   delete _pGolfBallBitmap;

   _pGame->CleanupSprites( );

   delete _pGame;
}

void GameActivate( HWND hWindow )
{ }

void GameDeactivate( HWND hWindow )
{ }

void GamePaint( HDC hDC )
{
   // Draw the background forest
   _pForestBitmap->Draw( hDC, 0, 0 );

   // Draw the sprites
   _pGame->DrawSprites( hDC );
}

void GameCycle( )
{
   // Update the sprites
   _pGame->UpdateSprites( );

   // Obtain a device context for repainting the game
   HWND  hWindow = _pGame->GetWindow( );
   HDC   hDC = GetDC( hWindow );

   // Paint the game to the offscreen device context
   GamePaint( _hOffscreenDC );

   // Blit the offscreen bitmap to the game screen
   BitBlt( hDC, 0, 0, _pGame->GetWidth( ), _pGame->GetHeight( ),
           _hOffscreenDC, 0, 0, SRCCOPY );

   // Cleanup
   ReleaseDC( hWindow, hDC );
}

void HandleKeys( )
{ }

void MouseButtonDown( int x, int y, BOOL bLeft )
{
   if ( bLeft && ( _pDragSprite == NULL ) )
   {
      if ( ( _pDragSprite = _pGame->IsPointInSprite( x, y ) ) != NULL )
      {
         SetCapture( _pGame->GetWindow( ) );

         MouseMove( x, y );
      }
   }
}

void MouseButtonUp( int x, int y, BOOL bLeft )
{
   ReleaseCapture( );

   _pDragSprite = NULL;
}

void MouseMove( int x, int y )
{
   if ( _pDragSprite != NULL )
   {
      _pDragSprite->SetPosition( x - ( _pDragSprite->GetWidth( ) / 2 ),
                                 y - ( _pDragSprite->GetHeight( ) / 2 ) );

      InvalidateRect( _pGame->GetWindow( ), NULL, FALSE );
   }
}

void HandleJoystick( JOYSTATE jsJoystickState )
{ }

BOOL SpriteCollision( Sprite* pSpriteHitter, Sprite* pSpriteHittee )
{
   // Swap the sprite velocities so that they appear to bounce
   POINT ptSwapVelocity = pSpriteHitter->GetVelocity( );
   pSpriteHitter->SetVelocity( pSpriteHittee->GetVelocity( ) );
   pSpriteHittee->SetVelocity( ptSwapVelocity );
   return TRUE;
}
