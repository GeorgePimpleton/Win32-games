// "Talk to me like I'm a 3 year old!" Programming Lessons - Windows Games

// Battle Office 2.cpp - Game Source

#include "Battle Office 2.hpp"

HRESULT GameInitialize(HINSTANCE hInstance)
{
   _pGame = new GameEngine(hInstance, L"Battle Office 2", L"Animating the Appearance of Sprites",
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

   // load the custom cursor
   SetClassLongPtrW(hWindow, GCLP_HCURSOR, (LONG64) LoadImageW(GetModuleHandleW(NULL), MAKEINTRESOURCEW(IDC_CURSOR), IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR));

   _hOffscreenDC     = CreateCompatibleDC(GetDC(hWindow));
   _hOffscreenBitmap = CreateCompatibleBitmap(GetDC(hWindow), _pGame->GetWidth(), _pGame->GetHeight());

   SelectObject(_hOffscreenDC, _hOffscreenBitmap);

   HINSTANCE hInstance = GetModuleHandle(NULL);

   _pOfficeBitmap   = new Bitmap(IDB_OFFICE,   hInstance);
   _pPowBitmap      = new Bitmap(IDB_POW,      hInstance);
   _pGuyBitmaps[0]  = new Bitmap(IDB_GUY1,     hInstance);
   _pGuyBitmaps[1]  = new Bitmap(IDB_GUY2,     hInstance);
   _pGuyBitmaps[2]  = new Bitmap(IDB_GUY3,     hInstance);
   _pGuyBitmaps[3]  = new Bitmap(IDB_GUY4,     hInstance);
   _pGuyBitmaps[4]  = new Bitmap(IDB_GUY5,     hInstance);
   _pSmallGuyBitmap = new Bitmap(IDB_SMALLGUY, hInstance);
   _pGameOverBitmap = new Bitmap(IDB_GAMEOVER, hInstance);

   _pGame->PlayMIDISong(L"Music.mid");

   NewGame();
}

void GameEnd()
{
   _pGame->CloseMIDIPlayer();

   DeleteObject(_hOffscreenBitmap);
   DeleteDC(_hOffscreenDC);

   delete _pOfficeBitmap;
   delete _pPowBitmap;

   for (int i = 0; i < 5; i++)
   {
      delete _pGuyBitmaps[i];
   }
   delete _pSmallGuyBitmap;
   delete _pGameOverBitmap;

   _pGame->CleanupSprites();

   delete _pGame;
}

void GameActivate(HWND hWindow)
{
   _pGame->PlayMIDISong(TEXT(""), FALSE);
}

void GameDeactivate(HWND hWindow)
{
   _pGame->PauseMIDISong();
}

void GamePaint(HDC hDC)
{
   _pOfficeBitmap->Draw(hDC, 0, 0);

   _pGame->DrawSprites(hDC);

   // draw the number of guys who were hit
   WCHAR szText[STR_LENGTH];
   RECT  rect = { 237, 360, 301, 390 };
   wsprintfW(szText, L"%d", _iHits);
   DrawTextW(hDC, szText, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

   // draw the number of guys who were missed (got away)
   for (int i = 0; i < _iMisses; i++)
   {
      _pSmallGuyBitmap->Draw(hDC, 389 + (_pSmallGuyBitmap->GetWidth() * i), 359, TRUE);
   }

   // draw the game over message, if necessary
   if (_bGameOver)
   {
      _pGameOverBitmap->Draw(hDC, 120, 110, TRUE);
   }
}

void GameCycle()
{
   if (!_bGameOver)
   {
      // randomly show and hide the guys
      for (int i = 0; i < 5; i++)
      {
         if (_pGuySprites[i]->IsHidden())
         {
            if (rtk::rand(0, 60) == 0)
            {
               // show the guy
               _pGuySprites[i]->SetHidden(FALSE);

               // start the countdown delay
               if (i == 3)
               {
                  // start the guy running left
                  _iGuyDelay[i] = 80;
                  _pGuySprites[i]->SetPosition(500, 10);
               }
               else if (i == 4)
               {
                  // start the guy running right
                  _iGuyDelay[i] = 45;
                  _pGuySprites[i]->SetPosition(260, 60);
               }
               else
               {
                  _iGuyDelay[i] = 20 + (rtk::rand(0, _iGuyMasterDelay));
               }
            }
         }
         else
         {
            if (--_iGuyDelay[i] == 0)
            {
               // play a sound for the guy getting away
               PlaySoundW((PCWSTR) IDW_TAUNT, GetModuleHandleW(NULL), SND_ASYNC | SND_RESOURCE);

               // hide the guy
               _pGuySprites[i]->SetHidden(TRUE);

               // increment the misses
               if (++_iMisses == 5)
               {
                  // play a sound for the game ending
                  PlaySoundW((PCWSTR) IDW_BOO, GetModuleHandleW(NULL), SND_ASYNC | SND_RESOURCE);

                  // end the game
                  for (int i = 0; i < 5; i++)
                  {
                     _pGuySprites[i]->SetHidden(TRUE);
                  }
                  _bGameOver = TRUE;

                  // pause the background music
                  _pGame->PauseMIDISong();
               }
            }
         }
      }
   }

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
   case IDM_GAME_NEW:
      if (_bGameOver == TRUE)
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
}

void MouseButtonDown(LONG x, LONG y, BOOL bLeft)
{
   // only check the left mouse button
   if (_bGameOver == FALSE && bLeft)
   {
      // temporarily hide the POW sprite
      _pPowSprite->SetHidden(TRUE);

      // see if a guy sprite was clicked
      Sprite* pSprite;

      if ((pSprite = _pGame->IsPointInSprite(x, y)) != NULL)
      {
         // play a sound for hitting the guy
         PlaySoundW((PCWSTR) IDW_WHACK, GetModuleHandleW(NULL), SND_ASYNC | SND_RESOURCE);

         // position and show the pow sprite
         _pPowSprite->SetPosition(x - (_pPowSprite->GetWidth() / 2), y - (_pPowSprite->GetHeight() / 2));
         _pPowSprite->SetHidden(FALSE);

         // hide the guy that was clicked
         pSprite->SetHidden(TRUE);

         // increment the hits and make the game harder, if necessary
         if ((++_iHits % 5) == 0)
         {
            if (--_iGuyMasterDelay == 0)
            {
               _iGuyMasterDelay = 1;
            }
         }
      }
   }
   else if (_bGameOver == TRUE && !bLeft)
   {
      // start a new game
      NewGame();

      // restart the background music
      _pGame->PlayMIDISong();
   }
}

void MouseButtonUp(LONG x, LONG y, BOOL bLeft)
{ }

void MouseMove(LONG x, LONG y)
{ }

void HandleJoystick(JOYSTATE jsJoystickState)
{
}

BOOL SpriteCollision(Sprite * pSpriteHitter, Sprite * pSpriteHittee)
{
   return FALSE;
}

void NewGame()
{
   _pGame->CleanupSprites();

   // create the POW and guy sprites
   RECT rcBounds = { 0, 0, 500, 400 };
   _pPowSprite   = new Sprite(_pPowBitmap, rcBounds, BA_STOP);
   _pPowSprite->SetZOrder(3);
   _pPowSprite->SetHidden(TRUE);
   _pGame->AddSprite(_pPowSprite);

   _pGuySprites[0] = new Sprite(_pGuyBitmaps[0], rcBounds);
   _pGuySprites[0]->SetPosition(92, 175);
   _pGuySprites[0]->SetZOrder(2);
   _pGuySprites[0]->SetHidden(TRUE);
   _pGame->AddSprite(_pGuySprites[0]);

   _pGuySprites[1] = new Sprite(_pGuyBitmaps[1], rcBounds);
   _pGuySprites[1]->SetPosition(301, 184);
   _pGuySprites[1]->SetZOrder(2);
   _pGuySprites[1]->SetHidden(TRUE);
   _pGame->AddSprite(_pGuySprites[1]);

   _pGuySprites[2] = new Sprite(_pGuyBitmaps[2], rcBounds);
   _pGuySprites[2]->SetPosition(394, 61);
   _pGuySprites[2]->SetZOrder(2);
   _pGuySprites[2]->SetHidden(TRUE);
   _pGame->AddSprite(_pGuySprites[2]);

   rcBounds.left = 340;
   _pGuySprites[3] = new Sprite(_pGuyBitmaps[3], rcBounds, BA_WRAP);
   _pGuySprites[3]->SetPosition(500, 10);
   _pGuySprites[3]->SetVelocity(-3, 0);
   _pGuySprites[3]->SetZOrder(1);
   _pGuySprites[3]->SetHidden(TRUE);
   _pGuySprites[3]->SetNumFrames(4);
   _pGame->AddSprite(_pGuySprites[3]);

   rcBounds.left = 385;
   _pGuySprites[4] = new Sprite(_pGuyBitmaps[4], rcBounds, BA_WRAP);
   _pGuySprites[4]->SetPosition(260, 60);
   _pGuySprites[4]->SetVelocity(5, 0);
   _pGuySprites[4]->SetZOrder(1);
   _pGuySprites[4]->SetHidden(TRUE);
   _pGuySprites[4]->SetNumFrames(4);
   _pGuySprites[4]->SetFrameDelay(5);
   _pGame->AddSprite(_pGuySprites[4]);

   // Initialize the remaining global variables
   _iGuyMasterDelay = 50;
   _iHits           = 0;
   _iMisses         = 0;
   _bGameOver       = FALSE;

   // restart the background music
   _pGame->PlayMIDISong();
}