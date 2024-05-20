#pragma once

#include <windows.h>
#include "Sprite.hpp"
#include "externs.h"

class AlienSprite : public Sprite
{
public:
            AlienSprite( Bitmap*, RECT&, BOUNDSACTION = BA_STOP );
   virtual ~AlienSprite( );

   virtual SPRITEACTION Update( );
   virtual Sprite* AddSprite( );
};
