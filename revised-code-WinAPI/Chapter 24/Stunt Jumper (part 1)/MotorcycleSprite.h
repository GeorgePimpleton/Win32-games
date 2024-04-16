#pragma once

#include <windows.h>
#include "resource.h"
#include "Sprite.h"

class MotorcycleSprite : public Sprite
{
protected:
   const int m_iMINSPEED;
   const int m_iMAXSPEED;
   const int m_iHANGTIME;
   BOOL      m_bJumping;
   int       m_iJumpCounter;
   BOOL      m_bLandedSafely;

public:
            MotorcycleSprite( Bitmap* pBitmap, RECT& rcBounds,
                              BOUNDSACTION baBoundsAction = BA_STOP );
   virtual ~MotorcycleSprite( );

   virtual void UpdateFrame( );

   void IncreaseSpeed( );
   void DecreaseSpeed( );
   void StartJumping( );
   void LandedSafely( );
   BOOL HasLandedSafely( ) { return m_bLandedSafely; };
};
