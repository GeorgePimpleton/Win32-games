#include "AlienSprite.hpp"

extern Bitmap* g_BlobboBitmap;
extern Bitmap* g_BMissileBitmap;
extern Bitmap* g_JellyBitmap;
extern Bitmap* g_JMissileBitmap;
extern Bitmap* g_TimmyBitmap;
extern Bitmap* g_TMissileBitmap;
extern int     g_difficulty;

AlienSprite::AlienSprite(Bitmap* pBitmap, RECT& rcBounds,
                         BOUNDSACTION baBoundsAction)
   : Sprite(pBitmap, rcBounds, baBoundsAction)
{ }

AlienSprite::~AlienSprite( )
{ }

SPRITEACTION AlienSprite::Update( )
{
   // Call the base sprite Update() method
   SPRITEACTION saSpriteAction;
   saSpriteAction = Sprite::Update( );

   // See if the alien should fire a missile
   if ( (rand( ) % (g_difficulty / 2)) == 0 )
      saSpriteAction |= SA_ADDSPRITE;

   return saSpriteAction;
}

Sprite* AlienSprite::AddSprite( )
{
   // Create a new missile sprite
   RECT    rcBounds = { 0, 0, 640, 410 };
   RECT    rcPos = GetPosition( );
   Sprite* pSprite = NULL;
   if ( GetBitmap( ) == g_BlobboBitmap )
   {
      // Blobbo missile
      pSprite = new Sprite(g_BMissileBitmap, rcBounds, BA_DIE);
      pSprite->SetVelocity(0, 7);
   }
   else if ( GetBitmap( ) == g_JellyBitmap )
   {
      // Jelly missile
      pSprite = new Sprite(g_JMissileBitmap, rcBounds, BA_DIE);
      pSprite->SetVelocity(0, 5);
   }
   else
   {
      // Timmy missile
      pSprite = new Sprite(g_TMissileBitmap, rcBounds, BA_DIE);
      pSprite->SetVelocity(0, 3);
   }

   // Set the missile sprite's position and return it
   pSprite->SetPosition(rcPos.left + (GetWidth( ) / 2), rcPos.bottom);
   return pSprite;
}