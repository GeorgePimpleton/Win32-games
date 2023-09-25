#pragma once

#include <windows.h>
#include "random_toolkit.hpp"
#include "Bitmap.hpp"
#include <array>

enum class STARSIZE : WORD { SS_SMALL  = 0,
                             SS_MEDIUM = 1,
                             SS_LARGE  = 3 };

using enum STARSIZE;

class Background
{
protected:
   Bitmap*  m_bitmap;
   COLORREF m_color;
   int      m_width;
   int      m_height;

public:
            Background(int width, int height, COLORREF color);
            Background(Bitmap* bitmap);
   virtual ~Background( );

   virtual void Update( );
   virtual void Draw(HDC dc);

   int GetWidth( ) const  { return m_width; };
   int GetHeight( ) const { return m_height; };
};

class StarryBackground : Background
{
protected:
   int                       m_numStars;
   int                       m_twinkleDelay;
   std::array<POINT, 100>    m_stars;
   std::array<COLORREF, 100> m_starColors;
   std::array<STARSIZE, 100> m_sizeStars;

public:
            StarryBackground(int width, int height,
                             int numStars = 100,
                             int twinkleDelay = 50);
   virtual ~StarryBackground( );

   virtual void Update( );
   virtual void Draw(HDC dc);
};
