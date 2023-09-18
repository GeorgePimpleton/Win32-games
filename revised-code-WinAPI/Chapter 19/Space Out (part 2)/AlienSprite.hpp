#pragma once

#include <windows.h>
#include "Sprite.hpp"

class AlienSprite : public Sprite
{
public:
            AlienSprite(Bitmap* bitmap, RECT& rcBounds,
                        BOUNDSACTION baBoundsAction = BA_STOP);
   virtual ~AlienSprite( );

   virtual SPRITEACTION Update( );
   virtual Sprite*      AddSprite( );
};
