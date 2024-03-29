#include "UFO.h"

BOOL GameInitialize( HINSTANCE hInstance )
{
   g_pGame = new GameEngine( hInstance, TEXT( "UFO" ),
                             TEXT( "UFO 1: Controlling Games with the Keyboard and Mouse" ), IDI_UFO, IDI_UFO_SM, 500, 400 );
   if ( g_pGame == NULL )
      return FALSE;

   g_pGame->SetFrameRate( 30 );

   g_hInstance = hInstance;

   return TRUE;
}

void GameStart( HWND hWindow )
{
   HDC hDC = GetDC( hWindow );

   g_pBackground = new Bitmap( hDC, IDB_BACKGROUND, g_hInstance );
   g_pSaucer     = new Bitmap( hDC, IDB_SAUCER, g_hInstance );

   g_iSaucerX = 250 - ( g_pSaucer->GetWidth( )  / 2 );
   g_iSaucerY = 200 - ( g_pSaucer->GetHeight( ) / 2 );
   g_iSpeedX  = 0;
   g_iSpeedY  = 0;
}

void GameEnd( )
{
   delete g_pBackground;
   delete g_pSaucer;

   delete g_pGame;
}

void GameActivate( HWND hWindow )
{ }

void GameDeactivate( HWND hWindow )
{ }

void GamePaint( HDC hDC )
{
   g_pBackground->Draw( hDC, 0, 0 );
   g_pSaucer->Draw( hDC, g_iSaucerX, g_iSaucerY, TRUE );
}

void GameCycle( )
{
   // Update the saucer position
   g_iSaucerX = min( 500 - g_pSaucer->GetWidth( ), max( 0, g_iSaucerX + g_iSpeedX ) );
   g_iSaucerY = min( 320, max( 0, g_iSaucerY + g_iSpeedY ) );

   InvalidateRect( g_pGame->GetWindow( ), NULL, FALSE );
}

void HandleKeys( )
{
   // Change the speed of the saucer in response to arrow key presses
   if ( GetAsyncKeyState( VK_LEFT ) < 0 )
      g_iSpeedX = max( -g_iMAXSPEED, --g_iSpeedX );
   else if ( GetAsyncKeyState( VK_RIGHT ) < 0 )
      g_iSpeedX = min( g_iMAXSPEED, ++g_iSpeedX );
   if ( GetAsyncKeyState( VK_UP ) < 0 )
      g_iSpeedY = max( -g_iMAXSPEED, --g_iSpeedY );
   else if ( GetAsyncKeyState( VK_DOWN ) < 0 )
      g_iSpeedY = min( g_iMAXSPEED, ++g_iSpeedY );
}

void MouseButtonDown( int x, int y, BOOL bLeft )
{
   if ( bLeft )
   {
      // Set the saucer position to the mouse position
      g_iSaucerX = x - ( g_pSaucer->GetWidth( )  / 2 );
      g_iSaucerY = y - ( g_pSaucer->GetHeight( ) / 2 );
   }
   else
   {
      // Stop the saucer
      g_iSpeedX = 0;
      g_iSpeedY = 0;
   }
}

void MouseButtonUp( int x, int y, BOOL bLeft )
{ }

void MouseMove( int x, int y )
{ }
