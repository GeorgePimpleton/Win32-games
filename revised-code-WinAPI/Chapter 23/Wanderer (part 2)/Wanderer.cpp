#include "Wanderer.hpp"

BOOL GameInitialize(HINSTANCE inst)
{
   g_pGame = new GameEngine(inst, L"Wanderer", L"Wanderer",
                            IDI_ICON, IDI_ICON_SM, 256, 256);

   if ( g_pGame == NULL )
   {
      return FALSE;
   }

   g_pGame->SetFrameRate(30);

   g_pGame->InitJoystick( );

   g_inst = inst;

   return TRUE;
}

void GameStart(HWND wnd)
{
   g_hOffscreenDC = CreateCompatibleDC(GetDC(wnd));
   g_hOffscreenBitmap = CreateCompatibleBitmap(GetDC(wnd),
                                               g_pGame->GetWidth( ), g_pGame->GetHeight( ));

   SelectObject(g_hOffscreenDC, g_hOffscreenBitmap);

   // Create the scrolling background and landscape layer
   HDC dc = GetDC(wnd);
   g_pBackground = new ScrollingBackground(256, 256);
   g_pBGLandscapeLayer = new BackgroundLayer(dc, IDB_BG_LANDSCAPE, g_inst);

   RECT rcViewport = { 352, 352, 608, 608 };
   g_pBGLandscapeLayer->SetViewport(rcViewport);
   g_pBackground->AddLayer(g_pBGLandscapeLayer);

   // Create the scrolling foreground and clouds layer
   g_pForeground = new ScrollingBackground(256, 256);
   g_pFGCloudsLayer = new BackgroundLayer(dc, IDB_BG_CLOUDS, g_inst);
   rcViewport.left = rcViewport.top = 64;
   rcViewport.right = rcViewport.bottom = 320;
   g_pFGCloudsLayer->SetViewport(rcViewport);
   g_pForeground->AddLayer(g_pFGCloudsLayer);

   // Create and load the person bitmap
   g_pPersonBitmap = new Bitmap(dc, IDB_PERSON, g_inst);

   // Create the person sprite
   RECT rcBounds = { 115, 112, 26, 32 };
   g_pPersonSprite = new PersonSprite(g_pPersonBitmap, rcBounds, BA_STOP);
   g_pPersonSprite->SetNumFrames(2);
   g_pPersonSprite->SetPosition(115, 112);
   g_pGame->AddSprite(g_pPersonSprite);

   // Play the background music
   g_pGame->PlayMIDISong(TEXT("Music.mid"));
}

void GameNew( )
{ }

void GameEnd( )
{
   // Close the MIDI player for the background music
   g_pGame->CloseMIDIPlayer( );

   // Cleanup the offscreen device context and bitmap
   DeleteObject(g_hOffscreenBitmap);
   DeleteDC(g_hOffscreenDC);

   // Cleanup the bitmaps
   delete g_pPersonBitmap;

   // Cleanup the scrolling background and landscape layer
   delete g_pBackground;
   delete g_pBGLandscapeLayer;

   // Cleanup the scrolling foreground and clouds layer
   delete g_pForeground;
   delete g_pFGCloudsLayer;

   // Cleanup the sprites
   g_pGame->CleanupSprites( );

   // Cleanup the game engine
   delete g_pGame;
}

void GameActivate(HWND wnd)
{
   // Capture the joystick
   g_pGame->CaptureJoystick( );

   // Resume the background music
   g_pGame->PlayMIDISong(L"", FALSE);
}

void GameDeactivate(HWND wnd)
{
   // Release the joystick
   g_pGame->ReleaseJoystick( );

   // Pause the background music
   g_pGame->PauseMIDISong( );
}

void GamePaint(HDC dc)
{
   // Draw the scrolling background
   g_pBackground->Draw(dc);

   // Draw the sprites
   g_pGame->DrawSprites(dc);

   // Draw the scrolling foreground
   g_pForeground->Draw(dc, TRUE); // draw with transparency
}

void GameCycle( )
{
   // Update the background
 //  g_pBackground->Update();

   // Update the foreground
 //  g_pForeground->Update();

   // Update the sprites
   g_pGame->UpdateSprites( );

   // Obtain a device context for repainting the game
   HWND  wnd = g_pGame->GetWindow( );
   HDC   dc = GetDC(wnd);

   // Paint the game to the offscreen device context
   GamePaint(g_hOffscreenDC);

   // Blit the offscreen bitmap to the game screen
   BitBlt(dc, 0, 0, g_pGame->GetWidth( ), g_pGame->GetHeight( ),
          g_hOffscreenDC, 0, 0, SRCCOPY);

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
      DialogBoxW(g_pGame->GetInstance( ), MAKEINTRESOURCEW(IDD_ABOUT), g_pGame->GetWindow( ), (DLGPROC) DlgProc);
      return;
   }
}

