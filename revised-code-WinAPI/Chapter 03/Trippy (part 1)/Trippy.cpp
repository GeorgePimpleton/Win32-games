#include "Trippy.h"

BOOL GameInitialize( HINSTANCE hInstance )
{
   g_pGame = new GameEngine( hInstance, TEXT( "Trippy" ), TEXT( "Trippy a: Learning to Draw Basic Graphics" ),
                             IDI_TRIPPY, IDI_TRIPPY_SM );

   if ( g_pGame == NULL )
   {
      return FALSE;
   }

   g_pGame->SetFrameRate( 15 );

   return TRUE;
}

void GameStart( HWND hWindow )
{
   srand( GetTickCount( ) );

   g_rcRectangle.left   = g_pGame->GetWidth(  ) * 2 / 5;
   g_rcRectangle.top    = g_pGame->GetHeight( ) * 2 / 5;
   g_rcRectangle.right  = g_rcRectangle.left  + g_pGame->GetWidth(  ) / 5;
   g_rcRectangle.bottom = g_rcRectangle.top   + g_pGame->GetHeight( ) / 5;
}

void GameEnd( )
{
   delete g_pGame;
}

void GameActivate( HWND hWindow )
{ }

void GameDeactivate( HWND hWindow )
{ }

void GamePaint( HDC hDC )
{
   // draw grid lines as a background for the rectangles
   const int iGridLines = 50;

   for ( int i = 1; i <= iGridLines; i++ )
   {
      // draw a horizontal grid line
      MoveToEx( hDC, g_pGame->GetWidth( ) * i / iGridLines, 0, NULL );
      LineTo( hDC, g_pGame->GetWidth( ) * i / iGridLines, g_pGame->GetHeight( ) );

      // Draw a vertical grid line
      MoveToEx( hDC, 0, g_pGame->GetHeight( ) * i / iGridLines, NULL );
      LineTo( hDC, g_pGame->GetWidth( ), g_pGame->GetHeight( ) * i / iGridLines );
   }
}

void GameCycle( )
{
   HDC    hDC;
   HWND   hWindow = g_pGame->GetWindow( );
   HBRUSH hBrush;

   // randomly alter the position and size of the rectangle
   int iInflation = ( rand( ) % 21 ) - 10;

   InflateRect( &g_rcRectangle, iInflation, iInflation );
   OffsetRect( &g_rcRectangle, ( rand( ) % 19 ) - 9, ( rand( ) % 19 ) - 9 );

   // draw the new rectangle in a random color
   hBrush = CreateSolidBrush( RGB( rand( ) % 256, rand( ) % 256, rand( ) % 256 ) );
   hDC    = GetDC( hWindow );

   FillRect( hDC, &g_rcRectangle, hBrush );
   ReleaseDC( hWindow, hDC );
   DeleteObject( hBrush );
}
