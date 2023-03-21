//-----------------------------------------------------------------
// Background Object
// C++ Source - Background.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// include files
//-----------------------------------------------------------------
#include "Background.hpp"

//-----------------------------------------------------------------
// Background constructor(s)/destructor
//-----------------------------------------------------------------
Background::Background(int iWidth, int iHeight, COLORREF crColor)
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
   m_crColor = 0;
   m_pBitmap = pBitmap;
   m_iWidth  = pBitmap->GetWidth( );
   m_iHeight = pBitmap->GetHeight( );
}

Background::~Background( )
{ }

//-----------------------------------------------------------------
// Background general methods
//-----------------------------------------------------------------
void Background::Update( )
{
   // do nothing since the basic background is not animated
}

void Background::Draw(HDC hDC)
{
   // draw the background
   if ( m_pBitmap != NULL )
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

//-----------------------------------------------------------------
// StarryBackground constructor
//-----------------------------------------------------------------
StarryBackground::StarryBackground(int iWidth, int iHeight, int iNumStars, int iTwinkleDelay)
   : Background(iWidth, iHeight, 0)
{
   // initialize the member variables
   m_iNumStars     = min(iNumStars, 100);
   m_iTwinkleDelay = iTwinkleDelay;

   // create the stars
   for ( int i = 0; i < iNumStars; i++ )
   {
      m_ptStars[ i ].x    = rtk::rand(0, iWidth);
      m_ptStars[ i ].y    = rtk::rand(0, iHeight);
      m_crStarColors[ i ] = RGB(128, 128, 128);
   }
}

StarryBackground::~StarryBackground( )
{ }

//-----------------------------------------------------------------
// StarryBackground general methods
//-----------------------------------------------------------------
void StarryBackground::Update( )
{
   // randomly change the shade of the stars so that they twinkle
   for ( int i { }; i < m_iNumStars; i++ )
   {
      if ( (rand( ) % m_iTwinkleDelay) == 0 )
      {
         int iRGB            = rtk::rand(0, 255);
         m_crStarColors[ i ] = RGB(iRGB, iRGB, iRGB);
      }
   }
}

void StarryBackground::Draw(HDC hDC)
{
   // draw the solid black background
   RECT   rect   = { 0, 0, m_iWidth, m_iHeight };
   HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));

   FillRect(hDC, &rect, hBrush);
   DeleteObject(hBrush);

   // draw the stars
   for ( int i = 0; i < m_iNumStars; i++ )
   {
      SetPixel(hDC, m_ptStars[ i ].x, m_ptStars[ i ].y, m_crStarColors[ i ]);
   }
}
