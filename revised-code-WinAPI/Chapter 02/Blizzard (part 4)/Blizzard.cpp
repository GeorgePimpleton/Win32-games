#include "Blizzard.hpp"

BOOL GameInitialize( HINSTANCE inst )
{
   g_game = new GameEngine( inst, L"Blizzard", L"Blizzard d",
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
   srand( (unsigned int) GetTickCount64( ) );
}

void GameEnd( )
{
   delete g_game;
}

void GameActivate( HWND wnd )
{
   HDC  dc = GetDC( wnd );
   RECT rect;

   GetClientRect( wnd, &rect );

   DrawTextW( dc, L"Here comes the blizzard!", -1, &rect,
              DT_SINGLELINE | DT_CENTER | DT_VCENTER );

   ReleaseDC( wnd, dc );
}

void GameDeactivate( HWND wnd )
{
   HDC  dc = GetDC( wnd );
   RECT rect;

   GetClientRect( wnd, &rect );

   DrawTextW( dc, L"The blizzard has passed.", -1, &rect,
              DT_SINGLELINE | DT_CENTER | DT_VCENTER );

   ReleaseDC( wnd, dc );
}

void GamePaint( HDC dc )
{ }

void GameCycle( )
{
   HWND wnd = g_game-> GetWindow( );
   HDC  dc  = GetDC( wnd );

   DrawIcon( dc, rand( ) % g_game->GetWidth( ), rand( ) % g_game->GetHeight( ),
             ( HICON ) GetClassLongPtrW( wnd, GCLP_HICON ) );

   ReleaseDC( wnd, dc );
}
