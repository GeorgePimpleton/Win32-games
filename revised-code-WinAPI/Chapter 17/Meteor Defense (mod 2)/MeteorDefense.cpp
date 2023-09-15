#include "MeteorDefense.hpp"

BOOL GameInitialize(HINSTANCE inst)
{
   g_game = std::make_unique<GameEngine>(inst, L"Meteor Defense", L"Example Game: Meteor Defense",
                                         IDI_ICON, IDI_ICON_SM, 600, 450);

   if ( NULL == g_game )
   {
      return FALSE;
   }

   g_game->SetFrameRate(30);

   g_inst = inst;

   return TRUE;
}

void GameStart(HWND wnd)
{
   rtk::srand( );

   SetClassLongPtrW(wnd, GCLP_HCURSOR, (LONG64) LoadImageW(GetModuleHandleW(NULL), MAKEINTRESOURCEW(IDC_CURSOR), IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR));

   g_offscreenDC     = CreateCompatibleDC(GetDC(wnd));
   g_offscreenBitmap = CreateCompatibleBitmap(GetDC(wnd), g_game-> GetWidth( ), g_game-> GetHeight( ));

   SelectObject(g_offscreenDC, g_offscreenBitmap);

   HINSTANCE inst = GetModuleHandle(NULL);

   g_groundBitmap    = new Bitmap(IDB_GROUND, inst);
   g_cityBitmap      = new Bitmap(IDB_CITY, inst);
   g_meteorBitmap    = new Bitmap(IDB_METEOR, inst);
   g_missileBitmap   = new Bitmap(IDB_MISSILE, inst);
   g_explosionBitmap = new Bitmap(IDB_EXPLOSION, inst);
   g_gameOverBitmap  = new Bitmap(IDB_GAMEOVER, inst);

   g_background = new StarryBackground(600, 450);

   g_game->PlayMIDISong(L"Music.mid");

   GameNew( );
}

void GameEnd( )
{
   g_game->CloseMIDIPlayer( );

   DeleteObject(g_offscreenBitmap);
   DeleteDC(g_offscreenDC);

   delete g_groundBitmap;
   delete g_cityBitmap;
   delete g_meteorBitmap;
   delete g_missileBitmap;
   delete g_explosionBitmap;
   delete g_gameOverBitmap;

   delete g_background;

   g_game->CleanupSprites( );
}

void GameActivate(HWND hWindow)
{
   g_game->PlayMIDISong(L"", FALSE);
}

void GameDeactivate(HWND hWindow)
{
   g_game->PauseMIDISong( );
}

