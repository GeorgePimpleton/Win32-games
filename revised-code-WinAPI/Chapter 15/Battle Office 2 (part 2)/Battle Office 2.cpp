#include "Battle Office 2.hpp"

HRESULT GameInitialize(HINSTANCE inst)
{
   g_game = std::make_unique<GameEngine>(inst, L"Battle Office 2", L"Battle Office 2: Animating the Appearance of Sprites",
                                         IDI_ICON, IDI_ICON_SM, 500, 400);

   if ( NULL == g_game )
   {
      return E_FAIL;
   }

   g_game->SetFrameRate(30);

   return S_OK;
}

void GameStart(HWND hWindow)
{
   rtk::srand( );

   SetClassLongPtrW(hWindow, GCLP_HCURSOR, (LONG64) LoadImageW(GetModuleHandleW(NULL), MAKEINTRESOURCEW(IDC_CURSOR), IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR));

   g_offscreenDC     = CreateCompatibleDC(GetDC(hWindow));
   g_offscreenBitmap = CreateCompatibleBitmap(GetDC(hWindow), g_game-> GetWidth( ), g_game-> GetHeight( ));

   SelectObject(g_offscreenDC, g_offscreenBitmap);

   HINSTANCE inst = GetModuleHandle(NULL);

   g_officeBitmap    = std::make_unique<Bitmap>(IDB_OFFICE, inst);
   g_powBitmap       = std::make_unique<Bitmap>(IDB_POW, inst);
   g_guyBitmaps[ 0 ] = std::make_unique<Bitmap>(IDB_GUY1, inst);
   g_guyBitmaps[ 1 ] = std::make_unique<Bitmap>(IDB_GUY2, inst);
   g_guyBitmaps[ 2 ] = std::make_unique<Bitmap>(IDB_GUY3, inst);
   g_guyBitmaps[ 3 ] = std::make_unique<Bitmap>(IDB_GUY4, inst);
   g_guyBitmaps[ 4 ] = std::make_unique<Bitmap>(IDB_GUY5, inst);
   g_smallGuyBitmap  = std::make_unique<Bitmap>(IDB_SMALLGUY, inst);
   g_gameOverBitmap  = std::make_unique<Bitmap>(IDB_GAMEOVER, inst);

   RECT rcBounds = { 0, 0, 500, 400 };
   g_powSprite = std::make_unique<Sprite>(g_powBitmap.get( ), rcBounds, BA_STOP);
   g_powSprite->SetZOrder(3);
   g_powSprite->SetHidden(TRUE);
   g_game->AddSprite(g_powSprite.get( ));

   g_guySprites[ 0 ] = std::make_unique<Sprite>(g_guyBitmaps[ 0 ].get( ), rcBounds);
   g_guySprites[ 0 ]->SetPosition(92, 175);
   g_guySprites[ 0 ]->SetZOrder(2);
   g_guySprites[ 0 ]->SetHidden(TRUE);
   g_game->AddSprite(g_guySprites[ 0 ].get( ));

   g_guySprites[ 1 ] = std::make_unique<Sprite>(g_guyBitmaps[ 1 ].get( ), rcBounds);
   g_guySprites[ 1 ]->SetPosition(301, 184);
   g_guySprites[ 1 ]->SetZOrder(2);
   g_guySprites[ 1 ]->SetHidden(TRUE);
   g_game->AddSprite(g_guySprites[ 1 ].get( ));

   g_guySprites[ 2 ] = std::make_unique<Sprite>(g_guyBitmaps[ 2 ].get( ), rcBounds);
   g_guySprites[ 2 ]->SetPosition(394, 61);
   g_guySprites[ 2 ]->SetZOrder(2);
   g_guySprites[ 2 ]->SetHidden(TRUE);
   g_game->AddSprite(g_guySprites[ 2 ].get( ));

   rcBounds.left = 340;
   g_guySprites[ 3 ] = std::make_unique<Sprite>(g_guyBitmaps[ 3 ].get( ), rcBounds, BA_WRAP);
   g_guySprites[ 3 ]->SetPosition(500, 10);
   g_guySprites[ 3 ]->SetVelocity(-3, 0);
   g_guySprites[ 3 ]->SetZOrder(1);
   g_guySprites[ 3 ]->SetHidden(TRUE);
   g_guySprites[ 3 ]->SetNumFrames(4);
   g_game->AddSprite(g_guySprites[ 3 ].get( ));

   rcBounds.left = 385;
   g_guySprites[ 4 ] = std::make_unique<Sprite>(g_guyBitmaps[ 4 ].get( ), rcBounds, BA_WRAP);
   g_guySprites[ 4 ]->SetPosition(260, 60);
   g_guySprites[ 4 ]->SetVelocity(5, 0);
   g_guySprites[ 4 ]->SetZOrder(1);
   g_guySprites[ 4 ]->SetHidden(TRUE);
   g_guySprites[ 4 ]->SetNumFrames(4);
   g_guySprites[ 4 ]->SetFrameDelay(5);
   g_game->AddSprite(g_guySprites[ 4 ].get( ));

   g_game->PlayMIDISong(L"Music.mid");

   GameNew( );
}

