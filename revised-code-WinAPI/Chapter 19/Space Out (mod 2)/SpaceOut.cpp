#include "SpaceOut.hpp"

BOOL GameInitialize(HINSTANCE hInstance)
{
   g_pGame = new GameEngine(hInstance, TEXT("Space Out"),
                            TEXT("Space Out"), IDI_SPACEOUT, IDI_SPACEOUT_SM, 600, 450);
   if ( g_pGame == NULL )
      return FALSE;

   g_pGame->SetFrameRate(30);

   g_hInstance = hInstance;

   return TRUE;
}

void GameStart(HWND hWindow)
{
   srand(GetTickCount( ));

   g_hOffscreenDC = CreateCompatibleDC(GetDC(hWindow));
   g_hOffscreenBitmap = CreateCompatibleBitmap(GetDC(hWindow),
                                               g_pGame->GetWidth( ), g_pGame->GetHeight( ));
   SelectObject(g_hOffscreenDC, g_hOffscreenBitmap);

   HDC hDC = GetDC(hWindow);
   g_pDesertBitmap = new Bitmap(hDC, IDB_DESERT, g_hInstance);
   g_pCarBitmap = new Bitmap(hDC, IDB_CAR, g_hInstance);
   g_pSmCarBitmap = new Bitmap(hDC, IDB_SMCAR, g_hInstance);
   g_pMissileBitmap = new Bitmap(hDC, IDB_MISSILE, g_hInstance);
   g_pBlobboBitmap = new Bitmap(hDC, IDB_BLOBBO, g_hInstance);
   g_pBMissileBitmap = new Bitmap(hDC, IDB_BMISSILE, g_hInstance);
   g_pJellyBitmap = new Bitmap(hDC, IDB_JELLY, g_hInstance);
   g_pJMissileBitmap = new Bitmap(hDC, IDB_JMISSILE, g_hInstance);
   g_pTimmyBitmap = new Bitmap(hDC, IDB_TIMMY, g_hInstance);
   g_pTMissileBitmap = new Bitmap(hDC, IDB_TMISSILE, g_hInstance);
   g_pSmExplosionBitmap = new Bitmap(hDC, IDB_SMEXPLOSION, g_hInstance);
   g_pLgExplosionBitmap = new Bitmap(hDC, IDB_LGEXPLOSION, g_hInstance);
   g_pGameOverBitmap = new Bitmap(hDC, IDB_GAMEOVER, g_hInstance);

   g_pBackground = new StarryBackground(600, 450);

   g_pGame->PlayMIDISong(TEXT("Music.mid"));

   NewGame( );
}

void GameEnd( )
{
   g_pGame->CloseMIDIPlayer( );

   DeleteObject(g_hOffscreenBitmap);
   DeleteDC(g_hOffscreenDC);

   delete g_pDesertBitmap;
   delete g_pCarBitmap;
   delete g_pSmCarBitmap;
   delete g_pMissileBitmap;
   delete g_pBlobboBitmap;
   delete g_pBMissileBitmap;
   delete g_pJellyBitmap;
   delete g_pJMissileBitmap;
   delete g_pTimmyBitmap;
   delete g_pTMissileBitmap;
   delete g_pSmExplosionBitmap;
   delete g_pLgExplosionBitmap;
   delete g_pGameOverBitmap;

   delete g_pBackground;

   g_pGame->CleanupSprites( );

   delete g_pGame;
}

void GameActivate(HWND hWindow)
{
   g_pGame->PlayMIDISong(TEXT(""), FALSE);
}

void GameDeactivate(HWND hWindow)
{
   g_pGame->PauseMIDISong( );
}

