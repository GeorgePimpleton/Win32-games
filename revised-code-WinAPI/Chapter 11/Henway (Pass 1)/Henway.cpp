// "Talk to me like I'm a 3 year old!" Programming Lessons - Windows Games

// Henway.cpp - Henway Source

#include "Henway.hpp"

HRESULT GameInitialize(HINSTANCE hInstance)
{
   _pGame = new GameEngine(hInstance, L"Henway", L"Example Game: Henway",
                           IDI_ICON, IDI_ICON_SM, 465, 400);

   if (_pGame == NULL)
   {
      return E_FAIL;
   }

   _pGame->SetFrameRate(30);

   _pGame->InitJoystick();

   return S_OK;
}

void GameStart(HWND hWindow)
{
   rtk::srand();

   _hOffscreenDC     = CreateCompatibleDC(GetDC(hWindow));
   _hOffscreenBitmap = CreateCompatibleBitmap(GetDC(hWindow), _pGame->GetWidth(), _pGame->GetHeight());

   SelectObject(_hOffscreenDC, _hOffscreenBitmap);

   HINSTANCE hInstance = GetModuleHandleW(NULL);

   _pHighwayBitmap     = new Bitmap(IDB_HIGHWAY, hInstance);
   _pChickenBitmap     = new Bitmap(IDB_CHICKEN, hInstance);
   _pCarBitmaps[0]     = new Bitmap(IDB_CAR1, hInstance);
   _pCarBitmaps[1]     = new Bitmap(IDB_CAR2, hInstance);
   _pCarBitmaps[2]     = new Bitmap(IDB_CAR3, hInstance);
   _pCarBitmaps[3]     = new Bitmap(IDB_CAR4, hInstance);
   _pChickenHeadBitmap = new Bitmap(IDB_CHICKENHEAD, hInstance);

   NewGame();
}

void GameEnd()
{
   DeleteObject(_hOffscreenBitmap);
   DeleteDC(_hOffscreenDC);

   delete _pHighwayBitmap;
   delete _pChickenBitmap;

   for (int i = 0; i < 4; i++)
   {
      delete _pCarBitmaps[i];
   }

   delete _pChickenHeadBitmap;

   _pGame->CleanupSprites();

   delete _pGame;
}

void GameActivate(HWND hWindow)
{
   _pGame->CaptureJoystick();
}

void GameDeactivate(HWND hWindow)
{
   _pGame->ReleaseJoystick();
}

void GamePaint(HDC hDC)
{
   // draw the background highway
   _pHighwayBitmap->Draw(hDC, 0, 0);

   // draw the sprites
   _pGame->DrawSprites(hDC);

   // draw the number of remaining chicken lives
   for (UINT i = 0; i < _iNumLives; i++)
   {
      _pChickenHeadBitmap->Draw(hDC, 406 + (_pChickenHeadBitmap->GetWidth() * i), 382, TRUE);
   }
}

