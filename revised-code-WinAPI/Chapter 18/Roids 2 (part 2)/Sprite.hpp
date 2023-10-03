#pragma once

#include <windows.h>
#include "random_toolkit.hpp"
#include "Bitmap.hpp"

typedef WORD        SPRITEACTION;
const SPRITEACTION  SA_NONE   = 0x0000L,
                    SA_KILL   = 0x0001L;

enum class BOUNDSACTION : WORD { BA_STOP   = 0,
                                 BA_WRAP   = 1,
                                 BA_BOUNCE = 2,
                                 BA_DIE    = 3 };

using enum BOUNDSACTION;

class Sprite
{
protected:
   Bitmap*      m_bitmap;
   LONG         m_numFrames;
   LONG         m_curFrame;
   LONG         m_frameDelay;
   LONG         m_frameTrigger;
   RECT         m_position;
   RECT         m_collision;
   POINT        m_velocity;
   LONG         m_zOrder;
   RECT         m_bounds;
   BOUNDSACTION m_boundsAction;
   BOOL         m_hidden;
   BOOL         m_dying;
   BOOL         m_oneCycle;

   void         UpdateFrame( );
   virtual void CalcCollisionRect( );

public:
            Sprite(Bitmap* bitmap);
            Sprite(Bitmap* bitmap, RECT& bounds, BOUNDSACTION boundsAction = BA_STOP);
            Sprite(Bitmap* bitmap, POINT position, POINT velocity, LONG zOrder,
                   RECT& bounds, BOUNDSACTION boundsAction = BA_STOP);
   virtual ~Sprite( );

   virtual SPRITEACTION Update( );
   void                 Draw(HDC dc);
   BOOL                 IsPointInside(LONG x, LONG y);
   BOOL                 TestCollision(Sprite* testSprite);
   void                 Kill( ) { m_dying = TRUE; };

   Bitmap* GetBitmap( ) const                         { return m_bitmap; };
   void    SetNumFrames(LONG numFrames, BOOL oneCycle  = FALSE);
   void    SetFrameDelay(LONG frameDelay)              { m_frameDelay = frameDelay; };
   RECT&   GetPosition( )                             { return m_position; };
   void    SetPosition(LONG x, LONG y);
   void    SetPosition(POINT position);
   void    SetPosition(RECT& position);
   void    OffsetPosition(LONG x, LONG y);
   RECT&   GetCollision( )                            { return m_collision; };
   POINT   GetVelocity( ) const                       { return m_velocity; };
   void    SetVelocity(LONG x, LONG y);
   void    SetVelocity(POINT velocity);
   BOOL    GetZOrder( ) const                         { return m_zOrder; };
   void    SetZOrder(LONG zOrder)                     { m_zOrder = zOrder; };
   void    SetBounds(RECT& rcBounds)                  { CopyRect(&m_bounds, &rcBounds); };
   void    SetBoundsAction(BOUNDSACTION boundsAction) { m_boundsAction = boundsAction; };
   BOOL    IsHidden( ) const                          { return m_hidden; };
   void    SetHidden(BOOL hidden)                     { m_hidden = hidden; };
   LONG    GetWidth( ) const                          { return m_bitmap-> GetWidth( ); };
   LONG    GetHeight( ) const                         { return (m_bitmap-> GetHeight( ) / m_numFrames); };
};

inline void Sprite::UpdateFrame( )
{
   if ( (m_frameDelay >= 0) && (--m_frameTrigger <= 0) )
   {
      m_frameTrigger = m_frameDelay;

      if ( ++m_curFrame >= m_numFrames )
      {
         if ( m_oneCycle )
         {
            m_dying = TRUE;
         }
         else
         {
            m_curFrame = 0;
         }
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

   return m_collision.left <= rcTest.right &&
          rcTest.left <= m_collision.right &&
          m_collision.top <= rcTest.bottom &&
          rcTest.top <= m_collision.bottom;
}

inline BOOL Sprite::IsPointInside(LONG x, LONG y)
{
   POINT point = { };

   point.x = x;
   point.y = y;

   return PtInRect(&m_position, point);
}

inline void Sprite::SetNumFrames(LONG numFrames, BOOL oneCycle)
{
   m_numFrames = numFrames;
   m_oneCycle  = oneCycle;

   RECT rect   = GetPosition( );
   rect.bottom = rect.top + ((rect.bottom - rect.top) / numFrames);

   SetPosition(rect);
}

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

inline void Sprite::SetVelocity(POINT velocity)
{
   m_velocity.x = velocity.x;
   m_velocity.y = velocity.y;
}