void GamePaint(HDC hDC)
{
   g_pBackground->Draw(hDC);

  g_pDesertBitmap->Draw(hDC, 0, 371);

   g_pGame->DrawSprites(hDC);

   WCHAR szText[ 64 ];
   RECT  rect = { 460, 0, 510, 30 };
   wsprintf(szText, L"%d", g_iScore);
   SetBkMode(hDC, TRANSPARENT);
   SetTextColor(hDC, RGB(255, 255, 255));
   DrawText(hDC, szText, -1, &rect, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

   for ( int i = 0; i < g_iNumLives; i++ )
      g_pSmCarBitmap->Draw(hDC, 520 + (g_pSmCarBitmap->GetWidth( ) * i),
                           10, TRUE);

   if ( g_bGameOver )
      g_pGameOverBitmap->Draw(hDC, 190, 149, TRUE);
}

void GameCycle( )
{
   if ( !g_bGameOver )
   {
      // Randomly add aliens
      if ( (rand( ) % g_iDifficulty) == 0 )
         AddAlien( );

      g_pBackground->Update( );

      g_pGame->UpdateSprites( );

      HWND  hWindow = g_pGame->GetWindow( );
      HDC   hDC = GetDC(hWindow);

      GamePaint(g_hOffscreenDC);

      BitBlt(hDC, 0, 0, g_pGame->GetWidth( ), g_pGame->GetHeight( ),
             g_hOffscreenDC, 0, 0, SRCCOPY);

      ReleaseDC(hWindow, hDC);
   }
}

void HandleKeys( )
{
   if ( !g_bGameOver )
   {
      // move the car based upon left/right key presses
      POINT ptVelocity = g_pCarSprite->GetVelocity( );
      if ( GetAsyncKeyState(VK_LEFT) < 0 )
      {
         // move left
         ptVelocity.x = max(ptVelocity.x - 1, -4);
         g_pCarSprite->SetVelocity(ptVelocity);
      }
      else if ( GetAsyncKeyState(VK_RIGHT) < 0 )
      {
         // move right
         ptVelocity.x = min(ptVelocity.x + 2, 6);
         g_pCarSprite->SetVelocity(ptVelocity);
      }

      // fire missiles based upon spacebar presses
      if ( (++g_iFireInputDelay > 6) && GetAsyncKeyState(VK_SPACE) < 0 )
      {
         // create a new missile sprite
         RECT  rcBounds = { 0, 0, 600, 450 };
         RECT  rcPos = g_pCarSprite->GetPosition( );
         Sprite* pSprite = new Sprite(g_pMissileBitmap, rcBounds, BA_DIE);
         pSprite->SetPosition(rcPos.left + 15, 400);
         pSprite->SetVelocity(0, -7);
         g_pGame->AddSprite(pSprite);

         // play the missile (fire) sound
         PlaySound((PCWSTR) IDW_MISSILE, g_hInstance, SND_ASYNC |
                   SND_RESOURCE | SND_NOSTOP);

         // reset the input delay
         g_iFireInputDelay = 0;
      }
   }

   // start a new game based upon an Enter (Return) key press
   if ( g_bGameOver && (GetAsyncKeyState(VK_RETURN) < 0) )
      // start a new game
      NewGame( );
}

void MouseButtonDown(int x, int y, BOOL bLeft)
{ }

void MouseButtonUp(int x, int y, BOOL bLeft)
{ }

void MouseMove(int x, int y)
{ }

void HandleJoystick(JOYSTATE jsJoystickState)
{ }

BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee)
{
   // see if a player missile and an alien have collided
   Bitmap* pHitter = pSpriteHitter->GetBitmap( );
   Bitmap* pHittee = pSpriteHittee->GetBitmap( );
   if ( (pHitter == g_pMissileBitmap && (pHittee == g_pBlobboBitmap ||
                                         pHittee == g_pJellyBitmap || pHittee == g_pTimmyBitmap)) ||
       (pHittee == g_pMissileBitmap && (pHitter == g_pBlobboBitmap ||
                                        pHitter == g_pJellyBitmap || pHitter == g_pTimmyBitmap)) )
   {
      // play the small explosion sound
      PlaySound((PCWSTR) IDW_LGEXPLODE, g_hInstance, SND_ASYNC |
                SND_RESOURCE);

      // kill both sprites
      pSpriteHitter->Kill( );
      pSpriteHittee->Kill( );

      // create a large explosion sprite at the alien's position
      RECT rcBounds = { 0, 0, 600, 450 };
      RECT rcPos;
      if ( pHitter == g_pMissileBitmap )
         rcPos = pSpriteHittee->GetPosition( );
      else
         rcPos = pSpriteHitter->GetPosition( );
      Sprite* pSprite = new Sprite(g_pLgExplosionBitmap, rcBounds);
      pSprite->SetNumFrames(8, TRUE);
      pSprite->SetPosition(rcPos.left, rcPos.top);
      g_pGame->AddSprite(pSprite);

      // update the score
      g_iScore += 25;
      g_iDifficulty = max(80 - (g_iScore / 20), 20);
   }

   // see if an alien missile has collided with the car
   if ( (pHitter == g_pCarBitmap && (pHittee == g_pBMissileBitmap ||
                                     pHittee == g_pJMissileBitmap || pHittee == g_pTMissileBitmap)) ||
       (pHittee == g_pCarBitmap && (pHitter == g_pBMissileBitmap ||
                                    pHitter == g_pJMissileBitmap || pHitter == g_pTMissileBitmap)) )
   {
      // play the large explosion sound
      PlaySound((PCWSTR) IDW_LGEXPLODE, g_hInstance, SND_ASYNC |
                SND_RESOURCE);

      // kill the missile sprite
      if ( pHitter == g_pCarBitmap )
         pSpriteHittee->Kill( );
      else
         pSpriteHitter->Kill( );

      // create a large explosion sprite at the car's position
      RECT rcBounds = { 0, 0, 600, 480 };
      RECT rcPos;
      if ( pHitter == g_pCarBitmap )
         rcPos = pSpriteHitter->GetPosition( );
      else
         rcPos = pSpriteHittee->GetPosition( );
      Sprite* pSprite = new Sprite(g_pLgExplosionBitmap, rcBounds);
      pSprite->SetNumFrames(8, TRUE);
      pSprite->SetPosition(rcPos.left, rcPos.top);
      g_pGame->AddSprite(pSprite);

      // move the car back to the start
      g_pCarSprite->SetPosition(300, 405);

      // see if the game is over
      if ( --g_iNumLives == 0 )
      {
         // play the game over sound
         PlaySound((PCWSTR) IDW_GAMEOVER, g_hInstance, SND_ASYNC |
                   SND_RESOURCE);
         g_bGameOver = TRUE;
      }
   }

   return FALSE;
}

