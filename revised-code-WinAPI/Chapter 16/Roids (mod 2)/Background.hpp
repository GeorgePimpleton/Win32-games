#pragma once

#include <windows.h>
#include "random_toolkit.hpp"
#include "Bitmap.hpp"

using STARSIZE           = WORD;
const STARSIZE SS_SMALL  = 0;
const STARSIZE SS_MEDIUM = 1;
const STARSIZE SS_LARGE  = 2;

class Background
{
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

protected:
   Bitmap*  m_bitmap;
   COLORREF m_color;
   LONG     m_width;
   LONG     m_height;
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
   LONG     m_numStars;
   LONG     m_twinkleDelay;
   POINT    m_stars[ 100 ];
   COLORREF m_starColors[ 100 ];
   STARSIZE m_sizeStars[ 100 ];
};

inline LONG Background::GetWidth( ) const  { return m_width; }

inline LONG Background::GetHeight( ) const { return m_height; }
