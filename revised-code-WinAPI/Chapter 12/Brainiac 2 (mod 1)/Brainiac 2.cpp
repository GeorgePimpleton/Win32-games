#include "Brainiac 2.hpp"

HRESULT GameInitialize(HINSTANCE inst)
{
   g_game = new GameEngine(inst, L"Brainiac2", L"Playing Wave Sounds",
                           IDI_ICON, IDI_ICON_SM, 528, 508);

   if ( nullptr == g_game )
   {
      return E_FAIL;
   }

   g_game->SetFrameRate(1);

   return S_OK;
}

void GameStart(HWND wnd)
{
   rtk::srand( );

   HINSTANCE inst = GetModuleHandleW(nullptr);

   g_tile[ 0 ] = new Bitmap(IDB_TILEBLANK, inst);
   g_tile[ 1 ] = new Bitmap(IDB_TILE1, inst);
   g_tile[ 2 ] = new Bitmap(IDB_TILE2, inst);
   g_tile[ 3 ] = new Bitmap(IDB_TILE3, inst);
   g_tile[ 4 ] = new Bitmap(IDB_TILE4, inst);
   g_tile[ 5 ] = new Bitmap(IDB_TILE5, inst);
   g_tile[ 6 ] = new Bitmap(IDB_TILE6, inst);
   g_tile[ 7 ] = new Bitmap(IDB_TILE7, inst);
   g_tile[ 8 ] = new Bitmap(IDB_TILE8, inst);

   NewGame( );
}

void GameEnd( )
{
   for ( int i = 0; i < 9; i++ )
   {
      delete g_tile[ i ];
   }

   delete g_game;
}

void GameActivate(HWND hWindow)
{ }

void GameDeactivate(HWND hWindow)
{ }

void GamePaint(HDC dc)
{
   // draw the tiles
   UINT tileWidth  = g_tile[ 0 ]-> GetWidth( );
   UINT tileHeight = g_tile[ 0 ]-> GetHeight( );

   for ( UINT i = 0; i < 4; i++ )
   {
      for ( UINT j = 0; j < 4; j++ )
      {
         if ( g_tileStates[ i ][ j ] || ((i == g_tile1.x) && (j == g_tile1.y)) || ((i == g_tile2.x) && (j == g_tile2.y)) )
         {
            g_tile[ g_tiles[ i ][ j ] ]->Draw(dc, i * tileWidth, j * tileHeight, TRUE);
         }
         else
         {
            g_tile[ 0 ]->Draw(dc, i * tileWidth, j * tileHeight, TRUE);
         }
      }
   }
}

void GameCycle( )
{ }

void GameMenu(WPARAM wParam)
{
   switch ( LOWORD(wParam) )
   {
   case IDM_GAME_NEW:
      NewGame( );
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

void HandleKeys( )
{ }

void MouseButtonDown(LONG x, LONG y, BOOL bLeft)
{
   // determine which tile was clicked
   UINT tileX = x / g_tile[ 0 ]->GetWidth( );
   UINT tileY = y / g_tile[ 0 ]->GetHeight( );

   // make sure the tile hasn't already been matched
   if ( !g_tileStates[ tileX ][ tileY ] )
   {
      // see if this is the first tile selected
      if ( g_tile1.x == -1 )
      {
         // play a sound for the tile selection
         PlaySoundW((PCWSTR) IDW_SELECT, g_inst, SND_ASYNC | SND_RESOURCE);

         // set the first tile selection
         g_tile1.x = tileX;
         g_tile1.y = tileY;
      }
      else if ( (tileX != g_tile1.x) || (tileY != g_tile1.y) )
      {
         if ( g_tile2.x == -1 )
         {
            // play a sound for the tile selection
            PlaySoundW((PCWSTR) IDW_SELECT, g_inst, SND_ASYNC | SND_RESOURCE);

            // increase the number of tries
            g_tries++;

            // set the second tile selection
            g_tile2.x = tileX;
            g_tile2.y = tileY;

            // see if it's a match
            if ( g_tiles[ g_tile1.x ][ g_tile1.y ] == g_tiles[ g_tile2.x ][ g_tile2.y ] )
            {
               // play a sound for the tile match
               PlaySoundW((PCWSTR) IDW_MATCH, g_inst, SND_ASYNC | SND_RESOURCE);

               // set the tile state to indicate the match
               g_tileStates[ g_tile1.x ][ g_tile1.y ] = TRUE;
               g_tileStates[ g_tile2.x ][ g_tile2.y ] = TRUE;

               // clear the tile selections
               g_tile1.x = g_tile1.y = g_tile2.x = g_tile2.y = -1;

               // update the match count
               ++g_matches;
            }
            else
            {
               // play a sound for the tile mismatch
               PlaySoundW((PCWSTR) IDW_MISMATCH, g_inst, SND_ASYNC | SND_RESOURCE);
            }
         }
         else
         {
            // clear the tile selections
            g_tile1.x = g_tile1.y = g_tile2.x = g_tile2.y = -1;
         }
      }

      // force a repaint to update the tiles
      InvalidateRect(g_game->GetWindow( ), nullptr, FALSE);

      // check for winner
      if ( g_matches == 8 )
      {
         // play a victory sound
         PlaySoundW((PCWSTR) IDW_WIN, g_inst, SND_ASYNC | SND_RESOURCE);

         TCHAR szText[ 64 ];

         wsprintfW(szText, L"You won in %d tries.", g_tries);

         MessageBoxW(g_game->GetWindow( ), szText, L"Brainiac", MB_OK);
      }
   }
}

void MouseButtonUp(LONG x, LONG y, BOOL bLeft)
{ }

void MouseMove(LONG x, LONG y)
{ }

void HandleJoystick(JOYSTATE jsJoystickState)
{ }

BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee)
{
   return FALSE;
}

void NewGame( )
{
   // clear the tile states and images
   for ( int i = 0; i < 4; i++ )
   {
      for ( int j = 0; j < 4; j++ )
      {
         g_tileStates[ i ][ j ] = FALSE;
         g_tiles[ i ][ j ] = 0;
      }
   }

   // initialize the tile images randomly
   for ( int i = 0; i < 2; i++ )
   {
      for ( int j = 1; j < 9; j++ )
      {
         int x = rtk::rand(0, 3);
         int y = rtk::rand(0, 3);

         while ( g_tiles[ x ][ y ] != 0 )
         {
            x = rtk::rand(0, 3);
            y = rtk::rand(0, 3);
         }
         g_tiles[ x ][ y ] = j;
      }
   }

   // initialize the tile selections and match/try count
   g_tile1.x = g_tile1.y = -1;
   g_tile2.x = g_tile2.y = -1;
   g_matches = g_tries   = 0;

   // force a repaint to update the tiles
   InvalidateRect(g_game->GetWindow( ), nullptr, FALSE);
}