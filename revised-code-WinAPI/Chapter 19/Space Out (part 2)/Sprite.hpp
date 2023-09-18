#pragma once

#include <windows.h>
#include "Bitmap.hpp"

typedef WORD        SPRITEACTION;
const SPRITEACTION  SA_NONE      = 0x0000L,
                    SA_KILL      = 0x0001L,
                    SA_ADDSPRITE = 0x0002L;

typedef WORD        BOUNDSACTION;
const BOUNDSACTION  BA_STOP   = 0,
                    BA_WRAP   = 1,
                    BA_BOUNCE = 2,
                    BA_DIE    = 3;

class Sprite
{
protected:
   Bitmap*      m_bitmap;
   int          m_iNumFrames;
   int          m_iCurFrame;
   int          m_frameDelay;
   int          m_iFrameTrigger;
   RECT         m_rcPosition;
   RECT         m_rcCollision;
   POINT        m_ptVelocity;
   int          m_iZOrder;
   RECT         m_rcBounds;
   BOUNDSACTION m_baBoundsAction;
   BOOL         m_bHidden;
   BOOL         m_bDying;
   BOOL         m_bOneCycle;

   void         UpdateFrame( );
   virtual void CalcCollisionRect( );

public:
            Sprite(Bitmap* bitmap);
            Sprite(Bitmap* bitmap, RECT& rcBounds,
                   BOUNDSACTION baBoundsAction = BA_STOP);
            Sprite(Bitmap* bitmap, POINT ptPosition, POINT ptVelocity, int iZOrder,
                   RECT& rcBounds, BOUNDSACTION baBoundsAction = BA_STOP);
   virtual ~Sprite( );

   virtual SPRITEACTION Update( );
   virtual Sprite*      AddSprite( );
   void                 Draw(HDC dc);
   BOOL                 IsPointInside(int x, int y);
   BOOL                 TestCollision(Sprite* testSprite);
   void                 Kill( ) { m_bDying = TRUE; };

   Bitmap* GetBitmap( )                                { return m_bitmap; };
   void    SetNumFrames(int iNumFrames, BOOL bOneCycle = FALSE);
   void    SetFrameDelay(int iFrameDelay)              { m_frameDelay = iFrameDelay; };
   RECT&   GetPosition( )                              { return m_rcPosition; };
   void    SetPosition(int x, int y);
   void    SetPosition(POINT ptPosition);
   void    SetPosition(RECT& rcPosition);
   void    OffsetPosition(int x, int y);
   RECT&   GetCollision( )                             { return m_rcCollision; };
   POINT   GetVelocity( )                              { return m_ptVelocity; };
   void    SetVelocity(int x, int y);
   void    SetVelocity(POINT ptVelocity);
   BOOL    GetZOrder( )                                { return m_iZOrder; };
   void    SetZOrder(int iZOrder)                      { m_iZOrder = iZOrder; };
   void    SetBounds(RECT& rcBounds)                   { CopyRect(&m_rcBounds, &rcBounds); };
   void    SetBoundsAction(BOUNDSACTION ba)            { m_baBoundsAction = ba; };
   BOOL    IsHidden( )                                 { return m_bHidden; };
   void    SetHidden(BOOL bHidden)                     { m_bHidden = bHidden; };
   int     GetWidth( )                                 { return m_bitmap-> GetWidth( ); };
   int     GetHeight( )                                { return (m_bitmap-> GetHeight( ) / m_iNumFrames); };
};

inline void Sprite::UpdateFrame( )
{
   if ( (m_frameDelay >= 0) && (--m_iFrameTrigger <= 0) )
   {
      m_iFrameTrigger = m_frameDelay;

      if ( ++m_iCurFrame >= m_iNumFrames )
      {
         if ( m_bOneCycle )
         {
            m_bDying = TRUE;
         }
         else
         {
            m_iCurFrame = 0;
         }
      }
   }
}

inline void Sprite::CalcCollisionRect( )
{
   int iXShrink = (m_rcPosition.left - m_rcPosition.right) / 12;
   int iYShrink = (m_rcPosition.top - m_rcPosition.bottom) / 12;

   CopyRect(&m_rcCollision, &m_rcPosition);
   InflateRect(&m_rcCollision, iXShrink, iYShrink);
}

inline BOOL Sprite::TestCollision(Sprite* testSprite)
{
   RECT& rcTest = testSprite->GetCollision( );

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

inline void Sprite::SetNumFrames(int iNumFrames, BOOL bOneCycle)
{
   m_iNumFrames = iNumFrames;
   m_bOneCycle = bOneCycle;

   RECT rect = GetPosition( );
   rect.bottom = rect.top + ((rect.bottom - rect.top) / iNumFrames);
   SetPosition(rect);
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

inline void Sprite::SetPosition(RECT& rcPosition)
{
   CopyRect(&m_rcPosition, &rcPosition);
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
