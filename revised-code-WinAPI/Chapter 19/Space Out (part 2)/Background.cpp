#include "Background.hpp"

Background::Background(int width, int height, COLORREF color)
{
   m_width = width;
   m_height = height;
   m_color = color;
   m_pBitmap = NULL;
}

Background::Background(Bitmap* pBitmap)
{
   m_color = 0;
   m_pBitmap = pBitmap;
   m_width = pBitmap->GetWidth( );
   m_height = pBitmap->GetHeight( );
}

Background::~Background( )
{ }

void Background::Update( )
{
   // do nothing since the basic background is not animated
}

void Background::Draw(HDC dc)
{
   if ( m_pBitmap != NULL )
   {
      m_pBitmap->Draw(dc, 0, 0);
   }
   else
   {
      RECT    rect = { 0, 0, m_width, m_height };
      HBRUSH  hBrush = CreateSolidBrush(m_color);
      FillRect(dc, &rect, hBrush);
      DeleteObject(hBrush);
   }
}

StarryBackground::StarryBackground(int width, int height, int iNumStars,
                                   int iTwinkleDelay) : Background(width, height, 0)
{
   m_iNumStars = min(iNumStars, 100);
   m_iTwinkleDelay = iTwinkleDelay;

   for ( int i = 0; i < iNumStars; i++ )
   {
      m_ptStars[ i ].x = rand( ) % width;
      m_ptStars[ i ].y = rand( ) % height;
      m_crStarColors[ i ] = RGB(128, 128, 128);
   }
}

StarryBackground::~StarryBackground( )
{ }

void StarryBackground::Update( )
{
   int iRGB;

   for ( int i = 0; i < m_iNumStars; i++ )
   {
      if ( (rand( ) % m_iTwinkleDelay) == 0 )
      {
         iRGB = rand( ) % 256;
         m_crStarColors[ i ] = RGB(iRGB, iRGB, iRGB);
      }
   }
}

void StarryBackground::Draw(HDC dc)
{
   RECT    rect = { 0, 0, m_width, m_height };
   HBRUSH  hBrush = CreateSolidBrush(RGB(0, 0, 0));
   FillRect(dc, &rect, hBrush);
   DeleteObject(hBrush);

   for ( int i = 0; i < m_iNumStars; i++ )
   {
      SetPixel(dc, m_ptStars[ i ].x, m_ptStars[ i ].y, m_crStarColors[ i ]);
   }
}
