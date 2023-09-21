#include "SpaceOut.hpp"

BOOL GameInitialize(HINSTANCE inst)
{
   g_game = std::make_unique<GameEngine>(inst, L"Space Out 4", L"Space Out 4: Keeping Track of High Scores",
                                         IDI_ICON, IDI_ICON_SM, 600, 450);

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
   // read the hi scores
   ReadHiScores( );

   rtk::srand();

   g_hOffscreenDC     = CreateCompatibleDC(GetDC(wnd));
   g_hOffscreenBitmap = CreateCompatibleBitmap(GetDC(wnd),
                                               g_game->GetWidth( ), g_game->GetHeight( ));

   SelectObject(g_hOffscreenDC, g_hOffscreenBitmap);

   g_splashBitmap         = std::make_unique<Bitmap>(IDB_SPLASH, g_inst);
   g_desertBitmap         = std::make_unique<Bitmap>(IDB_DESERT, g_inst);
   g_carBitmap            = std::make_unique<Bitmap>(IDB_CAR, g_inst);
   g_smallCarBitmap       = std::make_unique<Bitmap>(IDB_SMCAR, g_inst);
   g_missileBitmap        = std::make_unique<Bitmap>(IDB_MISSILE, g_inst);
   g_BlobboBitmap         = std::make_unique<Bitmap>(IDB_BLOBBO, g_inst);
   g_BMissileBitmap       = std::make_unique<Bitmap>(IDB_BMISSILE, g_inst);
   g_JellyBitmap          = std::make_unique<Bitmap>(IDB_JELLY, g_inst);
   g_JMissileBitmap       = std::make_unique<Bitmap>(IDB_JMISSILE, g_inst);
   g_TimmyBitmap          = std::make_unique<Bitmap>(IDB_TIMMY, g_inst);
   g_TMissileBitmap       = std::make_unique<Bitmap>(IDB_TMISSILE, g_inst);
   g_smallExplosionBitmap = std::make_unique<Bitmap>(IDB_SMEXPLOSION, g_inst);
   g_largeExplosionBitmap = std::make_unique<Bitmap>(IDB_LGEXPLOSION, g_inst);
   g_gameOverBitmap       = std::make_unique<Bitmap>(IDB_GAMEOVER, g_inst);

   g_background = std::make_unique<StarryBackground>(600, 450);

   g_demo = TRUE;
   GameNew( );
}

void GameEnd( )
{
   g_game->CloseMIDIPlayer( );

   DeleteObject(g_hOffscreenBitmap);
   DeleteDC(g_hOffscreenDC);

   g_game->CleanupSprites( );

   // save the hi scores
   WriteHiScores( );
}

void GameActivate(HWND wnd)
{
   if ( !g_demo )
   {
      g_game->PlayMIDISong(L"", FALSE);
   }
}

void GameDeactivate(HWND wnd)
{
   if ( !g_demo )
   {
      g_game->PauseMIDISong( );
   }
}

