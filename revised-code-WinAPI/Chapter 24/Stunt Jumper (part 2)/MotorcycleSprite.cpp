#include "MotorcycleSprite.hpp"

extern HINSTANCE g_inst;
extern int       g_bGameOver;

MotorcycleSprite::MotorcycleSprite(Bitmap* bitmap, RECT& bounds,
                                   BOUNDSACTION boundsAction)
   : Sprite(bitmap, bounds, boundsAction),
   m_iMINSPEED(1), m_iMAXSPEED(10), m_iHANGTIME(6)
{
   // Initialize the motorcycle sprite variables
   m_curFrame = 6;  // the frame where the motorcycle is level
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
      // Start jumping
      if ( m_iJumpCounter-- >= 0 )
      {
         // See if the motorcycle is on the way up
         if ( m_iJumpCounter > (m_velocity.x * m_iHANGTIME / 2) )
         {
            // Change the frame to show the motorcycle tilt upward
            m_curFrame = min(m_curFrame + 1, 12);

            // Change the vertical velocity to cause the motorcycle to ascend
            if ( m_iJumpCounter % (m_iHANGTIME / 2) == 0 )
            {
               m_velocity.y++;
            }
         }

         // See if the motorcycle is on the way down
         else if ( m_iJumpCounter <= (m_velocity.x * m_iHANGTIME / 2) )
         {
            // Change the frame to show the motorcycle tilt downward
            m_curFrame = max(m_curFrame - 1, 0);

            // Change the vertical velocity to cause the motorcycle to descend
            if ( m_iJumpCounter % (m_iHANGTIME / 2) == 0 )
            {
               m_velocity.y++;
            }
         }
      }
      else
      {
         // Stop the jump and level the motorcycle
         m_bJumping = FALSE;
         m_curFrame = 6;
         m_velocity.y = 0;

         // See if the motorcycle overshot the landing ramp
         if ( !m_bLandedSafely )
         {
            // Play the crash sound
            PlaySoundW((PCWSTR) IDW_CRASH, g_inst, SND_ASYNC | SND_RESOURCE);

            // End the game
            m_velocity.x = 0;
            g_bGameOver = TRUE;
         }
      }
   }
}

void MotorcycleSprite::IncreaseSpeed( )
{
   if ( !m_bJumping )
   {
      // Increase the horizontal speed of the motorcycle
      m_velocity.x = min(m_velocity.x + 1, m_iMAXSPEED);
   }
}

void MotorcycleSprite::DecreaseSpeed( )
{
   if ( !m_bJumping )
   {
      // Decrease the horizontal speed of the motorcycle
      m_velocity.x = max(m_velocity.x - 1, m_iMINSPEED);
   }
}

void MotorcycleSprite::StartJumping( )
{
   if ( !m_bJumping )
   {
      // Start the motorcycle jumping
      m_iJumpCounter = m_velocity.x * m_iHANGTIME;
      m_velocity.y = -m_velocity.x;
      m_bJumping = TRUE;
      m_bLandedSafely = FALSE;
   }
}

void MotorcycleSprite::LandedSafely( )
{
   // Flag the motorcycle as having landed safely (hit the landing ramp)
   m_bLandedSafely = TRUE;
}