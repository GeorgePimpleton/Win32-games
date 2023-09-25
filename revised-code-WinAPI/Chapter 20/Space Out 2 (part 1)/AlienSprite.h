#pragma once

#include <windows.h>
#include "Sprite.h"

class AlienSprite : public Sprite
{
public:
            AlienSprite(Bitmap* pBitmap, RECT& rcBounds,
                        BOUNDSACTION baBoundsAction = BA_STOP);
   virtual ~AlienSprite( );

   virtual SPRITEACTION Update( );
   virtual Sprite*      AddSprite( );
};