void HandleKeys( )
{
   // Move the landscape/cloud layers based upon arrow key presses
   if ( g_iInputDelay++ > 1 )
   {
      if ( GetAsyncKeyState(VK_LEFT) < 0 )
      {
         // Make the person walk
         g_pPersonSprite->Walk( );

         // Move the landscape layer to the right
         g_pBGLandscapeLayer->SetSpeed(16);
         g_pBGLandscapeLayer->SetDirection(SD_RIGHT);
         g_pBGLandscapeLayer->Update( );
         g_pBGLandscapeLayer->SetSpeed(0);

         // Move the cloud layer to the right
         g_pFGCloudsLayer->SetSpeed(4);
         g_pFGCloudsLayer->SetDirection(SD_RIGHT);
         g_pFGCloudsLayer->Update( );
         g_pFGCloudsLayer->SetSpeed(0);
      }
      else if ( GetAsyncKeyState(VK_RIGHT) < 0 )
      {
         // Make the person walk
         g_pPersonSprite->Walk( );

         // Move the landscape layer to the left
         g_pBGLandscapeLayer->SetSpeed(16);
         g_pBGLandscapeLayer->SetDirection(SD_LEFT);
         g_pBGLandscapeLayer->Update( );
         g_pBGLandscapeLayer->SetSpeed(0);

         // Move the cloud layer to the left
         g_pFGCloudsLayer->SetSpeed(4);
         g_pFGCloudsLayer->SetDirection(SD_LEFT);
         g_pFGCloudsLayer->Update( );
         g_pFGCloudsLayer->SetSpeed(0);
      }
      else if ( GetAsyncKeyState(VK_UP) < 0 )
      {
         // Make the person walk
         g_pPersonSprite->Walk( );

         // Move the landscape layer down
         g_pBGLandscapeLayer->SetSpeed(16);
         g_pBGLandscapeLayer->SetDirection(SD_DOWN);
         g_pBGLandscapeLayer->Update( );
         g_pBGLandscapeLayer->SetSpeed(0);

         // Move the cloud layer down
         g_pFGCloudsLayer->SetSpeed(4);
         g_pFGCloudsLayer->SetDirection(SD_DOWN);
         g_pFGCloudsLayer->Update( );
         g_pFGCloudsLayer->SetSpeed(0);
      }
      else if ( GetAsyncKeyState(VK_DOWN) < 0 )
      {
         // Make the person walk
         g_pPersonSprite->Walk( );

         // Move the landscape layer up
         g_pBGLandscapeLayer->SetSpeed(16);
         g_pBGLandscapeLayer->SetDirection(SD_UP);
         g_pBGLandscapeLayer->Update( );
         g_pBGLandscapeLayer->SetSpeed(0);

         // Move the cloud layer up
         g_pFGCloudsLayer->SetSpeed(4);
         g_pFGCloudsLayer->SetDirection(SD_UP);
         g_pFGCloudsLayer->Update( );
         g_pFGCloudsLayer->SetSpeed(0);
      }

      // Reset the input delay
      g_iInputDelay = 0;
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
   if ( ++g_iInputDelay > 2 )
   {
      // Check horizontal movement
      if ( joyState & JOY_LEFT )
      {
         // Make the person walk
         g_pPersonSprite->Walk( );

         // Move the landscape layer to the right
         g_pBGLandscapeLayer->SetSpeed(16);
         g_pBGLandscapeLayer->SetDirection(SD_RIGHT);
         g_pBGLandscapeLayer->Update( );
         g_pBGLandscapeLayer->SetSpeed(0);

         // Move the cloud layer to the right
         g_pFGCloudsLayer->SetSpeed(4);
         g_pFGCloudsLayer->SetDirection(SD_RIGHT);
         g_pFGCloudsLayer->Update( );
         g_pFGCloudsLayer->SetSpeed(0);
      }
      else if ( joyState & JOY_RIGHT )
      {
         // Make the person walk
         g_pPersonSprite->Walk( );

         // Move the landscape layer to the left
         g_pBGLandscapeLayer->SetSpeed(16);
         g_pBGLandscapeLayer->SetDirection(SD_LEFT);
         g_pBGLandscapeLayer->Update( );
         g_pBGLandscapeLayer->SetSpeed(0);

         // Move the cloud layer to the left
         g_pFGCloudsLayer->SetSpeed(4);
         g_pFGCloudsLayer->SetDirection(SD_LEFT);
         g_pFGCloudsLayer->Update( );
         g_pFGCloudsLayer->SetSpeed(0);
      }
      else if ( joyState & JOY_UP )
      {
         // Make the person walk
         g_pPersonSprite->Walk( );

         // Move the landscape layer down
         g_pBGLandscapeLayer->SetSpeed(16);
         g_pBGLandscapeLayer->SetDirection(SD_DOWN);
         g_pBGLandscapeLayer->Update( );
         g_pBGLandscapeLayer->SetSpeed(0);

         // Move the cloud layer down
         g_pFGCloudsLayer->SetSpeed(4);
         g_pFGCloudsLayer->SetDirection(SD_DOWN);
         g_pFGCloudsLayer->Update( );
         g_pFGCloudsLayer->SetSpeed(0);
      }
      else if ( joyState & JOY_DOWN )
      {
         // Make the person walk
         g_pPersonSprite->Walk( );

         // Move the landscape layer up
         g_pBGLandscapeLayer->SetSpeed(16);
         g_pBGLandscapeLayer->SetDirection(SD_UP);
         g_pBGLandscapeLayer->Update( );
         g_pBGLandscapeLayer->SetSpeed(0);

         // Move the cloud layer up
         g_pFGCloudsLayer->SetSpeed(4);
         g_pFGCloudsLayer->SetDirection(SD_UP);
         g_pFGCloudsLayer->Update( );
         g_pFGCloudsLayer->SetSpeed(0);
      }

      // Reset the input delay
      g_iInputDelay = 0;
   }
}

BOOL SpriteCollision(Sprite* spriteHitter, Sprite* spriteHittee)
{
   return FALSE;
}

void SpriteDying(Sprite* spriteDying)
{ }
