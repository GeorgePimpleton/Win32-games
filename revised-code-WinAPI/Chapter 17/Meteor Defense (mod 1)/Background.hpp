#pragma once

typedef WORD   STARSIZE;
const STARSIZE SS_SMALL  = 0;
const STARSIZE SS_MEDIUM = 1;
const STARSIZE SS_LARGE  = 2;

class Background
{
public:
            Background(int width, int height, COLORREF color);
            Background(Bitmap* bitmap);
   virtual ~Background( );

public:
   virtual void Draw(HDC dc);
   virtual void Update( );

public:
   int GetWidth( ) const;
   int GetHeight( ) const;

protected:
   int      m_width;
   int      m_height;
   COLORREF m_color;
   Bitmap*  m_bitmap;
};

class StarryBackground : public Background
{
public:
            StarryBackground(int width, int height,
                             int numStars = 100, int twinkleDelay = 50);
   virtual ~StarryBackground( );

public:
   virtual void Draw(HDC dc);
   virtual void Update( );

protected:
   int      m_numStars;
   int      m_twinkleDelay;
   POINT    m_stars[ 100 ];
   COLORREF m_starColors[ 100 ];
   STARSIZE m_sizeStars[ 100 ];
};

inline int Background::GetWidth( ) const
{
   return m_width;
}

inline int Background::GetHeight( ) const
{
   return m_height;
}
