#include "Fore.hpp"

HRESULT GameInitialize(HINSTANCE hInstance)
{
   g_pGame = std::make_unique<GameEngine>(hInstance, L"Fore", L"Making Things Move with Sprites",
                                          IDI_ICON, IDI_ICON_SM, 600, 400);

   if (g_pGame == NULL)
   {
      return E_FAIL;
   }

   g_pGame->SetFrameRate(30);

   return S_OK;
}

void GameStart(HWND hWindow)
{
   rtk::srand();

   g_pForestBitmap   = std::make_unique<Bitmap>(IDB_FOREST, GetModuleHandle(NULL));
   g_pGolfBallBitmap = std::make_unique<Bitmap>(IDB_GOLFBALL, GetModuleHandle(NULL));

   RECT rcBounds        { 0, 0, 600, 400 };

   g_pGolfBallSprite[0] = std::make_unique<Sprite>(g_pGolfBallBitmap.get(), rcBounds);
   g_pGolfBallSprite[0]->SetVelocity(2, 1);

   g_pGolfBallSprite[1] = std::make_unique<Sprite>(g_pGolfBallBitmap.get(), rcBounds, BA_WRAP);
   g_pGolfBallSprite[1]->SetVelocity(3, -2);

   g_pGolfBallSprite[2] = std::make_unique<Sprite>(g_pGolfBallBitmap.get(), rcBounds, BA_BOUNCE);
   g_pGolfBallSprite[2]->SetVelocity(7, 4);

   g_bDragging = FALSE;
   g_iDragBall = -1;
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
   g_pForestBitmap->Draw(hDC, 0, 0);

   for (int i = 0; i < 3; i++)
   {
      g_pGolfBallSprite[i]->Draw(hDC);
   }
}

void GameCycle()
{
   for (int i = 0; i < 3; i++)
   {
      g_pGolfBallSprite[i]->Update();
   }

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
}

void MouseButtonDown(LONG x, LONG y, BOOL bLeft)
{
   if (bLeft && !g_bDragging)
   {
      for (int i = 0; i < 3; i++)
      {
         if (g_pGolfBallSprite[i]->IsPointInside(x, y))
         {
            SetCapture(g_pGame->GetWindow());

            g_bDragging = TRUE;
            g_iDragBall = i;

            MouseMove(x, y);

            break;
         }
      }
   }
}

void MouseButtonUp(LONG x, LONG y, BOOL bLeft)
{
   ReleaseCapture();

   g_bDragging = FALSE;
}

void MouseMove(LONG x, LONG y)
{
   if (g_bDragging)
   {
      g_pGolfBallSprite[g_iDragBall]->SetPosition(
      x - (g_pGolfBallBitmap->GetWidth() / 2),
      y - (g_pGolfBallBitmap->GetHeight() / 2));

      InvalidateRect(g_pGame->GetWindow(), NULL, FALSE);
   }
}

void HandleJoystick(JOYSTATE jsJoystickState)
{
}