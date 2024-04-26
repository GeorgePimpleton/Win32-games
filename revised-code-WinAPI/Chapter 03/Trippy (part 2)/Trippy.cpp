#include "Trippy.hpp"

BOOL GameInitialize( HINSTANCE inst )
{
   g_game = std::make_unique<GameEngine>( inst, L"Trippy", L"Trippy b: Learning to Draw Basic Graphics",
                                          IDI_ICON, IDI_ICON_SM );

   if ( g_game == NULL )
   {
      return FALSE;
   }

   g_game->SetFrameRate( 15 );

   return TRUE;
}

void GameStart( HWND wnd )
{
   srand( ( unsigned ) GetTickCount64( ) );

   g_rect.left   = g_game->GetWidth( )  * 2 / 5;
   g_rect.top    = g_game->GetHeight( ) * 2 / 5;
   g_rect.right  = g_rect.left + g_game->GetWidth( ) / 5;
   g_rect.bottom = g_rect.top + g_game->GetHeight( ) / 5;
}

void GameEnd( )
{ }

void GameActivate( HWND wnd )
{ }

void GameDeactivate( HWND wnd )
{ }

void GamePaint( HDC dc )
{
   const unsigned iGridLines = 50;

   for ( unsigned i = 1; i <= iGridLines; i++ )
   {
      MoveToEx( dc, g_game->GetWidth( ) * i / iGridLines, 0, NULL );
      LineTo( dc, g_game->GetWidth( ) * i / iGridLines, g_game->GetHeight( ) );

      MoveToEx( dc, 0, g_game->GetHeight( ) * i / iGridLines, NULL );
      LineTo( dc, g_game->GetWidth( ), g_game->GetHeight( ) * i / iGridLines );
   }
}

void GameCycle( )
{
   HDC    dc;
   HWND   wnd = g_game->GetWindow( );
   HBRUSH brush;

   int inflation = ( rand( ) % 21 ) - 10;

   InflateRect( &g_rect, inflation, inflation );
   OffsetRect( &g_rect, ( rand( ) % 19 ) - 9, ( rand( ) % 19 ) - 9 );

   brush = CreateSolidBrush( RGB( rand( ) % 256, rand( ) % 256, rand( ) % 256 ) );
   dc    = GetDC( wnd );

   FillRect( dc, &g_rect, brush );
   ReleaseDC( wnd, dc );
   DeleteObject( brush );
}

void GameMenu( WPARAM wParam )
{
   switch ( LOWORD( wParam ) )
   {
   case IDM_GAME_NEW:
      return;

   case IDM_GAME_EXIT:
      DestroyWindow( g_game->GetWindow( ) );
      return;

   case IDM_HELP_ABOUT:
      DialogBoxParamW( g_game->GetInstance( ), MAKEINTRESOURCEW( IDD_ABOUT ),
                       g_game->GetWindow( ), ( DLGPROC ) DlgProc, 0L );
      return;
   }
}
