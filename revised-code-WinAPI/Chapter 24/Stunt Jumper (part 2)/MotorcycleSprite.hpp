#pragma once

#include <windows.h>
#include "resource.h"
#include "Sprite.hpp"

class MotorcycleSprite : public Sprite
{
protected:
   const LONG m_MINSPEED;
   const LONG m_MAXSPEED;
   const LONG m_HANGTIME;
   BOOL       m_jumping;
   int        m_jumpCounter;
   BOOL       m_landedSafely;

public:
            MotorcycleSprite( Bitmap*, RECT&, BOUNDSACTION = BA_STOP );
   virtual ~MotorcycleSprite( );

   virtual void UpdateFrame( );

   void IncreaseSpeed( );
   void DecreaseSpeed( );
   void StartJumping( );
   void LandedSafely( );
   BOOL HasLandedSafely( ) { return m_landedSafely; };
};
