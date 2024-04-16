#include "PersonSprite.hpp"

PersonSprite::PersonSprite( Bitmap* bitmap, RECT& bounds, BOUNDSACTION boundsAction )
   : Sprite( bitmap, bounds, boundsAction )
{ }

PersonSprite::~PersonSprite( )
{ }

inline void PersonSprite::UpdateFrame( )
{ }

void PersonSprite::Walk( )
{
   m_curFrame = 1 - m_curFrame;
}
