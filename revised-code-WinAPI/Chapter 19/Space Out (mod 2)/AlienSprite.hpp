#pragma once

#include <windows.h>
#include <memory>
#include "random_toolkit.hpp"
#include "Sprite.hpp"

class AlienSprite : public Sprite
{
public:
            AlienSprite(Bitmap* bitmap, RECT& bounds,
                        BOUNDSACTION boundsAction = BA_STOP);
   virtual ~AlienSprite( );

   virtual SPRITEACTION Update( );
   virtual Sprite*      AddSprite( );
};
