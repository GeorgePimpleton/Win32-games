// "Talk to me like I'm a 3 year old!" Programming Lessons - Windows Games

// Roids.cpp - Game Source

#include "Roids.hpp"

HRESULT GameInitialize(HINSTANCE hInstance)
{
   _pGame = new GameEngine(hInstance, L"Roids", L"Creating Backgrounds for Your Sprites",
                           IDI_ICON, IDI_ICON_SM, 500, 400);

   if (_pGame == NULL)
   {
      return E_FAIL;
   }

   _pGame->SetFrameRate(30);

   return S_OK;
}

void GameStart(HWND hWindow)
{
   rtk::srand();

   _hOffscreenDC     = CreateCompatibleDC(GetDC(hWindow));
   _hOffscreenBitmap = CreateCompatibleBitmap(GetDC(hWindow), _pGame->GetWidth(), _pGame->GetHeight());

   SelectObject(_hOffscreenDC, _hOffscreenBitmap);

   HINSTANCE hInstance = GetModuleHandleW(NULL);

   _pAsteroidBitmap = new Bitmap(IDB_ASTEROID, hInstance);

   // create the starry background
   _pBackground = new StarryBackground(500, 400);

   // create the asteroid sprites
   RECT    rcBounds = { 0, 0, 500, 400 };
   Sprite* pSprite;

   pSprite = new Sprite(_pAsteroidBitmap, rcBounds, BA_WRAP);
   pSprite->SetNumFrames(14);
   pSprite->SetFrameDelay(1);
   pSprite->SetPosition(250, 200);
   pSprite->SetVelocity(-3, 1);
   _pGame->AddSprite(pSprite);

   pSprite = new Sprite(_pAsteroidBitmap, rcBounds, BA_WRAP);
   pSprite->SetNumFrames(14);
   pSprite->SetFrameDelay(2);
   pSprite->SetPosition(250, 200);
   pSprite->SetVelocity(3, -2);
   _pGame->AddSprite(pSprite);

   pSprite = new Sprite(_pAsteroidBitmap, rcBounds, BA_WRAP);
   pSprite->SetNumFrames(14);
   pSprite->SetFrameDelay(3);
   pSprite->SetPosition(250, 200);
   pSprite->SetVelocity(-2, -4);
   _pGame->AddSprite(pSprite);

   pSprite = new Sprite(_pAsteroidBitmap, rcBounds, BA_WRAP);
   pSprite->SetNumFrames(14);
   pSprite->SetFrameDelay(2);
   pSprite->SetPosition(150, 200);
   pSprite->SetVelocity(3, 4);
   _pGame->AddSprite(pSprite);
}

void GameEnd()
{
   // cleanup the offscreen device context and bitmap
   DeleteObject(_hOffscreenBitmap);
   DeleteDC(_hOffscreenDC);

   // cleanup the asteroid bitmap
   delete _pAsteroidBitmap;

   // cleanup the background
   delete _pBackground;

   // cleanup the sprites
   _pGame->CleanupSprites();

   // cleanup the game engine
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
   // draw the background
   _pBackground->Draw(hDC);

   // draw the sprites
   _pGame->DrawSprites(hDC);
}

void GameCycle()
{
   // update the background
   _pBackground->Update();

   // update the sprites
   _pGame->UpdateSprites();

   // obtain a device context for repainting the game
   HWND hWindow = _pGame->GetWindow();
   HDC  hDC     = GetDC(hWindow);

   // paint the game to the offscreen device context
   GamePaint(_hOffscreenDC);

   // blit the offscreen bitmap to the game screen
   BitBlt(hDC, 0, 0, _pGame->GetWidth(), _pGame->GetHeight(), _hOffscreenDC, 0, 0, SRCCOPY);

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
      DialogBoxW(_pGame->GetInstance(), MAKEINTRESOURCEW(IDD_ABOUT), _pGame->GetWindow(), (DLGPROC) DlgProc);
      return;
   }
}

void HandleKeys()
{
}

void MouseButtonDown(LONG x, LONG y, BOOL bLeft)
{
}

void MouseButtonUp(LONG x, LONG y, BOOL bLeft)
{
}

void MouseMove(LONG x, LONG y)
{
}

void HandleJoystick(JOYSTATE jsJoystickState)
{
}

BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee)
{
   return FALSE;
}

void NewGame()
{
}