void GamePaint(HDC dc)
{
   g_background->Draw(dc);

   g_desertBitmap->Draw(dc, 0, 371);

   g_game->DrawSprites(dc);

   if ( g_demo )
   {
      g_splashBitmap->Draw(dc, 142, 20, TRUE);

      // draw the hi scores
      WCHAR szText[ 64 ];
      RECT  rect = { 275, 250, 325, 270 };

      SetBkMode(dc, TRANSPARENT);
      SetTextColor(dc, RGB(255, 255, 255));

      for ( int i = 0; i < 5; i++ )
      {
         wsprintfW(szText, L"%d", g_hiScores[ i ]);

         DrawTextW(dc, szText, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

         rect.top    += 20;
         rect.bottom += 20;
      }
   }
   else
   {
      // draw the score
      WCHAR szText[ 64 ];
      RECT  rect = { 460, 0, 510, 30 };

      wsprintfW(szText, L"%d", g_score);

      SetBkMode(dc, TRANSPARENT);
      SetTextColor(dc, RGB(255, 255, 255));

      DrawTextW(dc, szText, -1, &rect, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

      for ( int i = 0; i < g_numLives; i++ )
      {
         g_smallCarBitmap->Draw(dc, 520 + (g_smallCarBitmap->GetWidth( ) * i),
                                10, TRUE);
      }

      if ( g_gameOver )
      {
         g_gameOverBitmap->Draw(dc, 170, 100, TRUE);
      }
   }
}

void GameCycle( )
{
   if ( !g_gameOver )
   {
      if ( !g_demo )
      {
         if ( (rtk::rand(0, g_difficulty)) == 0 )
         {
            AddAlien( );
         }
      }

      g_background->Update( );
      g_game->UpdateSprites( );

      HWND  wnd = g_game->GetWindow( );
      HDC   dc = GetDC(wnd);

      GamePaint(g_hOffscreenDC);

      BitBlt(dc, 0, 0, g_game->GetWidth( ), g_game->GetHeight( ),
             g_hOffscreenDC, 0, 0, SRCCOPY);

      ReleaseDC(wnd, dc);
   }
   else if ( --g_gameOverDelay == 0 )
   {
      g_game->PauseMIDISong( );
      g_demo = TRUE;
      GameNew( );
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

void HandleKeys( )
{
   if ( !g_gameOver && !g_demo )
   {
      POINT velocity = g_carSprite->GetVelocity( );

      if ( GetAsyncKeyState(VK_LEFT) < 0 )
      {
         velocity.x = max(velocity.x - 1, -4);
         g_carSprite->SetVelocity(velocity);
      }
      else if ( GetAsyncKeyState(VK_RIGHT) < 0 )
      {
         velocity.x = min(velocity.x + 2, 6);
         g_carSprite->SetVelocity(velocity);
      }

      if ( (++g_fireInputDelay > 6) && GetAsyncKeyState(VK_SPACE) < 0 )
      {
         RECT  bounds = { 0, 0, 600, 450 };
         RECT  pos = g_carSprite->GetPosition( );
         Sprite* sprite = new Sprite(g_missileBitmap.get( ), bounds, BA_DIE);
         sprite->SetPosition(pos.left + 15, 400);
         sprite->SetVelocity(0, -7);
         g_game->AddSprite(sprite);

         PlaySoundW((PCWSTR) IDW_MISSILE, g_inst, SND_ASYNC | SND_RESOURCE | SND_NOSTOP);

         g_fireInputDelay = 0;
      }
   }

   if ( GetAsyncKeyState(VK_RETURN) < 0 )
   {
      if ( g_demo )
      {
         g_demo = FALSE;
         GameNew( );
      }
      else if ( g_gameOver )
      {
         GameNew( );
      }
   }
}

void MouseButtonDown(int x, int y, BOOL left)
{ }

void MouseButtonUp(int x, int y, BOOL left)
{ }

void MouseMove(int x, int y)
{ }

void HandleJoystick(JOYSTATE joyState)
{ }

BOOL SpriteCollision(Sprite* spriteHitter, Sprite* spriteHittee)
{
   Bitmap* pHitter = spriteHitter->GetBitmap( );
   Bitmap* pHittee = spriteHittee->GetBitmap( );

   if ( (pHitter == g_missileBitmap.get( ) && (pHittee == g_BlobboBitmap.get( ) ||
                                               pHittee == g_JellyBitmap.get( ) ||
                                               pHittee == g_TimmyBitmap.get( ))) ||
        (pHittee == g_missileBitmap.get( ) && (pHitter == g_BlobboBitmap.get( ) ||
                                               pHitter == g_JellyBitmap.get( ) ||
                                               pHitter == g_TimmyBitmap.get( ))) )
   {
      PlaySoundW((PCWSTR) IDW_LGEXPLODE, g_inst, SND_ASYNC | SND_RESOURCE);

      spriteHitter->Kill( );
      spriteHittee->Kill( );

      RECT bounds = { 0, 0, 600, 450 };
      RECT pos    = { };

      if ( pHitter == g_missileBitmap.get( ) )
      {
         pos = spriteHittee->GetPosition( );
      }
      else
      {
         pos = spriteHitter->GetPosition( );
      }

      Sprite* sprite = new Sprite(g_largeExplosionBitmap.get( ), bounds);
      sprite->SetNumFrames(8, TRUE);
      sprite->SetPosition(pos.left, pos.top);
      g_game->AddSprite(sprite);

      g_score      += 25;
      g_difficulty  = max(80 - (g_score / 20), 20);
   }

   if ( (pHitter == g_carBitmap.get( ) && (pHittee == g_BMissileBitmap.get( ) ||
                                           pHittee == g_JMissileBitmap.get( ) ||
                                           pHittee == g_TMissileBitmap.get( ))) ||
        (pHittee == g_carBitmap.get( ) && (pHitter == g_BMissileBitmap.get( ) ||
                                           pHitter == g_JMissileBitmap.get( ) ||
                                           pHitter == g_TMissileBitmap.get( ))) )
   {
      PlaySoundW((PCWSTR) IDW_LGEXPLODE, g_inst, SND_ASYNC | SND_RESOURCE);

      if ( pHitter == g_carBitmap.get( ) )
      {
         spriteHittee->Kill( );
      }
      else
      {
         spriteHitter->Kill( );
      }

      RECT bounds = { 0, 0, 600, 480 };
      RECT pos    = { };

      if ( pHitter == g_carBitmap.get( ) )
      {
         pos = spriteHitter->GetPosition( );
      }
      else
      {
         pos = spriteHittee->GetPosition( );
      }

      Sprite* sprite = new Sprite(g_largeExplosionBitmap.get( ), bounds);
      sprite->SetNumFrames(8, TRUE);
      sprite->SetPosition(pos.left, pos.top);
      g_game->AddSprite(sprite);

      g_carSprite->SetPosition(300, 405);

      if ( --g_numLives == 0 )
      {
         PlaySoundW((PCWSTR) IDW_GAMEOVER, g_inst, SND_ASYNC | SND_RESOURCE);

         g_gameOver      = TRUE;
         g_gameOverDelay = 150;

         EnableMenuItem(GetMenu(g_game->GetWindow( )), (UINT) MAKEINTRESOURCEW(IDM_GAME_NEW), MF_ENABLED);

         // update the hi scores
         UpdateHiScores( );
      }
   }

   return FALSE;
}

void SpriteDying(Sprite* spriteDying)
{
   if ( spriteDying->GetBitmap( ) == g_BMissileBitmap.get( ) ||
        spriteDying->GetBitmap( ) == g_JMissileBitmap.get( ) ||
        spriteDying->GetBitmap( ) == g_TMissileBitmap.get( ) )
   {
      if ( !g_demo )
      {
         PlaySoundW((PCWSTR) IDW_SMEXPLODE, g_inst, SND_ASYNC | SND_RESOURCE | SND_NOSTOP);
      }

      RECT    bounds = { 0, 0, 600, 450 };
      RECT    pos    = spriteDying-> GetPosition( );
      Sprite* sprite = new Sprite(g_smallExplosionBitmap.get( ), bounds);
      sprite->SetNumFrames(8, TRUE);
      sprite->SetPosition(pos.left, pos.top);
      g_game->AddSprite(sprite);
   }
}

void GameNew( )
{
   g_game->CleanupSprites( );

   g_fireInputDelay = 0;
   g_score          = 0;
   g_numLives       = 3;
   g_difficulty     = 80;
   g_gameOver       = FALSE;

   if ( g_demo )
   {
      for ( int i = 0; i < 6; i++ )
      {
         AddAlien( );
      }
   }
   else
   {
      RECT bounds = { 0, 0, 600, 450 };
      g_carSprite = new Sprite(g_carBitmap.get( ), bounds, BA_WRAP);
      g_carSprite->SetPosition(300, 405);
      g_game->AddSprite(g_carSprite);

      EnableMenuItem(GetMenu(g_game->GetWindow( )), (UINT) MAKEINTRESOURCEW(IDM_GAME_NEW), MF_GRAYED);

      g_game->PlayMIDISong(L"Music.mid");
   }
}

void AddAlien( )
{
   RECT         bounds = { 0, 0, 600, 410 };
   AlienSprite* sprite = NULL;

   switch ( rtk::rand(0, 3) )
   {
   case 0:
      // Blobbo
      sprite = new AlienSprite(g_BlobboBitmap.get( ), bounds, BA_BOUNCE);
      sprite->SetNumFrames(8);
      sprite->SetPosition((rtk::rand(0, 1) == 0) ? 0 : 600, rtk::rand(0, 370));
      sprite->SetVelocity(rtk::rand(-2, 5), rtk::rand(-2, 5));
      break;

   case 1:
      // Jelly
      sprite = new AlienSprite(g_JellyBitmap.get( ), bounds, BA_BOUNCE);
      sprite->SetNumFrames(8);
      sprite->SetPosition(rtk::rand(0, 600), rtk::rand(0, 370));
      sprite->SetVelocity(rtk::rand(-2, 3), rtk::rand(3, 8));
      break;

   case 2:
      // Timmy
      sprite = new AlienSprite(g_TimmyBitmap.get( ), bounds, BA_WRAP);
      sprite->SetNumFrames(8);
      sprite->SetPosition(rtk::rand(0, 600), rtk::rand(0, 370));
      sprite->SetVelocity(rtk::rand(3, 10), 0);
      break;
   }

   g_game->AddSprite(sprite);
}

void UpdateHiScores( )
{
   // see if the current score made the hi score list
   int i;

   for ( i = 0; i < 5; i++ )
   {
      if ( g_score > g_hiScores[ i ] )
      {
         break;
      }
   }

   // insert the current score into the hi score list
   if ( i < 5 )
   {
      for ( int j = 4; j > i; j-- )
      {
         g_hiScores[ j ] = g_hiScores[ j - 1 ];
      }
      g_hiScores[ i ] = g_score;
   }
}

BOOL ReadHiScores( )
{
   // open the hi score file (HiScores.dat)
   std::ifstream fin("HiScores.dat");

   if ( !fin.is_open( ) )
   {
      // the hi score file doesn't exist, so initialize the scores to 0
      for ( int i = 0; i < 5; i++ )
      {
         g_hiScores[ i ] = 0;
      }
      return FALSE;
   }

   // read the scores
   for ( int i = 0; i < 5; i++ )
   {
      // read the score
      std::string s;

      fin >> s;

      // extract each integer score from the score data
      g_hiScores[ i ] = std::stoi(s);
   }

   // close the file
   fin.close( );
   return TRUE;
}

BOOL WriteHiScores( )
{
   // create the hi score file (HiScores.dat) for writing
   std::ofstream fout ("HiScores.dat");

   if ( !fout.is_open( ) )
   {
      // the hi score file couldn't be created, so bail
      return FALSE;
   }

   // write the scores
   for ( int i = 0; i < 5; i++ )
   {
      // format each score for writing
      std::string s = std::format("{:06}", g_hiScores[ i ]);

      // write the score
      fout << s << '\n';
   }

   // close the file
   fout.close( );
   return TRUE;
}