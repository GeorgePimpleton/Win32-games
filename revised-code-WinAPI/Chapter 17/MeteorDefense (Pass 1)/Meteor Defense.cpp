//-----------------------------------------------------------------
// Meteor Defense Application
// C++ Source - Meteor Defense.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// include files
//-----------------------------------------------------------------
#include "Meteor Defense.hpp"

//-----------------------------------------------------------------
// game engine functions
//-----------------------------------------------------------------
BOOL GameInitialize(HINSTANCE hInstance)
{
   // Create the game engine
   g_pGame = new GameEngine(hInstance, TEXT("Meteor Defense"), TEXT("Meteor Defense"),
                            IDI_METEORDEFENSE, IDI_METEORDEFENSE_SM, 600, 450);

   if (g_pGame == NULL)
   {
      return FALSE;
   }

   // set the frame rate
   g_pGame->SetFrameRate(30);

   // store the instance handle
   g_hInstance = hInstance;

   return TRUE;
}


void GameStart(HWND hWindow)
{
   // seed the random number generator
   rtk::srand();

   SetClassLongPtr(hWindow, GCLP_HCURSOR,
      (LONG64) LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_METEORDEFENSE), IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR));

   // create the offscreen device context and bitmap
   g_hOffscreenDC = CreateCompatibleDC(GetDC(hWindow));
   g_hOffscreenBitmap = CreateCompatibleBitmap(GetDC(hWindow), g_pGame->GetWidth(), g_pGame->GetHeight());

   SelectObject(g_hOffscreenDC, g_hOffscreenBitmap);

   // create and load the bitmaps
   g_pGroundBitmap    = new Bitmap(IDB_GROUND, g_hInstance);
   g_pCityBitmap      = new Bitmap(IDB_CITY, g_hInstance);
   g_pMeteorBitmap    = new Bitmap(IDB_METEOR, g_hInstance);
   g_pMissileBitmap   = new Bitmap(IDB_MISSILE, g_hInstance);
   g_pExplosionBitmap = new Bitmap(IDB_EXPLOSION, g_hInstance);
   g_pGameOverBitmap  = new Bitmap(IDB_GAMEOVER, g_hInstance);

   // create the starry background
   g_pBackground = new StarryBackground(600, 450);

   // play the background music
   g_pGame->PlayMIDISong(TEXT("Music.mid"));

   // start the game
   NewGame();
}


void GameEnd()
{
   // close the MIDI player for the background music
   g_pGame->CloseMIDIPlayer();

   // cleanup the offscreen device context and bitmap
   DeleteObject(g_hOffscreenBitmap);
   DeleteDC(g_hOffscreenDC);

   // cleanup the bitmaps
   delete g_pGroundBitmap;
   delete g_pCityBitmap;
   delete g_pMeteorBitmap;
   delete g_pMissileBitmap;
   delete g_pExplosionBitmap;
   delete g_pGameOverBitmap;

   // cleanup the background
   delete g_pBackground;

   // cleanup the sprites
   g_pGame->CleanupSprites();

   // cleanup the game engine
   delete g_pGame;
}


void GameActivate(HWND hWindow)
{
   // resume the background music
   g_pGame->PlayMIDISong(TEXT(""), FALSE);
}


void GameDeactivate(HWND hWindow)
{
   // pause the background music
   g_pGame->PauseMIDISong();
}


