#pragma once

#include <windows.h>
#include "Sprite.hpp"

class PersonSprite : public Sprite
{
public:
            PersonSprite(Bitmap* bitmap, RECT& bounds,
                         BOUNDSACTION boundsAction = BA_STOP);
   virtual ~PersonSprite( );

   virtual void UpdateFrame( );

   void Walk( );
};
