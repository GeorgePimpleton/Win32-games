#include "Blizzard.hpp"

BOOL GameInitialize(HINSTANCE inst)
{
  g_pGame = new GameEngine(inst, L"Blizzard", L"Blizzard",
                           IDI_BLIZZARD, IDI_BLIZZARD_SM);

  if ( g_pGame == NULL )
  {
     return FALSE;
  }

  g_pGame->SetFrameRate(15);

  return TRUE;
}

void GameStart(HWND wnd)
{
  rtk::srand();
}

void GameEnd()
{
  delete g_pGame;
}

void GameActivate(HWND wnd)
{
  HDC  dc = GetDC(wnd);
  RECT rect;

  GetClientRect(wnd, &rect);

  DrawTextW(dc, L"Here comes the blizzard!", -1, &rect,
            DT_SINGLELINE | DT_CENTER | DT_VCENTER);

  ReleaseDC(wnd, dc);
}

void GameDeactivate(HWND wnd)
{
  HDC  dc = GetDC(wnd);
  RECT rect;

  GetClientRect(wnd, &rect);

  DrawTextW(dc, L"The blizzard has passed.", -1, &rect,
            DT_SINGLELINE | DT_CENTER | DT_VCENTER);

  ReleaseDC(wnd, dc);
}

void GamePaint(HDC dc)
{ }

void GameCycle()
{
   HWND wnd = g_pGame-> GetWindow( );
   HDC  dc  = GetDC(wnd);

   DrawIcon(dc, rtk::rand(0, g_pGame->GetWidth( )), rtk::rand(0, g_pGame->GetHeight( )),
            (HICON) GetClassLongPtr(wnd, GCLP_HICON));

    ReleaseDC(wnd, dc);
}