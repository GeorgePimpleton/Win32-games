#include "Battle Office.hpp"

HRESULT GameInitialize(HINSTANCE inst)
{
   g_game = new GameEngine(inst, L"Battle Office", L"Example Game: Battle Office",
                           IDI_ICON, IDI_ICON_SM, 500, 400);

   if ( NULL == g_game )
   {
      return E_FAIL;
   }

   g_game->SetFrameRate(30);

   return S_OK;
}

void GameStart(HWND wnd)
{
   rtk::srand( );

   // load the custom cursor
   SetClassLongPtrW(wnd, GCLP_HCURSOR,
                    (LONG64) LoadImageW(GetModuleHandleW(NULL), MAKEINTRESOURCEW(IDC_CURSOR), IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR));

   g_offscreenDC     = CreateCompatibleDC(GetDC(wnd));
   g_offscreenBitmap = CreateCompatibleBitmap(GetDC(wnd), g_game-> GetWidth( ), g_game-> GetHeight( ));

   SelectObject(g_offscreenDC, g_offscreenBitmap);

   HINSTANCE inst = GetModuleHandleW(NULL);

   g_officeBitmap    = new Bitmap(IDB_OFFICE, inst);
   g_powBitmap       = new Bitmap(IDB_POW, inst);
   g_guyBitmaps[ 0 ] = new Bitmap(IDB_GUY1, inst);
   g_guyBitmaps[ 1 ] = new Bitmap(IDB_GUY2, inst);
   g_guyBitmaps[ 2 ] = new Bitmap(IDB_GUY3, inst);
   g_guyBitmaps[ 3 ] = new Bitmap(IDB_GUY4, inst);
   g_guyBitmaps[ 4 ] = new Bitmap(IDB_GUY5, inst);
   g_smallGuyBitmap  = new Bitmap(IDB_SMALLGUY, inst);
   g_gameOverBitmap  = new Bitmap(IDB_GAMEOVER, inst);

   g_game->PlayMIDISong(L"Music.mid");

   NewGame( );
}

void GameEnd( )
{
   g_game->CloseMIDIPlayer( );

   DeleteObject(g_offscreenBitmap);
   DeleteDC(g_offscreenDC);

   delete g_officeBitmap;
   delete g_powBitmap;

   for ( int i = 0; i < 5; i++ )
   {
      delete g_guyBitmaps[ i ];
   }

   delete g_smallGuyBitmap;
   delete g_gameOverBitmap;

   g_game->CleanupSprites( );

   delete g_game;
}

void GameActivate(HWND wnd)
{
   g_game->PlayMIDISong(TEXT(""), FALSE);
}

void GameDeactivate(HWND wnd)
{
   g_game->PauseMIDISong( );
}

