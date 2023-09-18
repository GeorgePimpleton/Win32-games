#pragma once

#include <windows.h>
#include "Bitmap.hpp"

class Background
{
protected:
   int      m_width;
   int      m_height;
   COLORREF m_color;
   Bitmap*  m_pBitmap;

public:
            Background(int width, int height, COLORREF color);
            Background(Bitmap* pBitmap);
   virtual ~Background( );

   virtual void Update( );
   virtual void Draw(HDC dc);

   int GetWidth( )  { return m_width; };
   int GetHeight( ) { return m_height; };
};

class StarryBackground : Background
{
protected:
   int      m_iNumStars;
   int      m_iTwinkleDelay;
   POINT    m_ptStars[ 100 ];
   COLORREF m_crStarColors[ 100 ];

public:
            StarryBackground(int width, int height, int iNumStars = 100,
                             int iTwinkleDelay = 50);
   virtual ~StarryBackground( );

   virtual void Update( );
   virtual void Draw(HDC dc);
};
