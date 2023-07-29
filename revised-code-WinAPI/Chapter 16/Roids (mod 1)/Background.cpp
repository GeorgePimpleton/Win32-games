// "Talk to me like I'm a 3 year old!" Programming Lessons - Windows Games

// Background.cpp - Background Class Source

#include "Background.hpp"

Background::Background(LONG iWidth, LONG iHeight, COLORREF crColor)
{
   // initialize the member variables
   m_iWidth  = iWidth;
   m_iHeight = iHeight;
   m_crColor = crColor;
   m_pBitmap = NULL;
}

Background::Background(Bitmap* pBitmap)
{
   // initialize the member variables
   m_iWidth  = pBitmap->GetWidth();
   m_iHeight = pBitmap->GetHeight();
   m_crColor = 0;
   m_pBitmap = pBitmap;
}

Background::~Background()
{
}

void Background::Draw(HDC hDC)
{
   // draw the background
   if (m_pBitmap != NULL)
   {
      m_pBitmap->Draw(hDC, 0, 0);
   }
   else
   {
      RECT   rect   = { 0, 0, m_iWidth, m_iHeight };
      HBRUSH hBrush = CreateSolidBrush(m_crColor);

      FillRect(hDC, &rect, hBrush);
      DeleteObject(hBrush);
   }
}

void Background::Update()
{
  // do nothing since the basic background is not animated
}

StarryBackground::StarryBackground(LONG iWidth, LONG iHeight, LONG iNumStars, LONG iTwinkleDelay) :
   Background(iWidth, iHeight, 0)
{
   // initialize the member variables
   m_iNumStars     = min(iNumStars, 100);
   m_iTwinkleDelay = iTwinkleDelay;

   // Create the stars
   for (LONG i = 0; i < m_iNumStars; i++)
   {
      m_ptStars[i].x    = rtk::rand(0, iWidth);
      m_ptStars[i].y    = rtk::rand(0, iHeight);
      m_crStarColors[i] = RGB(128, 128, 128);

      // adjust the size of some random stars, small is the default size
      m_ssSizeStars[i] = SS_SMALL;

      // is the star medium sized?
      if (rtk::rand(0, 100) < 20)
      {
         m_ssSizeStars[i] = SS_MEDIUM;
      }
      // is the star large sized?
      else if (rtk::rand(0, 100) < 5)
      {
         m_ssSizeStars[i] = SS_LARGE;
      }
   }
}

StarryBackground::~StarryBackground()
{
}

void StarryBackground::Draw(HDC hDC)
{
   // draw the solid black background
   RECT   rect   = { 0, 0, m_iWidth, m_iHeight };
   HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));

   FillRect(hDC, &rect, hBrush);
   DeleteObject(hBrush);

   // draw the stars
   for (LONG i = 0; i < m_iNumStars; i++)
   {
      SetPixel(hDC, m_ptStars[i].x, m_ptStars[i].y, m_crStarColors[i]);

      // draw extra pixels if the star is medium
      if (m_ssSizeStars[i] == SS_MEDIUM)
      {
         for (int x = -1; x < 2; x++)
         {
            for (int y = -1; y < 2; y++)
            {
               SetPixel(hDC, m_ptStars[i].x + x, m_ptStars[i].y + y, m_crStarColors[i]);
            }
         }
      }
      // draw even more pixels if the star is large
      else if (m_ssSizeStars[i] == SS_LARGE)
      {
         for (int x = -2; x < 3; x++)
         {
            for (int y = -2; y < 3; y++)
            {
               // don't draw the corners
               if ((abs(x) == 2) && (abs(y) == 2))
               {
                  break;
               }

               SetPixel(hDC, m_ptStars[i].x + x, m_ptStars[i].y + y, m_crStarColors[i]);
            }
         }
      }
   }
}

void StarryBackground::Update()
{
   // randomly change the shade of the stars so that they twinkle
   for (LONG i = 0; i < m_iNumStars; i++)
   {
      if ((rtk::rand(0, m_iTwinkleDelay)) == 0)
      {
         LONG iRGB         = rtk::rand(0, 255);
         m_crStarColors[i] = RGB(iRGB, iRGB, iRGB);

         // adjust the size of some random stars, small is the default size
         m_ssSizeStars[i] = SS_SMALL;

         // is the star medium sized?
         if (rtk::rand(0, 100) < 20)
         {
            m_ssSizeStars[i] = SS_MEDIUM;
         }
         // is the star large sized?
         else if (rtk::rand(0, 100) < 5)
         {
            m_ssSizeStars[i] = SS_LARGE;
         }
      }
   }
}