#pragma once

#include <windows.h>
#include "random_toolkit.hpp"
#include "Bitmap.hpp"
#include <array>

enum class STARSIZE : WORD { SS_SMALL  = 0,
                             SS_MEDIUM = 1,
                             SS_LARGE  = 2 };

using enum STARSIZE;

class Background
{
protected:
   Bitmap*  m_bitmap;
   COLORREF m_color;
   LONG     m_width;
   LONG     m_height;

public:
            Background(LONG width, LONG height, COLORREF color);
            Background(Bitmap* bitmap);
   virtual ~Background( );

   virtual void Update( );
   virtual void Draw(HDC dc);

   constexpr LONG GetWidth( ) const  { return m_width; };
   constexpr LONG GetHeight( ) const { return m_height; };
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
            StarryBackground(LONG width, LONG height,
                             LONG numStars     = 100,
                             LONG twinkleDelay = 50);
   virtual ~StarryBackground( );

   virtual void Update( );
   virtual void Draw(HDC dc);
};
