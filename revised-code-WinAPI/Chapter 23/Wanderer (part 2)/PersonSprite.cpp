#include "PersonSprite.hpp"

PersonSprite::PersonSprite(Bitmap* bitmap, RECT& bounds, BOUNDSACTION boundsAction)
   : Sprite(bitmap, bounds, boundsAction)
{ }

PersonSprite::~PersonSprite( )
{ }

inline void PersonSprite::UpdateFrame( )
{ }

void PersonSprite::Walk( )
{
   // toggle between the two frames to give the illusion of walking
   m_curFrame = 1 - m_curFrame;
}
