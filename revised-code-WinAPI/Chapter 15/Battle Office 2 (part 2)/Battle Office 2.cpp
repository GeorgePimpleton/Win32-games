#include "Battle Office 2.hpp"

BOOL GameInitialize(HINSTANCE inst)
{
   g_game = std::make_unique<GameEngine>(inst, L"Battle Office 2", L"Battle Office 2: Animating the Appearance of Sprites",
                                         IDI_ICON, IDI_ICON_SM, 500, 400);

   if ( g_game == NULL )
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

   g_offscreenDC     = CreateCompatibleDC(GetDC(wnd));
   g_offscreenBitmap = CreateCompatibleBitmap(GetDC(wnd),
                                              g_game->GetWidth( ), g_game->GetHeight( ));

   SelectObject(g_offscreenDC, g_offscreenBitmap);

   g_officeBitmap    = std::make_unique<Bitmap>(IDB_OFFICE);
   g_powBitmap       = std::make_unique<Bitmap>(IDB_POW);
   g_guyBitmaps[ 0 ] = std::make_unique<Bitmap>(IDB_GUY1);
   g_guyBitmaps[ 1 ] = std::make_unique<Bitmap>(IDB_GUY2);
   g_guyBitmaps[ 2 ] = std::make_unique<Bitmap>(IDB_GUY3);
   g_guyBitmaps[ 3 ] = std::make_unique<Bitmap>(IDB_GUY4);
   g_guyBitmaps[ 4 ] = std::make_unique<Bitmap>(IDB_GUY5);
   g_smallGuyBitmap  = std::make_unique<Bitmap>(IDB_SMALLGUY);
   g_gameOverBitmap  = std::make_unique<Bitmap>(IDB_GAMEOVER);

   RECT    bounds = { 0, 0, 500, 400 };

   g_powSprite = std::make_unique<Sprite>(g_powBitmap.get( ), bounds, BA_STOP);
   g_powSprite->SetZOrder(3);
   g_powSprite->SetHidden(TRUE);
   g_game->AddSprite(g_powSprite.get( ));

   g_guySprites[ 0 ] = std::make_unique<Sprite>(g_guyBitmaps[ 0 ].get( ), bounds);
   g_guySprites[ 0 ]->SetPosition(92, 175);
   g_guySprites[ 0 ]->SetZOrder(2);
   g_guySprites[ 0 ]->SetHidden(TRUE);
   g_game->AddSprite(g_guySprites[ 0 ].get( ));

   g_guySprites[ 1 ] = std::make_unique<Sprite>(g_guyBitmaps[ 1 ].get( ), bounds);
   g_guySprites[ 1 ]->SetPosition(301, 184);
   g_guySprites[ 1 ]->SetZOrder(2);
   g_guySprites[ 1 ]->SetHidden(TRUE);
   g_game->AddSprite(g_guySprites[ 1 ].get( ));

   g_guySprites[ 2 ] = std::make_unique<Sprite>(g_guyBitmaps[ 2 ].get( ), bounds);
   g_guySprites[ 2 ]->SetPosition(394, 61);
   g_guySprites[ 2 ]->SetZOrder(2);
   g_guySprites[ 2 ]->SetHidden(TRUE);
   g_game->AddSprite(g_guySprites[ 2 ].get( ));

   bounds.left = 340;
   g_guySprites[ 3 ] = std::make_unique<Sprite>(g_guyBitmaps[ 3 ].get( ), bounds, BA_WRAP);
   g_guySprites[ 3 ]->SetNumFrames(4);
   g_guySprites[ 3 ]->SetPosition(500, 10);
   g_guySprites[ 3 ]->SetVelocity(-3, 0);
   g_guySprites[ 3 ]->SetZOrder(1);
   g_guySprites[ 3 ]->SetHidden(TRUE);
   g_game->AddSprite(g_guySprites[ 3 ].get( ));

   bounds.left = 385;

   g_guySprites[ 4 ] = std::make_unique<Sprite>(g_guyBitmaps[ 4 ].get( ), bounds, BA_WRAP);
   g_guySprites[ 4 ]->SetNumFrames(4);
   g_guySprites[ 4 ]->SetFrameDelay(5);
   g_guySprites[ 4 ]->SetPosition(260, 60);
   g_guySprites[ 4 ]->SetVelocity(5, 0);
   g_guySprites[ 4 ]->SetZOrder(1);
   g_guySprites[ 4 ]->SetHidden(TRUE);
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

void GameActivate(HWND)
{
   g_game->PlayMIDISong(L"", FALSE);
}

void GameDeactivate(HWND)
{
   g_game->PauseMIDISong( );
}

void GamePaint(HDC dc)
{
   g_officeBitmap->Draw(dc, 0, 0);

   g_game->DrawSprites(dc);

   WCHAR text[ 64 ];
   RECT  rect = { 237, 360, 301, 390 };

   wsprintfW(text, L"%d", g_hits);
   DrawTextW(dc, text, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

   for ( int i = 0; i < g_misses; i++ )
   {
      g_smallGuyBitmap->Draw(dc, 389 + (g_smallGuyBitmap->GetWidth( ) * i), 359, TRUE);
   }

   if ( g_gameOver )
   {
      g_gameOverBitmap->Draw(dc, 120, 110, TRUE);
   }
}

void GameCycle( )
{
   if ( !g_gameOver )
   {
      for ( int i = 0; i < 5; i++ )
      {
         if ( g_guySprites[ i ]->IsHidden( ) )
         {
            if ( rtk::rand(0, 60) == 0 )
            {
               g_guySprites[ i ]->SetHidden(FALSE);

               if ( i == 3 )
               {
                  g_guyDelay[ i ] = 80;
                  g_guySprites[ i ]->SetPosition(500, 10);
               }
               else if ( i == 4 )
               {
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
               PlaySoundW((PCWSTR) IDW_TAUNT, g_inst, SND_ASYNC | SND_RESOURCE);

               g_guySprites[ i ]->SetHidden(TRUE);

               if ( ++g_misses == 5 )
               {
                  PlaySoundW((PCWSTR) IDW_BOO, g_inst, SND_ASYNC | SND_RESOURCE);

                  for ( int i = 0; i < 5; i++ )
                  {
                     g_guySprites[ i ]->SetHidden(TRUE);
                  }

                  g_powSprite->SetHidden(TRUE);
                  g_gameOver = TRUE;

                  EnableMenuItem(GetMenu(g_game->GetWindow( )), (UINT) MAKEINTRESOURCEW(IDM_GAME_NEW), MF_ENABLED);

                  g_game->PauseMIDISong( );
               }
            }
         }
      }

      g_game->UpdateSprites( );

      HWND  wnd = g_game->GetWindow( );
      HDC   dc = GetDC(wnd);

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
      DialogBoxParamW(g_game->GetInstance( ), MAKEINTRESOURCEW(IDD_ABOUT),
                      g_game->GetWindow( ), (DLGPROC) DlgProc, 0L);
      return;
   }
}

void HandleKeys( )
{ }

void MouseButtonDown(LONG x, LONG y, BOOL left)
{
   if ( !g_gameOver && left )
   {
      g_powSprite->SetHidden(TRUE);

      Sprite* sprite;

      if ( (sprite = g_game->IsPointInSprite(x, y)) != NULL )
      {
         PlaySoundW((PCWSTR) IDW_WHACK, g_inst, SND_ASYNC | SND_RESOURCE);

         g_powSprite->SetPosition(x - (g_powSprite->GetWidth( ) / 2),
                                  y - (g_powSprite->GetHeight( ) / 2));
         g_powSprite->SetHidden(FALSE);

         sprite->SetHidden(TRUE);

         if ( (++g_hits % 5) == 0 )
         {
            if ( --g_guyMasterDelay == 0 )
            {
               g_guyMasterDelay = 1;
            }
         }
      }
   }
   else if ( g_gameOver && !left )
   {
      g_game->PlayMIDISong( );

      GameNew( );
   }
}

void MouseButtonUp(LONG, LONG, BOOL)
{ }

void MouseMove(LONG, LONG)
{ }

void HandleJoystick(JOYSTATE)
{ }

BOOL SpriteCollision(Sprite*, Sprite*)
{
   return FALSE;
}