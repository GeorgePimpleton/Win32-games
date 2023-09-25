#pragma once

#include <windows.h>
#include "Bitmap.hpp"
#include "random_toolkit.hpp"

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
   Bitmap* m_bitmap;
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
            Sprite(Bitmap* bitmap, POINT position, POINT velocity, int zOrder, RECT& bounds,
                   BOUNDSACTION boundsAction = BA_STOP);
   virtual ~Sprite( );

public:
   virtual SPRITEACTION Update( );
   void                 Draw(HDC dc);
   BOOL                 IsPointInside(LONG x, LONG y);
   BOOL                 TestCollision(Sprite* testSprite);

protected:
   virtual void CalcCollisionRect( );

public:
   RECT& GetPosition( );
   void  SetPosition(LONG x, LONG y);
   void  SetPosition(POINT position);
   void  SetPosition(RECT& position);
   void  OffsetPosition(LONG x, LONG y);
   POINT GetVelocity( );
   void  SetVelocity(LONG x, LONG y);
   void  SetVelocity(POINT velocity);
   BOOL  GetZOrder( );
   void  SetZOrder(int zOrder);
   void  SetBounds(RECT& bounds);
   void  SetBoundsAction(BOUNDSACTION boundsAction);
   BOOL  IsHidden( );
   void  SetHidden(BOOL hidden);
   LONG  GetWidth( );
   LONG  GetHeight( );
   RECT& GetCollision( );
};

inline BOOL Sprite::IsPointInside(LONG x, LONG y)
{
   POINT point = { };

   point.x = x;
   point.y = y;

   return PtInRect(&m_position, point);
}

inline BOOL Sprite::TestCollision(Sprite* testSprite)
{
   RECT& rcTest = testSprite->GetCollision( );

   return m_collision.left <= rcTest.right && rcTest.left <= m_collision.right &&
          m_collision.top <= rcTest.bottom && rcTest.top <= m_collision.bottom;
}

inline RECT& Sprite::GetPosition( )
{
   return m_position;
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
}

inline void Sprite::OffsetPosition(LONG x, LONG y)
{
   OffsetRect(&m_position, x, y);
   CalcCollisionRect( );
}

inline POINT Sprite::GetVelocity( )
{
   return m_velocity;
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

inline BOOL Sprite::GetZOrder( )
{
   return m_zOrder;
}

inline void Sprite::SetZOrder(int zOrder)
{
   m_zOrder = zOrder;
}

inline void Sprite::SetBounds(RECT& bounds)
{
   CopyRect(&m_bounds, &bounds);
}

inline void Sprite::SetBoundsAction(BOUNDSACTION boundsAction)
{
   m_boundsAction = boundsAction;
}

inline BOOL Sprite::IsHidden( )
{
   return m_hidden;
}

inline void Sprite::SetHidden(BOOL hidden)
{
   m_hidden = hidden;
}

inline LONG Sprite::GetWidth( )
{
   return m_bitmap->GetWidth( );
}

inline LONG Sprite::GetHeight( )
{
   return m_bitmap->GetHeight( );
}

inline RECT& Sprite::GetCollision( )
{
   return m_collision;
}