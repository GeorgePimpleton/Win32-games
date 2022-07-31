#include "Brainiac.hpp"

HRESULT GameInitialize(HINSTANCE hInstance)
{
   g_pGame = std::make_unique<GameEngine>(hInstance, L"Brainiac", L"Example Game: Brainiac",
                                          IDI_ICON, IDI_ICON_SM, 528, 508);

   if (g_pGame == NULL)
   {
      return E_FAIL;
   }

   g_pGame->SetFrameRate(1);

   return S_OK;
}

void GameStart(HWND hWindow)
{
   rtk::srand();

   HINSTANCE hInstance { g_pGame->GetInstance() };

   g_pTiles[0] = std::make_unique<Bitmap>(IDB_TILEBLANK, hInstance);
   g_pTiles[1] = std::make_unique<Bitmap>(IDB_TILE1, hInstance);
   g_pTiles[2] = std::make_unique<Bitmap>(IDB_TILE2, hInstance);
   g_pTiles[3] = std::make_unique<Bitmap>(IDB_TILE3, hInstance);
   g_pTiles[4] = std::make_unique<Bitmap>(IDB_TILE4, hInstance);
   g_pTiles[5] = std::make_unique<Bitmap>(IDB_TILE5, hInstance);
   g_pTiles[6] = std::make_unique<Bitmap>(IDB_TILE6, hInstance);
   g_pTiles[7] = std::make_unique<Bitmap>(IDB_TILE7, hInstance);
   g_pTiles[8] = std::make_unique<Bitmap>(IDB_TILE8, hInstance);

   for (UINT i { }; i < 4; i++)
   {
      for (UINT j { }; j < 4; j++)
      {
         g_bTileStates[i][j] = FALSE;
         g_iTiles[i][j]      = 0;
      }
   }

   for (UINT i { }; i < 2; i++)
   {
      for (UINT j { 1 }; j < 9; j++)
      {
         int x { rtk::rand(0, 3) };
         int y { rtk::rand(0, 3) };

         while (g_iTiles[x][y] != 0)
         {
            x = rtk::rand(0, 3);
            y = rtk::rand(0, 3);
         }

         g_iTiles[x][y] = j;
      }
   }

   g_ptTile1.x = g_ptTile1.y = -1;
   g_ptTile2.x = g_ptTile2.y = -1;
   g_iMatches  = g_iTries    = 0;
}

void GameEnd()
{
}

void GameActivate(HWND hWindow)
{}

void GameDeactivate(HWND hWindow)
{}

void GamePaint(HDC hDC)
{
   UINT iTileWidth  { (UINT) g_pTiles[0]->GetWidth() };
   UINT iTileHeight { (UINT) g_pTiles[0]->GetHeight() };

   for (UINT i { }; i < 4; i++)
   {
      for (UINT j { }; j < 4; j++)
      {
         if (g_bTileStates[i][j] || ((i == g_ptTile1.x) && (j == g_ptTile1.y)) ||
                                    ((i == g_ptTile2.x) && (j == g_ptTile2.y)))
         {
            g_pTiles[g_iTiles[i][j]]->Draw(hDC, i * iTileWidth, j * iTileHeight, TRUE);
         }
         else
         {
            g_pTiles[0]->Draw(hDC, i * iTileWidth, j * iTileHeight, TRUE);
         }
      }
   }
}

void GameCycle()
{}

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
{}

void MouseButtonDown(LONG x, LONG y, BOOL bLeft)
{
   int iTileX { x / g_pTiles[0]->GetWidth() };
   int iTileY { y / g_pTiles[0]->GetHeight() };

   if (!g_bTileStates[iTileX][iTileY])
   {
      if (g_ptTile1.x == -1)
      {
         g_ptTile1.x = iTileX;
         g_ptTile1.y = iTileY;
      }
      else if ((iTileX != g_ptTile1.x) || (iTileY != g_ptTile1.y))
      {
         if (g_ptTile2.x == -1)
         {
            g_iTries++;

            g_ptTile2.x = iTileX;
            g_ptTile2.y = iTileY;

            if (g_iTiles[g_ptTile1.x][g_ptTile1.y] == g_iTiles[g_ptTile2.x][g_ptTile2.y])
            {
               g_bTileStates[g_ptTile1.x][g_ptTile1.y] = TRUE;
               g_bTileStates[g_ptTile2.x][g_ptTile2.y] = TRUE;

               g_ptTile1.x = g_ptTile1.y = g_ptTile2.x = g_ptTile2.y = -1;

               ++g_iMatches;
            }
         }
         else
         {
            g_ptTile1.x = g_ptTile1.y = g_ptTile2.x = g_ptTile2.y = -1;
         }
      }

      InvalidateRect(g_pGame->GetWindow(), NULL, FALSE);

      if (g_iMatches == 8)
      {
         TCHAR szText[64];
         wsprintfW(szText, L"You won in %d tries.", g_iTries);
         MessageBoxW(g_pGame->GetWindow(), szText, L"Brainiac", MB_OK);
      }
   }
}

void MouseButtonUp(LONG x, LONG y, BOOL bLeft)
{}

void MouseMove(LONG x, LONG y)
{
}