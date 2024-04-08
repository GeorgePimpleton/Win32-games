#include "Slideshow.h"

BOOL GameInitialize( HINSTANCE hInstance )
{
   g_pGame = new GameEngine( hInstance, TEXT( "Slideshow" ),
                             TEXT( "Slideshow b: Drawing Graphical Images" ), IDI_SLIDESHOW, IDI_SLIDESHOW_SM );
   if ( g_pGame == NULL )
      return FALSE;

   g_pGame->SetFrameRate( 1 );

   g_hInstance = hInstance;

   return TRUE;
}

void GameStart( HWND hWindow )
{
   HDC hDC = GetDC( hWindow );

   g_pSlides[0] = new Bitmap( hDC, 640, 480, RGB( 128, 128, 64 ) );
   g_pSlides[1] = new Bitmap( TEXT( "Res/Image1.bmp" ) );
   g_pSlides[2] = new Bitmap( TEXT( "Res/Image2.bmp" ) );
   g_pSlides[3] = new Bitmap( TEXT( "Res/Image3.bmp" ) );
   g_pSlides[4] = new Bitmap( IDB_IMAGE4 );
   g_pSlides[5] = new Bitmap( IDB_IMAGE5 );

   g_iCurSlide = 1;
}

void GameEnd( )
{
   for ( int i = 0; i < g_iNUMSLIDES; i++ )
      delete g_pSlides[i];

   delete g_pGame;
}

void GameActivate( HWND hWindow )
{ }

void GameDeactivate( HWND hWindow )
{ }

void GamePaint( HDC hDC )
{
   g_pSlides[g_iCurSlide]->Draw( hDC, 0, 0 );
}

void GameCycle( )
{
   static int iDelay = 0;

   if ( ++iDelay > 3 )
   {
      iDelay = 0;

      if ( ++g_iCurSlide == g_iNUMSLIDES )
         g_iCurSlide = 0;

      InvalidateRect( g_pGame->GetWindow( ), NULL, FALSE );
   }
}
