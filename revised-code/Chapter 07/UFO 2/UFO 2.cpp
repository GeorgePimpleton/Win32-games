#include "UFO 2.hpp"

HRESULT GameInitialize(HINSTANCE hInstance)
{
   g_pGame = std::make_unique<GameEngine>(hInstance, L"UFO 2", L"Improving Input with Joysticks",
                                          IDI_ICON, IDI_ICON_SM, 500, 400);

   if (g_pGame == NULL)
   {
      return E_FAIL;
   }

   g_pGame->SetFrameRate(30);

   g_pGame->InitJoystick();

   return S_OK;
}

void GameStart(HWND hWindow)
{
   rtk::srand();

   HINSTANCE hInstance = g_pGame->GetInstance();

   g_pBackground = std::make_unique<Bitmap>(IDB_BACKGROUND, hInstance);
   g_pSaucer[0]  = std::make_unique<Bitmap>(IDB_SAUCER, hInstance);
   g_pSaucer[1]  = std::make_unique<Bitmap>(IDB_SAUCERFLAME, hInstance);

   g_iSaucerX = 250 - (g_pSaucer[0]->GetWidth() / 2);
   g_iSaucerY = 200 - (g_pSaucer[0]->GetHeight() / 2);
   g_iSpeedX  = 0;
   g_iSpeedY  = 0;
}

void GameEnd()
{
}

void GameActivate(HWND hWindow)
{
   g_pGame->CaptureJoystick();
}

void GameDeactivate(HWND hWindow)
{
   g_pGame->ReleaseJoystick();
}

void GamePaint(HDC hDC)
{
   g_pBackground->Draw(hDC, 0, 0);
   g_pSaucer[g_bSaucerFlame ? 1 : 0]->Draw(hDC, g_iSaucerX, g_iSaucerY, TRUE);
}

void GameCycle()
{
   g_iSaucerX = min(500 - (LONG) g_pSaucer[0]->GetWidth(), max(0, g_iSaucerX + g_iSpeedX));
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
      g_iSaucerX = x - (g_pSaucer[0]->GetWidth() / 2);
      g_iSaucerY = y - (g_pSaucer[0]->GetHeight() / 2);
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

void HandleJoystick(JOYSTATE jsJoystickState)
{
   if (jsJoystickState & JOY_LEFT)
   {
      g_iSpeedX = max(-g_iMAXSPEED, g_iSpeedX - 2);
   }
   else if (jsJoystickState & JOY_RIGHT)
   {
      g_iSpeedX = min(g_iMAXSPEED, g_iSpeedX + 2);
   }

   if (jsJoystickState & JOY_UP)
   {
      g_iSpeedY = max(-g_iMAXSPEED, g_iSpeedY - 2);
   }
   else if (jsJoystickState & JOY_DOWN)
   {
      g_iSpeedY = min(g_iMAXSPEED, g_iSpeedY + 2);
   }

   g_bSaucerFlame = (jsJoystickState & JOY_FIRE1);

   if (jsJoystickState & JOY_FIRE2)
   {
      g_iSaucerX = rtk::rand(0, (500 - g_pSaucer[0]->GetWidth()));
      g_iSaucerY = rtk::rand(0, (400 - g_pSaucer[0]->GetHeight()));
   }
}