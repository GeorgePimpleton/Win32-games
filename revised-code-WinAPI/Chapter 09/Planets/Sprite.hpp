#pragma once

#include <windows.h>
#include <memory>
#include "Bitmap.hpp"
#include "random_toolkit.hpp"

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

class Sprite
{
protected:
   std::unique_ptr<Bitmap> m_bitmap;
   RECT                    m_position;
   POINT                   m_velocity;
   int                     m_zOrder;
   RECT                    m_bounds;
   BOUNDSACTION            m_boundsAction;
   BOOL                    m_hidden;

public:
            Sprite(Bitmap* bitmap);
            Sprite(Bitmap* bitmap, RECT& bounds, BOUNDSACTION boundsAction = BA_STOP);
            Sprite(Bitmap* bitmap, POINT position, POINT velocity, int zOrder, RECT& bounds,
                   BOUNDSACTION boundsAction = BA_STOP);
   virtual ~Sprite( );

public:
   virtual void Update( );
   void         Draw(HDC dc);
   BOOL         IsPointInside(LONG x, LONG y);

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
};

inline BOOL Sprite::IsPointInside(LONG x, LONG y)
{
   POINT point;

   point.x = x;
   point.y = y;

   return PtInRect(&m_position, point);
}

inline RECT& Sprite::GetPosition( )
{
   return m_position;
}

inline void Sprite::SetPosition(LONG x, LONG y)
{
   OffsetRect(&m_position, x - m_position.left, y - m_position.top);
}

inline void Sprite::SetPosition(POINT position)
{
   OffsetRect(&m_position, position.x - m_position.left,
              position.y - m_position.top);
}

inline void Sprite::SetPosition(RECT& position)
{
   CopyRect(&m_position, &position);
}

inline void Sprite::OffsetPosition(LONG x, LONG y)
{
   OffsetRect(&m_position, x, y);
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
