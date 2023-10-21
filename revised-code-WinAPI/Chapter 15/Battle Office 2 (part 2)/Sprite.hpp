#pragma once

#include <windows.h>
#include "Bitmap.hpp"
#include "random_toolkit.hpp"

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
   Bitmap*      m_bitmap;
   UINT         m_numFrames;
   UINT         m_curFrame;
   LONG         m_frameDelay;
   LONG         m_frameTrigger;
   RECT         m_position;
   RECT         m_collision;
   POINT        m_velocity;
   LONG         m_zOrder;
   RECT         m_bounds;
   BOUNDSACTION m_boundsAction;
   BOOL         m_hidden;

   void          UpdateFrame( );
   virtual void  CalcCollisionRect( );

public:
            Sprite(Bitmap*);
            Sprite(Bitmap*, RECT&, BOUNDSACTION = BA_STOP);
            Sprite(Bitmap*, POINT, POINT, LONG, RECT&, BOUNDSACTION = BA_STOP);
   virtual ~Sprite( );

   virtual SPRITEACTION  Update( );
   void                  Draw(HDC);
   BOOL                  IsPointInside(LONG, LONG);
   BOOL                  TestCollision(Sprite*);

   Bitmap* GetBitmap( )                               { return m_bitmap; };
   void    SetNumFrames(UINT numFrames);
   void    SetFrameDelay(UINT frameDelay)             { m_frameDelay = frameDelay; };
   RECT&   GetPosition( )                             { return m_position; };
   void    SetPosition(LONG, LONG);
   void    SetPosition(POINT);
   void    SetPosition(RECT&);
   void    OffsetPosition(LONG, LONG);
   RECT&   GetCollision( )                            { return m_collision; };
   POINT   GetVelocity( ) const                       { return m_velocity; };
   void    SetVelocity(LONG, LONG);
   void    SetVelocity(POINT);
   BOOL    GetZOrder( ) const                         { return m_zOrder; };
   void    SetZOrder(LONG zOrder)                     { m_zOrder = zOrder; };
   void    SetBounds(RECT& bounds)                    { CopyRect(&m_bounds, &bounds); };
   void    SetBoundsAction(BOUNDSACTION boundsAction) { m_boundsAction = boundsAction; };
   BOOL    IsHidden( ) const                          { return m_hidden; };
   void    SetHidden(BOOL hidden)                     { m_hidden = hidden; };
   int     GetWidth( ) const                          { return (m_bitmap->GetWidth( ) / m_numFrames); };
   int     GetHeight( ) const                         { return m_bitmap->GetHeight( ); };
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
   int xShrink = (m_position.left - m_position.right) / 12;
   int yShrink = (m_position.top - m_position.bottom) / 12;

   CopyRect(&m_collision, &m_position);
   InflateRect(&m_collision, xShrink, yShrink);
}

inline BOOL Sprite::TestCollision(Sprite* testSprite)
{
   RECT& test = testSprite->GetCollision( );

   return m_collision.left <= test.right &&
          test.left <= m_collision.right &&
          m_collision.top <= test.bottom &&
          test.top <= m_collision.bottom;
}

inline BOOL Sprite::IsPointInside(LONG x, LONG y)
{
   POINT point = { };

   point.x = x;
   point.y = y;

   return PtInRect(&m_position, point);
}

inline void Sprite::SetNumFrames(UINT numFrames)
{
   m_numFrames = numFrames;

   RECT rect  = GetPosition( );
   rect.right = rect.left + ((rect.right - rect.left) / numFrames);

   SetPosition(rect);
}

inline void Sprite::SetPosition(LONG x, LONG y)
{
   OffsetRect(&m_position, x - m_position.left, y - m_position.top);
   CalcCollisionRect( );
}

inline void Sprite::SetPosition(POINT position)
{
   OffsetRect(&m_position, position.x - m_position.left,
              position.y - m_position.top);
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
