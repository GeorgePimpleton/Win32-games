// "Talk to me like I'm a 3 year old!" Programming Lessons - Windows Games

// Background.hpp - Background Class Header

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
            Background(LONG iWidth, LONG iHeight, COLORREF crColor);
            Background(Bitmap* pBitmap);
   virtual ~Background();

public:
   virtual void Draw(HDC hDC);
   virtual void Update();

public:
   LONG GetWidth() const;
   LONG GetHeight() const;

protected:
   LONG     m_iWidth;
   LONG     m_iHeight;
   COLORREF m_crColor;
   Bitmap*  m_pBitmap;
};

class StarryBackground : public Background
{
public:
            StarryBackground(LONG iWidth, LONG iHeight,
                             LONG iNumStars     = 100,
                             LONG iTwinkleDelay = 50);
   virtual ~StarryBackground();

public:
   virtual void Draw(HDC hDC);
   virtual void Update();

protected:
   LONG     m_iNumStars;
   LONG     m_iTwinkleDelay;
   POINT    m_ptStars[100];
   COLORREF m_crStarColors[100];
   STARSIZE m_ssSizeStars[100];
};

inline LONG Background::GetWidth() const
{
   return m_iWidth;
}

inline LONG Background::GetHeight() const
{
   return m_iHeight;
}
