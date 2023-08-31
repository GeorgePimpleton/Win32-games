#include "Henway.hpp"

HRESULT GameInitialize(HINSTANCE inst)
{
   g_game = new GameEngine(inst, L"Henway", L"Example Game: Henway",
                           IDI_ICON, IDI_ICON_SM, 465, 400);

   if ( NULL == g_game )
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

   HINSTANCE inst = GetModuleHandleW(NULL);

   g_highwayBitmap     = new Bitmap(IDB_HIGHWAY, inst);
   g_chickenBitmap     = new Bitmap(IDB_CHICKEN, inst);
   g_carBitmaps[ 0 ]   = new Bitmap(IDB_CAR1, inst);
   g_carBitmaps[ 1 ]   = new Bitmap(IDB_CAR2, inst);
   g_carBitmaps[ 2 ]   = new Bitmap(IDB_CAR3, inst);
   g_carBitmaps[ 3 ]   = new Bitmap(IDB_CAR4, inst);
   g_chickenHeadBitmap = new Bitmap(IDB_CHICKENHEAD, inst);

   NewGame( );
}

void GameEnd( )
{
   DeleteObject(g_offscreenBitmap);
   DeleteDC(g_offscreenDC);

   delete g_highwayBitmap;
   delete g_chickenBitmap;

   for ( int i = 0; i < 4; i++ )
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
}

void GameDeactivate(HWND hWindow)
{
   g_game->ReleaseJoystick( );
}

void GamePaint(HDC dc)
{
   g_highwayBitmap->Draw(dc, 0, 0);

   g_game->DrawSprites(dc);

   for ( UINT i = 0; i < g_numLives; i++ )
   {
      g_chickenHeadBitmap->Draw(dc, 406 + (g_chickenHeadBitmap->GetWidth( ) * i), 382, TRUE);
   }
}

void GameCycle( )
{
   if ( FALSE == g_gameOver )
   {
      g_game->UpdateSprites( );

      HWND wnd = g_game-> GetWindow( );
      HDC  dc  = GetDC(wnd);

      GamePaint(g_offscreenDC);

      BitBlt(dc, 0, 0, g_game->GetWidth( ), g_game->GetHeight( ), g_offscreenDC, 0, 0, SRCCOPY);

      ReleaseDC(wnd, dc);
   }
}

void GameMenu(WPARAM wParam)
{
   switch ( LOWORD(wParam) )
   {
   case IDM_GAME_NEW:
      // starts a new game
      if ( g_gameOver )
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
   if ( g_gameOver )
   {
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

   if ( g_gameOver && (joyState & JOY_FIRE1) )
   {
      NewGame( );
   }
}

BOOL SpriteCollision(Sprite* spriteHitter, Sprite* spriteHittee)
{
   if ( spriteHittee == g_chickenSprite )
   {
      g_chickenSprite->SetPosition(4, 175);

      if ( --g_numLives > 0 )
      {
         MessageBoxW(g_game->GetWindow( ), L"Ouch!", L"Henway", MB_OK);
      }
      else
      {
         WCHAR szText[ 64 ];
         wsprintfW(szText, L"Game Over! You scored %d points.", g_score);
         MessageBoxW(g_game->GetWindow( ), szText, L"Henway", MB_OK);
         g_gameOver = TRUE;
      }

      return FALSE;
   }

   return TRUE;
}

void NewGame( )
{
   g_game->CleanupSprites( );

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

   // initialize the remaining global variables
   g_inputDelay = 0;
   g_numLives   = 3;
   g_score      = 0;
   g_gameOver   = FALSE;
}

void MoveChicken(int xDistance, int yDistance)
{
   g_chickenSprite->OffsetPosition(xDistance, yDistance);

   if ( g_chickenSprite->GetPosition( ).left > 400 )
   {
      g_chickenSprite->SetPosition(4, 175);
      g_score += 150;
      MessageBoxW(g_game->GetWindow( ), L"You made it!", L"Henway", MB_OK);
   }
}