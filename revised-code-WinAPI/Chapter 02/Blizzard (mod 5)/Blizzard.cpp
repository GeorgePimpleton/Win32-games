// Blizzard.cpp - Blizzard application source

#include "Blizzard.hpp"

HRESULT GameInitialize(HINSTANCE hInstance)
{
   g_pGame = std::make_unique<GameEngine>(hInstance, L"Blizzard", L"Creating An Engine For Games",
                                          IDI_ICON, IDI_ICON_SM);

   if (nullptr == g_pGame)
   {
      return E_FAIL;
   }

   g_pGame->SetFrameRate(15);

   return S_OK;
}

void GameStart(HWND hWindow)
{
   rtk::srand();
}

void GameEnd()
{
   // cleanup automatically done via std::unique_ptr<> now

   // cleanup the game engine
   // delete g_pGame;
}

void GameActivate(HWND hWindow)
{
   RECT rect;

   GetClientRect(hWindow, &rect);

   HDC hDC = GetDC(hWindow);

   DrawTextW(hDC, L"Here comes the blizzard!", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

   ReleaseDC(hWindow, hDC);
}

void GameDeactivate(HWND hWindow)
{
   RECT rect;

   GetClientRect(hWindow, &rect);

   HDC hDC = GetDC(hWindow);

   DrawTextW(hDC, L"The blizzard has passed.", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

   ReleaseDC(hWindow, hDC);
}

void GamePaint(HDC hDC)
{ }

void GameCycle()
{
   HWND hWindow = g_pGame->GetWindow();

   HDC hDC { GetDC(hWindow) };

   DrawIcon(hDC, rtk::rand(0, g_pGame->GetWidth()), rtk::rand(0, g_pGame->GetHeight()),
            (HICON) GetClassLongPtrW(hWindow, GCLP_HICON));

   ReleaseDC(hWindow, hDC);
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