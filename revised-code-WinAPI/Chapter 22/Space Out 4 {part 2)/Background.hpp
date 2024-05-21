#pragma once

#include <windows.h>
#include "random_toolkit_v131.hpp"
#include "Bitmap.hpp"
#include <array>

using STARSIZE = WORD;
const STARSIZE SS_SMALL  = 0;
const STARSIZE SS_MEDIUM = 1;
const STARSIZE SS_LARGE  = 2;

class Background
{
protected:
   Bitmap*  m_bitmap;
   COLORREF m_color;
   int      m_width;
   int      m_height;

public:
            Background( int, int, COLORREF );
            Background( Bitmap* );
   virtual ~Background( );

   virtual void Update( );
   virtual void Draw( HDC ) const;

   int GetWidth( )  const { return m_width; };
   int GetHeight( ) const { return m_height; };
};

class StarryBackground : Background
{
protected:
   LONG                      m_numStars;
   LONG                      m_twinkleDelay;
   std::array<POINT, 100>    m_stars;
   std::array<COLORREF, 100> m_starColors;
   std::array<STARSIZE, 100> m_sizeStars;

public:
            StarryBackground( int, int,
                              int = 100, int = 50 );
   virtual ~StarryBackground( );

   virtual void Update( );
   virtual void Draw( HDC ) const;
};
