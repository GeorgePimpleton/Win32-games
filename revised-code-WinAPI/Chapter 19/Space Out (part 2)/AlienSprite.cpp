#include "AlienSprite.hpp"

extern Bitmap* g_pBlobboBitmap;
extern Bitmap* g_pBMissileBitmap;
extern Bitmap* g_pJellyBitmap;
extern Bitmap* g_pJMissileBitmap;
extern Bitmap* g_pTimmyBitmap;
extern Bitmap* g_pTMissileBitmap;
extern int     g_iDifficulty;

AlienSprite::AlienSprite(Bitmap* bitmap, RECT& bounds,
                         BOUNDSACTION boundsAction)
   : Sprite(bitmap, bounds, boundsAction)
{ }

AlienSprite::~AlienSprite( )
{ }

SPRITEACTION AlienSprite::Update( )
{
   SPRITEACTION saSpriteAction = Sprite::Update( );

   if ( (rtk::rand(0, (g_iDifficulty / 2))) == 0 )
   {
      saSpriteAction |= SA_ADDSPRITE;
   }

   return saSpriteAction;
}

Sprite* AlienSprite::AddSprite( )
{
   RECT    bounds = { 0, 0, 640, 410 };
   RECT    pos    = GetPosition( );
   Sprite* sprite = NULL;

   if ( GetBitmap( ) == g_pBlobboBitmap )
   {
      // Blobbo missile
      sprite = new Sprite(g_pBMissileBitmap, bounds, BA_DIE);
      sprite->SetVelocity(0, 7);
   }
   else if ( GetBitmap( ) == g_pJellyBitmap )
   {
      // Jelly missile
      sprite = new Sprite(g_pJMissileBitmap, bounds, BA_DIE);
      sprite->SetVelocity(0, 5);
   }
   else
   {
      // Timmy missile
      sprite = new Sprite(g_pTMissileBitmap, bounds, BA_DIE);
      sprite->SetVelocity(0, 3);
   }

   sprite->SetPosition(pos.left + (GetWidth( ) / 2), pos.bottom);
   return sprite;
}