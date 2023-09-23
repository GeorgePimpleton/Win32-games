#pragma once

#include <windows.h>
#include "Bitmap.h"

typedef WORD        SPRITEACTION;
const SPRITEACTION  SA_NONE   = 0x0000L,
                    SA_KILL   = 0x0001L;

typedef WORD        BOUNDSACTION;
const BOUNDSACTION  BA_STOP   = 0,
                    BA_WRAP   = 1,
                    BA_BOUNCE = 2,
                    BA_DIE    = 3;

class Sprite
{
protected:
   Bitmap*      m_pBitmap;
   RECT         m_rcPosition, m_rcCollision;
   POINT        m_ptVelocity;
   int          m_iZOrder;
   RECT         m_rcBounds;
   BOUNDSACTION m_baBoundsAction;
   BOOL         m_bHidden;

   virtual void  CalcCollisionRect( );

public:
            Sprite(Bitmap* pBitmap);
            Sprite(Bitmap* pBitmap, RECT& rcBounds,
                   BOUNDSACTION baBoundsAction = BA_STOP);
            Sprite(Bitmap* pBitmap, POINT ptPosition, POINT ptVelocity, int iZOrder,
                   RECT& rcBounds, BOUNDSACTION baBoundsAction = BA_STOP);
   virtual ~Sprite( );

   virtual SPRITEACTION  Update( );
   void                  Draw(HDC hDC);
   BOOL                  IsPointInside(int x, int y);
   BOOL                  TestCollision(Sprite* pTestSprite);

   RECT& GetPosition( )                   { return m_rcPosition; };
   void  SetPosition(int x, int y);
   void  SetPosition(POINT ptPosition);
   void  SetPosition(RECT& rcPosition)    { CopyRect(&m_rcPosition, &rcPosition); };
   void  OffsetPosition(int x, int y);
   RECT& GetCollision( )                  { return m_rcCollision; };
   POINT GetVelocity( )                   { return m_ptVelocity; };
   void  SetVelocity(int x, int y);
   void  SetVelocity(POINT ptVelocity);
   BOOL  GetZOrder( )                     { return m_iZOrder; };
   void  SetZOrder(int iZOrder)           { m_iZOrder = iZOrder; };
   void  SetBounds(RECT& rcBounds)        { CopyRect(&m_rcBounds, &rcBounds); };
   void  SetBoundsAction(BOUNDSACTION ba) { m_baBoundsAction = ba; };
   BOOL  IsHidden( )                      { return m_bHidden; };
   void  SetHidden(BOOL bHidden)          { m_bHidden = bHidden; };
   int   GetWidth( )                      { return m_pBitmap-> GetWidth( ); };
   int   GetHeight( )                     { return m_pBitmap-> GetHeight( ); };
};

inline void Sprite::CalcCollisionRect( )
{
   int iXShrink = (m_rcPosition.left - m_rcPosition.right) / 12;
   int iYShrink = (m_rcPosition.top - m_rcPosition.bottom) / 12;
   CopyRect(&m_rcCollision, &m_rcPosition);
   InflateRect(&m_rcCollision, iXShrink, iYShrink);
}

inline BOOL Sprite::TestCollision(Sprite* pTestSprite)
{
   RECT& rcTest = pTestSprite->GetCollision( );
   return m_rcCollision.left <= rcTest.right &&
      rcTest.left <= m_rcCollision.right &&
      m_rcCollision.top <= rcTest.bottom &&
      rcTest.top <= m_rcCollision.bottom;
}

inline BOOL Sprite::IsPointInside(int x, int y)
{
   POINT ptPoint;
   ptPoint.x = x;
   ptPoint.y = y;
   return PtInRect(&m_rcPosition, ptPoint);
}

inline void Sprite::SetPosition(int x, int y)
{
   OffsetRect(&m_rcPosition, x - m_rcPosition.left, y - m_rcPosition.top);
   CalcCollisionRect( );
}

inline void Sprite::SetPosition(POINT ptPosition)
{
   OffsetRect(&m_rcPosition, ptPosition.x - m_rcPosition.left,
              ptPosition.y - m_rcPosition.top);
   CalcCollisionRect( );
}

inline void Sprite::OffsetPosition(int x, int y)
{
   OffsetRect(&m_rcPosition, x, y);
   CalcCollisionRect( );
}

inline void Sprite::SetVelocity(int x, int y)
{
   m_ptVelocity.x = x;
   m_ptVelocity.y = y;
}

inline void Sprite::SetVelocity(POINT ptVelocity)
{
   m_ptVelocity.x = ptVelocity.x;
   m_ptVelocity.y = ptVelocity.y;
}
