#pragma once

#include <windows.h>
#include "Bitmap.hpp"
#include "random_toolkit_v131.hpp"

using BOUNDSACTION = WORD;
const BOUNDSACTION BA_STOP   = 0,
                   BA_WRAP   = 1,
                   BA_BOUNCE = 2,
                   BA_DIE    = 3;

class Sprite
{
protected:
   Bitmap*      m_bitmap;
   RECT         m_position;
   POINT        m_velocity;
   LONG         m_zOrder;
   RECT         m_bounds;
   BOUNDSACTION m_boundsAction;
   BOOL         m_hidden;

public:
            Sprite( Bitmap* );
            Sprite( Bitmap*, RECT&, BOUNDSACTION = BA_STOP );
            Sprite( Bitmap*, POINT, POINT, LONG, RECT&,
                    BOUNDSACTION = BA_STOP );
   virtual ~Sprite( );

public:
   virtual void Update( );
   void         Draw( HDC ) const;
   BOOL         IsPointInside( LONG, LONG );

public:
   RECT& GetPosition( );
   void  SetPosition( LONG, LONG );
   void  SetPosition( POINT );
   void  SetPosition( RECT& );
   void  OffsetPosition( LONG, LONG );
   POINT GetVelocity( );
   void  SetVelocity( LONG, LONG );
   void  SetVelocity( POINT );
   BOOL  GetZOrder( ) const;
   void  SetZOrder( LONG );
   void  SetBounds( RECT& );
   void  SetBoundsAction( BOUNDSACTION );
   BOOL  IsHidden( ) const;
   void  SetHidden( BOOL );
   LONG  GetWidth( ) const;
   LONG  GetHeight( ) const;
};

inline BOOL Sprite::IsPointInside( LONG x, LONG y )
{
   POINT point = { };

   point.x = x;
   point.y = y;

   return PtInRect( &m_position, point );
}

inline RECT& Sprite::GetPosition( )
{
   return m_position;
}

inline void Sprite::SetPosition( LONG x, LONG y )
{
   OffsetRect( &m_position, x - m_position.left, y - m_position.top );
}

inline void Sprite::SetPosition( POINT position )
{
   OffsetRect( &m_position, position.x - m_position.left,
               position.y - m_position.top );
}

inline void Sprite::SetPosition( RECT& position )
{
   CopyRect( &m_position, &position );
}

inline void Sprite::OffsetPosition( LONG x, LONG y )
{
   OffsetRect( &m_position, x, y );
}

inline POINT Sprite::GetVelocity( )
{
   return m_velocity;
}

inline void Sprite::SetVelocity( LONG x, LONG y )
{
   m_velocity.x = x;
   m_velocity.y = y;
}

inline void Sprite::SetVelocity( POINT velocity )
{
   m_velocity.x = velocity.x;
   m_velocity.y = velocity.y;
}

inline BOOL Sprite::GetZOrder( ) const                           { return m_zOrder; }
inline void Sprite::SetZOrder( LONG zOrder )                     { m_zOrder = zOrder; }
inline void Sprite::SetBounds( RECT& bounds )                    { CopyRect( &m_bounds, &bounds ); }
inline void Sprite::SetBoundsAction( BOUNDSACTION boundsAction ) { m_boundsAction = boundsAction; }
inline BOOL Sprite::IsHidden( ) const                            { return m_hidden; }
inline void Sprite::SetHidden( BOOL hidden )                     { m_hidden = hidden; }
inline LONG Sprite::GetWidth( ) const                            { return m_bitmap-> GetWidth( ); }
inline LONG Sprite::GetHeight( ) const                           { return m_bitmap-> GetHeight( ); }
