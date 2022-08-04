// "Talk to me like I'm a 3 year old!" Programming Lessons - Windows Games

// Blizzard.cpp - Blizzard application source

// include files
#include "Blizzard.hpp"

// game engine functions
HRESULT GameInitialize(HINSTANCE hInstance)
{
   // create the game engine
   g_pGame = new GameEngine(hInstance, L"Blizzard", L"Creating An Engine For Games",
                            IDI_ICON, IDI_ICON_SM);

   if (!g_pGame)
   {
      return E_FAIL;
   }

   // set the frame rate
   g_pGame->SetFrameRate(15);

   return S_OK;
}

void GameStart(HWND hWindow)
{
   // seed the random number generator
   rtk::srand();
}

void GameEnd()
{
   // cleanup the game engine

   // cleanup automatically done via std::unique_ptr<>
   // delete g_pGame;
}

void GameActivate(HWND hWindow)
{
   HDC  hDC;
   RECT rect;

   // draw activation text on the game screen
   GetClientRect(hWindow, &rect);

   hDC = GetDC(hWindow);

   DrawTextW(hDC, L"Here comes the blizzard!", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

   ReleaseDC(hWindow, hDC);
}

void GameDeactivate(HWND hWindow)
{
   HDC  hDC;
   RECT rect;

   // draw deactivation text on the game screen
   GetClientRect(hWindow, &rect);

   hDC = GetDC(hWindow);

   DrawTextW(hDC, L"The blizzard has passed.", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

   ReleaseDC(hWindow, hDC);
}

void GamePaint(HDC hDC)
{ }

void GameCycle()
{
   HWND hWindow = g_pGame->GetWindow();

   // draw the snowflake icon at random positions on the game screen
   HDC hDC { GetDC(hWindow) };

   DrawIcon(hDC, rtk::rand(0, g_pGame->GetWidth()), rtk::rand(0, g_pGame->GetHeight()),
            (HICON) GetClassLongPtrW(hWindow, GCLP_HICON));

   ReleaseDC(hWindow, hDC);
}

// handles game menu messages
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