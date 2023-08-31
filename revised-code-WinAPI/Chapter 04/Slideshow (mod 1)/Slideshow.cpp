#include "Slideshow.hpp"

HRESULT GameInitialize(HINSTANCE hInst)
{
   g_game = std::make_unique<GameEngine>(hInst, L"Slideshow", L"Drawing Graphical Images",
                                         IDI_ICON, IDI_ICON_SM);

   if ( nullptr == g_game )
   {
      return E_FAIL;
   }

   g_game->SetFrameRate(1);

   return S_OK;
}

void GameStart(HWND hwnd)
{
   // create/load the slide bitmaps

   // create a blank colored bitmap
   g_slides[ 0 ] = std::make_unique<Bitmap>(g_game->GetWindow(), 640, 480, RGB(128, 128, 64));

   // load bitmaps from a file
   g_slides[ 1 ] = std::make_unique<Bitmap>(L"Res/Image1.bmp");
   g_slides[ 2 ] = std::make_unique<Bitmap>(L"Res/Image2.bmp");
   g_slides[ 3 ] = std::make_unique<Bitmap>(L"Res/Image3.bmp");

   // load bitmaps as resources
   HINSTANCE inst = g_game->GetInstance();

   g_slides[ 4 ] = std::make_unique<Bitmap>(IDB_IMAGE4, inst);
   g_slides[ 5 ] = std::make_unique<Bitmap>(IDB_IMAGE5, inst);
   g_slides[ 6 ] = std::make_unique<Bitmap>(IDB_IMAGE6, inst);

   // set the index to the second slide
   g_currentSlide = 1;
}

void GameEnd( )
{ }

void GameActivate(HWND hwnd)
{ }

void GameDeactivate(HWND hwnd)
{ }

void GamePaint(HDC hDC)
{
   g_slides[ g_currentSlide ]->Draw(hDC, 0, 0);
}

void GameCycle( )
{
   static UINT delay = 0;

   // establish a 3-second delay before moving to the next slide
   if ( ++delay > 3 )
   {
      // reset the delay counter
      delay = 0;

      // move to the next slide
      if ( ++g_currentSlide == g_NUMSLIDES )
      {
         g_currentSlide = 0;
      }

      // force a repaint to draw the next slide
      InvalidateRect(g_game->GetWindow(), NULL, FALSE);
   }
}

void GameMenu(WPARAM wParam)
{
   switch ( LOWORD(wParam) )
   {
   case IDM_GAME_EXIT:
      GameEnd();
      PostQuitMessage(0);
      return;

   case IDM_HELP_ABOUT:
      DialogBoxW(g_game->GetInstance(), MAKEINTRESOURCEW(IDD_ABOUT), g_game->GetWindow(), (DLGPROC) DlgProc);
      return;
   }
}