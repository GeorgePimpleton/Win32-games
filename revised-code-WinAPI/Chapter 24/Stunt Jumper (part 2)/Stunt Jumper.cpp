#include "Stunt Jumper.hpp"

BOOL GameInitialize(HINSTANCE inst)
{
   g_game = std::make_unique<GameEngine>(inst, L"Stunt Jumper", L"Example Game: Stunt Jumper",
                                         IDI_ICON, IDI_ICON_SM, 750, 250);

   if ( g_game == NULL )
   {
      return FALSE;
   }

   g_game->SetFrameRate(30);

   g_game->InitJoystick( );

   g_inst = inst;

   return TRUE;
}

void GameStart(HWND wnd)
{
   rtk::srand( );

   g_offscreenDC = CreateCompatibleDC(GetDC(wnd));
   g_offscreenBitmap = CreateCompatibleBitmap(GetDC(wnd),
                                              g_game->GetWidth( ), g_game->GetHeight( ));

   SelectObject(g_offscreenDC, g_offscreenBitmap);

   g_splashBitmap    = std::make_unique <Bitmap>(IDB_SPLASH);
   g_pJumperBitmap   = std::make_unique<Bitmap>(IDB_JUMPER);
   g_busBitmap       = std::make_unique<Bitmap>(IDB_BUS);
   g_rampBitmap[ 0 ] = std::make_unique<Bitmap>(IDB_RAMPLEFT);
   g_rampBitmap[ 1 ] = std::make_unique<Bitmap>(IDB_RAMPRIGHT);
   g_gameOverBitmap  = std::make_unique<Bitmap>(IDB_GAMEOVER);

   g_background = std::make_unique<ScrollingBackground>(750, 250);
   g_BGSkyLayer = std::make_unique<BackgroundLayer>(IDB_BG_SKY, 1, SD_LEFT);
   g_background->AddLayer(g_BGSkyLayer.get( ));
   g_BGMountainsLayer = std::make_unique<BackgroundLayer>(IDB_BG_MOUNTAINS, 2, SD_LEFT);
   g_background->AddLayer(g_BGMountainsLayer.get( ));
   g_BGTreesLayer = std::make_unique<BackgroundLayer>(IDB_BG_TREES, 3, SD_LEFT);
   g_background->AddLayer(g_BGTreesLayer.get( ));
   g_BGRoadLayer = std::make_unique<BackgroundLayer>(IDB_BG_ROAD);
   g_background->AddLayer(g_BGRoadLayer.get( ));

   g_splash   = TRUE;
   g_gameOver = TRUE;
}

void GameEnd( )
{
   g_game->CloseMIDIPlayer( );

   DeleteObject(g_offscreenBitmap);
   DeleteDC(g_offscreenDC);

   g_game->CleanupSprites( );
}

void GameActivate(HWND wnd)
{
   g_game->CaptureJoystick( );

   if ( !g_splash )
   {
      g_game->PlayMIDISong(TEXT(""), FALSE);
   }
}

void GameDeactivate(HWND wnd)
{
   g_game->ReleaseJoystick( );

   if ( !g_splash )
   {
      g_game->PauseMIDISong( );
   }
}

void GamePaint(HDC dc)
{
   g_background->Draw(dc, TRUE);

   if ( g_splash )
   {
      g_splashBitmap->Draw(dc, 175, 15, TRUE);
   }
   else
   {
      g_game->DrawSprites(dc);

      if ( g_gameOver )
      {
         g_gameOverBitmap->Draw(dc, 175, 15, FALSE);
      }
   }
}

void GameCycle( )
{
   if ( !g_gameOver )
   {
      g_background->Update( );

      g_game->UpdateSprites( );

      HWND wnd = g_game-> GetWindow( );
      HDC  dc  = GetDC(wnd);

      GamePaint(g_offscreenDC);

      BitBlt(dc, 0, 0, g_game->GetWidth( ), g_game->GetHeight( ),
             g_offscreenDC, 0, 0, SRCCOPY);

      ReleaseDC(wnd, dc);

      // see if the motorcycle has crossed the screen
      RECT& rc = g_jumperSprite->GetPosition( );

      if ( rc.right > g_game->GetWidth( ) )
      {
         NewJump(rtk::rand(1, 7));
      }
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
      DialogBoxParamW(g_game->GetInstance( ), MAKEINTRESOURCEW(IDD_ABOUT),
                      g_game->GetWindow( ), (DLGPROC) DlgProc, 0L);
      return;
   }
}

void HandleKeys( )
{
   if ( !g_gameOver )
   {
      // move the jumper based upon left/right key presses
      POINT velocity = g_jumperSprite->GetVelocity( );

      if ( g_inputDelay++ > 1 )
      {
         if ( GetAsyncKeyState(VK_LEFT) < 0 )
         {
            // play the brake sound
            PlaySoundW((PCWSTR) IDW_BRAKES, g_inst, SND_ASYNC |
                      SND_RESOURCE);

            // decrease speed
            g_jumperSprite->DecreaseSpeed( );

            // reset the input delay
            g_inputDelay = 0;
         }
         else if ( GetAsyncKeyState(VK_RIGHT) < 0 )
         {
            // play the engine sound
            PlaySoundW((PCWSTR) IDW_ENGINE, g_inst, SND_ASYNC |
                      SND_RESOURCE);

            // increase speed
            g_jumperSprite->IncreaseSpeed( );

            // reset the input delay
            g_inputDelay = 0;
         }
      }
   }

   // start a new game based upon an enter (return) key press
   if ( GetAsyncKeyState(VK_RETURN) < 0 )
   {
      if ( g_splash )
      {
         // start a new game without the splash screen
         g_splash = FALSE;
         GameNew( );
      }
      else if ( g_gameOver )
      {
         // start a new game
         GameNew( );
      }
   }
}

