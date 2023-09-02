#include "Trippy.hpp"

HRESULT GameInitialize(HINSTANCE inst)
{
   g_game = std::make_unique<GameEngine>(inst, L"Trippy", L"Learning to Draw Basic Graphics",
                                         IDI_ICON, IDI_ICON_SM);

   if ( nullptr == g_game )
   {
      return E_FAIL;
   }

   g_game->SetFrameRate(15);

   return S_OK;
}

void GameStart(HWND hWindow)
{
   rtk::srand( );

   g_rect.left   = g_game->GetWidth() * 2 / 5;
   g_rect.top    = g_game->GetHeight() * 2 / 5;
   g_rect.right  = g_rect.left + g_game->GetWidth() / 5;
   g_rect.bottom = g_rect.top + g_game->GetHeight() / 5;
}

void GameEnd( )
{ }

void GameActivate(HWND hwnd)
{ }

void GameDeactivate(HWND hwnd)
{ }

void GamePaint(HDC hDC)
{
   // draw grid lines as a background for the rectangles
   const UINT gridLines = 50;

   for (UINT i = 1; i <= gridLines; i++)
   {
      // draw a horizontal grid line
      MoveToEx(hDC, g_game->GetWidth() * i / gridLines, 0, nullptr);
      LineTo(hDC, g_game->GetWidth() * i / gridLines, g_game->GetHeight());

      // draw a vertical grid line
      MoveToEx(hDC, 0, g_game->GetHeight() * i / gridLines, NULL);
      LineTo(hDC, g_game->GetWidth(), g_game->GetHeight() * i / gridLines);
   }
}

void GameCycle( )
{
   // randomly alter the position and size of the rectangle
   int inflation = rtk::rand(-10, 10);

   InflateRect(&g_rect, inflation, inflation);

   int dx = rtk::rand(-9, 9);
   int dy = rtk::rand(-9, 9);

   OffsetRect(&g_rect, dx, dy);

   // check if the rectangle might be outside the window
   if (g_rect.right < 0)
   {
      g_rect.right = 10;
   }

   if (g_rect.left > (int) g_game->GetWidth())
   {
      g_rect.left = (int) g_game->GetWidth() - 10;
   }

   if (g_rect.bottom < 0)
   {
      g_rect.bottom = 10;
   }

   if (g_rect.top > (int) g_game->GetHeight())
   {
      g_rect.top = (int) g_game->GetHeight() - 10;
   }

   // draw the new rectangle in a random color
   UINT   red   = rtk::rand(0, 255);
   UINT   green = rtk::rand(0, 255);
   UINT   blue  = rtk::rand(0, 255);
   HBRUSH brush = CreateSolidBrush(RGB(red, green, blue));

   HWND wnd = g_game-> GetWindow();
   HDC  dc  = GetDC(wnd);

   FillRect(dc, &g_rect, brush);

   ReleaseDC(wnd, dc);
   DeleteObject(brush);

   // add a bit of delay so the rectangles aren't drawn so fast
   Sleep(10);
}

void GameMenu(WPARAM wParam)
{
   switch (LOWORD(wParam))
   {
   case IDM_GAME_EXIT:
      GameEnd( );
      PostQuitMessage(0);
      return;

   case IDM_HELP_ABOUT:
      DialogBoxW(g_game->GetInstance(), MAKEINTRESOURCEW(IDD_ABOUT),
                 g_game->GetWindow(), (DLGPROC) DlgProc);
      return;
   }
}