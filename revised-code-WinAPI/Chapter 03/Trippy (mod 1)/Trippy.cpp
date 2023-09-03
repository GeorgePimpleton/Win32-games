//-----------------------------------------------------------------
// Trippy Application
// C++ Source - Trippy.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Trippy.h"

//-----------------------------------------------------------------
// Game Engine Functions
//-----------------------------------------------------------------
BOOL GameInitialize(HINSTANCE hInstance)
{
   // Create the game engine
   _pGame = new GameEngine(hInstance, TEXT("Trippy"), TEXT("Trippy"), IDI_TRIPPY, IDI_TRIPPY_SM);
   if ( _pGame == NULL )
      return FALSE;

   // Set the frame rate
   _pGame->SetFrameRate(15);

   return TRUE;
}

void GameStart(HWND hWindow)
{
   // Seed the random number generator
   srand(GetTickCount( ));

   // Set the position and size of the initial rectangle
   _rcRectangle.left = _pGame->GetWidth( ) * 2 / 5;
   _rcRectangle.top = _pGame->GetHeight( ) * 2 / 5;
   _rcRectangle.right = _rcRectangle.left + _pGame->GetWidth( ) / 5;
   _rcRectangle.bottom = _rcRectangle.top + _pGame->GetHeight( ) / 5;
}

void GameEnd( )
{
   // Cleanup the game engine
   delete _pGame;
}

void GameActivate(HWND hWindow)
{
}

void GameDeactivate(HWND hWindow)
{
}

void GamePaint(HDC hDC)
{
   // Draw grid lines as a background for the rectangles
   const int iGridLines = 50;
   for ( int i = 1; i <= iGridLines; i++ )
   {
      // Draw a horizontal grid line
      MoveToEx(hDC, _pGame->GetWidth( ) * i / iGridLines, 0, NULL);
      LineTo(hDC, _pGame->GetWidth( ) * i / iGridLines, _pGame->GetHeight( ));

      // Draw a vertical grid line
      MoveToEx(hDC, 0, _pGame->GetHeight( ) * i / iGridLines, NULL);
      LineTo(hDC, _pGame->GetWidth( ), _pGame->GetHeight( ) * i / iGridLines);
   }
}

void GameCycle( )
{
   HDC         hDC;
   HWND        hWindow = _pGame->GetWindow( );
   HBRUSH      hBrush;

   // Randomly alter the position and size of the rectangle
   int iInflation = (rand( ) % 21) - 10;
   InflateRect(&_rcRectangle, iInflation, iInflation);
   OffsetRect(&_rcRectangle, (rand( ) % 19) - 9, (rand( ) % 19) - 9);

   // Draw the new rectangle in a random color
   hBrush = CreateSolidBrush(RGB(rand( ) % 256, rand( ) % 256, rand( ) % 256));
   hDC = GetDC(hWindow);
   FillRect(hDC, &_rcRectangle, hBrush);
   ReleaseDC(hWindow, hDC);
   DeleteObject(hBrush);
}
