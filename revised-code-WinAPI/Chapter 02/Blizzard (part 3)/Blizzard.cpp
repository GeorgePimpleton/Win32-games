#include "Blizzard.hpp"

BOOL GameInitialize(HINSTANCE hInstance)
{
   g_pGame = new GameEngine(hInstance, TEXT("Blizzard"), TEXT("Blizzard"),
                            IDI_BLIZZARD, IDI_BLIZZARD_SM);

   if ( g_pGame == NULL )
   {
      return FALSE;
   }

   g_pGame->SetFrameRate(15);

   return TRUE;
}

void GameStart(HWND hWindow)
{
   rtk::srand( );
}

void GameEnd( )
{
   delete g_pGame;
}

void GameActivate(HWND hWindow)
{
   HDC  hDC = GetDC(hWindow);
   RECT rect;

   GetClientRect(hWindow, &rect);

   DrawText(hDC, TEXT("Here comes the blizzard!"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

   ReleaseDC(hWindow, hDC);
}

void GameDeactivate(HWND hWindow)
{
   HDC  hDC = GetDC(hWindow);
   RECT rect;

   GetClientRect(hWindow, &rect);

   DrawText(hDC, TEXT("The blizzard has passed."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

   ReleaseDC(hWindow, hDC);
}

void GamePaint(HDC hDC)
{ }

void GameCycle( )
{
   HWND hWindow = g_pGame-> GetWindow( );
   HDC  hDC     = GetDC(hWindow);

   DrawIcon(hDC, rtk::rand(0, g_pGame->GetWidth( )), rtk::rand(0, g_pGame->GetHeight( )),
            (HICON) GetClassLongPtr(hWindow, GCLP_HICON));

   ReleaseDC(hWindow, hDC);
}