void GamePaint(HDC hDC)
{
   g_background->Draw(hDC);

   g_groundBitmap->Draw(hDC, 0, 398, TRUE);

   g_game->DrawSprites(hDC);

   WCHAR szText[ 64 ];
   RECT  rect = { 275, 0, 325, 50 };

   wsprintfW(szText, L"%d", g_score);

   SetBkMode(hDC, TRANSPARENT);
   SetTextColor(hDC, RGB(255, 255, 255));

   DrawTextW(hDC, szText, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

   if ( g_gameOver )
   {
      g_gameOverBitmap->Draw(hDC, 170, 150, TRUE);
   }
}

void GameCycle( )
{
   if ( !g_gameOver )
   {
      if ( 0 == (rtk::rand(0,  g_difficulty)) )
      {
         AddMeteor( );
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

void GameMenu(WPARAM wParam)
{
   switch ( LOWORD(wParam) )
   {
   case IDM_GAME_NEW:
      if ( g_gameOver )
      {
         GameNew( );
      }
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

void HandleKeys( )
{ }

void MouseButtonDown(int x, int y, BOOL bLeft)
{
   if ( !g_gameOver && bLeft )
   {
      // Create a new missile sprite and set its position
      RECT rcBounds = { 0, 0, 600, 450 };
      int  iXPos    = (x < 300) ? 144 : 449;

      Sprite* pSprite = new Sprite(g_missileBitmap, rcBounds, BA_DIE);

      pSprite->SetPosition(iXPos, 365);

      // Calculate the velocity so that it is aimed at the target
      int iXVel;
      int iYVel = -6;

      y     = min(y, 300);
      iXVel = (iYVel * ((iXPos + 8) - x)) / (365 - y);

      pSprite->SetVelocity(iXVel, iYVel);

      // Add the missile sprite
      g_game->AddSprite(pSprite);

      // Play the fire sound
      PlaySoundW((PCTSTR) IDW_FIRE, g_inst, SND_ASYNC | SND_RESOURCE | SND_NOSTOP);

      // Update the score
      g_score = max(--g_score, 0);
   }
   else if ( g_gameOver && !bLeft )
      // Start a new game
      GameNew( );
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
   if ( (pSpriteHitter->GetBitmap( ) == g_missileBitmap &&
         pSpriteHittee->GetBitmap( ) == g_meteorBitmap) ||
        (pSpriteHitter->GetBitmap( ) == g_meteorBitmap &&
         pSpriteHittee->GetBitmap( ) == g_missileBitmap) )
   {
      // Kill both sprites
      pSpriteHitter->Kill( );
      pSpriteHittee->Kill( );

      // Update the score
      g_score      += 6;
      g_difficulty  = max(50 - (g_score / 10), 5);
   }

   // See if a meteor has collided with a city
   if ( pSpriteHitter->GetBitmap( ) == g_meteorBitmap &&
        pSpriteHittee->GetBitmap( ) == g_cityBitmap )
   {
      // Play the big explosion sound
      PlaySoundW((PCTSTR) IDW_BIGEXPLODE, g_inst, SND_ASYNC | SND_RESOURCE);

      // Kill both sprites
      pSpriteHitter->Kill( );
      pSpriteHittee->Kill( );

      // See if the game is over
      if ( --g_numCities == 0 )
      {
         g_gameOver = TRUE;
      }
   }

   return FALSE;
}

void SpriteDying(Sprite* pSpriteDying)
{
   // See if a meteor sprite is dying
   if ( pSpriteDying->GetBitmap( ) == g_meteorBitmap )
   {
      // Play the explosion sound
      PlaySoundW((PCTSTR) IDW_EXPLODE, g_inst, SND_ASYNC | SND_RESOURCE | SND_NOSTOP);

      // Create an explosion sprite at the meteor's position
      RECT rcBounds = { 0, 0, 600, 450 };
      RECT rcPos    = pSpriteDying-> GetPosition( );

      Sprite* pSprite = new Sprite(g_explosionBitmap, rcBounds);

      pSprite->SetNumFrames(12, TRUE);
      pSprite->SetPosition(rcPos.left, rcPos.top);
      g_game->AddSprite(pSprite);
   }
}

void GameNew( )
{
   // Clear the sprites
   g_game->CleanupSprites( );

   RECT rcBounds = { 0, 0, 600, 450 };

   // Create the city sprites
   Sprite* pSprite = new Sprite(g_cityBitmap, rcBounds);
   pSprite->SetPosition(2, 370);
   g_game->AddSprite(pSprite);
   pSprite = new Sprite(g_cityBitmap, rcBounds);
   pSprite->SetPosition(186, 370);
   g_game->AddSprite(pSprite);
   pSprite = new Sprite(g_cityBitmap, rcBounds);
   pSprite->SetPosition(302, 370);
   g_game->AddSprite(pSprite);
   pSprite = new Sprite(g_cityBitmap, rcBounds);
   pSprite->SetPosition(490, 370);
   g_game->AddSprite(pSprite);

   // Initialize the game variables
   g_score      = 0;
   g_numCities  = 4;
   g_difficulty = 50;
   g_gameOver   = FALSE;

   // Play the background music
   g_game->PlayMIDISong( );
}

void AddMeteor( )
{
   // Create a new meteor sprite and set its position
   RECT    rcBounds = { 0, 0, 600, 390 };
   int     iXPos = rtk::rand(0, 600);
   Sprite* pSprite = new Sprite(g_meteorBitmap, rcBounds, BA_DIE);
   pSprite->SetNumFrames(14);
   pSprite->SetPosition(iXPos, 0);

   // Calculate the velocity so that it is aimed at one of the cities
   int iXVel = 0;
   int iYVel = rtk::rand(3, 4 );

   switch ( rtk::rand(0, 4) )
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
   g_game->AddSprite(pSprite);
}