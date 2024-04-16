#include "PersonSprite.h"

PersonSprite::PersonSprite( Bitmap* pBitmap, RECT& rcBounds, BOUNDSACTION baBoundsAction )
   : Sprite( pBitmap, rcBounds, baBoundsAction )
{ }

PersonSprite::~PersonSprite( )
{ }

inline void PersonSprite::UpdateFrame( )
{ }

void PersonSprite::Walk( )
{
   m_iCurFrame = 1 - m_iCurFrame;
}