void GameNew( )
{
   g_guyMasterDelay = 50;
   g_hits           = 0;
   g_misses         = 0;
   g_gameOver       = FALSE;

   EnableMenuItem(GetMenu(g_game->GetWindow( )), (UINT) MAKEINTRESOURCEW(IDM_GAME_NEW), MF_GRAYED);

   g_game->PlayMIDISong( );
}

void GameEnd( )
{
   g_game->CloseMIDIPlayer( );

   DeleteObject(g_offscreenBitmap);
   DeleteDC(g_offscreenDC);

   g_game->CleanupSprites( );
}

void GameActivate(HWND hWindow)
{
   g_game->PlayMIDISong(TEXT(""), FALSE);
}

void GameDeactivate(HWND hWindow)
{
   g_game->PauseMIDISong( );
}

void GamePaint(HDC hDC)
{
   g_officeBitmap->Draw(hDC, 0, 0);

   g_game->DrawSprites(hDC);

   WCHAR szText[ 64 ];
   RECT  rect = { 237, 360, 301, 390 };
   wsprintfW(szText, L"%d", g_hits);
   DrawTextW(hDC, szText, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

   for ( int i = 0; i < g_misses; i++ )
   {
      g_smallGuyBitmap->Draw(hDC, 389 + (g_smallGuyBitmap->GetWidth( ) * i), 359, TRUE);
   }

   if ( g_gameOver )
   {
      g_gameOverBitmap->Draw(hDC, 120, 110, TRUE);
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
               PlaySoundW((PCWSTR) IDW_TAUNT, GetModuleHandleW(NULL), SND_ASYNC | SND_RESOURCE);

               // hide the guy
               g_guySprites[ i ]->SetHidden(TRUE);

               // increment the misses
               if ( ++g_misses == 5 )
               {
                  // play a sound for the game ending
                  PlaySoundW((PCWSTR) IDW_BOO, GetModuleHandleW(NULL), SND_ASYNC | SND_RESOURCE);

                  // end the game
                  for ( int i = 0; i < 5; i++ )
                  {
                     g_guySprites[ i ]->SetHidden(TRUE);
                  }

                  g_gameOver = TRUE;

                  g_powSprite->SetHidden(TRUE);

                  // pause the background music
                  g_game->PauseMIDISong( );

                  EnableMenuItem(GetMenu(g_game->GetWindow( )), (UINT) MAKEINTRESOURCEW(IDM_GAME_NEW), MF_ENABLED);
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

void MouseButtonDown(LONG x, LONG y, BOOL left)
{
   // only check the left mouse button
   if ( g_gameOver == FALSE && left )
   {
      // temporarily hide the POW sprite
      g_powSprite->SetHidden(TRUE);

      // see if a guy sprite was clicked
      Sprite* sprite;

      if ( (sprite = g_game->IsPointInSprite(x, y)) != NULL )
      {
         // play a sound for hitting the guy
         PlaySoundW((PCWSTR) IDW_WHACK, GetModuleHandleW(NULL), SND_ASYNC | SND_RESOURCE);

         // position and show the pow sprite
         g_powSprite->SetPosition(x - (g_powSprite->GetWidth( ) / 2), y - (g_powSprite->GetHeight( ) / 2));
         g_powSprite->SetHidden(FALSE);

         // hide the guy that was clicked
         sprite->SetHidden(TRUE);

         // increment the hits and make the game harder, if necessary
         if ( 0 == (++g_hits % 5) )
         {
            if ( 0 == --g_guyMasterDelay )
            {
               g_guyMasterDelay = 1;
            }
         }
      }
   }
   else if ( g_gameOver && !left )
   {
      // start a new game
      GameNew( );

      // restart the background music
      g_game->PlayMIDISong( );
   }
}

void MouseButtonUp(LONG x, LONG y, BOOL left)
{ }

void MouseMove(LONG x, LONG y)
{ }

void HandleJoystick(JOYSTATE joyState)
{ }

BOOL SpriteCollision(Sprite* spriteHitter, Sprite* spriteHittee)
{
   return FALSE;
}