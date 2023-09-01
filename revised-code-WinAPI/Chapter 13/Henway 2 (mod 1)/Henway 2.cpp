#include "Henway 2.hpp"

HRESULT GameInitialize(HINSTANCE inst)
{
   g_game = new GameEngine(inst, L"Henway 2", L"Playing MIDI Music",
                           IDI_ICON, IDI_ICON_SM, 465, 400);

   if ( nullptr == g_game )
   {
      return E_FAIL;
   }

   g_game->SetFrameRate(30);

   g_game->InitJoystick( );

   return S_OK;
}

void GameStart(HWND wnd)
{
   rtk::srand( );

   g_offscreenDC     = CreateCompatibleDC(GetDC(wnd));
   g_offscreenBitmap = CreateCompatibleBitmap(GetDC(wnd), g_game-> GetWidth( ), g_game-> GetHeight( ));

   SelectObject(g_offscreenDC, g_offscreenBitmap);

   HINSTANCE inst = GetModuleHandleW(nullptr);

   g_highwayBitmap     = new Bitmap(IDB_HIGHWAY, inst);
   g_chickenBitmap     = new Bitmap(IDB_CHICKEN, inst);
   g_carBitmaps[ 0 ]   = new Bitmap(IDB_CAR1, inst);
   g_carBitmaps[ 1 ]   = new Bitmap(IDB_CAR2, inst);
   g_carBitmaps[ 2 ]   = new Bitmap(IDB_CAR3, inst);
   g_carBitmaps[ 3 ]   = new Bitmap(IDB_CAR4, inst);
   g_chickenHeadBitmap = new Bitmap(IDB_CHICKENHEAD, inst);

   // play the background music
   g_game->PlayMIDISong(L"Music.mid");

   NewGame( );
}

void GameEnd( )
{
   // close the MIDI player for the background music
   g_game->CloseMIDIPlayer( );

   DeleteObject(g_offscreenBitmap);
   DeleteDC(g_offscreenDC);

   delete g_highwayBitmap;
   delete g_chickenBitmap;

   for ( UINT i = 0; i < 4; i++ )
   {
      delete g_carBitmaps[ i ];
   }
   delete g_chickenHeadBitmap;

   g_game->CleanupSprites( );

   delete g_game;
}

void GameActivate(HWND wnd)
{
   g_game->CaptureJoystick( );

   // resume the background music
   g_game->PlayMIDISong(L"", FALSE);
}

void GameDeactivate(HWND hWindow)
{
   g_game->ReleaseJoystick( );

   // pause the background music
   g_game->PauseMIDISong( );
}

void GamePaint(HDC hDC)
{
   // draw the background highway
   g_highwayBitmap->Draw(hDC, 0, 0);

   // draw the sprites
   g_game->DrawSprites(hDC);

   // draw the number of remaining chicken lives
   for ( UINT i = 0; i < g_numLives; i++ )
   {
      g_chickenHeadBitmap->Draw(hDC, 406 + (g_chickenHeadBitmap->GetWidth( ) * i), 382, TRUE);
   }
}

void GameCycle( )
{
   if ( !g_gameOver )
   {
      HINSTANCE inst = GetModuleHandleW(nullptr);

      // play a random car sound randomly
      if ( rtk::rand(0, 99) == 0 )
      {
         if ( rtk::rand(0, 1) == 0 )
         {
            PlaySoundW((PCWSTR) IDW_CARHORN1, inst, SND_ASYNC | SND_RESOURCE);
         }
         else
         {
            PlaySoundW((PCWSTR) IDW_CARHORN2, inst, SND_ASYNC | SND_RESOURCE);
         }
      }

      // update the sprites
      g_game->UpdateSprites( );

      // obtain a device context for repainting the game
      HWND wnd = g_game-> GetWindow( );
      HDC  dc  = GetDC(wnd);

      // paint the game to the offscreen device context
      GamePaint(g_offscreenDC);

      // blit the offscreen bitmap to the game screen
      BitBlt(dc, 0, 0, g_game->GetWidth( ), g_game->GetHeight( ), g_offscreenDC, 0, 0, SRCCOPY);

      // cleanup
      ReleaseDC(wnd, dc);
   }
}

