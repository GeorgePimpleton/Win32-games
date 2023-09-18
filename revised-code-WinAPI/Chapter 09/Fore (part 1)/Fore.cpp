#include "Fore.h"

BOOL GameInitialize(HINSTANCE hInstance)
{
   _pGame = new GameEngine(hInstance, TEXT("Fore"),
                           TEXT("Fore: Making Things Move with Sprites"), IDI_FORE, IDI_FORE_SM, 600, 400);
   if ( _pGame == NULL )
      return FALSE;

   _pGame->SetFrameRate(30);

   _hInstance = hInstance;

   return TRUE;
}

void GameStart(HWND hWindow)
{
   srand(GetTickCount( ));

   HDC hDC = GetDC(hWindow);
   _pForestBitmap = new Bitmap(hDC, IDB_FOREST, _hInstance);
   _pGolfBallBitmap = new Bitmap(hDC, IDB_GOLFBALL, _hInstance);

   RECT rcBounds = { 0, 0, 600, 400 };
   _pGolfBallSprite[ 0 ] = new Sprite(_pGolfBallBitmap, rcBounds);
   _pGolfBallSprite[ 1 ] = new Sprite(_pGolfBallBitmap, rcBounds, BA_WRAP);
   _pGolfBallSprite[ 2 ] = new Sprite(_pGolfBallBitmap, rcBounds, BA_BOUNCE);
   _pGolfBallSprite[ 0 ]->SetVelocity(2, 1);
   _pGolfBallSprite[ 1 ]->SetVelocity(3, -2);
   _pGolfBallSprite[ 2 ]->SetVelocity(7, 4);

   _bDragging = FALSE;
   _iDragBall = -1;
}

void GameEnd( )
{
   delete _pForestBitmap;
   delete _pGolfBallBitmap;

   for ( int i = 0; i < 3; i++ )
      delete _pGolfBallSprite[ i ];

   delete _pGame;
}

void GameActivate(HWND hWindow)
{ }

void GameDeactivate(HWND hWindow)
{ }

void GamePaint(HDC hDC)
{
   _pForestBitmap->Draw(hDC, 0, 0);

   for ( int i = 0; i < 3; i++ )
      _pGolfBallSprite[ i ]->Draw(hDC);
}

void GameCycle( )
{
   for ( int i = 0; i < 3; i++ )
      _pGolfBallSprite[ i ]->Update( );

   InvalidateRect(_pGame->GetWindow( ), NULL, FALSE);
}

void HandleKeys( )
{ }

void MouseButtonDown(int x, int y, BOOL bLeft)
{
   // See if a ball was clicked with the left mouse button
   if ( bLeft && !_bDragging )
   {
      for ( int i = 0; i < 3; i++ )
         if ( _pGolfBallSprite[ i ]->IsPointInside(x, y) )
         {
            // Capture the mouse
            SetCapture(_pGame->GetWindow( ));

            // Set the drag state and the drag ball
            _bDragging = TRUE;
            _iDragBall = i;

            // Simulate a mouse move to get started
            MouseMove(x, y);

            // Don't check for more balls
            break;
         }
   }
}

void MouseButtonUp(int x, int y, BOOL bLeft)
{
   ReleaseCapture( );

   _bDragging = FALSE;
}

void MouseMove(int x, int y)
{
   if ( _bDragging )
   {
      _pGolfBallSprite[ _iDragBall ]->SetPosition(
         x - (_pGolfBallBitmap->GetWidth( ) / 2),
         y - (_pGolfBallBitmap->GetHeight( ) / 2));

      // Force a repaint to redraw the golf balls
      InvalidateRect(_pGame->GetWindow( ), NULL, FALSE);
   }
}

void HandleJoystick(JOYSTATE jsJoystickState)
{ }