#include "Roids.h"

BOOL GameInitialize(HINSTANCE hInstance)
{
   g_pGame = new GameEngine(hInstance, TEXT("Roids 2"), TEXT("Roids 2: Teaching Games to Think"),
                            IDI_ROIDS, IDI_ROIDS_SM, 500, 400);

   if ( g_pGame == NULL )
   {
      return FALSE;
   }

   g_pGame->SetFrameRate(30);

   g_hInstance = hInstance;

   return TRUE;
}

void GameStart(HWND hWindow)
{
   srand(GetTickCount( ));

   g_hOffscreenDC = CreateCompatibleDC(GetDC(hWindow));
   g_hOffscreenBitmap = CreateCompatibleBitmap(GetDC(hWindow),
                                               g_pGame->GetWidth( ), g_pGame->GetHeight( ));

   SelectObject(g_hOffscreenDC, g_hOffscreenBitmap);

   HDC hDC = GetDC(hWindow);

   g_pAsteroidBitmap = new Bitmap(hDC, IDB_ASTEROID, g_hInstance);
   g_pSaucerBitmap = new Bitmap(hDC, IDB_SAUCER, g_hInstance);

   g_pBackground = new StarryBackground(500, 400);

   RECT    rcBounds = { 0, 0, 500, 400 };

   g_pAsteroids[ 0 ] = new Sprite(g_pAsteroidBitmap, rcBounds, BA_WRAP);
   g_pAsteroids[ 0 ]->SetNumFrames(14);
   g_pAsteroids[ 0 ]->SetFrameDelay(1);
   g_pAsteroids[ 0 ]->SetPosition(250, 200);
   g_pAsteroids[ 0 ]->SetVelocity(-3, 1);
   g_pGame->AddSprite(g_pAsteroids[ 0 ]);

   g_pAsteroids[ 1 ] = new Sprite(g_pAsteroidBitmap, rcBounds, BA_WRAP);
   g_pAsteroids[ 1 ]->SetNumFrames(14);
   g_pAsteroids[ 1 ]->SetFrameDelay(2);
   g_pAsteroids[ 1 ]->SetPosition(250, 200);
   g_pAsteroids[ 1 ]->SetVelocity(3, -2);
   g_pGame->AddSprite(g_pAsteroids[ 1 ]);

   g_pAsteroids[ 2 ] = new Sprite(g_pAsteroidBitmap, rcBounds, BA_WRAP);
   g_pAsteroids[ 2 ]->SetNumFrames(14);
   g_pAsteroids[ 2 ]->SetFrameDelay(3);
   g_pAsteroids[ 2 ]->SetPosition(250, 200);
   g_pAsteroids[ 2 ]->SetVelocity(-2, -4);
   g_pGame->AddSprite(g_pAsteroids[ 2 ]);

   g_pSaucer = new Sprite(g_pSaucerBitmap, rcBounds, BA_WRAP);
   g_pSaucer->SetPosition(0, 0);
   g_pSaucer->SetVelocity(3, 1);
   g_pGame->AddSprite(g_pSaucer);
}

void GameEnd( )
{
   DeleteObject(g_hOffscreenBitmap);
   DeleteDC(g_hOffscreenDC);

   delete g_pAsteroidBitmap;
   delete g_pSaucerBitmap;

   delete g_pBackground;

   g_pGame->CleanupSprites( );

   delete g_pGame;
}

void GameActivate(HWND hWindow)
{ }

void GameDeactivate(HWND hWindow)
{ }

void GamePaint(HDC hDC)
{
   g_pBackground->Draw(hDC);

   g_pGame->DrawSprites(hDC);
}

void GameCycle( )
{
   g_pBackground->Update( );

   g_pGame->UpdateSprites( );

   UpdateSaucer( );

   HWND  hWindow = g_pGame->GetWindow( );
   HDC   hDC = GetDC(hWindow);

   GamePaint(g_hOffscreenDC);

   BitBlt(hDC, 0, 0, g_pGame->GetWidth( ), g_pGame->GetHeight( ),
          g_hOffscreenDC, 0, 0, SRCCOPY);

   ReleaseDC(hWindow, hDC);
}

void HandleKeys( )
{ }

void MouseButtonDown(int x, int y, BOOL bLeft)
{ }

void MouseButtonUp(int x, int y, BOOL bLeft)
{ }

void MouseMove(int x, int y)
{ }

void HandleJoystick(JOYSTATE jsJoystickState)
{ }

BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee)
{
   return FALSE;
}

void SpriteDying(Sprite* pSpriteDying)
{ }

void UpdateSaucer( )
{
   // obtain the saucer's position
   RECT rcSaucer, rcRoid;
   rcSaucer = g_pSaucer->GetPosition( );

   // find out which asteroid is closest to the saucer
   int iXCollision = 500, iYCollision = 400, iXYCollision = 900;

   for ( int i = 0; i < 3; i++ )
   {
      // Get the asteroid position
      rcRoid = g_pAsteroids[ i ]->GetPosition( );

      // Calculate the minimum XY collision distance
      int iXCollisionDist = (rcSaucer.left +
                            (rcSaucer.right - rcSaucer.left) / 2) -
                            (rcRoid.left +
                            (rcRoid.right - rcRoid.left) / 2);

      int iYCollisionDist = (rcSaucer.top +
                            (rcSaucer.bottom - rcSaucer.top) / 2) -
                            (rcRoid.top +
                            (rcRoid.bottom - rcRoid.top) / 2);

      if ( (abs(iXCollisionDist) < abs(iXCollision)) ||
          (abs(iYCollisionDist) < abs(iYCollision)) )
      {
         if ( (abs(iXCollisionDist) + abs(iYCollisionDist)) < iXYCollision )
         {
            iXYCollision = abs(iXCollision) + abs(iYCollision);
            iXCollision = iXCollisionDist;
            iYCollision = iYCollisionDist;
         }
      }
   }

   // Move to dodge the asteroids, if necessary
   POINT ptVelocity;

   ptVelocity = g_pSaucer->GetVelocity( );

   if ( abs(iXCollision) < 60 )
   {
      // Adjust the X velocity
      if ( iXCollision < 0 )
      {
         ptVelocity.x = max(ptVelocity.x - 1, -8);
      }
      else
      {
         ptVelocity.x = min(ptVelocity.x + 1, 8);
      }
   }

   if ( abs(iYCollision) < 60 )
   {
      // Adjust the Y velocity
      if ( iYCollision < 0 )
      {
         ptVelocity.y = max(ptVelocity.y - 1, -8);
      }
      else
      {
         ptVelocity.y = min(ptVelocity.y + 1, 8);
      }
   }

   // Update the saucer to the new position
   g_pSaucer->SetVelocity(ptVelocity);
}