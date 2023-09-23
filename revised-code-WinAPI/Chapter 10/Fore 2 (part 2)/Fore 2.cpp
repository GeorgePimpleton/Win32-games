#include "Fore 2.hpp"

HRESULT GameInitialize(HINSTANCE inst)
{
   g_game = std::make_unique<GameEngine>(inst, L"Fore 2", L"Fore 2: Managing a World Sprites",
                                         IDI_ICON, IDI_ICON_SM, 600, 400);

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

   g_offscreenDC     = CreateCompatibleDC(GetDC(wnd));
   g_offscreenBitmap = CreateCompatibleBitmap(GetDC(wnd), g_game-> GetWidth( ), g_game-> GetHeight( ));

   SelectObject(g_offscreenDC, g_offscreenBitmap);

   g_forestBitmap   = std::make_unique<Bitmap>(IDB_FOREST, GetModuleHandle(NULL));
   g_golfBallBitmap = std::make_unique<Bitmap>(IDB_GOLFBALL, GetModuleHandle(NULL));

   RECT    bounds = { 0, 0, 600, 400 };
   Sprite* sprite;

   sprite = new Sprite(g_golfBallBitmap.get(), bounds, BA_WRAP);
   sprite->SetVelocity(5, 3);
   g_game->AddSprite(sprite);

   sprite = new Sprite(g_golfBallBitmap.get(), bounds, BA_WRAP);
   sprite->SetVelocity(3, 2);
   g_game->AddSprite(sprite);

   bounds.left = 265; bounds.right = 500; bounds.bottom = 335;
   sprite = new Sprite(g_golfBallBitmap.get(), bounds, BA_BOUNCE);
   sprite->SetVelocity(-6, 5);
   g_game->AddSprite(sprite);

   bounds.right = 470;
   sprite = new Sprite(g_golfBallBitmap.get(), bounds, BA_BOUNCE);
   sprite->SetVelocity(7, -3);
   g_game->AddSprite(sprite);

   g_dragSprite = NULL;
}

void GameNew( )
{ }

void GameEnd( )
{
   DeleteObject(g_offscreenBitmap);
   DeleteDC(g_offscreenDC);
}

void GameActivate(HWND wnd)
{ }

void GameDeactivate(HWND wnd)
{ }

void GamePaint(HDC dc)
{
   g_forestBitmap->Draw(dc, 0, 0);

   g_game->DrawSprites(dc);
}

void GameCycle( )
{
   g_game->UpdateSprites( );

   HWND wnd = g_game->GetWindow( );
   HDC  dc  = GetDC(wnd);

   GamePaint(g_offscreenDC);

   BitBlt(dc, 0, 0, g_game->GetWidth( ), g_game->GetHeight( ), g_offscreenDC, 0, 0, SRCCOPY);

   // cleanup
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
{ }

void MouseButtonDown(LONG x, LONG y, BOOL left)
{
   if ( left && (NULL == g_dragSprite) )
   {
      if ( (g_dragSprite = g_game->IsPointInSprite(x, y)) != NULL )
      {
         SetCapture(g_game->GetWindow( ));

         MouseMove(x, y);
      }
   }
}

void MouseButtonUp(LONG x, LONG y, BOOL left)
{
   ReleaseCapture( );

   g_dragSprite = NULL;
}

void MouseMove(LONG x, LONG y)
{
   if ( g_dragSprite != NULL )
   {
      g_dragSprite->SetPosition(x - (g_dragSprite->GetWidth( ) / 2),
                                y - (g_dragSprite->GetHeight( ) / 2));

      InvalidateRect(g_game->GetWindow( ), NULL, FALSE);
   }
}

void HandleJoystick(JOYSTATE jsJoystickState)
{ }

BOOL SpriteCollision(Sprite* spriteHitter, Sprite* spriteHittee)
{
   POINT swapVelocity = spriteHitter->GetVelocity( );

   spriteHitter->SetVelocity(spriteHittee->GetVelocity( ));
   spriteHittee->SetVelocity(swapVelocity);

   return TRUE;
}