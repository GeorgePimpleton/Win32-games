#include "Brainiac.hpp"

HRESULT GameInitialize( HINSTANCE inst )
{
   g_game = std::make_unique<GameEngine>( inst, L"Brainiac", L"Example Game: Brainiac d",
                                          IDI_ICON, IDI_ICON_SM, 528, 508 );

   if ( NULL == g_game )
   {
      return E_FAIL;
   }

   g_game->SetFrameRate( 1 );

   return S_OK;
}

void GameStart( HWND wnd )
{
   rtk::srand( );

   g_tiles[0] = std::make_unique<Bitmap>( IDB_TILEBLANK );
   g_tiles[1] = std::make_unique<Bitmap>( IDB_TILE1 );
   g_tiles[2] = std::make_unique<Bitmap>( IDB_TILE2 );
   g_tiles[3] = std::make_unique<Bitmap>( IDB_TILE3 );
   g_tiles[4] = std::make_unique<Bitmap>( IDB_TILE4 );
   g_tiles[5] = std::make_unique<Bitmap>( IDB_TILE5 );
   g_tiles[6] = std::make_unique<Bitmap>( IDB_TILE6 );
   g_tiles[7] = std::make_unique<Bitmap>( IDB_TILE7 );
   g_tiles[8] = std::make_unique<Bitmap>( IDB_TILE8 );

   g_gameOver = TRUE;

   GameNew( );
}

void GameNew( )
{
   if ( g_gameOver )
   {
      for ( UINT i = 0; i < 4; i++ )
      {
         for ( UINT j = 0; j < 4; j++ )
         {
            g_tileState[i][j] = FALSE;
            g_tile[i][j]      = 0;
         }
      }

      for ( UINT i = 0; i < 2; i++ )
      {
         for ( UINT j = 1; j < 9; j++ )
         {
            int x = rtk::rand( 0, 3 );
            int y = rtk::rand( 0, 3 );

            while ( g_tile[x][y] != 0 )
            {
               x = rtk::rand( 0, 3 );
               y = rtk::rand( 0, 3 );
            }

            g_tile[x][y] = j;
         }
      }

      g_tile1.x = g_tile1.y = -1;
      g_tile2.x = g_tile2.y = -1;
      g_matches = g_tries   = 0;

      g_gameOver = FALSE;

      EnableMenuItem( GetMenu( g_game->GetWindow( ) ),
                      ( UINT ) ( ULONGLONG ) MAKEINTRESOURCEW( IDM_GAME_NEW ), MF_GRAYED );

      InvalidateRect( g_game->GetWindow( ), NULL, FALSE );
   }
}

void GameEnd( )
{ }

void GameActivate( HWND wnd )
{ }

void GameDeactivate( HWND wnd )
{ }

void GamePaint( HDC dc )
{
   UINT tileWidth  { ( UINT ) g_tiles[0]->GetWidth( ) };
   UINT tileHeight { ( UINT ) g_tiles[0]->GetHeight( ) };

   for ( UINT i = 0; i < 4; i++ )
   {
      for ( UINT j = 0; j < 4; j++ )
      {
         if ( g_tileState[i][j] || ( ( i == g_tile1.x ) && ( j == g_tile1.y ) ) ||
              ( ( i == g_tile2.x ) && ( j == g_tile2.y ) ) )
         {
            g_tiles[g_tile[i][j]]->Draw( dc, i * tileWidth, j * tileHeight, TRUE );
         }
         else
         {
            g_tiles[0]->Draw( dc, i * tileWidth, j * tileHeight, TRUE );
         }
      }
   }
}

void GameCycle( )
{ }

void GameMenu( WPARAM wParam )
{
   switch ( LOWORD( wParam ) )
   {
   case IDM_GAME_NEW:
      GameNew( );
      return;

   case IDM_GAME_EXIT:
      DestroyWindow( g_game->GetWindow( ) );
      return;

   case IDM_HELP_ABOUT:
      DialogBoxW( g_game->GetInstance( ), MAKEINTRESOURCEW( IDD_ABOUT ),
                  g_game->GetWindow( ), ( DLGPROC ) DlgProc );
      return;
   }
}

void HandleKeys( )
{ }

void MouseButtonDown( LONG x, LONG y, BOOL left )
{
   int tileX = x / g_tiles[0]->GetWidth( );
   int tileY = y / g_tiles[0]->GetHeight( );

   if ( !g_tileState[tileX][tileY] )
   {
      if ( g_tile1.x == -1 )
      {
         g_tile1.x = tileX;
         g_tile1.y = tileY;
      }
      else if ( ( tileX != g_tile1.x ) || ( tileY != g_tile1.y ) )
      {
         if ( g_tile2.x == -1 )
         {
            g_tries++;

            g_tile2.x = tileX;
            g_tile2.y = tileY;

            if ( g_tile[g_tile1.x][g_tile1.y] == g_tile[g_tile2.x][g_tile2.y] )
            {
               g_tileState[g_tile1.x][g_tile1.y] = TRUE;
               g_tileState[g_tile2.x][g_tile2.y] = TRUE;

               g_tile1.x = g_tile1.y = g_tile2.x = g_tile2.y = -1;

               ++g_matches;
            }
         }
         else
         {
            g_tile1.x = g_tile1.y = g_tile2.x = g_tile2.y = -1;
         }
      }

      InvalidateRect( g_game->GetWindow( ), NULL, FALSE );

      if ( g_matches == 8 )
      {
         std::wstring s = std::format( L"You won in {} tries.", g_tries );

         MessageBoxW( g_game->GetWindow( ), s.c_str( ), g_game->GetTitle( ), MB_OK );

         g_gameOver = TRUE;

         EnableMenuItem( GetMenu( g_game->GetWindow( ) ),
                         ( UINT ) ( LONGLONG ) MAKEINTRESOURCEW( IDM_GAME_NEW ), MF_ENABLED );

      }
   }
}

void MouseButtonUp( LONG x, LONG y, BOOL left )
{ }

void MouseMove( LONG x, LONG y )
{ }
