#pragma once

#include <windows.h>
#include "random_toolkit.hpp"
#include "Bitmap.hpp"

enum class BOUNDSACTION : WORD { BA_STOP = 0,
                                 BA_WRAP = 1,
                                 BA_BOUNCE = 2,
                                 BA_DIE = 3 };

using enum BOUNDSACTION;

using SPRITEACTION         = WORD;
const SPRITEACTION SA_NONE = 0x0000L;
const SPRITEACTION SA_KILL = 0x0001L;

class Sprite
{
public:
            Sprite(Bitmap* pBitmap);
            Sprite(Bitmap* pBitmap, RECT& rcBounds, BOUNDSACTION baBoundsAction = BA_STOP);
            Sprite(Bitmap* pBitmap, POINT ptPosition, POINT ptVelocity, int iZOrder,
                   RECT& rcBounds, BOUNDSACTION baBoundsAction = BA_STOP);
   virtual ~Sprite( );

   void         UpdateFrame( );
   virtual void CalcCollisionRect( );

   virtual SPRITEACTION Update( );
   void                 Draw(HDC dc);
   BOOL                 IsPointInside(LONG x, LONG y);
   BOOL                 TestCollision(Sprite* pTestSprite);

   Bitmap* GetBitmap( ) const;
   void    SetNumFrames(UINT numFrames);
   void    SetFrameDelay(UINT frameDelay);
   RECT&   GetPosition( );
   void    SetPosition(LONG x, LONG y);
   void    SetPosition(POINT position);
   void    SetPosition(RECT& position);
   void    OffsetPosition(LONG x, LONG y);
   RECT&   GetCollision( );
   POINT   GetVelocity( ) const;
   void    SetVelocity(LONG x, LONG y);
   void    SetVelocity(POINT velocity);
   BOOL    GetZOrder( ) const;
   void    SetZOrder(int zOrder);
   void    SetBounds(RECT& bounds);
   void    SetBoundsAction(BOUNDSACTION boundsAction);
   BOOL    IsHidden( ) const;
   void    SetHidden(BOOL hidden);
   LONG    GetWidth( ) const;
   LONG    GetHeight( ) const;

protected:
   Bitmap* m_bitmap;
   UINT         m_numFrames;
   UINT         m_curFrame;
   LONG         m_frameDelay;
   LONG         m_frameTrigger;
   RECT         m_position;
   RECT         m_collision;
   POINT        m_ptVelocity;
   int          m_zOrder;
   RECT         m_bounds;
   BOUNDSACTION m_boundsAction;
   BOOL         m_hidden;
};

inline void Sprite::UpdateFrame( )
{
   if ( (m_frameDelay >= 0) && (--m_frameTrigger <= 0) )
   {
      m_frameTrigger = m_frameDelay;

      if ( ++m_curFrame >= m_numFrames )
      {
         m_curFrame = 0;
      }
   }
}

inline void Sprite::CalcCollisionRect( )
{
   LONG iXShrink = (m_position.left - m_position.right) / 12;
   LONG iYShrink = (m_position.top - m_position.bottom) / 12;

   CopyRect(&m_collision, &m_position);
   InflateRect(&m_collision, iXShrink, iYShrink);
}

inline BOOL Sprite::TestCollision(Sprite* pTestSprite)
{
   RECT& rcTest = pTestSprite->GetCollision( );

   return m_collision.left <= rcTest.right && rcTest.left <= m_collision.right &&
      m_collision.top <= rcTest.bottom && rcTest.top <= m_collision.bottom;
}

inline BOOL Sprite::IsPointInside(LONG x, LONG y)
{
   POINT ptPoint;

   ptPoint.x = x;
   ptPoint.y = y;

   return PtInRect(&m_position, ptPoint);
}

inline Bitmap* Sprite::GetBitmap( ) const
{
   return m_bitmap;
}

inline void Sprite::SetNumFrames(UINT iNumFrames)
{
   m_numFrames = iNumFrames;

   RECT rect = GetPosition( );
   rect.bottom = rect.top + ((rect.bottom - rect.top) / iNumFrames);

   SetPosition(rect);
}

inline void Sprite::SetFrameDelay(UINT iFrameDelay)
{
   m_frameDelay = iFrameDelay;
};

inline RECT& Sprite::GetPosition( )
{
   return m_position;
}

inline void Sprite::SetPosition(LONG x, LONG y)
{
   OffsetRect(&m_position, x - m_position.left, y - m_position.top);
   CalcCollisionRect( );
}

inline void Sprite::SetPosition(POINT ptPosition)
{
   OffsetRect(&m_position, ptPosition.x - m_position.left, ptPosition.y - m_position.top);
   CalcCollisionRect( );
}

inline void Sprite::SetPosition(RECT& rcPosition)
{
   CopyRect(&m_position, &rcPosition);
   CalcCollisionRect( );
}

inline void Sprite::OffsetPosition(LONG x, LONG y)
{
   OffsetRect(&m_position, x, y);
   CalcCollisionRect( );
}

inline void Sprite::SetVelocity(LONG x, LONG y)
{
   m_ptVelocity.x = x;
   m_ptVelocity.y = y;
}

inline RECT& Sprite::GetCollision( )
{
   return m_collision;
}

inline POINT Sprite::GetVelocity( ) const
{
   return m_ptVelocity;
}

inline void Sprite::SetVelocity(POINT ptVelocity)
{
   m_ptVelocity.x = ptVelocity.x;
   m_ptVelocity.y = ptVelocity.y;
}

inline BOOL Sprite::GetZOrder( ) const
{
   return m_zOrder;
}

inline void Sprite::SetZOrder(int iZOrder)
{
   m_zOrder = iZOrder;
}

inline void Sprite::SetBounds(RECT& rcBounds)
{
   CopyRect(&m_bounds, &rcBounds);
}

inline void Sprite::SetBoundsAction(BOUNDSACTION ba)
{
   m_boundsAction = ba;
}

inline BOOL Sprite::IsHidden( ) const
{
   return m_hidden;
};

inline void Sprite::SetHidden(BOOL bHidden)
{
   m_hidden = bHidden;
}

inline LONG Sprite::GetWidth( ) const
{
   return m_bitmap->GetWidth( );
}

inline LONG Sprite::GetHeight( ) const
{
   return (m_bitmap->GetHeight( ) / m_numFrames);
}
