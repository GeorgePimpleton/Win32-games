#include "UFO.hpp"

HRESULT GameInitialize(HINSTANCE inst)
{
   g_game = std::make_unique<GameEngine>(inst, L"UFO", L"UFO: Controlling Games with the Keyboard and Mouse",
                                         IDI_ICON, IDI_ICON_SM, 500, 400);

   if (nullptr == g_game)
   {
      return E_FAIL;
   }

   g_game->SetFrameRate(30);

   return S_OK;
}

void GameStart(HWND hwnd)
{
   // create and load the background and saucer bitmaps
   HINSTANCE hInst = g_game->GetInstance();

   g_background = std::make_unique<Bitmap>(IDB_BACKGROUND, hInst);
   g_saucer     = std::make_unique<Bitmap>(IDB_SAUCER, hInst);

   // set the initial saucer position and speed
   g_saucerX = 250 - (g_saucer->GetWidth() / 2);
   g_saucerY = 200 - (g_saucer->GetHeight() / 2);
   g_speedX  = 0;
   g_speedY  = 0;
}

void GameEnd( )
{ }

void GameActivate(HWND wnd)
{ }

void GameDeactivate(HWND wnd)
{ }

void GamePaint(HDC dc)
{
   // draw the background
   g_background->Draw(dc, 0, 0);

   // draw the saucer
   g_saucer->Draw(dc, g_saucerX, g_saucerY, TRUE);
}

void GameCycle( )
{
   // update the saucer position
   g_saucerX = min(500 - (LONG) g_saucer->GetWidth(), max(0, g_saucerX + g_speedX));
   g_saucerY = min(320, max(0, g_saucerY + g_speedY));

   InvalidateRect(g_game->GetWindow(), nullptr, FALSE);
}

void GameMenu(WPARAM wParam)
{
   switch (LOWORD(wParam))
   {
   case IDM_GAME_EXIT:
      GameEnd();
      PostQuitMessage(0);
      return;

   case IDM_HELP_ABOUT:
      DialogBoxW(g_game->GetInstance(), MAKEINTRESOURCEW(IDD_ABOUT), g_game->GetWindow(), (DLGPROC) DlgProc);
      return;
   }
}

// handles key presses
void HandleKeys( )
{
   // change the speed of the saucer in response to arrow key presses
   if (GetAsyncKeyState(VK_LEFT) < 0)
   {
      g_speedX = max(-g_MAXSPEED, --g_speedX);
   }
   else if (GetAsyncKeyState(VK_RIGHT) < 0)
   {
      g_speedX = min(g_MAXSPEED, ++g_speedX);
   }

   if (GetAsyncKeyState(VK_UP) < 0)
   {
      g_speedY = max(-g_MAXSPEED, --g_speedY);
   }
   else if (GetAsyncKeyState(VK_DOWN) < 0)
   {
      g_speedY = min(g_MAXSPEED, ++g_speedY);
   }
}

// handles a mouse button press
void MouseButtonDown(LONG x, LONG y, BOOL left)
{
   if (left)
   {
      // set the saucer position to the mouse position
      g_saucerX = x - (g_saucer->GetWidth() / 2);
      g_saucerY = y - (g_saucer->GetHeight() / 2);
   }
   else
   {
      // stop the saucer
      g_speedX = 0;
      g_speedY = 0;
   }
}

// handles a mouse button release
void MouseButtonUp(LONG x, LONG y, BOOL left)
{ }

// handles a mouse move
void MouseMove(LONG x, LONG y)
{ }