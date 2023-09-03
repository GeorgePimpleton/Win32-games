#include "Blizzard.hpp"

HRESULT GameInitialize(HINSTANCE inst)
{
   g_game = std::make_unique<GameEngine>(inst, L"Blizzard", L"Blizzard: Creating An Engine For Games",
                                          IDI_ICON, IDI_ICON_SM);

   if (nullptr == g_game)
   {
      return E_FAIL;
   }

   g_game->SetFrameRate(15);

   return S_OK;
}

void GameStart(HWND wnd)
{
   rtk::srand();
}

void GameEnd()
{
   // cleanup automatically done via std::unique_ptr<> now

   // cleanup the game engine
   // delete g_pGame;
}

void GameActivate(HWND wnd)
{
   RECT rect;

   GetClientRect(wnd, &rect);

   HDC dc = GetDC(wnd);

   DrawTextW(dc, L"Here comes the blizzard!", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

   ReleaseDC(wnd, dc);
}

void GameDeactivate(HWND wnd)
{
   RECT rect;

   GetClientRect(wnd, &rect);

   HDC dc = GetDC(wnd);

   DrawTextW(dc, L"The blizzard has passed.", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

   ReleaseDC(wnd, dc);
}

void GamePaint(HDC dc)
{ }

void GameCycle()
{
   HWND wnd = g_game->GetWindow();
   HDC  dc  = GetDC(wnd);

   DrawIcon(dc, rtk::rand(0, g_game->GetWidth()), rtk::rand(0, g_game->GetHeight()),
            (HICON) GetClassLongPtrW(wnd, GCLP_HICON));

   ReleaseDC(wnd, dc);
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