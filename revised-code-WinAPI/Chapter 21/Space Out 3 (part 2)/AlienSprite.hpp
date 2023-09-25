#pragma once

#include <windows.h>
#include "random_toolkit.hpp"
#include "Sprite.hpp"
#include "externs.h"

class AlienSprite : public Sprite
{
public:
            AlienSprite(Bitmap* bitmap, RECT& bounds,
                        BOUNDSACTION boundsAction = BA_STOP);
   virtual ~AlienSprite( );

   virtual SPRITEACTION Update( );
   virtual Sprite*      AddSprite( );
};
