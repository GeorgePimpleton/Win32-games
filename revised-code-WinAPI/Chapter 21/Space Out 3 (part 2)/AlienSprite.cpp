#include "AlienSprite.hpp"

std::unique_ptr<Bitmap> g_BlobboBitmap;
std::unique_ptr<Bitmap> g_BMissileBitmap;
std::unique_ptr<Bitmap> g_JellyBitmap;
std::unique_ptr<Bitmap> g_JMissileBitmap;
std::unique_ptr<Bitmap> g_TimmyBitmap;
std::unique_ptr<Bitmap> g_TMissileBitmap;
int                     g_difficulty;

AlienSprite::AlienSprite(Bitmap* bitmap, RECT& bounds, BOUNDSACTION boundsAction)
   : Sprite(bitmap, bounds, boundsAction)
{ }

AlienSprite::~AlienSprite( )
{ }

SPRITEACTION AlienSprite::Update( )
{
   SPRITEACTION spriteAction = Sprite::Update( );

   if ( (rtk::rand(0, g_difficulty / 2)) == 0 )
   {
      spriteAction |= SA_ADDSPRITE;
   }

   return spriteAction;
}

Sprite* AlienSprite::AddSprite( )
{
   RECT    bounds = { 0, 0, 640, 410 };
   RECT    pos    = GetPosition( );
   Sprite* sprite = NULL;

   if ( GetBitmap( ) == g_BlobboBitmap.get( ) )
   {
      // Blobbo missile
      sprite = new Sprite(g_BMissileBitmap.get( ), bounds, BA_DIE);
      sprite->SetVelocity(0, 7);
   }
   else if ( GetBitmap( ) == g_JellyBitmap.get( ) )
   {
      // Jelly missile
      sprite = new Sprite(g_JMissileBitmap.get( ), bounds, BA_DIE);
      sprite->SetVelocity(0, 5);
   }
   else
   {
      // Timmy missile
      sprite = new Sprite(g_TMissileBitmap.get( ), bounds, BA_DIE);
      sprite->SetVelocity(0, 3);
   }

   // Set the missile sprite's position and return it
   sprite->SetPosition(pos.left + (GetWidth( ) / 2), pos.bottom);
   return sprite;
}