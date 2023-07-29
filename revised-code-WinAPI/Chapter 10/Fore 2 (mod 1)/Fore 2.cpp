// "Talk to me like I'm a 3 year old!" Programming Lessons - Windows Games

// Fore 2.cpp - Game Source

#include "Fore 2.hpp"

HRESULT GameInitialize(HINSTANCE hInstance)
{
   g_pGame = new GameEngine(hInstance, L"Fore 2", L"Managing a World Sprites",
                           IDI_ICON, IDI_ICON_SM, 600, 400);

   if (g_pGame == NULL)
   {
      return E_FAIL;
   }

   g_pGame->SetFrameRate(30);

   return S_OK;
}

void GameStart(HWND hWindow)
{
   rtk::srand();

   // create the offscreen device context and bitmap
   g_hOffscreenDC     = CreateCompatibleDC(GetDC(hWindow));
   g_hOffscreenBitmap = CreateCompatibleBitmap(GetDC(hWindow), g_pGame->GetWidth(), g_pGame->GetHeight());

   SelectObject(g_hOffscreenDC, g_hOffscreenBitmap);

   g_pForestBitmap   = new Bitmap(IDB_FOREST, GetModuleHandle(NULL));
   g_pGolfBallBitmap = new Bitmap(IDB_GOLFBALL, GetModuleHandle(NULL));

   // create the golf ball sprites
   RECT    rcBounds = { 0, 0, 600, 400 };
   Sprite* pSprite;

   pSprite = new Sprite(g_pGolfBallBitmap, rcBounds, BA_WRAP);
   pSprite->SetVelocity(5, 3);
   g_pGame->AddSprite(pSprite);

   pSprite = new Sprite(g_pGolfBallBitmap, rcBounds, BA_WRAP);
   pSprite->SetVelocity(3, 2);
   g_pGame->AddSprite(pSprite);

   rcBounds.left = 265; rcBounds.right = 500; rcBounds.bottom = 335;
   pSprite = new Sprite(g_pGolfBallBitmap, rcBounds, BA_BOUNCE);
   pSprite->SetVelocity(-6, 5);
   g_pGame->AddSprite(pSprite);

   rcBounds.right = 470;
   pSprite = new Sprite(g_pGolfBallBitmap, rcBounds, BA_BOUNCE);
   pSprite->SetVelocity(7, -3);
   g_pGame->AddSprite(pSprite);

   // set the initial drag info
   g_pDragSprite = NULL;
}

void GameEnd()
{
   // cleanup the offscreen device context and bitmap
   DeleteObject(g_hOffscreenBitmap);
   DeleteDC(g_hOffscreenDC);

   // cleanup the bitmaps
   delete g_pForestBitmap;
   delete g_pGolfBallBitmap;

   // cleanup the sprites
   g_pGame->CleanupSprites();

   // cleanup the game engine
   delete g_pGame;
}

void GameActivate(HWND hWindow)
{
}

void GameDeactivate(HWND hWindow)
{
}

void GamePaint(HDC hDC)
{
   // draw the background forest
   g_pForestBitmap->Draw(hDC, 0, 0);

   // draw the sprites
   g_pGame->DrawSprites(hDC);
}

void GameCycle()
{
   // update the sprites
   g_pGame->UpdateSprites();

   // obtain a device context for repainting the game
   HWND hWindow = g_pGame->GetWindow();
   HDC  hDC     = GetDC(hWindow);

   // paint the game to the offscreen device context
   GamePaint(g_hOffscreenDC);

   // blit the offscreen bitmap to the game screen
   BitBlt(hDC, 0, 0, g_pGame->GetWidth(), g_pGame->GetHeight(), g_hOffscreenDC, 0, 0, SRCCOPY);

   // cleanup
   ReleaseDC(hWindow, hDC);
}

void GameMenu(WPARAM wParam)
{
   switch (LOWORD(wParam))
   {
   case IDM_GAME_EXIT:
      GameEnd();
      PostQuitMessage(0);
      return;

   case IDM_HELP_ABOUT:
      DialogBoxW(g_pGame->GetInstance(), MAKEINTRESOURCEW(IDD_ABOUT), g_pGame->GetWindow(), (DLGPROC) DlgProc);
      return;
   }
}

void HandleKeys()
{
}

void MouseButtonDown(LONG x, LONG y, BOOL bLeft)
{
   // see if a ball was clicked with the left mouse button
   if (bLeft && (g_pDragSprite == NULL))
   {
      if ((g_pDragSprite = g_pGame->IsPointInSprite(x, y)) != NULL)
      {
         // capture the mouse
         SetCapture(g_pGame->GetWindow());

         // simulate a mouse move to get started
         MouseMove(x, y);
      }
   }
}

void MouseButtonUp(LONG x, LONG y, BOOL bLeft)
{
   // release the mouse
   ReleaseCapture();

   // stop dragging
   g_pDragSprite = NULL;
}

void MouseMove(LONG x, LONG y)
{
   if (g_pDragSprite != NULL)
   {
      // move the sprite to the mouse cursor position
      g_pDragSprite->SetPosition(x - (g_pDragSprite->GetWidth() / 2),
                                y - (g_pDragSprite->GetHeight() / 2));

      // force a repaint to redraw the sprites
      InvalidateRect(g_pGame->GetWindow(), NULL, FALSE);
   }
}

void HandleJoystick(JOYSTATE jsJoystickState)
{
}

BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee)
{
   // swap the sprite velocities so that they appear to bounce
   POINT ptSwapVelocity = pSpriteHitter->GetVelocity();

   pSpriteHitter->SetVelocity(pSpriteHittee->GetVelocity());
   pSpriteHittee->SetVelocity(ptSwapVelocity);

   return TRUE;
}