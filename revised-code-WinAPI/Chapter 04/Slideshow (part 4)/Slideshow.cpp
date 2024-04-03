#include "Slideshow.hpp"

BOOL GameInitialize( HINSTANCE inst )
{
   g_game = std::make_unique<GameEngine>( inst, L"Slideshow", L"Slideshow 4: Drawing Graphical Images",
                                          IDI_ICON, IDI_ICON_SM );

   if ( g_game == NULL )
   {
      return FALSE;
   }

   g_game->SetFrameRate( 1 );

   return TRUE;
}

void GameStart( HWND wnd )
{
   HDC dc = GetDC( wnd );

   g_slides[0] = std::make_unique<Bitmap>( dc, 640, 480, RGB( 128, 128, 64 ) );

   g_slides[1] = std::make_unique<Bitmap>( L"Res/Image1.bmp" );
   g_slides[2] = std::make_unique<Bitmap>( L"Res/Image2.bmp" );
   g_slides[3] = std::make_unique<Bitmap>( L"Res/Image3.bmp" );

   HINSTANCE inst = g_game->GetInstance( );

   g_slides[4] = std::make_unique<Bitmap>( IDB_IMAGE4 );
   g_slides[5] = std::make_unique<Bitmap>( IDB_IMAGE5 );
   g_slides[6] = std::make_unique<Bitmap>( IDB_IMAGE6 );

   g_currentSlide = 1;
}

void GameEnd( )
{ }

void GameActivate( HWND wnd )
{ }

void GameDeactivate( HWND wnd )
{ }

void GamePaint( HDC dc )
{
   g_slides[g_currentSlide]->Draw( dc, 0, 0 );
}

void GameCycle( )
{
   static int iDelay = 0;

   if ( ++iDelay > 3 )
   {
      iDelay = 0;

      if ( ++g_currentSlide == g_NUMSLIDES )
      {
         g_currentSlide = 0;
      }

      InvalidateRect( g_game->GetWindow( ), NULL, FALSE );
   }
}

void GameMenu( WPARAM wParam )
{
   switch ( LOWORD( wParam ) )
   {
   case IDM_GAME_NEW:
      // spaceholder for later game examples
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
