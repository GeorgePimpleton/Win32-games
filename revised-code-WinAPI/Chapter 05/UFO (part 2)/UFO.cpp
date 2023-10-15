#include "UFO.hpp"

BOOL GameInitialize(HINSTANCE inst)
{
   g_game = std::make_unique<GameEngine>(inst, L"UFO", L"UFO: Controlling Games with the Keyboard and Mouse",
                                         IDI_ICON, IDI_ICON_SM, 500, 400);

  if ( g_game == NULL )
  {
     return FALSE;
  }

  g_game->SetFrameRate(30);

  return TRUE;
}

void GameStart(HWND wnd)
{
  g_background = std::make_unique<Bitmap>(IDB_BACKGROUND);
  g_saucer     = std::make_unique<Bitmap>(IDB_SAUCER);

  g_saucerX = 250 - (g_saucer-> GetWidth() / 2);
  g_saucerY = 200 - (g_saucer-> GetHeight() / 2);
  g_speedX  = 0;
  g_speedY  = 0;
}

void GameEnd()
{ }

void GameActivate(HWND wnd)
{ }

void GameDeactivate(HWND wnd)
{ }

void GamePaint(HDC dc)
{
  g_background->Draw(dc, 0, 0);
  g_saucer->Draw(dc, g_saucerX, g_saucerY, TRUE);
}

void GameCycle()
{
  g_saucerX = min(500 - g_saucer->GetWidth(), max(0, g_saucerX + g_speedX));
  g_saucerY = min(320, max(0, g_saucerY + g_speedY));

  InvalidateRect(g_game->GetWindow(), NULL, FALSE);
}

void GameMenu(WPARAM wParam)
{
   switch ( LOWORD(wParam) )
   {
   case IDM_GAME_NEW:
      // spaceholder for later game examples
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

void HandleKeys()
{
   if ( GetAsyncKeyState(VK_LEFT) < 0 )
   {
      g_speedX = max(-g_MAXSPEED, --g_speedX);
   }
   else if ( GetAsyncKeyState(VK_RIGHT) < 0 )
   {
      g_speedX = min(g_MAXSPEED, ++g_speedX);
   }

   if ( GetAsyncKeyState(VK_UP) < 0 )
   {
      g_speedY = max(-g_MAXSPEED, --g_speedY);
   }
   else if ( GetAsyncKeyState(VK_DOWN) < 0 )
   {
      g_speedY = min(g_MAXSPEED, ++g_speedY);
   }
}

void MouseButtonDown(int x, int y, BOOL left)
{
  if (left)
  {
    g_saucerX = x - (g_saucer->GetWidth() / 2);
    g_saucerY = y - (g_saucer->GetHeight() / 2);
  }
  else
  {
    g_speedX = 0;
    g_speedY = 0;
  }
}

void MouseButtonUp(int x, int y, BOOL left)
{ }

void MouseMove(int x, int y)
{ }