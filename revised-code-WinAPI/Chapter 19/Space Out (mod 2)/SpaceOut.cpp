#include "SpaceOut.hpp"

BOOL GameInitialize(HINSTANCE hInstance)
{
   g_game = new GameEngine(hInstance, L"Space Out", L"Example Game: Space Out",
                            IDI_ICON, IDI_ICON_SM, 600, 450);

   if ( g_game == NULL )
   {
      return FALSE;
   }

   g_game->SetFrameRate(30);

   g_inst = hInstance;

   return TRUE;
}

void GameStart(HWND wnd)
{
   rtk::srand( );

   g_offscreenDC     = CreateCompatibleDC(GetDC(wnd));
   g_offscreenBitmap = CreateCompatibleBitmap(GetDC(wnd), g_game->GetWidth( ), g_game->GetHeight( ));

   SelectObject(g_offscreenDC, g_offscreenBitmap);

   HDC hDC = GetDC(wnd);

   g_desertBitmap         = new Bitmap(hDC, IDB_DESERT, g_inst);
   g_carBitmap            = new Bitmap(hDC, IDB_CAR, g_inst);
   g_smallCarBitmap       = new Bitmap(hDC, IDB_SMCAR, g_inst);
   g_missileBitmap        = new Bitmap(hDC, IDB_MISSILE, g_inst);
   g_BlobboBitmap         = new Bitmap(hDC, IDB_BLOBBO, g_inst);
   g_BMissileBitmap       = new Bitmap(hDC, IDB_BMISSILE, g_inst);
   g_JellyBitmap          = new Bitmap(hDC, IDB_JELLY, g_inst);
   g_JMissileBitmap       = new Bitmap(hDC, IDB_JMISSILE, g_inst);
   g_TimmyBitmap          = new Bitmap(hDC, IDB_TIMMY, g_inst);
   g_TMissileBitmap       = new Bitmap(hDC, IDB_TMISSILE, g_inst);
   g_smallExplosionBitmap = new Bitmap(hDC, IDB_SMEXPLOSION, g_inst);
   g_largeExplosionBitmap = new Bitmap(hDC, IDB_LGEXPLOSION, g_inst);
   g_gameOverBitmap       = new Bitmap(hDC, IDB_GAMEOVER, g_inst);

   g_background = new StarryBackground(600, 450);

   EnableMenuItem(GetMenu(g_game->GetWindow( )), (UINT) MAKEINTRESOURCEW(IDM_GAME_NEW), MF_GRAYED);

   g_game->PlayMIDISong(L"Music.mid");

   GameNew( );
}

void GameNew( )
{
   g_game->CleanupSprites( );

   RECT rcBounds = { 0, 0, 600, 450 };

   g_carSprite = new Sprite(g_carBitmap, rcBounds, BA_WRAP);
   g_carSprite->SetPosition(300, 405);
   g_game->AddSprite(g_carSprite);

   g_fireInputDelay = 0;
   g_score          = 0;
   g_numLives       = 3;
   g_difficulty     = 80;
   g_gameOver       = FALSE;

   g_game->PlayMIDISong( );
}

void GameEnd( )
{
   g_game->CloseMIDIPlayer( );

   DeleteObject(g_offscreenBitmap);
   DeleteDC(g_offscreenDC);

   delete g_desertBitmap;
   delete g_carBitmap;
   delete g_smallCarBitmap;
   delete g_missileBitmap;
   delete g_BlobboBitmap;
   delete g_BMissileBitmap;
   delete g_JellyBitmap;
   delete g_JMissileBitmap;
   delete g_TimmyBitmap;
   delete g_TMissileBitmap;
   delete g_smallExplosionBitmap;
   delete g_largeExplosionBitmap;
   delete g_gameOverBitmap;

   delete g_background;

   g_game->CleanupSprites( );

   delete g_game;
}

void GameActivate(HWND hWindow)
{
   g_game->PlayMIDISong(L"", FALSE);
}

void GameDeactivate(HWND hWindow)
{
   g_game->PauseMIDISong( );
}