void SpriteDying(Sprite* pSpriteDying)
{
   // see if an alien missile sprite is dying
   if ( pSpriteDying->GetBitmap( ) == g_pBMissileBitmap ||
       pSpriteDying->GetBitmap( ) == g_pJMissileBitmap ||
       pSpriteDying->GetBitmap( ) == g_pTMissileBitmap )
   {
      // play the small explosion sound
      PlaySound((PCWSTR) IDW_SMEXPLODE, g_hInstance, SND_ASYNC |
                SND_RESOURCE | SND_NOSTOP);

      // create a small explosion sprite at the missile's position
      RECT rcBounds = { 0, 0, 600, 450 };
      RECT rcPos = pSpriteDying->GetPosition( );
      Sprite* pSprite = new Sprite(g_pSmExplosionBitmap, rcBounds);
      pSprite->SetNumFrames(8, TRUE);
      pSprite->SetPosition(rcPos.left, rcPos.top);
      g_pGame->AddSprite(pSprite);
   }
}

void NewGame( )
{
   g_pGame->CleanupSprites( );

   // create the car sprite
   RECT rcBounds = { 0, 0, 600, 450 };
   g_pCarSprite = new Sprite(g_pCarBitmap, rcBounds, BA_WRAP);
   g_pCarSprite->SetPosition(300, 405);
   g_pGame->AddSprite(g_pCarSprite);

   // initialize the game variables
   g_iFireInputDelay = 0;
   g_iScore = 0;
   g_iNumLives = 3;
   g_iDifficulty = 80;
   g_bGameOver = FALSE;

   // play the background music
   g_pGame->PlayMIDISong( );
}

void AddAlien( )
{
   // create a new random alien sprite
   RECT          rcBounds = { 0, 0, 600, 410 };
   AlienSprite* pSprite = { };
   switch ( rand( ) % 3 )
   {
   case 0:
      // Blobbo
      pSprite = new AlienSprite(g_pBlobboBitmap, rcBounds, BA_BOUNCE);
      pSprite->SetNumFrames(8);
      pSprite->SetPosition(((rand( ) % 2) == 0) ? 0 : 600, rand( ) % 370);
      pSprite->SetVelocity((rand( ) % 7) - 2, (rand( ) % 7) - 2);
      break;

   case 1:
      // Jelly
      pSprite = new AlienSprite(g_pJellyBitmap, rcBounds, BA_BOUNCE);
      pSprite->SetNumFrames(8);
      pSprite->SetPosition(rand( ) % 600, rand( ) % 370);
      pSprite->SetVelocity((rand( ) % 5) - 2, (rand( ) % 5) + 3);
      break;

   case 2:
      // Timmy
      pSprite = new AlienSprite(g_pTimmyBitmap, rcBounds, BA_WRAP);
      pSprite->SetNumFrames(8);
      pSprite->SetPosition(rand( ) % 600, rand( ) % 370);
      pSprite->SetVelocity((rand( ) % 7) + 3, 0);
      break;
   }

   // add the alien sprite
   g_pGame->AddSprite(pSprite);
}