#include "Slideshow.hpp"

HRESULT GameInitialize( HINSTANCE inst )
{
   g_game = std::make_unique<GameEngine>( inst, L"Slideshow", L"Slideshow d: Drawing Graphical Images",
                                          IDI_ICON, IDI_ICON_SM );

   if ( NULL == g_game )
   {
      return E_FAIL;
   }

   g_game->SetFrameRate( 1 );

   g_inst = inst;

   return S_OK;
}

void GameStart( HWND wnd )
{
   g_slides[0] = std::make_unique<Bitmap>( GetDC( wnd ), 640, 480, RGB( 128, 128, 64 ) );
   g_slides[1] = std::make_unique<Bitmap>( L"Res/Image1.bmp" );
   g_slides[2] = std::make_unique<Bitmap>( L"Res/Image2.bmp" );
   g_slides[3] = std::make_unique<Bitmap>( L"Res/Image3.bmp" );
   g_slides[4] = std::make_unique<Bitmap>( IDB_IMAGE4 );
   g_slides[5] = std::make_unique<Bitmap>( IDB_IMAGE5 );
   g_slides[6] = std::make_unique<Bitmap>( IDB_IMAGE6 );

   g_currentSlide = 1;
}

void GameEnd( )
{ }

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
