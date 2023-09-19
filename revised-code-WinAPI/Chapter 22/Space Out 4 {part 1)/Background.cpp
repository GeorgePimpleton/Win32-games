#include "Background.h"

Background::Background(int iWidth, int iHeight, COLORREF crColor)
{
   m_pBitmap = NULL;
   m_crColor = crColor;
   m_iWidth = iWidth;
   m_iHeight = iHeight;
}

Background::Background(Bitmap* pBitmap)
{
   m_pBitmap = pBitmap;
   m_crColor = 0;
   m_iWidth = pBitmap->GetWidth( );
   m_iHeight = pBitmap->GetHeight( );
}

Background::~Background( )
{ }

void Background::Update( )
{
   // do nothing since the basic background is not animated
}

void Background::Draw(HDC hDC)
{
   if ( m_pBitmap != NULL )
   {
      m_pBitmap->Draw(hDC, 0, 0);
   }
   else
   {
      HBRUSH  hBrush = CreateSolidBrush(m_crColor);
      RECT    rect = { 0, 0, m_iWidth, m_iHeight };

      FillRect(hDC, &rect, hBrush);
      DeleteObject(hBrush);
   }
}

StarryBackground::StarryBackground(int iWidth, int iHeight, int iNumStars,
                                   int iTwinkleDelay) : Background(iWidth, iHeight, 0)
{
   m_iNumStars = min(iNumStars, 100);
   m_iTwinkleDelay = iTwinkleDelay;

   for ( int i = 0; i < iNumStars; i++ )
   {
      m_ptStars[ i ].x = rand( ) % iWidth;
      m_ptStars[ i ].y = rand( ) % iHeight;
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

void StarryBackground::Draw(HDC hDC)
{
   HBRUSH  hBrush = CreateSolidBrush(RGB(0, 0, 0));
   RECT    rect = { 0, 0, m_iWidth, m_iHeight };

   FillRect(hDC, &rect, hBrush);
   DeleteObject(hBrush);

   for ( int i = 0; i < m_iNumStars; i++ )
   {
      SetPixel(hDC, m_ptStars[ i ].x, m_ptStars[ i ].y, m_crStarColors[ i ]);
   }
}