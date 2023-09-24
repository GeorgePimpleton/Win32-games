#pragma once

#include <windows.h>
#include "random_toolkit.hpp"
#include "Bitmap.hpp"

enum class BOUNDSACTION : WORD { BA_STOP   = 0,
                                 BA_WRAP   = 1,
                                 BA_BOUNCE = 2,
                                 BA_DIE    = 3 };

using enum BOUNDSACTION;

using SPRITEACTION           = WORD;
const SPRITEACTION SA_NONE   = 0x0000L;
const SPRITEACTION SA_KILL   = 0x0001L;

class Sprite
{
protected:
   Bitmap*      m_bitmap;
   UINT         m_numFrames;
   UINT         m_curFrame;
   LONG         m_frameDelay;
   LONG         m_frameTrigger;
   RECT         m_position;
   RECT         m_collision;
   POINT        m_velocity;
   int          m_zOrder;
   RECT         m_bounds;
   BOUNDSACTION m_boundsAction;
   BOOL         m_hidden;

public:
            Sprite(Bitmap* bitmap);
            Sprite(Bitmap* bitmap, RECT& bounds, BOUNDSACTION boundsAction = BA_STOP);
            Sprite(Bitmap* bitmap, POINT position, POINT velocity, int zOrder,
                   RECT& bounds, BOUNDSACTION boundsAction = BA_STOP);
   virtual ~Sprite( );

   void         UpdateFrame( );
   virtual void CalcCollisionRect( );

   virtual SPRITEACTION Update( );
   void                 Draw(HDC dc);
   BOOL                 IsPointInside(LONG x, LONG y);
   BOOL                 TestCollision(Sprite* testSprite);

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
   LONG xShrink = (m_position.left - m_position.right) / 12;
   LONG yShrink = (m_position.top - m_position.bottom) / 12;

   CopyRect(&m_collision, &m_position);
   InflateRect(&m_collision, xShrink, yShrink);
}

inline BOOL Sprite::TestCollision(Sprite* testSprite)
{
   RECT& rcTest = testSprite->GetCollision( );

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

inline Bitmap* Sprite::GetBitmap( ) const { return m_bitmap; }

inline void Sprite::SetNumFrames(UINT numFrames)
{
   m_numFrames = numFrames;

   RECT rect   = GetPosition( );
   rect.bottom = rect.top + ((rect.bottom - rect.top) / numFrames);

   SetPosition(rect);
}

inline void Sprite::SetFrameDelay(UINT frameDelay) { m_frameDelay = frameDelay; };

inline RECT& Sprite::GetPosition( )                { return m_position; }

inline void Sprite::SetPosition(LONG x, LONG y)
{
   OffsetRect(&m_position, x - m_position.left, y - m_position.top);
   CalcCollisionRect( );
}

inline void Sprite::SetPosition(POINT position)
{
   OffsetRect(&m_position, position.x - m_position.left, position.y - m_position.top);
   CalcCollisionRect( );
}

inline void Sprite::SetPosition(RECT& position)
{
   CopyRect(&m_position, &position);
   CalcCollisionRect( );
}

inline void Sprite::OffsetPosition(LONG x, LONG y)
{
   OffsetRect(&m_position, x, y);
   CalcCollisionRect( );
}

inline void Sprite::SetVelocity(LONG x, LONG y)
{
   m_velocity.x = x;
   m_velocity.y = y;
}

inline RECT& Sprite::GetCollision( )      { return m_collision; }

inline POINT Sprite::GetVelocity( ) const { return m_velocity; }

inline void Sprite::SetVelocity(POINT velocity)
{
   m_velocity.x = velocity.x;
   m_velocity.y = velocity.y;
}

inline BOOL Sprite::GetZOrder( ) const                         { return m_zOrder; }
inline void Sprite::SetZOrder(int zOrder)                      { m_zOrder = zOrder; }
inline void Sprite::SetBounds(RECT& bounds)                    { CopyRect(&m_bounds, &bounds); }
inline void Sprite::SetBoundsAction(BOUNDSACTION boundsAction) { m_boundsAction = boundsAction; }
inline BOOL Sprite::IsHidden( ) const                          { return m_hidden; };
inline void Sprite::SetHidden(BOOL hidden)                     { m_hidden = hidden; }
inline LONG Sprite::GetWidth( ) const                          { return m_bitmap-> GetWidth( ); }
inline LONG Sprite::GetHeight( ) const                         { return (m_bitmap-> GetHeight( ) / m_numFrames); }
