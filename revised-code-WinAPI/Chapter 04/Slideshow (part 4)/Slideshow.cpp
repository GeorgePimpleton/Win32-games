#include "Slideshow.hpp"

BOOL GameInitialize( HINSTANCE inst )
{
   g_game = new GameEngine( inst, L"Slideshow", L"Slideshow d: Drawing Graphical Images",
                            IDI_ICON, IDI_ICON_SM );

   if ( g_game == NULL )
   {
      return FALSE;
   }

   g_game->SetFrameRate( 1 );

   g_inst = inst;

   return TRUE;
}

void GameStart( HWND wnd )
{
   g_slides[0] = new Bitmap( GetDC( wnd ), 640, 480, RGB( 128, 128, 64 ) );
   g_slides[1] = new Bitmap( L"Res/Image1.bmp" );
   g_slides[2] = new Bitmap( L"Res/Image2.bmp" );
   g_slides[3] = new Bitmap( L"Res/Image3.bmp" );
   g_slides[4] = new Bitmap( IDB_IMAGE4 );
   g_slides[5] = new Bitmap( IDB_IMAGE5 );
   g_slides[6] = new Bitmap( IDB_IMAGE6 );

   g_currentSlide = 1;
}

void GameEnd( )
{
   for ( int i = 0; i < g_NUMSLIDES; i++ )
   {
      delete g_slides[i];
   }

   delete g_game;
}

void GameActivate( HWND hWindow )
{ }

void GameDeactivate( HWND hWindow )
{ }

void GamePaint( HDC hDC )
{
   g_slides[g_currentSlide]->Draw( hDC, 0, 0 );
}

void GameCycle( )
{
   static UINT delay = 0;

   if ( ++delay > 3 )
   {
      delay = 0;

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
