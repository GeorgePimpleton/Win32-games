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

   // create the scrolling background and landscape layer
   HDC dc = GetDC(wnd);

   g_background       = std::make_unique<ScrollingBackground>(256, 256);
   g_BGLandscapeLayer = std::make_unique<BackgroundLayer>(IDB_BG_LANDSCAPE);

   RECT viewport = { 352, 352, 608, 608 };
   g_BGLandscapeLayer->SetViewport(viewport);
   g_background->AddLayer(g_BGLandscapeLayer.get( ));

   // create the scrolling foreground and clouds layer
   g_foreground    = std::make_unique<ScrollingBackground>(256, 256);
   g_FGCloudsLayer = std::make_unique<BackgroundLayer>(IDB_BG_CLOUDS);
   viewport.left   = viewport.top = 64;
   viewport.right  = viewport.bottom = 320;
   g_FGCloudsLayer->SetViewport(viewport);
   g_foreground->AddLayer(g_FGCloudsLayer.get( ));

   // create and load the person bitmap
   g_personBitmap = std::make_unique<Bitmap>(IDB_PERSON);

   // create the person sprite
   RECT bounds = { 115, 112, 26, 32 };
   g_personSprite = std::make_unique<PersonSprite>(g_personBitmap.get( ), bounds, BA_STOP);
   g_personSprite->SetNumFrames(2);
   g_personSprite->SetPosition(115, 112);
   g_game->AddSprite(g_personSprite.get( ));

   // play the background music
   g_game->PlayMIDISong(L"Music.mid");
}

void GameNew( )
{ }

void GameEnd( )
{
   // close the MIDI player for the background music
   g_game->CloseMIDIPlayer( );

   // cleanup the offscreen device context and bitmap
   DeleteObject(g_offscreenBitmap);
   DeleteDC(g_offscreenDC);

   // cleanup the sprites
   g_game->CleanupSprites( );
}

void GameActivate(HWND wnd)
{
   // capture the joystick
   g_game->CaptureJoystick( );

   // resume the background music
   g_game->PlayMIDISong(L"", FALSE);
}

void GameDeactivate(HWND wnd)
{
   // release the joystick
   g_game->ReleaseJoystick( );

   // pause the background music
   g_game->PauseMIDISong( );
}

void GamePaint(HDC dc)
{
   // draw the scrolling background
   g_background->Draw(dc);

   // draw the sprites
   g_game->DrawSprites(dc);

   // Draw the scrolling foreground
   g_foreground->Draw(dc, TRUE); // draw with transparency
}

void GameCycle( )
{
   // update the background
 //  g_pBackground->Update();

   // update the foreground
 //  g_pForeground->Update();

   // update the sprites
   g_game->UpdateSprites( );

   // obtain a device context for repainting the game
   HWND wnd = g_game-> GetWindow( );
   HDC  dc  = GetDC(wnd);

   // paint the game to the offscreen device context
   GamePaint(g_offscreenDC);

   // blit the offscreen bitmap to the game screen
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
      DialogBoxParamW(g_game->GetInstance( ), MAKEINTRESOURCEW(IDD_ABOUT),
                      g_game->GetWindow( ), (DLGPROC) DlgProc, 0L);
      return;
   }
}

void HandleKeys( )
{
   // move the landscape/cloud layers based upon arrow key presses
   if ( g_inputDelay++ > 1 )
   {
      if ( GetAsyncKeyState(VK_LEFT) < 0 )
      {
         // move the wanderer to the left,
         // move the landscape layer & the cloud layer to the right
         UpdateView(SD_RIGHT);
      }
      else if ( GetAsyncKeyState(VK_RIGHT) < 0 )
      {
         // move the wanderer to the right,
         // move the landscape layer & the cloud layer to the left
         UpdateView(SD_LEFT);
      }
      else if ( GetAsyncKeyState(VK_UP) < 0 )
      {
         // move the wanderer up,
         // move the landscape layer & the cloud layer down
         UpdateView(SD_DOWN);
      }
      else if ( GetAsyncKeyState(VK_DOWN) < 0 )
      {
         // move the wanderer down,
         // move the landscape layer & the cloud layer up
         UpdateView(SD_UP);
      }

      // reset the input delay
      g_inputDelay = 0;
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
   if ( ++g_inputDelay > 2 )
   {
      // check horizontal movement
      if ( joyState & JOY_LEFT )
      {
         // move the wanderer to the left,
         // move the landscape layer & the cloud layer to the right
         UpdateView(SD_RIGHT);
      }
      else if ( joyState & JOY_RIGHT )
      {
         // move the wanderer to the right,
         // move the landscape layer & the cloud layer to the left
         UpdateView(SD_LEFT);
      }
      else if ( joyState & JOY_UP )
      {
         // move the wanderer up,
         // move the landscape layer & the cloud layer down
         UpdateView(SD_DOWN);
      }
      else if ( joyState & JOY_DOWN )
      {
         // move the wanderer down,
         // move the landscape layer & the cloud layer up
         UpdateView(SD_UP);
      }

      // reset the input delay
      g_inputDelay = 0;
   }
}

void UpdateView(SCROLLDIR sd)
{
   // make the person walk
   g_personSprite->Walk( );

   // move the landscape layer
   g_BGLandscapeLayer->SetSpeed(16);
   g_BGLandscapeLayer->SetDirection(sd);
   g_BGLandscapeLayer->Update( );
   g_BGLandscapeLayer->SetSpeed(0);

   // move the cloud layer
   g_FGCloudsLayer->SetSpeed(4);
   g_FGCloudsLayer->SetDirection(sd);
   g_FGCloudsLayer->Update( );
   g_FGCloudsLayer->SetSpeed(0);
}

BOOL SpriteCollision(Sprite*, Sprite*)
{
   return FALSE;
}

void SpriteDying(Sprite*)
{ }