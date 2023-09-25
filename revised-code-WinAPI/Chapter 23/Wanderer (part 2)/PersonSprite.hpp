#pragma once

#include <windows.h>
#include "Sprite.hpp"

class PersonSprite : public Sprite
{
public:
            PersonSprite(Bitmap* pBitmap, RECT& rcBounds,
                         BOUNDSACTION baBoundsAction = BA_STOP);
   virtual ~PersonSprite( );

   virtual void UpdateFrame( );

   void Walk( );
};
