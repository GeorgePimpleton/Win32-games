#include "UFO.h"

BOOL GameInitialize( HINSTANCE hInstance )
{
   g_pGame = new GameEngine( hInstance, TEXT( "UFO 2" ),
                             TEXT( "UFO 2 a" ), IDI_UFO, IDI_UFO_SM, 500, 400 );
   if ( g_pGame == NULL )
      return FALSE;

   g_pGame->SetFrameRate( 30 );

   // initialize the joystick
   g_pGame->InitJoystick( );

   g_hInstance = hInstance;

   return TRUE;
}

void GameStart( HWND hWindow )
{
   srand( GetTickCount( ) );

   HDC hDC = GetDC( hWindow );

   g_pBackground = new Bitmap( hDC, IDB_BACKGROUND, g_hInstance );
   g_pSaucer[0]  = new Bitmap( hDC, IDB_SAUCER, g_hInstance );
   g_pSaucer[1]  = new Bitmap( hDC, IDB_SAUCERFLAME, g_hInstance );

   // set the initial saucer position and speed
   g_iSaucerX = 250 - ( g_pSaucer[0]->GetWidth( )  / 2 );
   g_iSaucerY = 200 - ( g_pSaucer[0]->GetHeight( ) / 2 );
   g_iSpeedX  = 0;
   g_iSpeedY  = 0;
}

void GameEnd( )
{
   delete g_pBackground;
   delete g_pSaucer[0];
   delete g_pSaucer[1];

   delete g_pGame;
}

void GameActivate( HWND hWindow )
{
   // capture the joystick
   g_pGame->CaptureJoystick( );
}

void GameDeactivate( HWND hWindow )
{
   // Release the joystick
   g_pGame->ReleaseJoystick( );
}

void GamePaint( HDC hDC )
{
   g_pBackground->Draw( hDC, 0, 0 );
   g_pSaucer[g_bSaucerFlame ? 1 : 0]->Draw( hDC, g_iSaucerX, g_iSaucerY, TRUE );
}

void GameCycle( )
{
   g_iSaucerX = min( 500 - g_pSaucer[0]->GetWidth( ), max( 0, g_iSaucerX + g_iSpeedX ) );
   g_iSaucerY = min( 320, max( 0, g_iSaucerY + g_iSpeedY ) );

   InvalidateRect( g_pGame->GetWindow( ), NULL, FALSE );
}

void HandleKeys( )
{
   // change the speed of the saucer in response to arrow key presses
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
      // set the saucer position to the mouse position
      g_iSaucerX = x - ( g_pSaucer[0]->GetWidth( ) / 2 );
      g_iSaucerY = y - ( g_pSaucer[0]->GetHeight( ) / 2 );
   }
   else
   {
      // stop the saucer
      g_iSpeedX = 0;
      g_iSpeedY = 0;
   }
}

void MouseButtonUp( int x, int y, BOOL bLeft )
{ }

void MouseMove( int x, int y )
{ }

void HandleJoystick( JOYSTATE jsJoystickState )
{
   // check horizontal movement
   if ( jsJoystickState & JOY_LEFT )
      g_iSpeedX = max( -g_iMAXSPEED, g_iSpeedX - 2 );
   else if ( jsJoystickState & JOY_RIGHT )
      g_iSpeedX = min( g_iMAXSPEED, g_iSpeedX + 2 );

   // check vertical movement
   if ( jsJoystickState & JOY_UP )
      g_iSpeedY = max( -g_iMAXSPEED, g_iSpeedY - 2 );
   else if ( jsJoystickState & JOY_DOWN )
      g_iSpeedY = min( g_iMAXSPEED, g_iSpeedY + 2 );

   // check primary joystick button
   g_bSaucerFlame = ( jsJoystickState & JOY_FIRE1 );

   // check secondary joystick button
   if ( jsJoystickState & JOY_FIRE2 )
   {
      // force the flying saucer into hyperspace
      g_iSaucerX = rand( ) % ( 500 - g_pSaucer[0]->GetWidth( ) );
      g_iSaucerY = rand( ) % 320;
   }
}
