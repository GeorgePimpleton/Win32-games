// "Talk to me like I'm a 3 year old!" Programming Lessons - Windows Games

// Trippy.cpp - game source

#include "Trippy.hpp"

HRESULT GameInitialize(HINSTANCE hInstance)
{
   g_pGame = std::make_unique<GameEngine>(hInstance, L"Trippy", L"Learning to Draw Basic Graphics",
                                          IDI_ICON, IDI_ICON_SM);

   if (g_pGame == nullptr)
   {
      return E_FAIL;
   }

   g_pGame->SetFrameRate(15);

   return S_OK;
}

void GameStart(HWND hWindow)
{
   rtk::srand();

   g_rcRectangle.left   = g_pGame->GetWidth() * 2 / 5;
   g_rcRectangle.top    = g_pGame->GetHeight() * 2 / 5;
   g_rcRectangle.right  = g_rcRectangle.left + g_pGame->GetWidth() / 5;
   g_rcRectangle.bottom = g_rcRectangle.top + g_pGame->GetHeight() / 5;
}

void GameEnd()
{
   // delete g_pGame;
}

void GameActivate(HWND hWindow)
{
}

void GameDeactivate(HWND hWindow)
{
}

void GamePaint(HDC hDC)
{
   // draw grid lines as a background for the rectangles
   const UINT iGridLines = 50;

   for (UINT i = 1; i <= iGridLines; i++)
   {
      // draw a horizontal grid line
      MoveToEx(hDC, g_pGame->GetWidth() * i / iGridLines, 0, NULL);
      LineTo(hDC, g_pGame->GetWidth() * i / iGridLines, g_pGame->GetHeight());

      // draw a vertical grid line
      MoveToEx(hDC, 0, g_pGame->GetHeight() * i / iGridLines, NULL);
      LineTo(hDC, g_pGame->GetWidth(), g_pGame->GetHeight() * i / iGridLines);
   }
}

void GameCycle()
{
   // randomly alter the position and size of the rectangle
   int iInflation = rtk::rand(-10, 10);

   InflateRect(&g_rcRectangle, iInflation, iInflation);

   int dx = rtk::rand(-9, 9);
   int dy = rtk::rand(-9, 9);

   OffsetRect(&g_rcRectangle, dx, dy);

   // check if the rectangle might be outside the window
   if (g_rcRectangle.right < 0)
   {
      g_rcRectangle.right = 10;
   }

   if (g_rcRectangle.left > (int) g_pGame->GetWidth())
   {
      g_rcRectangle.left = (int) g_pGame->GetWidth() - 10;
   }

   if (g_rcRectangle.bottom < 0)
   {
      g_rcRectangle.bottom = 10;
   }

   if (g_rcRectangle.top > (int) g_pGame->GetHeight())
   {
      g_rcRectangle.top = (int) g_pGame->GetHeight() - 10;
   }

   // draw the new rectangle in a random color
   UINT   red    = rtk::rand(0, 255);
   UINT   green  = rtk::rand(0, 255);
   UINT   blue   = rtk::rand(0, 255);
   HBRUSH hBrush = CreateSolidBrush(RGB(red, green, blue));

   HWND hWindow = g_pGame->GetWindow();
   HDC  hDC = GetDC(hWindow);

   FillRect(hDC, &g_rcRectangle, hBrush);

   ReleaseDC(hWindow, hDC);
   DeleteObject(hBrush);

   // add a bit of delay so the rectangles aren't drawn so fast
   Sleep(10);
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
      DialogBoxW(g_pGame->GetInstance(), MAKEINTRESOURCEW(IDD_ABOUT),
                 g_pGame->GetWindow(), (DLGPROC) DlgProc);
      return;
   }
}