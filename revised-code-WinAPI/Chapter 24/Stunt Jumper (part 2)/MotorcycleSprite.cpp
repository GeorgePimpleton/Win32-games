#include "MotorcycleSprite.hpp"

extern HINSTANCE g_inst;
extern int       g_gameOver;

MotorcycleSprite::MotorcycleSprite(Bitmap* bitmap, RECT& bounds,
                                   BOUNDSACTION boundsAction)
   : Sprite(bitmap, bounds, boundsAction),
   m_MINSPEED(1), m_MAXSPEED(10), m_HANGTIME(6)
{
   // initialize the motorcycle sprite variables
   m_curFrame     = 6;  // the frame where the motorcycle is level
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
      // start jumping
      if ( m_jumpCounter-- >= 0 )
      {
         // see if the motorcycle is on the way up
         if ( m_jumpCounter > (m_velocity.x * m_HANGTIME / 2) )
         {
            // change the frame to show the motorcycle tilt upward
            m_curFrame = min(m_curFrame + 1, 12);

            // change the vertical velocity to cause the motorcycle to ascend
            if ( m_jumpCounter % (m_HANGTIME / 2) == 0 )
            {
               m_velocity.y++;
            }
         }

         // see if the motorcycle is on the way down
         else if ( m_jumpCounter <= (m_velocity.x * m_HANGTIME / 2) )
         {
            // change the frame to show the motorcycle tilt downward
            m_curFrame = max(m_curFrame - 1, 0);

            // change the vertical velocity to cause the motorcycle to descend
            if ( m_jumpCounter % (m_HANGTIME / 2) == 0 )
            {
               m_velocity.y++;
            }
         }
      }
      else
      {
         // stop the jump and level the motorcycle
         m_jumping    = FALSE;
         m_curFrame   = 6;
         m_velocity.y = 0;

         // see if the motorcycle overshot the landing ramp
         if ( !m_landedSafely )
         {
            // play the crash sound
            PlaySoundW((PCWSTR) IDW_CRASH, g_inst, SND_ASYNC | SND_RESOURCE);

            // end the game
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
      // increase the horizontal speed of the motorcycle
      m_velocity.x = min(m_velocity.x + 1, m_MAXSPEED);
   }
}

void MotorcycleSprite::DecreaseSpeed( )
{
   if ( !m_jumping )
   {
      // decrease the horizontal speed of the motorcycle
      m_velocity.x = max(m_velocity.x - 1, m_MINSPEED);
   }
}

void MotorcycleSprite::StartJumping( )
{
   if ( !m_jumping )
   {
      // start the motorcycle jumping
      m_jumpCounter  = m_velocity.x * m_HANGTIME;
      m_velocity.y   = -m_velocity.x;
      m_jumping      = TRUE;
      m_landedSafely = FALSE;
   }
}

void MotorcycleSprite::LandedSafely( )
{
   // flag the motorcycle as having landed safely (hit the landing ramp)
   m_landedSafely = TRUE;
}