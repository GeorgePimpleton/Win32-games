#include "Brainiac 2.hpp"

HRESULT GameInitialize(HINSTANCE inst)
{
   g_game = std::make_unique<GameEngine>(inst, L"Brainiac2", L"Brainiac 2: Playing Wave Sounds",
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

   g_tile[ 0 ] = std::make_unique<Bitmap>(IDB_TILEBLANK, inst);
   g_tile[ 1 ] = std::make_unique<Bitmap>(IDB_TILE1, inst);
   g_tile[ 2 ] = std::make_unique<Bitmap>(IDB_TILE2, inst);
   g_tile[ 3 ] = std::make_unique<Bitmap>(IDB_TILE3, inst);
   g_tile[ 4 ] = std::make_unique<Bitmap>(IDB_TILE4, inst);
   g_tile[ 5 ] = std::make_unique<Bitmap>(IDB_TILE5, inst);
   g_tile[ 6 ] = std::make_unique<Bitmap>(IDB_TILE6, inst);
   g_tile[ 7 ] = std::make_unique<Bitmap>(IDB_TILE7, inst);
   g_tile[ 8 ] = std::make_unique<Bitmap>(IDB_TILE8, inst);

   GameNew( );
}

void GameNew( )
{
   for ( int i = 0; i < 4; i++ )
   {
      for ( int j = 0; j < 4; j++ )
      {
         g_tileStates[ i ][ j ] = FALSE;
         g_tiles[ i ][ j ] = 0;
      }
   }

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

   g_tile1.x = g_tile1.y = -1;
   g_tile2.x = g_tile2.y = -1;
   g_matches = g_tries = 0;

   EnableMenuItem(GetMenu(g_game->GetWindow( )), (UINT) MAKEINTRESOURCEW(IDM_GAME_NEW), MF_GRAYED);

   InvalidateRect(g_game->GetWindow( ), nullptr, FALSE);
}

void GameEnd( )
{
}

void GameActivate(HWND hWindow)
{ }

void GameDeactivate(HWND hWindow)
{ }

void GamePaint(HDC dc)
{
   UINT tileWidth  = g_tile[ 0 ]->GetWidth( );
   UINT tileHeight = g_tile[ 0 ]->GetHeight( );

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

void HandleKeys( )
{ }

void MouseButtonDown(LONG x, LONG y, BOOL left)
{
   UINT tileX = x / g_tile[ 0 ]->GetWidth( );
   UINT tileY = y / g_tile[ 0 ]->GetHeight( );

   if ( !g_tileStates[ tileX ][ tileY ] )
   {
      if ( g_tile1.x == -1 )
      {
         PlaySoundW((PCWSTR) IDW_SELECT, g_inst, SND_ASYNC | SND_RESOURCE);

         g_tile1.x = tileX;
         g_tile1.y = tileY;
      }
      else if ( (tileX != g_tile1.x) || (tileY != g_tile1.y) )
      {
         if ( g_tile2.x == -1 )
         {
            PlaySoundW((PCWSTR) IDW_SELECT, g_inst, SND_ASYNC | SND_RESOURCE);

            g_tries++;

            g_tile2.x = tileX;
            g_tile2.y = tileY;

            if ( g_tiles[ g_tile1.x ][ g_tile1.y ] == g_tiles[ g_tile2.x ][ g_tile2.y ] )
            {
               PlaySoundW((PCWSTR) IDW_MATCH, g_inst, SND_ASYNC | SND_RESOURCE);

               g_tileStates[ g_tile1.x ][ g_tile1.y ] = TRUE;
               g_tileStates[ g_tile2.x ][ g_tile2.y ] = TRUE;

               g_tile1.x = g_tile1.y = g_tile2.x = g_tile2.y = -1;

               ++g_matches;
            }
            else
            {
               PlaySoundW((PCWSTR) IDW_MISMATCH, g_inst, SND_ASYNC | SND_RESOURCE);
            }
         }
         else
         {
            g_tile1.x = g_tile1.y = g_tile2.x = g_tile2.y = -1;
         }
      }

      InvalidateRect(g_game->GetWindow( ), nullptr, FALSE);

      if ( g_matches == 8 )
      {
         PlaySoundW((PCWSTR) IDW_WIN, g_inst, SND_ASYNC | SND_RESOURCE);

         TCHAR szText[ 64 ];

         wsprintfW(szText, L"You won in %d tries.", g_tries);

         MessageBoxW(g_game->GetWindow( ), szText, L"Brainiac", MB_OK);

         EnableMenuItem(GetMenu(g_game->GetWindow( )), (UINT) MAKEINTRESOURCEW(IDM_GAME_NEW), MF_ENABLED);
      }
   }
}

void MouseButtonUp(LONG x, LONG y, BOOL left)
{ }

void MouseMove(LONG x, LONG y)
{ }

void HandleJoystick(JOYSTATE jsJoystickState)
{ }

BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee)
{
   return FALSE;
}