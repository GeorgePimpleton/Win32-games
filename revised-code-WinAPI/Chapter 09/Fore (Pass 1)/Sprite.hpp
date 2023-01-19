// "Talk to me like I'm a 3 year old!" Programming Lessons - Windows Games

// Sprite.hpp - Sprite Header

#pragma once

#include <windows.h>
#include <memory>
#include "Bitmap.hpp"
#include "random_toolkit.hpp"

// CUSTOM DATA TYPES ===========================================================
//using BOUNDSACTION            = WORD;
//const BOUNDSACTION  BA_STOP   { 0 };
//const BOUNDSACTION  BA_WRAP   { 1 };
//const BOUNDSACTION  BA_BOUNCE { 2 };
//const BOUNDSACTION  BA_DIE    { 3 };
// (look into converting this to an enum) (done!)

enum class BOUNDSACTION : WORD { BA_STOP   = 0,
                                 BA_WRAP   = 1,
                                 BA_BOUNCE = 2,
                                 BA_DIE    = 3 };

// C++20, so you don't have to specify the class name again and again and again.
using enum BOUNDSACTION;

// Sprite CLASS ================================================================
class Sprite
{
public:
            Sprite(Bitmap* pBitmap);
            Sprite(Bitmap* pBitmap, RECT& rcBounds, BOUNDSACTION baBoundsAction = BA_STOP);
            Sprite(Bitmap* pBitmap, POINT ptPosition, POINT ptVelocity, int iZOrder, RECT& rcBounds,
                   BOUNDSACTION baBoundsAction = BA_STOP);
   virtual ~Sprite();

public:
   virtual void Update();
   void         Draw(HDC hDC);
   BOOL         IsPointInside(LONG x, LONG y);

public:
   RECT& GetPosition();
   void  SetPosition(LONG x, LONG y);
   void  SetPosition(POINT ptPosition);
   void  SetPosition(RECT& rcPosition);
   void  OffsetPosition(LONG x, LONG y);
   POINT GetVelocity();
   void  SetVelocity(LONG x, LONG y);
   void  SetVelocity(POINT ptVelocity);
   BOOL  GetZOrder();
   void  SetZOrder(int iZOrder);
   void  SetBounds(RECT & rcBounds);
   void  SetBoundsAction(BOUNDSACTION ba);
   BOOL  IsHidden();
   void  SetHidden(BOOL bHidden);
   LONG  GetWidth();
   LONG  GetHeight();

protected:
   std::unique_ptr<Bitmap> m_pBitmap;
   RECT                    m_rcPosition;
   POINT                   m_ptVelocity;
   int                     m_iZOrder;
   RECT                    m_rcBounds;
   BOUNDSACTION            m_baBoundsAction;
   BOOL                    m_bHidden;
};

inline BOOL Sprite::IsPointInside(LONG x, LONG y)
{
   POINT ptPoint;

   ptPoint.x = x;
   ptPoint.y = y;

   return PtInRect(&m_rcPosition, ptPoint);
}

inline RECT& Sprite::GetPosition()
{
   return m_rcPosition;
}

inline void Sprite::SetPosition(LONG x, LONG y)
{
   OffsetRect(&m_rcPosition, x - m_rcPosition.left, y - m_rcPosition.top);
}

inline void Sprite::SetPosition(POINT ptPosition)
{
   OffsetRect(&m_rcPosition, ptPosition.x - m_rcPosition.left,
                             ptPosition.y - m_rcPosition.top);
}

inline void Sprite::SetPosition(RECT & rcPosition)
{
   CopyRect(&m_rcPosition, &rcPosition);
}

inline void Sprite::OffsetPosition(LONG x, LONG y)
{
   OffsetRect(&m_rcPosition, x, y);
}

inline POINT Sprite::GetVelocity()
{
   return m_ptVelocity;
}

inline void Sprite::SetVelocity(LONG x, LONG y)
{
   m_ptVelocity.x = x;
   m_ptVelocity.y = y;
}

inline void Sprite::SetVelocity(POINT ptVelocity)
{
   m_ptVelocity.x = ptVelocity.x;
   m_ptVelocity.y = ptVelocity.y;
}

inline BOOL Sprite::GetZOrder()
{
   return m_iZOrder;
}

inline void Sprite::SetZOrder(int iZOrder)
{
   m_iZOrder = iZOrder;
}

inline void Sprite::SetBounds(RECT & rcBounds)
{
   CopyRect(&m_rcBounds, &rcBounds);
}

inline void Sprite::SetBoundsAction(BOUNDSACTION ba)
{
   m_baBoundsAction = ba;
}

inline BOOL Sprite::IsHidden()
{
   return m_bHidden;
}

inline void Sprite::SetHidden(BOOL bHidden)
{
   m_bHidden = bHidden;
}

inline LONG Sprite::GetWidth()
{
   return m_pBitmap->GetWidth();
}

inline LONG Sprite::GetHeight()
{
   return m_pBitmap->GetHeight();
}