void MouseButtonDown(int, int, BOOL)
{ }

void MouseButtonUp(int, int, BOOL)
{ }

void MouseMove(int, int)
{ }

void HandleJoystick(JOYSTATE joyState)
{
   if ( !g_gameOver )
   {
      // move the jumper based upon left/right joystick movement
      POINT velocity = g_jumperSprite->GetVelocity( );

      if ( g_inputDelay++ > 1 )
      {
         if ( joyState & JOY_LEFT )
         {
            // play the brake sound
            PlaySoundW((PCWSTR) IDW_BRAKES, g_inst, SND_ASYNC | SND_RESOURCE);

            // decrease speed
            g_jumperSprite->DecreaseSpeed( );

            // reset the input delay
            g_inputDelay = 0;
         }
         else if ( joyState & JOY_RIGHT )
         {
            // play the engine sound
            PlaySoundW((PCWSTR) IDW_ENGINE, g_inst, SND_ASYNC | SND_RESOURCE);

            // increase speed
            g_jumperSprite->IncreaseSpeed( );

            // reset the input delay
            g_inputDelay = 0;
         }
      }
   }

   // start a new game based upon the first joystick button
   if ( joyState & JOY_FIRE1 )
      if ( g_splash )
      {
         // start a new game without the splash screen
         g_splash = FALSE;
         GameNew( );
      }
      else if ( g_gameOver )
      {
         // start a new game
         GameNew( );
      }
}

BOOL SpriteCollision(Sprite* spriteHitter, Sprite* spriteHittee)
{
   Bitmap* hitter = spriteHitter->GetBitmap( );
   Bitmap* hittee = spriteHittee->GetBitmap( );

   // only check for collision between sprites that aren't hidden
   if ( !spriteHitter->IsHidden( ) && !spriteHittee->IsHidden( ) )
   {
      // see if the motorcycle has hit the launch ramp
      if ( (hitter == g_pJumperBitmap.get( )) && (hittee == g_rampBitmap[ 0 ].get( )) )
      {
         // start jumping
         g_jumperSprite->StartJumping( );
      }
      // see if the motorcycle has hit the landing ramp
      else if ( (hitter == g_pJumperBitmap.get( )) && (hittee == g_rampBitmap[ 1 ].get( )) )
      {
         if ( !g_jumperSprite->HasLandedSafely( ) )
         {
            // play the celebration sound
            PlaySoundW((PCWSTR) IDW_CELEBRATION, g_inst, SND_ASYNC | SND_RESOURCE);

            // indicate that the motorcycle landed safely
            g_jumperSprite->LandedSafely( );
         }
      }
      // see if the motorcycle has hit a bus
      else if ( (hitter == g_pJumperBitmap.get( )) && (hittee == g_busBitmap.get( )) )
      {
         // play the crash sound
         PlaySoundW((PCWSTR) IDW_CRASH, g_inst, SND_ASYNC | SND_RESOURCE);

         // end the game
         g_gameOver = TRUE;
      }
   }

   return FALSE;
}

void SpriteDying(Sprite*)
{ }

void GameNew( )
{
   // clear the sprites
   g_game->CleanupSprites( );

   // initialize the game variables
   g_inputDelay = 0;
   g_gameOver   = FALSE;

   // create the ramp and bus sprites
   RECT bounds = { 0, 0, 750, 250 };

   g_launchRampSprite.release( );
   g_launchRampSprite = std::make_unique<Sprite>(g_rampBitmap[ 0 ].get( ), bounds);
   g_game->AddSprite(g_launchRampSprite.get( ));

   g_landingRampSprite.release( );
   g_landingRampSprite = std::make_unique<Sprite>(g_rampBitmap[ 1 ].get( ), bounds);
   g_game->AddSprite(g_landingRampSprite.get( ));

   for ( int i = 0; i < 7; i++ )
   {
      g_busSprite[ i ].release( );
      g_busSprite[ i ] = std::make_unique<Sprite>(g_busBitmap.get( ), bounds);
      g_game->AddSprite(g_busSprite[ i ].get( ));
   }

   // create the motorcycle jumper sprite
   g_jumperSprite.release( );
   g_jumperSprite = std::make_unique<MotorcycleSprite>(g_pJumperBitmap.get( ), bounds, BA_WRAP);
   g_jumperSprite->SetNumFrames(13);
   g_jumperSprite->SetVelocity(4, 0);
   g_jumperSprite->SetPosition(0, 200);
   g_game->AddSprite(g_jumperSprite.get( ));

   // setup the first jump (maximum of 3 buses)
   NewJump(rtk::rand(1, 3));

   // play the background music
   g_game->PlayMIDISong(L"Music.mid");
}

void NewJump(int numBuses)
{
   // set the position of the launch ramp
   int xStart = (g_game->GetWidth( ) / 2) - (numBuses * 40);

   g_launchRampSprite->SetPosition(xStart, 215);

   // set the positions and visibility of the buses
   for ( int i = 0; i < 7; i++ )
   {
      if ( i < numBuses )
      {
         // arrange and show these buses
         g_busSprite[ i ]->SetPosition(xStart + g_rampBitmap[ 0 ]->GetWidth( ) +
                                       5 + i * g_busBitmap->GetWidth( ), 200);
         g_busSprite[ i ]->SetHidden(FALSE);
      }
      else
      {
         // hide these buses
         g_busSprite[ i ]->SetPosition(0, 0);
         g_busSprite[ i ]->SetHidden(TRUE);
      }
   }

   // set the position of the landing ramp
   g_landingRampSprite->SetPosition(xStart + g_rampBitmap[ 0 ]->GetWidth( ) +
                                    5 + numBuses * g_busBitmap->GetWidth( ) + 5, 215);
}