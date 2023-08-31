// "Talk to me like I'm a 3 year old!" Programming Lessons - Windows

// Listing 26-19 -  Sprite Header

#pragma once

typedef WORD        BOUNDSACTION;
const BOUNDSACTION  BA_STOP   = 0;
const BOUNDSACTION  BA_WRAP   = 1;
const BOUNDSACTION  BA_BOUNCE = 2;
const BOUNDSACTION  BA_DIE    = 3;

typedef WORD        SPRITEACTION;
const SPRITEACTION  SA_NONE   = 0x0000L;
const SPRITEACTION  SA_KILL   = 0x0001L;



class Sprite
{
public:
            Sprite(Bitmap * pBitmap);
            Sprite(Bitmap * pBitmap, RECT & rcBounds,
                   BOUNDSACTION baBoundsAction = BA_STOP);
            Sprite(Bitmap * pBitmap, POINT ptPosition, POINT ptVelocity,
                   int iZOrder, RECT & rcBounds,
                   BOUNDSACTION baBoundsAction = BA_STOP);
   virtual ~Sprite();

public:
   virtual SPRITEACTION    Update();
   void                    Draw(HDC hDC);
   BOOL                    IsPointInside(int x, int y);
   BOOL                    TestCollision(Sprite * pTestSprite);
   void                    Kill();

public:
   Bitmap *    GetBitmap();
   RECT &      GetPosition();
   void        SetPosition(int x, int y);
   void        SetPosition(POINT ptPosition);
   void        SetPosition(RECT & rcPosition);
   void        OffsetPosition(int x, int y);
   POINT       GetVelocity();
   void        SetVelocity(int x, int y);
   void        SetVelocity(POINT ptVelocity);
   BOOL        GetZOrder();
   void        SetZOrder(int iZOrder);
   void        SetBounds(RECT& rcBounds);
   void        SetBoundsAction(BOUNDSACTION ba);
   BOOL        IsHidden();
   void        SetHidden(BOOL bHidden);
   int         GetWidth();
   int         GetHeight();
   RECT &      GetCollision();
   void        SetNumFrames(int iNumFrames, BOOL bOneCycle = FALSE);
   void        SetFrameDelay(int iFrameDelay);
   int         GetCurFrame() const;

protected:
   virtual void   CalcCollisionRect();
   void           UpdateFrame();

protected:
   // Member Variables
   Bitmap *       m_pBitmap;
   RECT           m_rcPosition;
   RECT           m_rcCollision;
   POINT          m_ptVelocity;
   int            m_iZOrder;
   RECT           m_rcBounds;
   BOUNDSACTION   m_baBoundsAction;
   BOOL           m_bHidden;
   int            m_iNumFrames;
   int            m_iCurFrame;
   int            m_iFrameDelay;
   int            m_iFrameTrigger;
   BOOL           m_bDying;
   BOOL           m_bOneCycle;
};



inline BOOL Sprite::IsPointInside(int x, int y)
{
   POINT ptPoint;

   ptPoint.x = x;
   ptPoint.y = y;

   return PtInRect(&m_rcPosition, ptPoint);
}


inline BOOL Sprite::TestCollision(Sprite * pTestSprite)
{
   RECT & rcTest = pTestSprite->GetCollision();
   
   return   m_rcCollision.left <= rcTest.right &&
            rcTest.left <= m_rcCollision.right &&
            m_rcCollision.top <= rcTest.bottom &&
            rcTest.top <= m_rcCollision.bottom;
}


inline void Sprite::Kill()
{
   m_bDying = TRUE;
}


inline Bitmap * Sprite::GetBitmap()
{
   return m_pBitmap;
}


inline RECT & Sprite::GetPosition()
{
   return m_rcPosition;
}


inline void Sprite::SetPosition(int x, int y)
{
   OffsetRect(&m_rcPosition, x - m_rcPosition.left, y - m_rcPosition.top);
   CalcCollisionRect();
}


inline void Sprite::SetPosition(POINT ptPosition)
{
   OffsetRect(&m_rcPosition, ptPosition.x - m_rcPosition.left,
              ptPosition.y - m_rcPosition.top);
   CalcCollisionRect();
}


inline void Sprite::SetPosition(RECT & rcPosition)
{
   CopyRect(&m_rcPosition, &rcPosition);
}


inline void Sprite::OffsetPosition(int x, int y)
{
   OffsetRect(&m_rcPosition, x, y);
   CalcCollisionRect();
}


inline POINT Sprite::GetVelocity()
{
   return m_ptVelocity;
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


inline int Sprite::GetWidth()
{
   return (m_pBitmap->GetWidth() / m_iNumFrames);
}


inline int Sprite::GetHeight()
{
   return m_pBitmap->GetHeight();
}


inline RECT & Sprite::GetCollision()
{
   return m_rcCollision;
}


inline void Sprite::SetNumFrames(int iNumFrames, BOOL bOneCycle)
{
   // Set the number of frames
   m_iNumFrames   = iNumFrames;
   m_bOneCycle    = bOneCycle;

   // Recalculate the position
   RECT rect = GetPosition();
   rect.right = rect.left + ((rect.right - rect.left) / iNumFrames);
   SetPosition(rect);
}


inline void Sprite::SetFrameDelay(int iFrameDelay)
{
   m_iFrameDelay = iFrameDelay;
}


inline int Sprite::GetCurFrame() const
{
   return m_iCurFrame;
}
