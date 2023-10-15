#include "Blizzard.hpp"

BOOL GameInitialize(HINSTANCE inst)
{
   // Create the game engine
   g_pGame = new GameEngine(inst, TEXT("Blizzard"), TEXT("Blizzard"),
                            IDI_BLIZZARD, IDI_BLIZZARD_SM);

   if ( g_pGame == NULL )
   {
      return FALSE;
   }

   // Set the frame rate
   g_pGame->SetFrameRate(15);

   return TRUE;
}

void GameStart(HWND wnd)
{
   // Seed the random number generator
   srand(GetTickCount( ));
}

void GameEnd( )
{
   // Cleanup the game engine
   delete g_pGame;
}

void GameActivate(HWND wnd)
{
   HDC  dc = GetDC(wnd);
   RECT rect;

   // Draw activation text on the game screen
   GetClientRect(wnd, &rect);

   DrawText(dc, TEXT("Here comes the blizzard!"), -1, &rect,
            DT_SINGLELINE | DT_CENTER | DT_VCENTER);

   ReleaseDC(wnd, dc);
}

void GameDeactivate(HWND wnd)
{
   HDC  dc = GetDC(wnd);
   RECT rect;

   // Draw deactivation text on the game screen
   GetClientRect(wnd, &rect);

   DrawText(dc, TEXT("The blizzard has passed."), -1, &rect,
            DT_SINGLELINE | DT_CENTER | DT_VCENTER);

   ReleaseDC(wnd, dc);
}

void GamePaint(HDC dc)
{ }

void GameCycle( )
{
   HWND wnd = g_pGame->GetWindow( );
   HDC  dc = GetDC(wnd);

   // Draw the snowflake icon at random positions on the game screen
   DrawIcon(dc, rand( ) % g_pGame->GetWidth( ), rand( ) % g_pGame->GetHeight( ),
            (HICON) GetClassLongPtr(wnd, GCLP_HICON));

   ReleaseDC(wnd, dc);
}