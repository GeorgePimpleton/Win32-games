#pragma once

#include <windows.h>
#include "random_toolkit.hpp"
#include "Bitmap.hpp"

//using STARSIZE           = WORD;
//const STARSIZE SS_SMALL  = 0;
//const STARSIZE SS_MEDIUM = 1;
//const STARSIZE SS_LARGE  = 2;

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
   virtual void Draw(HDC hDC);

   int GetWidth( ) const  { return m_width; };
   int GetHeight( ) const { return m_height; };
};

class StarryBackground : Background
{
protected:
   int      m_numStars;
   int      m_twinkleDelay;
   POINT    m_stars[ 100 ];
   COLORREF m_starColors[ 100 ];
   STARSIZE m_sizeStars[ 100 ];

public:
            StarryBackground(int width, int height,
                             int numStars = 100,
                             int twinkleDelay = 50);
   virtual ~StarryBackground( );

   virtual void Update( );
   virtual void Draw(HDC hDC);
};