void GamePaint(HDC dc)
{
   g_officeBitmap->Draw(dc, 0, 0);

   g_game->DrawSprites(dc);

   // draw the number of guys who were hit
   WCHAR text[ STR_LENGTH ];
   RECT  rect = { 237, 360, 301, 390 };

   wsprintfW(text, L"%d", g_hits);

   DrawTextW(dc, text, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

   // draw the number of guys who were missed (got away)
   for ( int i = 0; i < g_misses; i++ )
   {
      g_smallGuyBitmap->Draw(dc, 389 + (g_smallGuyBitmap->GetWidth( ) * i), 359, TRUE);
   }

   // draw the game over message, if necessary
   if ( g_gameOver )
   {
      g_gameOverBitmap->Draw(dc, 120, 110, TRUE);
   }
}

void GameCycle( )
{
   if ( !g_gameOver )
   {
      // randomly show and hide the guys
      for ( int i = 0; i < 5; i++ )
      {
         if ( g_guySprites[ i ]->IsHidden( ) )
         {
            if ( rtk::rand(0, 60) == 0 )
            {
               // show the guy
               g_guySprites[ i ]->SetHidden(FALSE);

               // start the countdown delay
               if ( i == 3 )
               {
                  // start the guy running left
                  g_guyDelay[ i ] = 80;
                  g_guySprites[ i ]->SetPosition(500, 10);
               }
               else if ( i == 4 )
               {
                  // start the guy running right
                  g_guyDelay[ i ] = 45;
                  g_guySprites[ i ]->SetPosition(260, 60);
               }
               else
               {
                  g_guyDelay[ i ] = 20 + (rtk::rand(0, g_guyMasterDelay));
               }
            }
         }
         else
         {
            if ( --g_guyDelay[ i ] == 0 )
            {
               // play a sound for the guy getting away
               PlaySoundW((PCWSTR) IDW_TAUNT, GetModuleHandle(NULL), SND_ASYNC | SND_RESOURCE);

               // hide the guy
               g_guySprites[ i ]->SetHidden(TRUE);

               // increment the misses
               if ( ++g_misses == 5 )
               {
                  // play a sound for the game ending
                  PlaySoundW((PCWSTR) IDW_BOO, GetModuleHandle(NULL), SND_ASYNC | SND_RESOURCE);

                  // end the game
                  for ( int i = 0; i < 5; i++ )
                  {
                     g_guySprites[ i ]->SetHidden(TRUE);
                  }
                  g_gameOver = TRUE;

                  // pause the background music
                  g_game->PauseMIDISong( );
               }
            }
         }
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

void GameMenu(WPARAM wParam)
{
   switch ( LOWORD(wParam) )
   {
   case IDM_GAME_NEW:
      if ( g_gameOver == TRUE )
      {
         // start a new game
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
}

void MouseButtonDown(LONG x, LONG y, BOOL left)
{
   // only check the left mouse button
   if ( g_gameOver == FALSE && left )
   {
      // temporarily hide the POW sprite
      g_powSprite->SetHidden(TRUE);

      // see if a guy sprite was clicked
      Sprite* pSprite;

      if ( (pSprite = g_game->IsPointInSprite(x, y)) != NULL )
      {
         // play a sound for hitting the guy
         PlaySoundW((PCWSTR) IDW_WHACK, GetModuleHandle(NULL), SND_ASYNC | SND_RESOURCE);

         // position and show the pow sprite
         g_powSprite->SetPosition(x - (g_powSprite->GetWidth( ) / 2), y - (g_powSprite->GetHeight( ) / 2));
         g_powSprite->SetHidden(FALSE);

         // hide the guy that was clicked
         pSprite->SetHidden(TRUE);

         // increment the hits and make the game harder, if necessary
         if ( (++g_hits % 5) == 0 )
         {
            if ( --g_guyMasterDelay == 0 )
            {
               g_guyMasterDelay = 1;
            }
         }
      }
   }
   else if ( g_gameOver == TRUE && !left )
   {
      // start a new game
      NewGame( );

      // restart the background music
      g_game->PlayMIDISong( );
   }
}

void MouseButtonUp(LONG x, LONG y, BOOL left)
{ }

void MouseMove(LONG x, LONG y)
{ }

void HandleJoystick(JOYSTATE jsJoystickState)
{ }

BOOL SpriteCollision(Sprite* spriteHitter, Sprite* spriteHittee)
{
   return FALSE;
}

void NewGame( )
{
   g_game->CleanupSprites( );

   RECT bounds = { 0, 0, 500, 400 };

   g_powSprite = new Sprite(g_powBitmap, bounds, BA_STOP);
   g_powSprite->SetZOrder(3);
   g_powSprite->SetHidden(TRUE);
   g_game->AddSprite(g_powSprite);

   g_guySprites[ 0 ] = new Sprite(g_guyBitmaps[ 0 ], bounds);
   g_guySprites[ 0 ]->SetPosition(92, 175);
   g_guySprites[ 0 ]->SetZOrder(2);
   g_guySprites[ 0 ]->SetHidden(TRUE);
   g_game->AddSprite(g_guySprites[ 0 ]);

   g_guySprites[ 1 ] = new Sprite(g_guyBitmaps[ 1 ], bounds);
   g_guySprites[ 1 ]->SetPosition(301, 184);
   g_guySprites[ 1 ]->SetZOrder(2);
   g_guySprites[ 1 ]->SetHidden(TRUE);
   g_game->AddSprite(g_guySprites[ 1 ]);

   g_guySprites[ 2 ] = new Sprite(g_guyBitmaps[ 2 ], bounds);
   g_guySprites[ 2 ]->SetPosition(394, 61);
   g_guySprites[ 2 ]->SetZOrder(2);
   g_guySprites[ 2 ]->SetHidden(TRUE);
   g_game->AddSprite(g_guySprites[ 2 ]);

   bounds.left = 340;
   g_guySprites[ 3 ] = new Sprite(g_guyBitmaps[ 3 ], bounds, BA_WRAP);
   g_guySprites[ 3 ]->SetPosition(500, 10);
   g_guySprites[ 3 ]->SetVelocity(-3, 0);
   g_guySprites[ 3 ]->SetZOrder(1);
   g_guySprites[ 3 ]->SetHidden(TRUE);
   g_game->AddSprite(g_guySprites[ 3 ]);

   bounds.left = 385;
   g_guySprites[ 4 ] = new Sprite(g_guyBitmaps[ 4 ], bounds, BA_WRAP);
   g_guySprites[ 4 ]->SetPosition(260, 60);
   g_guySprites[ 4 ]->SetVelocity(5, 0);
   g_guySprites[ 4 ]->SetZOrder(1);
   g_guySprites[ 4 ]->SetHidden(TRUE);
   g_game->AddSprite(g_guySprites[ 4 ]);

   g_guyMasterDelay = 50;
   g_hits           = 0;
   g_misses         = 0;
   g_gameOver       = FALSE;

   // restart the background music
   g_game->PlayMIDISong( );
}