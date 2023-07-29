// "Talk to me like I'm a 3 year old!" Programming Lessons - Windows Games

// Brainiac 2.cpp - Brainiac 2 Game Source

#include "Brainiac 2.hpp"

HRESULT GameInitialize(HINSTANCE hInstance)
{
   _pGame = new GameEngine(hInstance, L"Brainiac2", L"Playing Wave Sounds",
                           IDI_ICON, IDI_ICON_SM, 528, 508);

   if (_pGame == NULL)
   {
      return E_FAIL;
   }

   _pGame->SetFrameRate(1);

   return S_OK;
}

void GameStart(HWND hWindow)
{
   rtk::srand();

   HINSTANCE hInstance = GetModuleHandleW(NULL);

   _pTiles[0] = new Bitmap(IDB_TILEBLANK, hInstance);
   _pTiles[1] = new Bitmap(IDB_TILE1, hInstance);
   _pTiles[2] = new Bitmap(IDB_TILE2, hInstance);
   _pTiles[3] = new Bitmap(IDB_TILE3, hInstance);
   _pTiles[4] = new Bitmap(IDB_TILE4, hInstance);
   _pTiles[5] = new Bitmap(IDB_TILE5, hInstance);
   _pTiles[6] = new Bitmap(IDB_TILE6, hInstance);
   _pTiles[7] = new Bitmap(IDB_TILE7, hInstance);
   _pTiles[8] = new Bitmap(IDB_TILE8, hInstance);

   NewGame();
}

void GameEnd()
{
   for (int i = 0; i < 9; i++)
   {
      delete _pTiles[i];
   }

   delete _pGame;
}

void GameActivate(HWND hWindow)
{
}

void GameDeactivate(HWND hWindow)
{
}

void GamePaint(HDC hDC)
{
   // draw the tiles
   UINT iTileWidth  = _pTiles[0]->GetWidth();
   UINT iTileHeight = _pTiles[0]->GetHeight();

   for (UINT i = 0; i < 4; i++)
   {
      for (UINT j = 0; j < 4; j++)
      {
         if (_bTileStates[i][j] || ((i == _ptTile1.x) && (j == _ptTile1.y)) || ((i == _ptTile2.x) && (j == _ptTile2.y)))
         {
            _pTiles[_iTiles[i][j]]->Draw(hDC, i * iTileWidth, j * iTileHeight, TRUE);
         }
         else
         {
            _pTiles[0]->Draw(hDC, i * iTileWidth, j * iTileHeight, TRUE);
         }
      }
   }
}

void GameCycle()
{
}

void GameMenu(WPARAM wParam)
{
   switch (LOWORD(wParam))
   {
   case IDM_GAME_NEW:
      NewGame();
      return;

   case IDM_GAME_EXIT:
      GameEnd();
      PostQuitMessage(0);
      return;

   case IDM_HELP_ABOUT:
      DialogBoxW(_pGame->GetInstance(), MAKEINTRESOURCEW(IDD_ABOUT), _pGame->GetWindow(), (DLGPROC) DlgProc);
      return;
   }
}

void HandleKeys()
{
}

void MouseButtonDown(LONG x, LONG y, BOOL bLeft)
{
   // determine which tile was clicked
   UINT iTileX = x / _pTiles[0]->GetWidth();
   UINT iTileY = y / _pTiles[0]->GetHeight();

   // make sure the tile hasn't already been matched
   if (!_bTileStates[iTileX][iTileY])
   {
      // see if this is the first tile selected
      if (_ptTile1.x == -1)
      {
         // play a sound for the tile selection
         PlaySoundW((PCWSTR) IDW_SELECT, _hInstance, SND_ASYNC | SND_RESOURCE);

         // set the first tile selection
         _ptTile1.x = iTileX;
         _ptTile1.y = iTileY;
      }
      else if ((iTileX != _ptTile1.x) || (iTileY != _ptTile1.y))
      {
         if (_ptTile2.x == -1)
         {
            // play a sound for the tile selection
            PlaySoundW((PCWSTR) IDW_SELECT, _hInstance, SND_ASYNC | SND_RESOURCE);

            // increase the number of tries
            _iTries++;

            // set the second tile selection
            _ptTile2.x = iTileX;
            _ptTile2.y = iTileY;

            // see if it's a match
            if (_iTiles[_ptTile1.x][_ptTile1.y] == _iTiles[_ptTile2.x][_ptTile2.y])
            {
               // play a sound for the tile match
               PlaySoundW((PCWSTR) IDW_MATCH, _hInstance, SND_ASYNC | SND_RESOURCE);

               // set the tile state to indicate the match
               _bTileStates[_ptTile1.x][_ptTile1.y] = TRUE;
               _bTileStates[_ptTile2.x][_ptTile2.y] = TRUE;

               // clear the tile selections
               _ptTile1.x = _ptTile1.y = _ptTile2.x = _ptTile2.y = -1;

               // update the match count
               ++_iMatches;
            }
            else
            {
               // play a sound for the tile mismatch
               PlaySoundW((PCWSTR) IDW_MISMATCH, _hInstance, SND_ASYNC | SND_RESOURCE);
            }
         }
         else
         {
            // clear the tile selections
            _ptTile1.x = _ptTile1.y = _ptTile2.x = _ptTile2.y = -1;
         }
      }

      // force a repaint to update the tiles
      InvalidateRect(_pGame->GetWindow(), NULL, FALSE);

      // check for winner
      if (_iMatches == 8)
      {
         // play a victory sound
         PlaySoundW((PCWSTR) IDW_WIN, _hInstance, SND_ASYNC | SND_RESOURCE);

         TCHAR szText[64];

         wsprintfW(szText, L"You won in %d tries.", _iTries);

         MessageBoxW(_pGame->GetWindow(), szText, L"Brainiac", MB_OK);
      }
   }
}

void MouseButtonUp(LONG x, LONG y, BOOL bLeft)
{
}

void MouseMove(LONG x, LONG y)
{
}

void HandleJoystick(JOYSTATE jsJoystickState)
{
}

BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee)
{
   return FALSE;
}

void NewGame()
{
   // clear the tile states and images
   for (int i = 0; i < 4; i++)
   {
      for (int j = 0; j < 4; j++)
      {
         _bTileStates[i][j] = FALSE;
         _iTiles[i][j]      = 0;
      }
   }

   // initialize the tile images randomly
   for (int i = 0; i < 2; i++)
   {
      for (int j = 1; j < 9; j++)
      {
         int x = rtk::rand(0, 3);
         int y = rtk::rand(0, 3);

         while (_iTiles[x][y] != 0)
         {
            x = rtk::rand(0, 3);
            y = rtk::rand(0, 3);
         }
         _iTiles[x][y] = j;
      }
   }

   // initialize the tile selections and match/try count
   _ptTile1.x = _ptTile1.y = -1;
   _ptTile2.x = _ptTile2.y = -1;
   _iMatches  = _iTries = 0;

   // force a repaint to update the tiles
   InvalidateRect(_pGame->GetWindow(), NULL, FALSE);
}