#pragma once

#include <windows.h>
#include "resource.h"
#include "Sprite.hpp"

class MotorcycleSprite : public Sprite
{
protected:
   const int m_MINSPEED;
   const int m_MAXSPEED;
   const int m_HANGTIME;
   BOOL      m_jumping;
   int       m_jumpCounter;
   BOOL      m_landedSafely;

public:
            MotorcycleSprite(Bitmap* bitmap, RECT& bounds,
                             BOUNDSACTION boundsAction = BA_STOP);
   virtual ~MotorcycleSprite( );

   virtual void UpdateFrame( );

   void IncreaseSpeed( );
   void DecreaseSpeed( );
   void StartJumping( );
   void LandedSafely( );
   BOOL HasLandedSafely( ) { return m_landedSafely; };
};
