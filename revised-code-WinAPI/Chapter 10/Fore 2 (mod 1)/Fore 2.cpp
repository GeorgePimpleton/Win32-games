#include "Fore 2.hpp"

HRESULT GameInitialize(HINSTANCE inst)
{
   g_game = new GameEngine(inst, L"Fore 2", L"Managing a World Sprites",
                           IDI_ICON, IDI_ICON_SM, 600, 400);

   if ( NULL == g_game )
   {
      return E_FAIL;
   }

   g_game->SetFrameRate(30);

   return S_OK;
}

void GameStart(HWND wnd)
{
   rtk::srand( );

   // create the offscreen device context and bitmap
   g_offscreenDC     = CreateCompatibleDC(GetDC(wnd));
   g_offscreenBitmap = CreateCompatibleBitmap(GetDC(wnd), g_game-> GetWidth( ), g_game-> GetHeight( ));

   SelectObject(g_offscreenDC, g_offscreenBitmap);

   g_forestBitmap   = new Bitmap(IDB_FOREST, GetModuleHandle(NULL));
   g_golfBallBitmap = new Bitmap(IDB_GOLFBALL, GetModuleHandle(NULL));

   // create the golf ball sprites
   RECT    bounds = { 0, 0, 600, 400 };
   Sprite* sprite;

   sprite = new Sprite(g_golfBallBitmap, bounds, BA_WRAP);
   sprite->SetVelocity(5, 3);
   g_game->AddSprite(sprite);

   sprite = new Sprite(g_golfBallBitmap, bounds, BA_WRAP);
   sprite->SetVelocity(3, 2);
   g_game->AddSprite(sprite);

   bounds.left = 265; bounds.right = 500; bounds.bottom = 335;
   sprite = new Sprite(g_golfBallBitmap, bounds, BA_BOUNCE);
   sprite->SetVelocity(-6, 5);
   g_game->AddSprite(sprite);

   bounds.right = 470;
   sprite = new Sprite(g_golfBallBitmap, bounds, BA_BOUNCE);
   sprite->SetVelocity(7, -3);
   g_game->AddSprite(sprite);

   // set the initial drag info
   g_dragSprite = NULL;
}

void GameEnd( )
{
   // cleanup the offscreen device context and bitmap
   DeleteObject(g_offscreenBitmap);
   DeleteDC(g_offscreenDC);

   // cleanup the bitmaps
   delete g_forestBitmap;
   delete g_golfBallBitmap;

   // cleanup the sprites
   g_game->CleanupSprites( );

   // cleanup the game engine
   delete g_game;
}

void GameActivate(HWND wnd)
{ }

void GameDeactivate(HWND wnd)
{ }

void GamePaint(HDC dc)
{
   // draw the background forest
   g_forestBitmap->Draw(dc, 0, 0);

   // draw the sprites
   g_game->DrawSprites(dc);
}

void GameCycle( )
{
   // update the sprites
   g_game->UpdateSprites( );

   // obtain a device context for repainting the game
   HWND wnd = g_game->GetWindow( );
   HDC  dc  = GetDC(wnd);

   // paint the game to the offscreen device context
   GamePaint(g_offscreenDC);

   // blit the offscreen bitmap to the game screen
   BitBlt(dc, 0, 0, g_game->GetWidth( ), g_game->GetHeight( ), g_offscreenDC, 0, 0, SRCCOPY);

   // cleanup
   ReleaseDC(wnd, dc);
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
   if ( left && (NULL == g_dragSprite) )
   {
      if ( (g_dragSprite = g_game->IsPointInSprite(x, y)) != NULL )
      {
         // capture the mouse
         SetCapture(g_game->GetWindow( ));

         // simulate a mouse move to get started
         MouseMove(x, y);
      }
   }
}

void MouseButtonUp(LONG x, LONG y, BOOL left)
{
   // release the mouse
   ReleaseCapture( );

   // stop dragging
   g_dragSprite = NULL;
}

void MouseMove(LONG x, LONG y)
{
   if ( g_dragSprite != NULL )
   {
      // move the sprite to the mouse cursor position
      g_dragSprite->SetPosition(x - (g_dragSprite->GetWidth( ) / 2),
                                y - (g_dragSprite->GetHeight( ) / 2));

      // force a repaint to redraw the sprites
      InvalidateRect(g_game->GetWindow( ), NULL, FALSE);
   }
}

void HandleJoystick(JOYSTATE jsJoystickState)
{ }

BOOL SpriteCollision(Sprite* spriteHitter, Sprite* spriteHittee)
{
   // swap the sprite velocities so that they appear to bounce
   POINT swapVelocity = spriteHitter->GetVelocity( );

   spriteHitter->SetVelocity(spriteHittee->GetVelocity( ));
   spriteHittee->SetVelocity(swapVelocity);

   return TRUE;
}