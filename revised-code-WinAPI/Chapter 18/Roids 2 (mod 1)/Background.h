#pragma once

#include <windows.h>
#include "Bitmap.h"

class Background
{
protected:
   Bitmap*  m_pBitmap;
   COLORREF m_crColor;
   int      m_iWidth;
   int      m_iHeight;

public:
            Background(int iWidth, int iHeight, COLORREF crColor);
            Background(Bitmap* pBitmap);
   virtual ~Background( );

   virtual void Update( );
   virtual void Draw(HDC hDC);

   int GetWidth( )  { return m_iWidth; };
   int GetHeight( ) { return m_iHeight; };
};

class StarryBackground : Background
{
protected:
   int       m_iNumStars;
   int       m_iTwinkleDelay;
   POINT     m_ptStars[ 100 ];
   COLORREF  m_crStarColors[ 100 ];

public:
            StarryBackground(int iWidth, int iHeight, int iNumStars = 100,
                             int iTwinkleDelay = 50);
   virtual ~StarryBackground( );

   virtual void Update( );
   virtual void Draw(HDC hDC);
};
