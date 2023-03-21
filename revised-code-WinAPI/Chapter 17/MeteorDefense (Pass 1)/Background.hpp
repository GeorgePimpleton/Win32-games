//-----------------------------------------------------------------
// Background Object
// C++ Header - Background.hpp
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include <windows.h>
#include "random_toolkit.hpp"
#include "Bitmap.hpp"

//-----------------------------------------------------------------
// Background class
//-----------------------------------------------------------------
class Background
{
protected:
   // member variables
   int      m_iWidth;
   int      m_iHeight;
   COLORREF m_crColor;
   Bitmap*  m_pBitmap;

public:
   // constructor(s)/destructor
            Background(int iWidth, int iHeight, COLORREF crColor);
            Background(Bitmap* pBitmap);
   virtual ~Background( );

   // general methods
   virtual void  Update( );
   virtual void  Draw(HDC hDC);

   // accessor methods
   int GetWidth( )  { return m_iWidth; };
   int GetHeight( ) { return m_iHeight; };
};

//-----------------------------------------------------------------
// Starry Background class
//-----------------------------------------------------------------
class StarryBackground : Background
{
protected:
   // member variables
   int      m_iNumStars;
   int      m_iTwinkleDelay;
   POINT    m_ptStars[ 100 ];
   COLORREF m_crStarColors[ 100 ];

public:
   // constructor(s)/destructor
   StarryBackground(int iWidth, int iHeight, int iNumStars = 100, int iTwinkleDelay = 50);
   virtual ~StarryBackground( );

   // general methods
   virtual void Update( );
   virtual void Draw(HDC hDC);
};
