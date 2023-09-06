#include "Brainiac.hpp"

HRESULT GameInitialize(HINSTANCE inst)
{
   g_game = std::make_unique<GameEngine>(inst, L"Brainiac", L"Example Game: Brainiac",
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
   rtk::srand();

   HINSTANCE inst = g_game->GetInstance();

   g_tiles[ 0 ] = std::make_unique<Bitmap>(IDB_TILEBLANK, inst);
   g_tiles[ 1 ] = std::make_unique<Bitmap>(IDB_TILE1, inst);
   g_tiles[ 2 ] = std::make_unique<Bitmap>(IDB_TILE2, inst);
   g_tiles[ 3 ] = std::make_unique<Bitmap>(IDB_TILE3, inst);
   g_tiles[ 4 ] = std::make_unique<Bitmap>(IDB_TILE4, inst);
   g_tiles[ 5 ] = std::make_unique<Bitmap>(IDB_TILE5, inst);
   g_tiles[ 6 ] = std::make_unique<Bitmap>(IDB_TILE6, inst);
   g_tiles[ 7 ] = std::make_unique<Bitmap>(IDB_TILE7, inst);
   g_tiles[ 8 ] = std::make_unique<Bitmap>(IDB_TILE8, inst);

   g_gameOver = TRUE;

   GameNew( );
}

void GameNew( )
{
   // check if the game is over so a new one can be started
   if ( g_gameOver )
   {
      // clear the tile states and images
      for ( UINT i = 0; i < 4; i++ )
      {
         for ( UINT j = 0; j < 4; j++ )
         {
            g_tileState[ i ][ j ] = FALSE;
            g_tile[ i ][ j ] = 0;
         }
      }

      // initialize the tile images randomly
      for ( UINT i = 0; i < 2; i++ )
      {
         for ( UINT j = 1; j < 9; j++ )
         {
            int x = rtk::rand(0, 3);
            int y = rtk::rand(0, 3);

            while ( g_tile[ x ][ y ] != 0 )
            {
               x = rtk::rand(0, 3);
               y = rtk::rand(0, 3);
            }

            g_tile[ x ][ y ] = j;
         }
      }

      // initialize the tile selections and match/try count
      g_tile1.x = g_tile1.y = -1;
      g_tile2.x = g_tile2.y = -1;
      g_matches = g_tries = 0;

      // force a repaint to update the tiles
      InvalidateRect(g_game->GetWindow( ), nullptr, FALSE);

      g_gameOver = FALSE;
   }
}

void GameEnd( )
{ }

void GameActivate(HWND wnd)
{ }

void GameDeactivate(HWND wnd)
{ }

void GamePaint(HDC dc)
{
   // draw the tiles
   UINT tileWidth  { (UINT) g_tiles[ 0 ]->GetWidth() };
   UINT tileHeight { (UINT) g_tiles[ 0 ]->GetHeight() };

   for ( UINT i = 0; i < 4; i++ )
   {
      for ( UINT j = 0; j < 4; j++ )
      {
         if ( g_tileState[ i ][ j ] || ((i == g_tile1.x) && (j == g_tile1.y)) ||
             ((i == g_tile2.x) && (j == g_tile2.y)) )
         {
            g_tiles[ g_tile[ i ][ j ] ]->Draw(dc, i * tileWidth, j * tileHeight, TRUE);
         }
         else
         {
            g_tiles[ 0 ]->Draw(dc, i * tileWidth, j * tileHeight, TRUE);
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
      GameNew( );
      return;

   case IDM_GAME_EXIT:
      GameEnd();
      PostQuitMessage(0);
      return;

   case IDM_HELP_ABOUT:
      DialogBoxW(g_game->GetInstance(), MAKEINTRESOURCEW(IDD_ABOUT), g_game->GetWindow(), (DLGPROC) DlgProc);
      return;
   }
}

void HandleKeys( )
{ }

void MouseButtonDown(LONG x, LONG y, BOOL left)
{
   // determine which tile was clicked
   int tileX = x / g_tiles[ 0 ]->GetWidth();
   int tileY = y / g_tiles[ 0 ]->GetHeight();

   // make sure the tile hasn't already been matched
   if ( !g_tileState[ tileX ][ tileY ] )
   {
      // see if this is the first tile selected
      if ( g_tile1.x == -1 )
      {
         // set the first tile selection
         g_tile1.x = tileX;
         g_tile1.y = tileY;
      }
      else if ( (tileX != g_tile1.x) || (tileY != g_tile1.y) )
      {
         if ( g_tile2.x == -1 )
         {
            // increase the number of tries
            g_tries++;

            // set the second tile selection
            g_tile2.x = tileX;
            g_tile2.y = tileY;

            // see if it's a match
            if ( g_tile[ g_tile1.x ][ g_tile1.y ] == g_tile[ g_tile2.x ][ g_tile2.y ] )
            {
               // set the tile state to indicate the match
               g_tileState[ g_tile1.x ][ g_tile1.y ] = TRUE;
               g_tileState[ g_tile2.x ][ g_tile2.y ] = TRUE;

               // clear the tile selections
               g_tile1.x = g_tile1.y = g_tile2.x = g_tile2.y = -1;

               // update the match count
               ++g_matches;
            }
         }
         else
         {
            // clear the tile selections
            g_tile1.x = g_tile1.y = g_tile2.x = g_tile2.y = -1;
         }
      }

      // force a repaint to update the tile
      InvalidateRect(g_game->GetWindow(), nullptr, FALSE);

      // check for winner
      if ( g_matches == 8 )
      {
         WCHAR szText[ 64 ];
         wsprintfW(szText, L"You won in %d tries.", g_tries);
         MessageBoxW(g_game->GetWindow(), szText, L"Brainiac", MB_OK);
         g_gameOver = TRUE;
      }
   }
}

void MouseButtonUp(LONG x, LONG y, BOOL left)
{ }

void MouseMove(LONG x, LONG y)
{ }