void GameMenu(WPARAM wParam)
{
   switch ( LOWORD(wParam) )
   {
   case IDM_GAME_NEW:
      if ( g_gameOver == TRUE )
      {
         NewGame( );
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
{
   if ( !g_gameOver && (++g_inputDelay > 2) )
   {
      // move the chicken based upon key presses
      if ( GetAsyncKeyState(VK_LEFT) < 0 )
      {
         MoveChicken(-20, 0);
      }
      else if ( GetAsyncKeyState(VK_RIGHT) < 0 )
      {
         MoveChicken(20, 0);
      }

      if ( GetAsyncKeyState(VK_UP) < 0 )
      {
         MoveChicken(0, -20);
      }
      else if ( GetAsyncKeyState(VK_DOWN) < 0 )
      {
         MoveChicken(0, 20);
      }

      // reset the input delay
      g_inputDelay = 0;
   }
}

void MouseButtonDown(LONG x, LONG y, BOOL left)
{
   // Start a new game, if necessary
   if ( g_gameOver == TRUE )
   {
      // restart the background music
      g_game->PlayMIDISong( );
      NewGame( );
   }
}

void MouseButtonUp(LONG x, LONG y, BOOL left)
{ }

void MouseMove(LONG x, LONG y)
{ }

void HandleJoystick(JOYSTATE joyState)
{
   if ( !g_gameOver && (++g_inputDelay > 2) )
   {
      // check horizontal movement
      if ( joyState & JOY_LEFT )
      {
         MoveChicken(-20, 0);
      }
      else if ( joyState & JOY_RIGHT )
      {
         MoveChicken(20, 0);
      }

      // check vertical movement
      if ( joyState & JOY_UP )
      {
         MoveChicken(0, -20);
      }
      else if ( joyState & JOY_DOWN )
      {
         MoveChicken(0, 20);
      }

      // reset the input delay
      g_inputDelay = 0;
   }

   // check the joystick button and start a new game, if necessary
   if ( g_gameOver && (joyState & JOY_FIRE1) )
   {
      // restart the background music
      g_game->PlayMIDISong( );
      NewGame( );
   }
}

BOOL SpriteCollision(Sprite* spriteHitter, Sprite* spriteHittee)
{
   // see if the chicken was hit
   if ( spriteHittee == g_chickenSprite )
   {
      // move the chicken back to the start
      g_chickenSprite->SetPosition(4, 175);

      HINSTANCE inst = GetModuleHandleW(nullptr);

      // see if the game is over
      if ( --g_numLives > 0 )
      {
         // play a sound for the chicken getting hit
         PlaySoundW((PCWSTR) IDW_SQUISH, inst, SND_ASYNC | SND_RESOURCE);
      }
      else
      {
         // play a sound for the game ending
         PlaySoundW((PCWSTR) IDW_GAMEOVER, inst, SND_ASYNC | SND_RESOURCE);

         // display game over message
         WCHAR szText[ 64 ];

         wsprintfW(szText, L"Game Over! You scored %d points.", g_score);

         MessageBoxW(g_game->GetWindow( ), szText, L"Henway 2", MB_OK);

         g_gameOver = TRUE;
      }

      return FALSE;
   }

   return TRUE;
}

void MoveChicken(int xDistance, int yDistance)
{
   // move the chicken to its new position
   g_chickenSprite->OffsetPosition(xDistance, yDistance);

   // see if the chicken made it across
   if ( g_chickenSprite->GetPosition( ).left > 400 )
   {
      // play a sound for the chicken making it safely across
      HINSTANCE inst = GetModuleHandleW(nullptr);

      PlaySoundW((PCWSTR) IDW_CELEBRATE, inst, SND_ASYNC | SND_RESOURCE);

      // move the chicken back to the start and add to the score
      g_chickenSprite->SetPosition(4, 175);
      g_score += 150;
   }
}

void NewGame( )
{
   // cleanup the sprites (if any)
   g_game->CleanupSprites( );

   // create the chicken and car sprites
   Sprite* sprite;
   RECT    bounds = { 0, 0, 465, 400 };

   g_chickenSprite = new Sprite(g_chickenBitmap, bounds, BA_STOP);
   g_chickenSprite->SetPosition(4, 175);
   g_chickenSprite->SetVelocity(0, 0);
   g_chickenSprite->SetZOrder(1);
   g_game->AddSprite(g_chickenSprite);

   sprite = new Sprite(g_carBitmaps[ 0 ], bounds, BA_WRAP);
   sprite->SetPosition(70, 0);
   sprite->SetVelocity(0, 7);
   sprite->SetZOrder(2);
   g_game->AddSprite(sprite);

   sprite = new Sprite(g_carBitmaps[ 1 ], bounds, BA_WRAP);
   sprite->SetPosition(160, 0);
   sprite->SetVelocity(0, 3);
   sprite->SetZOrder(2);
   g_game->AddSprite(sprite);

   sprite = new Sprite(g_carBitmaps[ 2 ], bounds, BA_WRAP);
   sprite->SetPosition(239, 400);
   sprite->SetVelocity(0, -5);
   sprite->SetZOrder(2);
   g_game->AddSprite(sprite);

   sprite = new Sprite(g_carBitmaps[ 3 ], bounds, BA_WRAP);
   sprite->SetPosition(329, 400);
   sprite->SetVelocity(0, -10);
   sprite->SetZOrder(2);
   g_game->AddSprite(sprite);

   // Initialize the game variables
   g_inputDelay = 0;
   g_numLives   = 3;
   g_score      = 0;
   g_gameOver   = FALSE;

   // play the background music
   g_game->PlayMIDISong( );
}