void GameCycle()
{
   if (_bGameOver == FALSE)
   {
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
}

void GameMenu(WPARAM wParam)
{
   switch (LOWORD(wParam))
   {
   case IDM_GAME_NEW:
      // starts a new game
      if (_bGameOver)
      {
         NewGame();
      }
      return;

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
   if (!_bGameOver && (++_iInputDelay > 2))
   {
      // move the chicken based upon key presses
      if (GetAsyncKeyState(VK_LEFT) < 0)
      {
         MoveChicken(-20, 0);
      }
      else if (GetAsyncKeyState(VK_RIGHT) < 0)
      {
         MoveChicken(20, 0);
      }

      if (GetAsyncKeyState(VK_UP) < 0)
      {
         MoveChicken(0, -20);
      }
      else if (GetAsyncKeyState(VK_DOWN) < 0)
      {
         MoveChicken(0, 20);
      }

      // reset the input delay
      _iInputDelay = 0;
   }
}

void MouseButtonDown(LONG x, LONG y, BOOL bLeft)
{
   // start a new game, if necessary
   if (_bGameOver)
   {
      NewGame();
   }
}

void MouseButtonUp(LONG x, LONG y, BOOL bLeft)
{
}

void MouseMove(LONG x, LONG y)
{
}

void HandleJoystick(JOYSTATE jsJoystickState)
{
   if (!_bGameOver && (++_iInputDelay > 2))
   {
      // check horizontal movement
      if (jsJoystickState & JOY_LEFT)
      {
         MoveChicken(-20, 0);
      }
      else if (jsJoystickState & JOY_RIGHT)
      {
         MoveChicken(20, 0);
      }

      // check vertical movement
      if (jsJoystickState & JOY_UP)
      {
         MoveChicken(0, -20);
      }
      else if (jsJoystickState & JOY_DOWN)
      {
         MoveChicken(0, 20);
      }

      // reset the input delay
      _iInputDelay = 0;
   }

   // check the joystick button and start a new game, if necessary
   if (_bGameOver && (jsJoystickState & JOY_FIRE1))
   {
      NewGame();
   }
}

BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee)
{
   // see if the chicken was hit
   if (pSpriteHittee == _pChickenSprite)
   {
      // move the chicken back to the start
      _pChickenSprite->SetPosition(4, 175);

      // see if the game is over
      if (--_iNumLives > 0)
      {
         MessageBoxW(_pGame->GetWindow(), L"Ouch!", L"Henway", MB_OK);
      }
      else
      {
         // display game over message
         WCHAR szText[64];
         wsprintfW(szText, L"Game Over! You scored %d points.", _iScore);
         MessageBoxW(_pGame->GetWindow(), szText, L"Henway", MB_OK);
         _bGameOver = TRUE;
      }

      return FALSE;
   }

   return TRUE;
}

// starts a new game
void NewGame()
{
   _pGame->CleanupSprites();

   // create the chicken and car sprites
   Sprite* pSprite;
   RECT    rcBounds = { 0, 0, 465, 400 };

   _pChickenSprite = new Sprite(_pChickenBitmap, rcBounds, BA_STOP);
   _pChickenSprite->SetPosition(4, 175);
   _pChickenSprite->SetVelocity(0, 0);
   _pChickenSprite->SetZOrder(1);
   _pGame->AddSprite(_pChickenSprite);

   pSprite = new Sprite(_pCarBitmaps[0], rcBounds, BA_WRAP);
   pSprite->SetPosition(70, 0);
   pSprite->SetVelocity(0, 7);
   pSprite->SetZOrder(2);
   _pGame->AddSprite(pSprite);

   pSprite = new Sprite(_pCarBitmaps[1], rcBounds, BA_WRAP);
   pSprite->SetPosition(160, 0);
   pSprite->SetVelocity(0, 3);
   pSprite->SetZOrder(2);
   _pGame->AddSprite(pSprite);

   pSprite = new Sprite(_pCarBitmaps[2], rcBounds, BA_WRAP);
   pSprite->SetPosition(239, 400);
   pSprite->SetVelocity(0, -5);
   pSprite->SetZOrder(2);
   _pGame->AddSprite(pSprite);

   pSprite = new Sprite(_pCarBitmaps[3], rcBounds, BA_WRAP);
   pSprite->SetPosition(329, 400);
   pSprite->SetVelocity(0, -10);
   pSprite->SetZOrder(2);
   _pGame->AddSprite(pSprite);

   // initialize the remaining global variables
   _iInputDelay = 0;
   _iNumLives   = 3;
   _iScore      = 0;
   _bGameOver   = FALSE;
}

void MoveChicken(int iXDistance, int iYDistance)
{
   // move the chicken to its new position
   _pChickenSprite->OffsetPosition(iXDistance, iYDistance);

   // see if the chicken made it across
   if (_pChickenSprite->GetPosition().left > 400)
   {
      // move the chicken back to the start and add to the score
      _pChickenSprite->SetPosition(4, 175);
      _iScore += 150;
      MessageBoxW(_pGame->GetWindow(), L"You made it!", L"Henway", MB_OK);
   }
}