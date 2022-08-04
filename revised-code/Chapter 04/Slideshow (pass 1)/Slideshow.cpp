// "Talk to me like I'm a 3 year old!" Programming Lessons - Windows Games

// Slideshow.cpp - game source

#include "Slideshow.hpp"

HRESULT GameInitialize(HINSTANCE hInstance)
{
   g_pGame = std::make_unique<GameEngine>(hInstance, L"Slideshow", L"Drawing Graphical Images",
                                          IDI_ICON, IDI_ICON_SM);

   if ( g_pGame == nullptr )
   {
      return E_FAIL;
   }

   g_pGame->SetFrameRate(1);

   return S_OK;
}

void GameStart(HWND hWindow)
{
   // create/load the slide bitmaps
   HINSTANCE hInstance = g_pGame->GetInstance();

   // create a blank bitmap
   g_pSlides[ 0 ] = std::make_unique<Bitmap>(g_pGame->GetWindow(), 640, 480, RGB(128, 128, 64));

   // load bitmaps from a file
   g_pSlides[ 1 ] = std::make_unique<Bitmap>(L"Res/Image1.bmp");
   g_pSlides[ 2 ] = std::make_unique<Bitmap>(L"Res/Image2.bmp");
   g_pSlides[ 3 ] = std::make_unique<Bitmap>(L"Res/Image3.bmp");

   // load bitmaps as resources
   g_pSlides[ 4 ] = std::make_unique<Bitmap>(IDB_IMAGE4, hInstance);
   g_pSlides[ 5 ] = std::make_unique<Bitmap>(IDB_IMAGE5, hInstance);
   g_pSlides[ 6 ] = std::make_unique<Bitmap>(IDB_IMAGE6, hInstance);

   // set the index to the second slide
   g_iCurSlide = 1;
}

void GameEnd()
{ }

void GameActivate(HWND hWindow)
{ }

void GameDeactivate(HWND hWindow)
{ }

void GamePaint(HDC hDC)
{
   // draw the current slide bitmap
   g_pSlides[ g_iCurSlide ]->Draw(hDC, 0, 0);
}

void GameCycle()
{
   static UINT iDelay = 0;

   // establish a 3-second delay before moving to the next slide
   if ( ++iDelay > 3 )
   {
      // reset the delay counter
      iDelay = 0;

      // move to the next slide
      if ( ++g_iCurSlide == g_iNUMSLIDES )
      {
         g_iCurSlide = 0;
      }

      // force a repaint to draw the next slide
      InvalidateRect(g_pGame->GetWindow(), NULL, FALSE);
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
      DialogBoxW(g_pGame->GetInstance(), MAKEINTRESOURCEW(IDD_ABOUT), g_pGame->GetWindow(), (DLGPROC) DlgProc);
      return;
   }
}