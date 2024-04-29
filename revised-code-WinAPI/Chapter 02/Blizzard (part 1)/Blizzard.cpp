#include "Blizzard.h"

BOOL GameInitialize( HINSTANCE hInstance )
{
   // create the game engine
   g_game = new GameEngine( hInstance, TEXT( "Blizzard" ), TEXT( "Blizzard a" ),
                            IDI_BLIZZARD, IDI_BLIZZARD_SM );

   if ( g_game == NULL )
   {
      return FALSE;
   }

   // set the frame rate
   g_game->SetFrameRate( 15 );

   return TRUE;
}

void GameStart( HWND hWindow )
{
   // seed the random number generator
   srand( GetTickCount( ) );
}

void GameEnd( )
{
   // cleanup the game engine
   delete g_game;
}

void GameActivate( HWND hWindow )
{
   HDC  hDC;
   RECT rect;

   // draw activation text on the game screen
   GetClientRect( hWindow, &rect );

   hDC = GetDC( hWindow );

   DrawText( hDC, TEXT( "Here comes the blizzard!" ), -1, &rect,
             DT_SINGLELINE | DT_CENTER | DT_VCENTER );

   ReleaseDC( hWindow, hDC );
}

void GameDeactivate( HWND hWindow )
{
   HDC  hDC;
   RECT rect;

   // draw deactivation text on the game screen
   GetClientRect( hWindow, &rect );

   hDC = GetDC( hWindow );

   DrawText( hDC, TEXT( "The blizzard has passed." ), -1, &rect,
             DT_SINGLELINE | DT_CENTER | DT_VCENTER );

   ReleaseDC( hWindow, hDC );
}

void GamePaint( HDC hDC )
{ }

void GameCycle( )
{
   HDC  hDC;
   HWND hWindow = g_game->GetWindow( );

   // draw the snowflake icon at random positions on the game screen
   hDC = GetDC( hWindow );

   DrawIcon( hDC, rand( ) % g_game->GetWidth( ), rand( ) % g_game->GetHeight( ),
             ( HICON ) GetClassLongPtr( hWindow, GCLP_HICON ) );

   ReleaseDC( hWindow, hDC );
}
