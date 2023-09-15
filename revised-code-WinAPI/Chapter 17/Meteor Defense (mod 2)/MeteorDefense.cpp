#include "MeteorDefense.hpp"

BOOL GameInitialize(HINSTANCE hInstance)
{
   g_pGame = new GameEngine(hInstance, L"Meteor Defense", L"Example Game: Meteor Defense",
                            IDI_ICON, IDI_ICON_SM, 600, 450);

   if ( NULL == g_pGame )
   {
      return FALSE;
   }

   g_pGame->SetFrameRate(30);

   g_hInstance = hInstance;

   return TRUE;
}

void GameStart(HWND hWindow)
{
   srand((UINT)GetTickCount64( ));

   SetClassLongPtrW(hWindow, GCLP_HCURSOR, (LONG64) LoadImageW(GetModuleHandleW(NULL), MAKEINTRESOURCEW(IDC_CURSOR), IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR));

   g_hOffscreenDC     = CreateCompatibleDC(GetDC(hWindow));
   g_hOffscreenBitmap = CreateCompatibleBitmap(GetDC(hWindow), g_pGame-> GetWidth( ), g_pGame-> GetHeight( ));

   SelectObject(g_hOffscreenDC, g_hOffscreenBitmap);

   HDC hDC = GetDC(hWindow);

   g_pGroundBitmap    = new Bitmap(hDC, IDB_GROUND, g_hInstance);
   g_pCityBitmap      = new Bitmap(hDC, IDB_CITY, g_hInstance);
   g_pMeteorBitmap    = new Bitmap(hDC, IDB_METEOR, g_hInstance);
   g_pMissileBitmap   = new Bitmap(hDC, IDB_MISSILE, g_hInstance);
   g_pExplosionBitmap = new Bitmap(hDC, IDB_EXPLOSION, g_hInstance);
   g_pGameOverBitmap  = new Bitmap(hDC, IDB_GAMEOVER, g_hInstance);

   g_pBackground = new StarryBackground(600, 450);

   g_pGame->PlayMIDISong(L"Music.mid");

   NewGame( );
}

void GameEnd( )
{
   g_pGame->CloseMIDIPlayer( );

   DeleteObject(g_hOffscreenBitmap);
   DeleteDC(g_hOffscreenDC);

   delete g_pGroundBitmap;
   delete g_pCityBitmap;
   delete g_pMeteorBitmap;
   delete g_pMissileBitmap;
   delete g_pExplosionBitmap;
   delete g_pGameOverBitmap;

   delete g_pBackground;

   g_pGame->CleanupSprites( );

   delete g_pGame;
}

void GameActivate(HWND hWindow)
{
   g_pGame->PlayMIDISong(L"", FALSE);
}

void GameDeactivate(HWND hWindow)
{
   g_pGame->PauseMIDISong( );
}