void GamePaint(HDC dc)
{
   g_background->Draw(dc);

  g_desertBitmap->Draw(dc, 0, 371);

   g_game->DrawSprites(dc);

   WCHAR szText[ 64 ];
   RECT  rect = { 460, 0, 510, 30 };

   wsprintfW(szText, L"%d", g_score);
   SetBkMode(dc, TRANSPARENT);
   SetTextColor(dc, RGB(255, 255, 255));

   DrawTextW(dc, szText, -1, &rect, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

   for ( int i = 0; i < g_numLives; i++ )
   {
      g_smallCarBitmap->Draw(dc, 520 + (g_smallCarBitmap->GetWidth( ) * i), 10, TRUE);
   }

   if ( g_gameOver )
   {
      g_gameOverBitmap->Draw(dc, 190, 149, TRUE);
   }
}

void GameMenu(WPARAM wParam)
{
   switch ( LOWORD(wParam) )
   {
   case IDM_GAME_NEW:
      GameNew( );
      return;

   case IDM_GAME_EXIT:
      GameEnd( );
      PostQuitMessage(0);
      return;

   case IDM_HELP_ABOUT:
      DialogBoxW(g_game->GetInstance( ), MAKEINTRESOURCEW(IDD_ABOUT), g_game->GetWindow( ), (DLGPROC) DlgProc);
      return;
   }
}

void GameCycle( )
{
   if ( !g_gameOver )
   {
      // randomly add aliens
      if ( (rtk::rand(0, g_difficulty)) == 0 )
      {
         AddAlien( );
      }

      g_background->Update( );

      g_game->UpdateSprites( );

      HWND wnd = g_game-> GetWindow( );
      HDC  dc  = GetDC(wnd);

      GamePaint(g_offscreenDC);

      BitBlt(dc, 0, 0, g_game->GetWidth( ), g_game->GetHeight( ),
             g_offscreenDC, 0, 0, SRCCOPY);

      ReleaseDC(wnd, dc);
   }
}

void HandleKeys( )
{
   if ( !g_gameOver )
   {
      // move the car based upon left/right key presses
      POINT velocity = g_carSprite->GetVelocity( );

      if ( GetAsyncKeyState(VK_LEFT) < 0 )
      {
         // move left
         velocity.x = max(velocity.x - 1, -4);
         g_carSprite->SetVelocity(velocity);
      }
      else if ( GetAsyncKeyState(VK_RIGHT) < 0 )
      {
         // move right
         velocity.x = min(velocity.x + 2, 6);
         g_carSprite->SetVelocity(velocity);
      }

      // fire missiles based upon spacebar presses
      if ( (++g_fireInputDelay > 6) && GetAsyncKeyState(VK_SPACE) < 0 )
      {
         // create a new missile sprite
         RECT    bounds = { 0, 0, 600, 450 };
         RECT    pos    = g_carSprite-> GetPosition( );
         Sprite* sprite = new Sprite(g_missileBitmap, bounds, BA_DIE);
         sprite->SetPosition(pos.left + 15, 400);
         sprite->SetVelocity(0, -7);
         g_game->AddSprite(sprite);

         // play the missile (fire) sound
         PlaySoundW((PCWSTR) IDW_MISSILE, g_inst, SND_ASYNC | SND_RESOURCE | SND_NOSTOP);

         // reset the input delay
         g_fireInputDelay = 0;
      }
   }

   // start a new game based upon an Enter (Return) key press
   if ( g_gameOver && (GetAsyncKeyState(VK_RETURN) < 0) )
   {
      // start a new game
      GameNew( );
   }
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
   if ( (pHitter == g_missileBitmap && (pHittee == g_BlobboBitmap ||
                                         pHittee == g_JellyBitmap || pHittee == g_TimmyBitmap)) ||
       (pHittee == g_missileBitmap && (pHitter == g_BlobboBitmap ||
                                        pHitter == g_JellyBitmap || pHitter == g_TimmyBitmap)) )
   {
      // play the small explosion sound
      PlaySoundW((PCWSTR) IDW_LGEXPLODE, g_inst, SND_ASYNC | SND_RESOURCE);

      // kill both sprites
      pSpriteHitter->Kill( );
      pSpriteHittee->Kill( );

      // create a large explosion sprite at the alien's position
      RECT rcBounds = { 0, 0, 600, 450 };
      RECT rcPos;

      if ( pHitter == g_missileBitmap )
         rcPos = pSpriteHittee->GetPosition( );
      else
         rcPos = pSpriteHitter->GetPosition( );
      Sprite* pSprite = new Sprite(g_largeExplosionBitmap, rcBounds);
      pSprite->SetNumFrames(8, TRUE);
      pSprite->SetPosition(rcPos.left, rcPos.top);
      g_game->AddSprite(pSprite);

      // update the score
      g_score      += 25;
      g_difficulty  = max(80 - (g_score / 20), 20);
   }

   // see if an alien missile has collided with the car
   if ( (pHitter == g_carBitmap && (pHittee == g_BMissileBitmap ||
                                     pHittee == g_JMissileBitmap || pHittee == g_TMissileBitmap)) ||
       (pHittee == g_carBitmap && (pHitter == g_BMissileBitmap ||
                                    pHitter == g_JMissileBitmap || pHitter == g_TMissileBitmap)) )
   {
      // play the large explosion sound
      PlaySoundW((PCWSTR) IDW_LGEXPLODE, g_inst, SND_ASYNC | SND_RESOURCE);

      // kill the missile sprite
      if ( pHitter == g_carBitmap )
         pSpriteHittee->Kill( );
      else
         pSpriteHitter->Kill( );

      // create a large explosion sprite at the car's position
      RECT rcBounds = { 0, 0, 600, 480 };
      RECT rcPos;
      if ( pHitter == g_carBitmap )
         rcPos = pSpriteHitter->GetPosition( );
      else
         rcPos = pSpriteHittee->GetPosition( );
      Sprite* pSprite = new Sprite(g_largeExplosionBitmap, rcBounds);
      pSprite->SetNumFrames(8, TRUE);
      pSprite->SetPosition(rcPos.left, rcPos.top);
      g_game->AddSprite(pSprite);

      // move the car back to the start
      g_carSprite->SetPosition(300, 405);

      // see if the game is over
      if ( --g_numLives == 0 )
      {
         // play the game over sound
         PlaySoundW((PCWSTR) IDW_GAMEOVER, g_inst, SND_ASYNC | SND_RESOURCE);
         g_gameOver = TRUE;

         EnableMenuItem(GetMenu(g_game->GetWindow( )), (UINT) MAKEINTRESOURCEW(IDM_GAME_NEW), MF_ENABLED);
      }
   }

   return FALSE;
}

void SpriteDying(Sprite* spriteDying)
{
   // see if an alien missile sprite is dying
   if ( spriteDying->GetBitmap( ) == g_BMissileBitmap ||
        spriteDying->GetBitmap( ) == g_JMissileBitmap ||
        spriteDying->GetBitmap( ) == g_TMissileBitmap )
   {
      // play the small explosion sound
      PlaySound((PCWSTR) IDW_SMEXPLODE, g_inst, SND_ASYNC | SND_RESOURCE | SND_NOSTOP);

      // create a small explosion sprite at the missile's position
      RECT rcBounds = { 0, 0, 600, 450 };
      RECT rcPos = spriteDying->GetPosition( );
      Sprite* pSprite = new Sprite(g_smallExplosionBitmap, rcBounds);
      pSprite->SetNumFrames(8, TRUE);
      pSprite->SetPosition(rcPos.left, rcPos.top);
      g_game->AddSprite(pSprite);
   }
}

void AddAlien( )
{
   // create a new random alien sprite
   RECT          rcBounds = { 0, 0, 600, 410 };
   AlienSprite* pSprite = { };
   switch ( rtk::rand(0, 3) )
   {
   case 0:
      // Blobbo
      pSprite = new AlienSprite(g_BlobboBitmap, rcBounds, BA_BOUNCE);
      pSprite->SetNumFrames(8);
      pSprite->SetPosition(((rtk::rand(0, 2)) == 0) ? 0 : 600, rtk::rand(0, 370));
      pSprite->SetVelocity(rtk::rand(-2, 5), rtk::rand(-2, 5));
      break;

   case 1:
      // Jelly
      pSprite = new AlienSprite(g_JellyBitmap, rcBounds, BA_BOUNCE);
      pSprite->SetNumFrames(8);
      pSprite->SetPosition(rtk::rand(0, 600), rtk::rand(0, 370));
      pSprite->SetVelocity(rtk::rand(-2, 3), rtk::rand(3, 8));
      break;

   case 2:
      // Timmy
      pSprite = new AlienSprite(g_TimmyBitmap, rcBounds, BA_WRAP);
      pSprite->SetNumFrames(8);
      pSprite->SetPosition(rtk::rand(0, 600), rtk::rand(0, 370));
      pSprite->SetVelocity(rtk::rand(3, 10), 0);
      break;
   }

   // add the alien sprite
   g_game->AddSprite(pSprite);
}