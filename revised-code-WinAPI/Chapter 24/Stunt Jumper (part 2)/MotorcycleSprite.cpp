#include "MotorcycleSprite.hpp"

extern HINSTANCE g_inst;
extern int       g_gameOver;

MotorcycleSprite::MotorcycleSprite( Bitmap* bitmap, RECT& bounds,
                                    BOUNDSACTION boundsAction )
   : Sprite( bitmap, bounds, boundsAction ),
   m_MINSPEED( 1 ), m_MAXSPEED( 10 ), m_HANGTIME( 6 )
{
   m_curFrame     = 6;
   m_jumping      = FALSE;
   m_jumpCounter  = 0;
   m_landedSafely = TRUE;
}

MotorcycleSprite::~MotorcycleSprite( )
{ }

void MotorcycleSprite::UpdateFrame( )
{
   if ( m_jumping )
   {
      if ( m_jumpCounter-- >= 0 )
      {
         if ( m_jumpCounter > ( m_velocity.x * m_HANGTIME / 2 ) )
         {
            m_curFrame = min( m_curFrame + 1, 12 );

            if ( m_jumpCounter % ( m_HANGTIME / 2 ) == 0 )
            {
               m_velocity.y++;
            }
         }

         else if ( m_jumpCounter <= ( m_velocity.x * m_HANGTIME / 2 ) )
         {
            m_curFrame = max( m_curFrame - 1, 0 );

            if ( m_jumpCounter % ( m_HANGTIME / 2 ) == 0 )
            {
               m_velocity.y++;
            }
         }
      }
      else
      {
         m_jumping    = FALSE;
         m_curFrame   = 6;
         m_velocity.y = 0;

         if ( !m_landedSafely )
         {
            PlaySoundW( ( PCWSTR ) IDW_CRASH, g_inst, SND_ASYNC | SND_RESOURCE );

            m_velocity.x = 0;
            g_gameOver   = TRUE;
         }
      }
   }
}

void MotorcycleSprite::IncreaseSpeed( )
{
   if ( !m_jumping )
   {
      m_velocity.x = min( m_velocity.x + 1, m_MAXSPEED );
   }
}

void MotorcycleSprite::DecreaseSpeed( )
{
   if ( !m_jumping )
   {
      m_velocity.x = max( m_velocity.x - 1, m_MINSPEED );
   }
}

void MotorcycleSprite::StartJumping( )
{
   if ( !m_jumping )
   {
      m_jumpCounter  = m_velocity.x * m_HANGTIME;
      m_velocity.y   = -m_velocity.x;
      m_jumping      = TRUE;
      m_landedSafely = FALSE;
   }
}

void MotorcycleSprite::LandedSafely( )
{
   m_landedSafely = TRUE;
}
