#include "UFO.hpp"

HRESULT GameInitialize(HINSTANCE hInstance)
{
   g_pGame = std::make_unique<GameEngine>(hInstance, L"UFO", L"Controlling Games with the Keyboard and Mouse",
                                          IDI_ICON, IDI_ICON_SM, 500, 400);

   if (g_pGame == NULL)
   {
      return E_FAIL;
   }

   g_pGame->SetFrameRate(30);

   return S_OK;
}

void GameStart(HWND hWindow)
{
   HINSTANCE hInstance = g_pGame->GetInstance();

   g_pBackground = std::make_unique<Bitmap>(IDB_BACKGROUND, hInstance);
   g_pSaucer     = std::make_unique<Bitmap>(IDB_SAUCER, hInstance);

   g_iSaucerX = 250 - (g_pSaucer->GetWidth() / 2);
   g_iSaucerY = 200 - (g_pSaucer->GetHeight() / 2);
   g_iSpeedX  = 0;
   g_iSpeedY  = 0;
}

void GameEnd()
{
}

void GameActivate(HWND hWindow)
{
}

void GameDeactivate(HWND hWindow)
{
}

void GamePaint(HDC hDC)
{
   g_pBackground->Draw(hDC, 0, 0);

   g_pSaucer->Draw(hDC, g_iSaucerX, g_iSaucerY, TRUE);
}

void GameCycle()
{
   g_iSaucerX = min(500 - (LONG) g_pSaucer->GetWidth(), max(0, g_iSaucerX + g_iSpeedX));
   g_iSaucerY = min(320, max(0, g_iSaucerY + g_iSpeedY));

   InvalidateRect(g_pGame->GetWindow(), NULL, FALSE);
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
      DialogBoxW(g_pGame->GetInstance(), MAKEINTRESOURCEW(IDD_ABOUT), g_pGame->GetWindow(), (DLGPROC) DlgProc);
      return;
   }
}

void HandleKeys()
{
   if (GetAsyncKeyState(VK_LEFT) < 0)
   {
      g_iSpeedX = max(-g_iMAXSPEED, --g_iSpeedX);
   }
   else if (GetAsyncKeyState(VK_RIGHT) < 0)
   {
      g_iSpeedX = min(g_iMAXSPEED, ++g_iSpeedX);
   }

   if (GetAsyncKeyState(VK_UP) < 0)
   {
      g_iSpeedY = max(-g_iMAXSPEED, --g_iSpeedY);
   }
   else if (GetAsyncKeyState(VK_DOWN) < 0)
   {
      g_iSpeedY = min(g_iMAXSPEED, ++g_iSpeedY);
   }
}

void MouseButtonDown(LONG x, LONG y, BOOL bLeft)
{
   if (bLeft)
   {
      g_iSaucerX = x - (g_pSaucer->GetWidth() / 2);
      g_iSaucerY = y - (g_pSaucer->GetHeight() / 2);
   }
   else
   {
      g_iSpeedX = 0;
      g_iSpeedY = 0;
   }
}

void MouseButtonUp(LONG x, LONG y, BOOL bLeft)
{
}

void MouseMove(LONG x, LONG y)
{
}