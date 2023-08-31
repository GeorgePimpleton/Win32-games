#include "Fore.hpp"

HRESULT GameInitialize(HINSTANCE inst)
{
   g_game = std::make_unique<GameEngine>(inst, L"Fore", L"Making Things Move with Sprites",
                                         IDI_ICON, IDI_ICON_SM, 600, 400);

   if ( nullptr == g_game )
   {
      return E_FAIL;
   }

   g_game->SetFrameRate(30);

   return S_OK;
}

void GameStart(HWND wnd)
{
   rtk::srand( );

   g_forestBitmap   = std::make_unique<Bitmap>(IDB_FOREST, GetModuleHandle(nullptr));
   g_golfBallBitmap = std::make_unique<Bitmap>(IDB_GOLFBALL, GetModuleHandle(nullptr));

   // create the golf ball sprites
   RECT rcBounds { 0, 0, 600, 400 };

   g_golfBallSprite[ 0 ] = std::make_unique<Sprite>(g_golfBallBitmap.get( ), rcBounds);
   g_golfBallSprite[ 0 ]->SetVelocity(2, 1);

   g_golfBallSprite[ 1 ] = std::make_unique<Sprite>(g_golfBallBitmap.get( ), rcBounds, BA_WRAP);
   g_golfBallSprite[ 1 ]->SetVelocity(3, -2);

   g_golfBallSprite[ 2 ] = std::make_unique<Sprite>(g_golfBallBitmap.get( ), rcBounds, BA_BOUNCE);
   g_golfBallSprite[ 2 ]->SetVelocity(7, 4);

   // set the initial drag info
   g_dragging = FALSE;
   g_dragBall = -1;
}

void GameEnd( )
{
   // cleanup the bitmaps
   //delete g_pForestBitmap;
   //delete g_pGolfBallBitmap;

   //// cleanup the sprites
   //for (int i = 0; i < 3; i++)
   //{
   //   delete g_pGolfBallSprite[i];
   //}

   //// cleanup the game engine
   //delete g_pGame;

   // smart pointers take care of the cleanup!
}

void GameActivate(HWND wnd)
{ }

void GameDeactivate(HWND wnd)
{ }

void GamePaint(HDC dc)
{
   // draw the background forest
   g_forestBitmap->Draw(dc, 0, 0);

   // draw the golf ball sprites
   for ( int i = 0; i < 3; i++ )
   {
      g_golfBallSprite[ i ]->Draw(dc);
   }
}

void GameCycle( )
{
   // update the golf ball sprites
   for ( int i = 0; i < 3; i++ )
   {
      g_golfBallSprite[ i ]->Update( );
   }

   // force a repaint to redraw the golf balls
   InvalidateRect(g_game->GetWindow( ), nullptr, FALSE);
}

void GameMenu(WPARAM wParam)
{
   switch ( LOWORD(wParam) )
   {
   case IDM_GAME_EXIT:
      GameEnd( );
      PostQuitMessage(0);
      return;

   case IDM_HELP_ABOUT:
      DialogBoxW(g_game->GetInstance( ), MAKEINTRESOURCEW(IDD_ABOUT), g_game->GetWindow( ), (DLGPROC) DlgProc);
      return;
   }
}

void HandleKeys( )
{ }

void MouseButtonDown(LONG x, LONG y, BOOL left)
{
   // see if a ball was clicked with the left mouse button
   if ( left && !g_dragging )
   {
      for ( int i { }; i < 3; i++ )
      {
         if ( g_golfBallSprite[ i ]->IsPointInside(x, y) )
         {
            // capture the mouse
            SetCapture(g_game->GetWindow( ));

            // set the drag state and the drag ball
            g_dragging = TRUE;
            g_dragBall = i;

            // simulate a mouse move to get started
            MouseMove(x, y);

            // don't check for more balls
            break;
         }
      }
   }
}

void MouseButtonUp(LONG x, LONG y, BOOL left)
{
   // release the mouse
   ReleaseCapture( );

   // stop dragging
   g_dragging = FALSE;
}

void MouseMove(LONG x, LONG y)
{
   if ( g_dragging )
   {
      // move the sprite to the mouse cursor position
      g_golfBallSprite[ g_dragBall ]->SetPosition(x - (g_golfBallBitmap->GetWidth( ) / 2),
                                                  y - (g_golfBallBitmap->GetHeight( ) / 2));

      // force a repaint to redraw the golf balls
      InvalidateRect(g_game->GetWindow( ), nullptr, FALSE);
   }
}

void HandleJoystick(JOYSTATE jsJoystickState)
{ }