#include "Trippy.hpp"

BOOL GameInitialize(HINSTANCE inst)
{
   g_game = std::make_unique<GameEngine>(inst, L"Trippy", L"Trippy: Learning to Draw Basic Graphics",
                                         IDI_ICON, IDI_ICON_SM);

   if ( g_game == NULL )
   {
      return FALSE;
   }

   g_game->SetFrameRate(15);

   return TRUE;
}

void GameStart(HWND wnd)
{
   rtk::srand( );

   g_rect.left   = g_game->GetWidth( ) * 2 / 5;
   g_rect.top    = g_game->GetHeight( ) * 2 / 5;
   g_rect.right  = g_rect.left + g_game->GetWidth( ) / 5;
   g_rect.bottom = g_rect.top + g_game->GetHeight( ) / 5;
}

void GameEnd( )
{ }

void GameActivate(HWND wnd)
{ }

void GameDeactivate(HWND wnd)
{ }

void GamePaint(HDC dc)
{
   const UINT gridLines = 50;

   for ( UINT i = 1; i <= gridLines; i++ )
   {
      MoveToEx(dc, g_game->GetWidth( ) * i / gridLines, 0, NULL);
      LineTo(dc, g_game->GetWidth( ) * i / gridLines, g_game->GetHeight( ));

      MoveToEx(dc, 0, g_game->GetHeight( ) * i / gridLines, NULL);
      LineTo(dc, g_game->GetWidth( ), g_game->GetHeight( ) * i / gridLines);
   }
}

void GameCycle( )
{
   HDC    dc;
   HWND   wnd = g_game->GetWindow( );
   HBRUSH brush;

   int inflation = rtk::rand(-10, 10);

   InflateRect(&g_rect, inflation, inflation);
   OffsetRect(&g_rect, rtk::rand(-9, 9), rtk::rand(-9, 9));

   brush = CreateSolidBrush(RGB(rtk::rand(0, 255), rtk::rand(0, 255), rtk::rand(0, 255)));
   dc    = GetDC(wnd);

   FillRect(dc, &g_rect, brush);
   ReleaseDC(wnd, dc);
   DeleteObject(brush);
}

void GameMenu(WPARAM wParam)
{
   switch ( LOWORD(wParam) )
   {
   case IDM_GAME_NEW:
      // spaceholder for later game examples
      return;

   case IDM_GAME_EXIT:
      GameEnd( );
      PostQuitMessage(0);
      return;

   case IDM_HELP_ABOUT:
      DialogBoxParamW(g_game->GetInstance( ), MAKEINTRESOURCEW(IDD_ABOUT),
                      g_game->GetWindow( ), (DLGPROC) DlgProc, 0L);
      return;
   }
}