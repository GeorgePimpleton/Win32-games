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
   Bitmap* m_bitmap;
   COLORREF m_color;
   LONG     m_width;
   LONG     m_height;

public:
            Background(LONG width, LONG height, COLORREF color);
            Background(Bitmap* bitmap);
   virtual ~Background();

public:
   virtual void Draw(HDC dc);
   virtual void Update( );

public:
   LONG GetWidth( ) const;
   LONG GetHeight( ) const;
};

class StarryBackground : public Background
{
public:
            StarryBackground(LONG width, LONG height,
                             LONG numStars = 100,
                             LONG twinkleDelay = 50);
   virtual ~StarryBackground( );

public:
   virtual void Draw(HDC dc);
   virtual void Update( );

protected:
   LONG                      m_numStars;
   LONG                      m_twinkleDelay;
   std::array<POINT, 100>    m_stars;
   std::array<COLORREF, 100> m_starColors;
   std::array<STARSIZE, 100> m_sizeStars;
};

inline LONG Background::GetWidth( ) const  { return m_width; }
inline LONG Background::GetHeight( ) const { return m_height; }
