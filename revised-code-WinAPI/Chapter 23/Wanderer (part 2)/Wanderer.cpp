#include "Wanderer.hpp"

BOOL GameInitialize(HINSTANCE inst)
{
   g_game = std::make_unique<GameEngine>(inst, L"Wanderer", L"Wanderer",
                                         IDI_ICON, IDI_ICON_SM, 256, 256);

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
   g_offscreenDC     = CreateCompatibleDC(GetDC(wnd));
   g_offscreenBitmap = CreateCompatibleBitmap(GetDC(wnd),
                                              g_game->GetWidth( ), g_game->GetHeight( ));

   SelectObject(g_offscreenDC, g_offscreenBitmap);

   // Create the scrolling background and landscape layer
   HDC dc = GetDC(wnd);
   g_background = std::make_unique<ScrollingBackground>(256, 256);
   g_BGLandscapeLayer = std::make_unique<BackgroundLayer>(IDB_BG_LANDSCAPE, g_inst);

   RECT viewport = { 352, 352, 608, 608 };
   g_BGLandscapeLayer->SetViewport(viewport);
   g_background->AddLayer(g_BGLandscapeLayer.get( ));

   // Create the scrolling foreground and clouds layer
   g_foreground = std::make_unique<ScrollingBackground>(256, 256);
   g_FGCloudsLayer = std::make_unique<BackgroundLayer>(IDB_BG_CLOUDS, g_inst);
   viewport.left = viewport.top = 64;
   viewport.right = viewport.bottom = 320;
   g_FGCloudsLayer->SetViewport(viewport);
   g_foreground->AddLayer(g_FGCloudsLayer.get( ));

   // Create and load the person bitmap
   g_personBitmap = std::make_unique<Bitmap>(IDB_PERSON, g_inst);

   // Create the person sprite
   RECT bounds = { 115, 112, 26, 32 };
   g_personSprite = std::make_unique<PersonSprite>(g_personBitmap.get( ), bounds, BA_STOP);
   g_personSprite->SetNumFrames(2);
   g_personSprite->SetPosition(115, 112);
   g_game->AddSprite(g_personSprite.get( ));

   // Play the background music
   g_game->PlayMIDISong(L"Music.mid");
}

void GameNew( )
{ }

void GameEnd( )
{
   // Close the MIDI player for the background music
   g_game->CloseMIDIPlayer( );

   // Cleanup the offscreen device context and bitmap
   DeleteObject(g_offscreenBitmap);
   DeleteDC(g_offscreenDC);

   // Cleanup the sprites
   g_game->CleanupSprites( );
}

void GameActivate(HWND wnd)
{
   // Capture the joystick
   g_game->CaptureJoystick( );

   // Resume the background music
   g_game->PlayMIDISong(L"", FALSE);
}

void GameDeactivate(HWND wnd)
{
   // Release the joystick
   g_game->ReleaseJoystick( );

   // Pause the background music
   g_game->PauseMIDISong( );
}

void GamePaint(HDC dc)
{
   // Draw the scrolling background
   g_background->Draw(dc);

   // Draw the sprites
   g_game->DrawSprites(dc);

   // Draw the scrolling foreground
   g_foreground->Draw(dc, TRUE); // draw with transparency
}

