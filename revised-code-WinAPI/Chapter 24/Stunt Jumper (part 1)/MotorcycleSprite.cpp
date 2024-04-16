#include "MotorcycleSprite.h"

extern HINSTANCE g_hInstance;
extern int       g_bGameOver;

MotorcycleSprite::MotorcycleSprite( Bitmap* pBitmap, RECT& rcBounds,
                                    BOUNDSACTION baBoundsAction )
   : Sprite( pBitmap, rcBounds, baBoundsAction ),
   m_iMINSPEED( 1 ), m_iMAXSPEED( 10 ), m_iHANGTIME( 6 )
{
   m_iCurFrame = 6;
   m_bJumping = FALSE;
   m_iJumpCounter = 0;
   m_bLandedSafely = TRUE;
}

MotorcycleSprite::~MotorcycleSprite( )
{ }

void MotorcycleSprite::UpdateFrame( )
{
   if ( m_bJumping )
   {
      if ( m_iJumpCounter-- >= 0 )
      {
         if ( m_iJumpCounter > ( m_ptVelocity.x * m_iHANGTIME / 2 ) )
         {
            m_iCurFrame = min( m_iCurFrame + 1, 12 );

            if ( m_iJumpCounter % ( m_iHANGTIME / 2 ) == 0 )
            {
               m_ptVelocity.y++;
            }
         }

         else if ( m_iJumpCounter <= ( m_ptVelocity.x * m_iHANGTIME / 2 ) )
         {
            m_iCurFrame = max( m_iCurFrame - 1, 0 );

            if ( m_iJumpCounter % ( m_iHANGTIME / 2 ) == 0 )
            {
               m_ptVelocity.y++;
            }
         }
      }
      else
      {
         m_bJumping = FALSE;
         m_iCurFrame = 6;
         m_ptVelocity.y = 0;

         if ( !m_bLandedSafely )
         {
            PlaySound( ( PCTSTR ) IDW_CRASH, g_hInstance, SND_ASYNC |
                       SND_RESOURCE );

            m_ptVelocity.x = 0;
            g_bGameOver = TRUE;
         }
      }
   }
}

void MotorcycleSprite::IncreaseSpeed( )
{
   if ( !m_bJumping )
   {
      m_ptVelocity.x = min( m_ptVelocity.x + 1, m_iMAXSPEED );
   }
}

void MotorcycleSprite::DecreaseSpeed( )
{
   if ( !m_bJumping )
   {
      m_ptVelocity.x = max( m_ptVelocity.x - 1, m_iMINSPEED );
   }
}

void MotorcycleSprite::StartJumping( )
{
   if ( !m_bJumping )
   {
      m_iJumpCounter = m_ptVelocity.x * m_iHANGTIME;
      m_ptVelocity.y = -m_ptVelocity.x;
      m_bJumping = TRUE;
      m_bLandedSafely = FALSE;
   }
}

void MotorcycleSprite::LandedSafely( )
{
   m_bLandedSafely = TRUE;
}
