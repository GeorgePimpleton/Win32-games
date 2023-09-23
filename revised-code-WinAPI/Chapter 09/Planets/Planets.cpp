#include "Planets.hpp"

HRESULT GameInitialize(HINSTANCE inst)
{
   g_game = std::make_unique<GameEngine>(inst, L"Planets", L"Planets: Making Things Move with Sprites",
                                         IDI_ICON, IDI_ICON_SM, 600, 400);

   if ( NULL == g_game )
   {
      return E_FAIL;
   }

   g_game->SetFrameRate(30);

   return S_OK;
}

void GameStart(HWND hWindow)
{
   rtk::srand();

   HINSTANCE inst = GetModuleHandleW(NULL);

   g_galaxyBitmap      = std::make_unique<Bitmap>(IDB_GALAXY, inst);
   g_planetBitmap[ 0 ] = std::make_unique<Bitmap>(IDB_PLANET1, inst);
   g_planetBitmap[ 1 ] = std::make_unique<Bitmap>(IDB_PLANET2, inst);
   g_planetBitmap[ 2 ] = std::make_unique<Bitmap>(IDB_PLANET3, inst);

   RECT rcBounds = { 0, 0, 600, 400 };

   g_panetSprite[ 0 ] = std::make_unique<Sprite>(g_planetBitmap[ 0 ].get( ), rcBounds);
   g_panetSprite[0]->SetPosition(0, 0);
   g_panetSprite[0]->SetVelocity(1, 1);

   g_panetSprite[ 1 ] = std::make_unique<Sprite>(g_planetBitmap[ 1 ].get( ), rcBounds, BA_WRAP);
   g_panetSprite[ 1 ]->SetVelocity(2, -1);

   g_panetSprite[ 2 ] = std::make_unique<Sprite>(g_planetBitmap[ 2 ].get( ), rcBounds, BA_BOUNCE);
   g_panetSprite[ 2 ]->SetVelocity(3, -2);
   g_dragging   = FALSE;
   g_dragPlanet = -1;
}

void GameNew( )
{ }

void GameEnd()
{ }

void GameActivate(HWND hWindow)
{ }

void GameDeactivate(HWND hWindow)
{ }

void GamePaint(HDC hDC)
{
   g_galaxyBitmap->Draw(hDC, 0, 0);

   for (int i = 0; i < 3; i++)
   {
      g_panetSprite[i]->Draw(hDC);
   }
}

void GameCycle()
{
   for (int i = 0; i < 3; i++)
   {
      g_panetSprite[i]->Update();
   }

   InvalidateRect(g_game->GetWindow(), NULL, FALSE);
}

void GameMenu(WPARAM wParam)
{
   switch ( LOWORD(wParam) )
   {
   case IDM_GAME_NEW:
      GameNew( );
      return;

   case IDM_GAME_EXIT:
      GameEnd( );
      PostQuitMessage(0);
      return;

   case IDM_HELP_ABOUT:
      DialogBoxW(g_game->GetInstance( ), MAKEINTRESOURCEW(IDD_ABOUT), g_game->GetWindow( ), (DLGPROC) DlgProc);
      return;
   }
}

void HandleKeys()
{ }

void MouseButtonDown(LONG x, LONG y, BOOL left)
{
   if (left && !g_dragging)
   {
      for (int i = 0; i < 3; i++)
      {
         if (g_panetSprite[i]->IsPointInside(x, y))
         {
            SetCapture(g_game->GetWindow());

            g_dragging = TRUE;
            g_dragPlanet = i;

            MouseMove(x, y);

            break;
         }
      }
   }
}

void MouseButtonUp(LONG x, LONG y, BOOL left)
{
   ReleaseCapture();

   g_dragging = FALSE;
}

void MouseMove(LONG x, LONG y)
{
   if (g_dragging)
   {
      g_panetSprite[ g_dragPlanet ]->SetPosition(x - (g_planetBitmap[ g_dragPlanet ]->GetWidth( ) / 2),
                                                 y - (g_planetBitmap[ g_dragPlanet ]->GetHeight( ) / 2));

      InvalidateRect(g_game->GetWindow(), NULL, FALSE);
   }
}

void HandleJoystick(JOYSTATE joyState)
{ }