void GamePaint(HDC hDC)
{
   // Draw the background
   g_pBackground->Draw(hDC);

   // Draw the ground bitmap
   g_pGroundBitmap->Draw(hDC, 0, 398, TRUE);

   // Draw the sprites
   g_pGame->DrawSprites(hDC);

   // Draw the score
   TCHAR szText[ 64 ];
   RECT  rect = { 275, 0, 325, 50 };

   wsprintfW(szText, L"%d", g_iScore);

   SetBkMode(hDC, TRANSPARENT);
   SetTextColor(hDC, RGB(255, 255, 255));

   DrawTextW(hDC, szText, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

   // Draw the game over message, if necessary
   if ( g_bGameOver )
   {
      g_pGameOverBitmap->Draw(hDC, 170, 150, TRUE);
   }
}

void GameCycle( )
{
   if ( !g_bGameOver )
   {
      // Randomly add meteors
      if ( (rand( ) % g_iDifficulty) == 0 )
      {
         AddMeteor( );
      }

      // Update the background
      g_pBackground->Update( );

      // Update the sprites
      g_pGame->UpdateSprites( );

      // Obtain a device context for repainting the game
      HWND hWindow = g_pGame-> GetWindow( );
      HDC  hDC     = GetDC(hWindow);

      // Paint the game to the offscreen device context
      GamePaint(g_hOffscreenDC);

      // Blit the offscreen bitmap to the game screen
      BitBlt(hDC, 0, 0, g_pGame->GetWidth( ), g_pGame->GetHeight( ),
             g_hOffscreenDC, 0, 0, SRCCOPY);

      // Cleanup
      ReleaseDC(hWindow, hDC);
   }
}

void HandleKeys( )
{
}

void MouseButtonDown(int x, int y, BOOL bLeft)
{
   if ( !g_bGameOver && bLeft )
   {
      // Create a new missile sprite and set its position
      RECT rcBounds = { 0, 0, 600, 450 };
      int  iXPos    = (x < 300) ? 144 : 449;

      Sprite* pSprite = new Sprite(g_pMissileBitmap, rcBounds, BA_DIE);

      pSprite->SetPosition(iXPos, 365);

      // Calculate the velocity so that it is aimed at the target
      int iXVel;
      int iYVel = -6;

      y     = min(y, 300);
      iXVel = (iYVel * ((iXPos + 8) - x)) / (365 - y);

      pSprite->SetVelocity(iXVel, iYVel);

      // Add the missile sprite
      g_pGame->AddSprite(pSprite);

      // Play the fire sound
      PlaySoundW((PCTSTR) IDW_FIRE, g_hInstance, SND_ASYNC | SND_RESOURCE | SND_NOSTOP);

      // Update the score
      g_iScore = max(--g_iScore, 0);
   }
   else if ( g_bGameOver && !bLeft )
      // Start a new game
      NewGame( );
}

void MouseButtonUp(int x, int y, BOOL bLeft)
{ }

void MouseMove(int x, int y)
{ }

void HandleJoystick(JOYSTATE jsJoystickState)
{ }

BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee)
{
   // See if a missile and a meteor have collided
   if ( (pSpriteHitter->GetBitmap( ) == g_pMissileBitmap &&
         pSpriteHittee->GetBitmap( ) == g_pMeteorBitmap) ||
        (pSpriteHitter->GetBitmap( ) == g_pMeteorBitmap &&
         pSpriteHittee->GetBitmap( ) == g_pMissileBitmap) )
   {
      // Kill both sprites
      pSpriteHitter->Kill( );
      pSpriteHittee->Kill( );

      // Update the score
      g_iScore      += 6;
      g_iDifficulty  = max(50 - (g_iScore / 10), 5);
   }

   // See if a meteor has collided with a city
   if ( pSpriteHitter->GetBitmap( ) == g_pMeteorBitmap &&
        pSpriteHittee->GetBitmap( ) == g_pCityBitmap )
   {
      // Play the big explosion sound
      PlaySoundW((PCTSTR) IDW_BIGEXPLODE, g_hInstance, SND_ASYNC | SND_RESOURCE);

      // Kill both sprites
      pSpriteHitter->Kill( );
      pSpriteHittee->Kill( );

      // See if the game is over
      if ( --g_iNumCities == 0 )
      {
         g_bGameOver = TRUE;
      }
   }

   return FALSE;
}

void SpriteDying(Sprite* pSpriteDying)
{
   // See if a meteor sprite is dying
   if ( pSpriteDying->GetBitmap( ) == g_pMeteorBitmap )
   {
      // Play the explosion sound
      PlaySoundW((PCTSTR) IDW_EXPLODE, g_hInstance, SND_ASYNC | SND_RESOURCE | SND_NOSTOP);

      // Create an explosion sprite at the meteor's position
      RECT rcBounds = { 0, 0, 600, 450 };
      RECT rcPos    = pSpriteDying-> GetPosition( );

      Sprite* pSprite = new Sprite(g_pExplosionBitmap, rcBounds);

      pSprite->SetNumFrames(12, TRUE);
      pSprite->SetPosition(rcPos.left, rcPos.top);
      g_pGame->AddSprite(pSprite);
   }
}

void NewGame( )
{
   // Clear the sprites
   g_pGame->CleanupSprites( );

   RECT rcBounds = { 0, 0, 600, 450 };

   // Create the city sprites
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

   // Initialize the game variables
   g_iScore      = 0;
   g_iNumCities  = 4;
   g_iDifficulty = 50;
   g_bGameOver   = FALSE;

   // Play the background music
   g_pGame->PlayMIDISong( );
}

void AddMeteor( )
{
   // Create a new meteor sprite and set its position
   RECT    rcBounds = { 0, 0, 600, 390 };
   int     iXPos = rand( ) % 600;
   Sprite* pSprite = new Sprite(g_pMeteorBitmap, rcBounds, BA_DIE);
   pSprite->SetNumFrames(14);
   pSprite->SetPosition(iXPos, 0);

   // Calculate the velocity so that it is aimed at one of the cities
   int iXVel = 0;
   int iYVel = (rand( ) % 4) + 3;

   switch ( rand( ) % 4 )
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

   // Add the meteor sprite
   g_pGame->AddSprite(pSprite);
}