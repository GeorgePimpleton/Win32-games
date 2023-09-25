#include "PersonSprite.hpp"

PersonSprite::PersonSprite(Bitmap* bitmap, RECT& rcBounds, BOUNDSACTION baBoundsAction)
   : Sprite(bitmap, rcBounds, baBoundsAction)
{ }

PersonSprite::~PersonSprite( )
{ }

inline void PersonSprite::UpdateFrame( )
{ }

void PersonSprite::Walk( )
{
   // toggle between the two frames to give the illusion of walking
   m_iCurFrame = 1 - m_iCurFrame;
}
