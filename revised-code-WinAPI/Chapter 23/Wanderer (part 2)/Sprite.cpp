#include "Sprite.hpp"

Sprite::Sprite(Bitmap* bitmap)
{
   m_bitmap    = bitmap;
   m_numFrames = 1;
   m_curFrame  = m_frameDelay = m_frameTrigger = 0;

   SetRect(&m_position, 0, 0, bitmap->GetWidth( ), bitmap->GetHeight( ));
   CalcCollisionRect( );

   m_velocity.x = m_velocity.y = 0;
   m_zOrder     = 0;

   SetRect(&m_bounds, 0, 0, 640, 480);

   m_boundsAction = BA_STOP;
   m_hidden       = FALSE;
   m_dying        = FALSE;
   m_oneCycle     = FALSE;
}

Sprite::Sprite(Bitmap* bitmap, RECT& bounds, BOUNDSACTION boundsAction)
{
   int xPos = rand( ) % (bounds.right - bounds.left);
   int yPos = rand( ) % (bounds.bottom - bounds.top);

   m_bitmap    = bitmap;
   m_numFrames = 1;
   m_curFrame  = m_frameDelay = m_frameTrigger = 0;

   SetRect(&m_position, xPos, yPos,
           xPos + bitmap->GetWidth( ),
           yPos + bitmap->GetHeight( ));
   CalcCollisionRect( );

   m_velocity.x = m_velocity.y = 0;
   m_zOrder     = 0;

   CopyRect(&m_bounds, &bounds);

   m_boundsAction = boundsAction;
   m_hidden       = FALSE;
   m_dying        = FALSE;
   m_oneCycle     = FALSE;
}

Sprite::Sprite(Bitmap* bitmap, POINT position, POINT velocity, LONG zOrder,
               RECT& bounds, BOUNDSACTION boundsAction)
{
   m_bitmap    = bitmap;
   m_numFrames = 1;
   m_curFrame  = m_frameDelay = m_frameTrigger = 0;

   SetRect(&m_position, position.x, position.y,
           position.x + bitmap->GetWidth( ), position.y + bitmap->GetHeight( ));
   CalcCollisionRect( );

   m_velocity = velocity;
   m_zOrder   = zOrder;

   CopyRect(&m_bounds, &bounds);

   m_boundsAction = boundsAction;
   m_hidden       = FALSE;
   m_dying        = FALSE;
   m_oneCycle     = FALSE;
}

Sprite::~Sprite( )
{ }

SPRITEACTION Sprite::Update( )
{
   if ( m_dying )
   {
      return SA_KILL;
   }

   UpdateFrame( );

   POINT ptNewPosition = { };
   POINT ptSpriteSize  = { };
   POINT ptBoundsSize  = { };

   ptNewPosition.x = m_position.left + m_velocity.x;
   ptNewPosition.y = m_position.top + m_velocity.y;
   ptSpriteSize.x = m_position.right - m_position.left;
   ptSpriteSize.y = m_position.bottom - m_position.top;
   ptBoundsSize.x = m_bounds.right - m_bounds.left;
   ptBoundsSize.y = m_bounds.bottom - m_bounds.top;

   // check the bounds
   // wrap?
   if ( m_boundsAction == BA_WRAP )
   {
      if ( (ptNewPosition.x + ptSpriteSize.x) < m_bounds.left )
      {
         ptNewPosition.x = m_bounds.right;
      }
      else if ( ptNewPosition.x > m_bounds.right )
      {
         ptNewPosition.x = m_bounds.left - ptSpriteSize.x;
      }

      if ( (ptNewPosition.y + ptSpriteSize.y) < m_bounds.top )
      {
         ptNewPosition.y = m_bounds.bottom;
      }
      else if ( ptNewPosition.y > m_bounds.bottom )
      {
         ptNewPosition.y = m_bounds.top - ptSpriteSize.y;
      }
   }
   // bounce?
   else if ( m_boundsAction == BA_BOUNCE )
   {
      BOOL  bBounce       = FALSE;
      POINT ptNewVelocity = m_velocity;

      if ( ptNewPosition.x < m_bounds.left )
      {
         bBounce         = TRUE;
         ptNewPosition.x = m_bounds.left;
         ptNewVelocity.x = -ptNewVelocity.x;
      }
      else if ( (ptNewPosition.x + ptSpriteSize.x) > m_bounds.right )
      {
         bBounce         = TRUE;
         ptNewPosition.x = m_bounds.right - ptSpriteSize.x;
         ptNewVelocity.x = -ptNewVelocity.x;
      }

      if ( ptNewPosition.y < m_bounds.top )
      {
         bBounce         = TRUE;
         ptNewPosition.y = m_bounds.top;
         ptNewVelocity.y = -ptNewVelocity.y;
      }
      else if ( (ptNewPosition.y + ptSpriteSize.y) > m_bounds.bottom )
      {
         bBounce         = TRUE;
         ptNewPosition.y = m_bounds.bottom - ptSpriteSize.y;
         ptNewVelocity.y = -ptNewVelocity.y;
      }
      if ( bBounce )
      {
         SetVelocity(ptNewVelocity);
      }
   }
   // die?
   else if ( m_boundsAction == BA_DIE )
   {
      if ( (ptNewPosition.x + ptSpriteSize.x) < m_bounds.left ||
            ptNewPosition.x > m_bounds.right ||
           (ptNewPosition.y + ptSpriteSize.y) < m_bounds.top ||
            ptNewPosition.y > m_bounds.bottom )
      {
         return SA_KILL;
      }
   }
   // stop (default)
   else
   {
      if ( ptNewPosition.x  < m_bounds.left ||
           ptNewPosition.x >(m_bounds.right - ptSpriteSize.x) )
      {
         ptNewPosition.x = max(m_bounds.left, min(ptNewPosition.x,
                                                    m_bounds.right - ptSpriteSize.x));
         SetVelocity(0, 0);
      }

      if ( ptNewPosition.y  < m_bounds.top ||
           ptNewPosition.y >(m_bounds.bottom - ptSpriteSize.y) )
      {
         ptNewPosition.y = max(m_bounds.top, min(ptNewPosition.y,
                                                   m_bounds.bottom - ptSpriteSize.y));
         SetVelocity(0, 0);
      }
   }

   SetPosition(ptNewPosition);

   return SA_NONE;
}

Sprite* Sprite::AddSprite( )
{
   return NULL;
}

void Sprite::Draw(HDC dc)
{
   if ( m_bitmap != NULL && !m_hidden )
   {
      if ( m_numFrames == 1 )
      {
         m_bitmap->Draw(dc, m_position.left, m_position.top, TRUE);
      }
      else
      {
         m_bitmap->DrawPart(dc, m_position.left, m_position.top,
                             0, m_curFrame * GetHeight( ), GetWidth( ), GetHeight( ), TRUE);
      }
   }
}