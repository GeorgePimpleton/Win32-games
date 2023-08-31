#include <windows.h>
#include "Bitmap.hpp"
#include "Background.hpp"
#include "random_toolkit.hpp"

Background::Background(int width, int height, COLORREF color)
{
   m_width  = width;
   m_height = height;
   m_color  = color;
   m_bitmap = nullptr;
}

Background::Background(Bitmap* bitmap)
{
   m_width  = bitmap-> GetWidth( );
   m_height = bitmap-> GetHeight( );
   m_color  = 0;
   m_bitmap = bitmap;
}

Background::~Background( )
{ }

void Background::Draw(HDC dc)
{
   if ( m_bitmap != nullptr )
   {
      m_bitmap->Draw(dc, 0, 0);
   }
   else
   {
      RECT   rect = { 0, 0, m_width, m_height };
      HBRUSH brush = CreateSolidBrush(m_color);

      FillRect(dc, &rect, brush);
      DeleteObject(brush);
   }
}

void Background::Update( )
{
   // do nothing since the basic background is not animated
}

StarryBackground::StarryBackground(int width, int height, int numStars, int twinkleDelay)
   : Background(width, height, 0)
{
   m_numStars     = min(numStars, 100);
   m_twinkleDelay = twinkleDelay;

   for ( int i = 0; i < m_numStars; i++ )
   {
      m_stars[ i ].x = rtk::rand(0, width);
      m_stars[ i ].y = rtk::rand(0, height);
      m_starColors[ i ] = RGB(128, 128, 128);
      m_sizeStars[ i ] = SS_SMALL;

      // is the star medium sized?
      if ( rtk::rand(0, 100) < 20 )
      {
         m_sizeStars[ i ] = SS_MEDIUM;
      }
      // is the star large sized?
      else if ( rtk::rand(0, 100) < 5 )
      {
         m_sizeStars[ i ] = SS_LARGE;
      }
   }
}

StarryBackground::~StarryBackground( )
{ }

void StarryBackground::Draw(HDC hDC)
{
   // draw the solid black background
   RECT   rect = { 0, 0, m_width, m_height };
   HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
   FillRect(hDC, &rect, brush);
   DeleteObject(brush);

   // draw the stars
   for ( int i = 0; i < m_numStars; i++ )
   {
      SetPixel(hDC, m_stars[ i ].x, m_stars[ i ].y, m_starColors[ i ]);

      int x;
      int y;

      // draw extra pixels if the star is medium
      if ( m_sizeStars[ i ] == SS_MEDIUM )
      {
         for ( x = -1; x < 2; x++ )
         {
            for ( y = -1; y < 2; y++ )
            {
               SetPixel(hDC, m_stars[ i ].x + x, m_stars[ i ].y + y,
                        m_starColors[ i ]);
            }
         }
      }
      // draw even more pixels if the star is large
      else if ( m_sizeStars[ i ] == SS_LARGE )
      {
         for ( x = -2; x < 3; x++ )
         {
            for ( y = -2; y < 3; y++ )
            {
               // don't draw the corners
               if ( (abs(x) == 2) && (abs(y) == 2) )
               {
                  break;
               }
               SetPixel(hDC, m_stars[ i ].x + x, m_stars[ i ].y + y, m_starColors[ i ]);
            }
         }
      }
   }
}

void StarryBackground::Update( )
{
   // randomly change the shade of the stars so that they twinkle
   for ( int i = 0; i < m_numStars; i++ )
   {
      if ( (rtk::rand(0, m_twinkleDelay) == 0) )
      {
         int iRGB = rtk::rand(0, 255);
         m_starColors[ i ] = RGB(iRGB, iRGB, iRGB);

         // Adjust the size of some random stars, small is the default size
         m_sizeStars[ i ] = SS_SMALL;

         // is the star medium sized?
         if ( rtk::rand(0, 100) < 20 )
         {
            m_sizeStars[ i ] = SS_MEDIUM;
         }
         // is the star large sized?
         else if ( rtk::rand(0, 100) < 5 )
         {
            m_sizeStars[ i ] = SS_LARGE;
         }
      }
   }
}