void GameCycle( )
{
   // Update the background
 //  g_pBackground->Update();

   // Update the foreground
 //  g_pForeground->Update();

   // Update the sprites
   g_game->UpdateSprites( );

   // Obtain a device context for repainting the game
   HWND  wnd = g_game->GetWindow( );
   HDC   dc = GetDC(wnd);

   // Paint the game to the offscreen device context
   GamePaint(g_offscreenDC);

   // Blit the offscreen bitmap to the game screen
   BitBlt(dc, 0, 0, g_game->GetWidth( ), g_game->GetHeight( ),
          g_offscreenDC, 0, 0, SRCCOPY);

   // Cleanup
   ReleaseDC(wnd, dc);
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
   // Move the landscape/cloud layers based upon arrow key presses
   if ( g_inputDelay++ > 1 )
   {
      if ( GetAsyncKeyState(VK_LEFT) < 0 )
      {
         // Make the person walk
         g_personSprite->Walk( );

         // Move the landscape layer to the right
         g_BGLandscapeLayer->SetSpeed(16);
         g_BGLandscapeLayer->SetDirection(SD_RIGHT);
         g_BGLandscapeLayer->Update( );
         g_BGLandscapeLayer->SetSpeed(0);

         // Move the cloud layer to the right
         g_FGCloudsLayer->SetSpeed(4);
         g_FGCloudsLayer->SetDirection(SD_RIGHT);
         g_FGCloudsLayer->Update( );
         g_FGCloudsLayer->SetSpeed(0);
      }
      else if ( GetAsyncKeyState(VK_RIGHT) < 0 )
      {
         // Make the person walk
         g_personSprite->Walk( );

         // Move the landscape layer to the left
         g_BGLandscapeLayer->SetSpeed(16);
         g_BGLandscapeLayer->SetDirection(SD_LEFT);
         g_BGLandscapeLayer->Update( );
         g_BGLandscapeLayer->SetSpeed(0);

         // Move the cloud layer to the left
         g_FGCloudsLayer->SetSpeed(4);
         g_FGCloudsLayer->SetDirection(SD_LEFT);
         g_FGCloudsLayer->Update( );
         g_FGCloudsLayer->SetSpeed(0);
      }
      else if ( GetAsyncKeyState(VK_UP) < 0 )
      {
         // Make the person walk
         g_personSprite->Walk( );

         // Move the landscape layer down
         g_BGLandscapeLayer->SetSpeed(16);
         g_BGLandscapeLayer->SetDirection(SD_DOWN);
         g_BGLandscapeLayer->Update( );
         g_BGLandscapeLayer->SetSpeed(0);

         // Move the cloud layer down
         g_FGCloudsLayer->SetSpeed(4);
         g_FGCloudsLayer->SetDirection(SD_DOWN);
         g_FGCloudsLayer->Update( );
         g_FGCloudsLayer->SetSpeed(0);
      }
      else if ( GetAsyncKeyState(VK_DOWN) < 0 )
      {
         // Make the person walk
         g_personSprite->Walk( );

         // Move the landscape layer up
         g_BGLandscapeLayer->SetSpeed(16);
         g_BGLandscapeLayer->SetDirection(SD_UP);
         g_BGLandscapeLayer->Update( );
         g_BGLandscapeLayer->SetSpeed(0);

         // Move the cloud layer up
         g_FGCloudsLayer->SetSpeed(4);
         g_FGCloudsLayer->SetDirection(SD_UP);
         g_FGCloudsLayer->Update( );
         g_FGCloudsLayer->SetSpeed(0);
      }

      // Reset the input delay
      g_inputDelay = 0;
   }
}

void MouseButtonDown(int x, int y, BOOL left)
{ }

void MouseButtonUp(int x, int y, BOOL left)
{ }

void MouseMove(int x, int y)
{ }

void HandleJoystick(JOYSTATE joyState)
{
   if ( ++g_inputDelay > 2 )
   {
      // Check horizontal movement
      if ( joyState & JOY_LEFT )
      {
         // Make the person walk
         g_personSprite->Walk( );

         // Move the landscape layer to the right
         g_BGLandscapeLayer->SetSpeed(16);
         g_BGLandscapeLayer->SetDirection(SD_RIGHT);
         g_BGLandscapeLayer->Update( );
         g_BGLandscapeLayer->SetSpeed(0);

         // Move the cloud layer to the right
         g_FGCloudsLayer->SetSpeed(4);
         g_FGCloudsLayer->SetDirection(SD_RIGHT);
         g_FGCloudsLayer->Update( );
         g_FGCloudsLayer->SetSpeed(0);
      }
      else if ( joyState & JOY_RIGHT )
      {
         // Make the person walk
         g_personSprite->Walk( );

         // Move the landscape layer to the left
         g_BGLandscapeLayer->SetSpeed(16);
         g_BGLandscapeLayer->SetDirection(SD_LEFT);
         g_BGLandscapeLayer->Update( );
         g_BGLandscapeLayer->SetSpeed(0);

         // Move the cloud layer to the left
         g_FGCloudsLayer->SetSpeed(4);
         g_FGCloudsLayer->SetDirection(SD_LEFT);
         g_FGCloudsLayer->Update( );
         g_FGCloudsLayer->SetSpeed(0);
      }
      else if ( joyState & JOY_UP )
      {
         // Make the person walk
         g_personSprite->Walk( );

         // Move the landscape layer down
         g_BGLandscapeLayer->SetSpeed(16);
         g_BGLandscapeLayer->SetDirection(SD_DOWN);
         g_BGLandscapeLayer->Update( );
         g_BGLandscapeLayer->SetSpeed(0);

         // Move the cloud layer down
         g_FGCloudsLayer->SetSpeed(4);
         g_FGCloudsLayer->SetDirection(SD_DOWN);
         g_FGCloudsLayer->Update( );
         g_FGCloudsLayer->SetSpeed(0);
      }
      else if ( joyState & JOY_DOWN )
      {
         // Make the person walk
         g_personSprite->Walk( );

         // Move the landscape layer up
         g_BGLandscapeLayer->SetSpeed(16);
         g_BGLandscapeLayer->SetDirection(SD_UP);
         g_BGLandscapeLayer->Update( );
         g_BGLandscapeLayer->SetSpeed(0);

         // Move the cloud layer up
         g_FGCloudsLayer->SetSpeed(4);
         g_FGCloudsLayer->SetDirection(SD_UP);
         g_FGCloudsLayer->Update( );
         g_FGCloudsLayer->SetSpeed(0);
      }

      // Reset the input delay
      g_inputDelay = 0;
   }
}

BOOL SpriteCollision(Sprite* spriteHitter, Sprite* spriteHittee)
{
   return FALSE;
}

void SpriteDying(Sprite* spriteDying)
{ }
