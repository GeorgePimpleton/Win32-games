#include "Light Cycles.hpp"

HRESULT GameInitialize(HINSTANCE hInstance)
{
   g_pGame = std::make_unique<GameEngine>(hInstance, L"Light Cycles", L"Light Cycles",
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
   HINSTANCE hInstance { GetModuleHandleW(NULL) };

   g_pBackground  = std::make_unique<Bitmap>(IDB_BACKGROUND, hInstance);
   g_pCycle[0][0] = std::make_unique<Bitmap>(IDB_CYCLEBLUE_0, hInstance);
   g_pCycle[0][1] = std::make_unique<Bitmap>(IDB_CYCLEBLUE_90, hInstance);
   g_pCycle[0][2] = std::make_unique<Bitmap>(IDB_CYCLEBLUE_180, hInstance);
   g_pCycle[0][3] = std::make_unique<Bitmap>(IDB_CYCLEBLUE_270, hInstance);
   g_pCycle[1][0] = std::make_unique<Bitmap>(IDB_CYCLEORANGE_0, hInstance);
   g_pCycle[1][1] = std::make_unique<Bitmap>(IDB_CYCLEORANGE_90, hInstance);
   g_pCycle[1][2] = std::make_unique<Bitmap>(IDB_CYCLEORANGE_180, hInstance);
   g_pCycle[1][3] = std::make_unique<Bitmap>(IDB_CYCLEORANGE_270, hInstance);

   NewGame();
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

   for (int i { }; i < 2; i++)
   {
      HPEN hPen { CreatePen(PS_SOLID, 5, (i == 0) ? RGB(0, 0, 255) : RGB(255, 146, 73)) };
      SelectObject(hDC, hPen);

      MoveToEx(hDC, g_ptCycleTrail[i][0].x, g_ptCycleTrail[i][0].y, NULL);

      for (int j { 1 }; j < g_iTrailLen[i]; j++)
      {
         LineTo(hDC, g_ptCycleTrail[i][j].x, g_ptCycleTrail[i][j].y);
      }

      DeleteObject(hPen);
   }

   int iDirection[2] = { 0, 0 };

   for (int i { }; i < 2; i++)
   {
      if (g_ptCycleSpeed[i].y < 0)
      {
         iDirection[i] = 0;
      }
      else if (g_ptCycleSpeed[i].x > 0)
      {
         iDirection[i] = 1;
      }
      else if (g_ptCycleSpeed[i].y > 0)
      {
         iDirection[i] = 2;
      }
      else if (g_ptCycleSpeed[i].x < 0)
      {
         iDirection[i] = 3;
      }
   }

   g_pCycle[0][iDirection[0]]->Draw(hDC, g_ptCyclePos[0].x, g_ptCyclePos[0].y, TRUE);
   g_pCycle[1][iDirection[1]]->Draw(hDC, g_ptCyclePos[1].x, g_ptCyclePos[1].y, TRUE);
}

void GameCycle()
{
   if (!g_bGameOver)
   {
      UpdateCycles();

      InvalidateRect(g_pGame->GetWindow(), NULL, FALSE);
   }
}

void GameMenu(WPARAM wParam)
{
   switch (LOWORD(wParam))
   {
   case IDM_GAME_NEW:
      if (g_bGameOver)
      {
         NewGame();
      }
      return;

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
   if (!g_bGameOver)
   {
      if (GetAsyncKeyState(VK_UP) < 0)
      {
         SteerCycle(0, 0);
      }
      else if (GetAsyncKeyState(VK_RIGHT) < 0)
      {
         SteerCycle(0, 1);
      }
      else if (GetAsyncKeyState(VK_DOWN) < 0)
      {
         SteerCycle(0, 2);
      }
      else if (GetAsyncKeyState(VK_LEFT) < 0)
      {
         SteerCycle(0, 3);
      }
   }
}

void MouseButtonDown(LONG x, LONG y, BOOL bLeft)
{ }

void MouseButtonUp(LONG x, LONG y, BOOL bLeft)
{ }

void MouseMove(LONG x, LONG y)
{ }

void HandleJoystick(JOYSTATE jsJoystickState)
{
   if (!g_bGameOver)
   {
      if (jsJoystickState & JOY_UP)
      {
         SteerCycle(1, 0);
      }
      else if (jsJoystickState & JOY_RIGHT)
      {
         SteerCycle(1, 1);
      }
      else if (jsJoystickState & JOY_DOWN)
      {
         SteerCycle(1, 2);
      }
      else if (jsJoystickState & JOY_LEFT)
      {
         SteerCycle(1, 3);
      }
   }
   else if (jsJoystickState & JOY_FIRE1)
   {
      NewGame();
   }
}

void NewGame()
{
   g_ptCyclePos[0].x   = 250 - (g_pCycle[0][0]->GetWidth() / 2);
   g_ptCyclePos[0].y   = 400 - g_pCycle[0][0]->GetHeight();
   g_ptCycleSpeed[0].x = 0;
   g_ptCycleSpeed[0].y = -g_iSPEED;

   g_ptCyclePos[1].x   = 250 - (g_pCycle[1][0]->GetWidth() / 2);
   g_ptCyclePos[1].y   = 0;
   g_ptCycleSpeed[1].x = 0;
   g_ptCycleSpeed[1].y = g_iSPEED;

   g_iTrailLen[0]         = g_iTrailLen[1] = 2;
   g_ptCycleTrail[0][0].x = g_ptCycleTrail[0][1].x = 250;
   g_ptCycleTrail[0][0].y = g_ptCycleTrail[0][1].y = 400;
   g_ptCycleTrail[1][0].x = g_ptCycleTrail[1][1].x = 250;
   g_ptCycleTrail[1][0].y = g_ptCycleTrail[1][1].y = 0;

   g_bGameOver = FALSE;
}

void UpdateCycles()
{
   for (int i { }; i < 2; i++)
   {
      g_ptCyclePos[i].x = g_ptCyclePos[i].x + g_ptCycleSpeed[i].x;
      g_ptCyclePos[i].y = g_ptCyclePos[i].y + g_ptCycleSpeed[i].y;

      g_ptCycleTrail[i][g_iTrailLen[i] - 1].x = g_ptCyclePos[i].x + (g_pCycle[i][0]->GetWidth() / 2);
      g_ptCycleTrail[i][g_iTrailLen[i] - 1].y = g_ptCyclePos[i].y + (g_pCycle[i][0]->GetHeight() / 2);

      if (g_ptCyclePos[i].x < 0 || g_ptCyclePos[i].x >(500 - g_pCycle[i][0]->GetWidth()) ||
          g_ptCyclePos[i].y < 0 || g_ptCyclePos[i].y >(400 - g_pCycle[i][0]->GetHeight()))
      {
         EndGame(1 - i);
         return;
      }

      RECT rcTmpTrail;

      if (g_iTrailLen[i] > 2)
      {
         for (int j { }; j < g_iTrailLen[i] - 2; j++)
         {
            rcTmpTrail.left   = min(g_ptCycleTrail[i][j].x, g_ptCycleTrail[i][j + 1].x) - 1;
            rcTmpTrail.right  = max(g_ptCycleTrail[i][j].x, g_ptCycleTrail[i][j + 1].x) + 1;
            rcTmpTrail.top    = min(g_ptCycleTrail[i][j].y, g_ptCycleTrail[i][j + 1].y) - 1;
            rcTmpTrail.bottom = max(g_ptCycleTrail[i][j].y, g_ptCycleTrail[i][j + 1].y) + 1;

            if (PtInRect(&rcTmpTrail, g_ptCycleTrail[i][g_iTrailLen[i] - 1]) != 0)
            {
               EndGame(1 - i);
               return;
            }
         }
      }

      for (int j { }; j <= g_iTrailLen[1 - i] - 2; j++)
      {
         rcTmpTrail.left   = min(g_ptCycleTrail[1 - i][j].x, g_ptCycleTrail[1 - i][j + 1].x) - 3;
         rcTmpTrail.right  = max(g_ptCycleTrail[1 - i][j].x, g_ptCycleTrail[1 - i][j + 1].x) + 3;
         rcTmpTrail.top    = min(g_ptCycleTrail[1 - i][j].y, g_ptCycleTrail[1 - i][j + 1].y) - 3;
         rcTmpTrail.bottom = max(g_ptCycleTrail[1 - i][j].y, g_ptCycleTrail[1 - i][j + 1].y) + 3;

         if (PtInRect(&rcTmpTrail, g_ptCycleTrail[i][g_iTrailLen[i] - 1]) != 0)
         {
            EndGame(1 - i);
            return;
         }
      }
   }
}

void SteerCycle(int iCycle, int iDirection)
{
   POINT ptOldSpeed;

   ptOldSpeed.x = g_ptCycleSpeed[iCycle].x;
   ptOldSpeed.y = g_ptCycleSpeed[iCycle].y;

   switch (iDirection)
   {
   case 0:
      if (g_ptCycleSpeed[iCycle].y == 0)
      {
         g_ptCycleSpeed[iCycle].x = 0;
         g_ptCycleSpeed[iCycle].y = -g_iSPEED;
      }
      break;

   case 1:
      if (g_ptCycleSpeed[iCycle].x == 0)
      {
         g_ptCycleSpeed[iCycle].x = g_iSPEED;
         g_ptCycleSpeed[iCycle].y = 0;
      }
      break;

   case 2:
      if (g_ptCycleSpeed[iCycle].y == 0)
      {
         g_ptCycleSpeed[iCycle].x = 0;
         g_ptCycleSpeed[iCycle].y = g_iSPEED;
      }
      break;

   case 3:
      if (g_ptCycleSpeed[iCycle].x == 0)
      {
         g_ptCycleSpeed[iCycle].x = -g_iSPEED;
         g_ptCycleSpeed[iCycle].y = 0;
      }
      break;
   }

   if ((g_ptCycleSpeed[iCycle].x != ptOldSpeed.x) || (g_ptCycleSpeed[iCycle].y != ptOldSpeed.y))
   {
      g_iTrailLen[iCycle]++;

      g_ptCycleTrail[iCycle][g_iTrailLen[iCycle] - 1].x = g_ptCyclePos[iCycle].x + (g_pCycle[iCycle][0]->GetWidth() / 2);
      g_ptCycleTrail[iCycle][g_iTrailLen[iCycle] - 1].y = g_ptCyclePos[iCycle].y + (g_pCycle[iCycle][0]->GetHeight() / 2);
   }
}

void EndGame(int iCycle)
{
   g_bGameOver = TRUE;

   if (iCycle == 0)
   {
      MessageBoxW(g_pGame->GetWindow(), L"Blue wins!", L"Light Cycles", MB_OK);
   }
   else
   {
      MessageBox(g_pGame->GetWindow(), L"Orange wins!", L"Light Cycles", MB_OK);
   }
}