void GamePaint(HDC hDC)
{
   // draw the background
   g_pBackground->Draw(hDC);

   // draw the ground bitmap
   g_pGroundBitmap->Draw(hDC, 0, 398, TRUE);

   // draw the sprites
   g_pGame->DrawSprites(hDC);

   // draw the score
   TCHAR szText[64];
   RECT  rect = { 275, 0, 325, 50 };

   wsprintf(szText, TEXT("%d"), g_iScore);
   SetBkMode(hDC, TRANSPARENT);
   SetTextColor(hDC, RGB(255, 255, 255));
   DrawText(hDC, szText, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

   // draw the game over message, if necessary
   if (g_bGameOver)
   {
      g_pGameOverBitmap->Draw(hDC, 170, 150, TRUE);
   }
}


void GameCycle()
{
   if (!g_bGameOver)
   {
      // randomly add meteors
      if (rtk::rand(0, g_iDifficulty) == 0)
      {
         AddMeteor();
      }

      // update the background
      g_pBackground->Update();

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
}


void HandleKeys()
{ }


void MouseButtonDown(int x, int y, BOOL bLeft)
{
   if (!g_bGameOver && bLeft)
   {
      // create a new missile sprite and set its position
      RECT    rcBounds = { 0, 0, 600, 450 };
      int     iXPos = (x < 300) ? 144 : 449;
      Sprite* pSprite = new Sprite(g_pMissileBitmap, rcBounds, BA_DIE);

      pSprite->SetPosition(iXPos, 365);

      // calculate the velocity so that it is aimed at the target
      y = min(y, 300);

      int iYVel = -6;
      int iXVel = (iYVel * ((iXPos + 8) - x)) / (365 - y);

      pSprite->SetVelocity(iXVel, iYVel);

      // add the missile sprite
      g_pGame->AddSprite(pSprite);

      // play the fire sound
      PlaySound((LPCTSTR) IDW_FIRE, g_hInstance, SND_ASYNC | SND_RESOURCE | SND_NOSTOP);

      // update the score
      g_iScore = max(--g_iScore, 0);
   }
   else if (g_bGameOver && !bLeft)
   {
      // start a new game
      NewGame();
   }
}


void MouseButtonUp(int x, int y, BOOL bLeft)
{ }


void MouseMove(int x, int y)
{ }


void HandleJoystick(JOYSTATE jsJoystickState)
{ }


BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee)
{
   // see if a missile and a meteor have collided
   if ((pSpriteHitter->GetBitmap() == g_pMissileBitmap && pSpriteHittee->GetBitmap() == g_pMeteorBitmap) ||
        (pSpriteHitter->GetBitmap() == g_pMeteorBitmap && pSpriteHittee->GetBitmap() == g_pMissileBitmap))
   {
      // kill both sprites
      pSpriteHitter->Kill();
      pSpriteHittee->Kill();

      // update the score
      g_iScore += 6;
      g_iDifficulty = max(50 - (g_iScore / 10), 5);
   }

   // see if a meteor has collided with a city
   if (pSpriteHitter->GetBitmap() == g_pMeteorBitmap && pSpriteHittee->GetBitmap() == g_pCityBitmap)
   {
      // play the big explosion sound
      PlaySound((LPCTSTR) IDW_BIGEXPLODE, g_hInstance, SND_ASYNC | SND_RESOURCE);

      // kill both sprites
      pSpriteHitter->Kill();
      pSpriteHittee->Kill();

      // see if the game is over
      if (--g_iNumCities == 0)
      {
         g_bGameOver = TRUE;
      }
   }

   return FALSE;
}


void SpriteDying(Sprite* pSpriteDying)
{
   // see if a meteor sprite is dying
   if (pSpriteDying->GetBitmap() == g_pMeteorBitmap)
   {
      // play the explosion sound
      PlaySound((LPCTSTR) IDW_EXPLODE, g_hInstance, SND_ASYNC | SND_RESOURCE | SND_NOSTOP);

      // create an explosion sprite at the meteor's position
      RECT rcBounds = { 0, 0, 600, 450 };
      RECT rcPos    = pSpriteDying->GetPosition();

      Sprite* pSprite = new Sprite(g_pExplosionBitmap, rcBounds);
      pSprite->SetNumFrames(12, TRUE);
      pSprite->SetPosition(rcPos.left, rcPos.top);
      g_pGame->AddSprite(pSprite);
   }
}

//-----------------------------------------------------------------
// Functions
//-----------------------------------------------------------------
void NewGame()
{
   // Clear the sprites
   g_pGame->CleanupSprites();

   // Create the city sprites
   RECT rcBounds = { 0, 0, 600, 450 };

   Sprite* pSprite = new Sprite(g_pCityBitmap, rcBounds);
   pSprite->SetPosition(2, 370);
   g_pGame->AddSprite(pSprite);

   pSprite = new Sprite(g_pCityBitmap, rcBounds);
   pSprite->SetPosition(186, 370);
   g_pGame->AddSprite(pSprite);

   pSprite = new Sprite(g_pCityBitmap, rcBounds);
   pSprite->SetPosition(302, 370);
   g_pGame->AddSprite(pSprite);

   pSprite = new Sprite(g_pCityBitmap, rcBounds);
   pSprite->SetPosition(490, 370);
   g_pGame->AddSprite(pSprite);

   // initialize the game variables
   g_iScore      = 0;
   g_iNumCities  = 4;
   g_iDifficulty = 50;
   g_bGameOver   = FALSE;

   // play the background music
   g_pGame->PlayMIDISong();
}


void AddMeteor()
{
   // create a new meteor sprite and set its position
   RECT rcBounds = { 0, 0, 600, 390 };
   int  iXPos    = rtk::rand(0, 600);

   Sprite* pSprite = new Sprite(g_pMeteorBitmap, rcBounds, BA_DIE);
   pSprite->SetNumFrames(14);
   pSprite->SetPosition(iXPos, 0);

   // Calculate the velocity so that it is aimed at one of the cities
   int iXVel;
   int iYVel = rtk::rand(3, 6);

   switch (rtk::rand(0, 3))
   {
   case 0:
      iXVel = (iYVel * (56 - (iXPos + 50))) / 400;
      break;

   case 1:
      iXVel = (iYVel * (240 - (iXPos + 50))) / 400;
      break;

   case 2:
      iXVel = (iYVel * (360 - (iXPos + 50))) / 400;
      break;

   case 3:
      iXVel = (iYVel * (546 - (iXPos + 50))) / 400;
      break;
   }

   pSprite->SetVelocity(iXVel, iYVel);

   // add the meteor sprite
   g_pGame->AddSprite(pSprite);
}