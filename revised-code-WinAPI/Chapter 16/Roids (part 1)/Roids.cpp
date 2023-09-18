#include "Roids.h"

BOOL GameInitialize(HINSTANCE hInstance)
{
   g_pGame = new GameEngine(hInstance, TEXT("Roids"), TEXT("Roids: Creating Backgrounds for Your Sprites"),
                            IDI_ROIDS, IDI_ROIDS_SM, 500, 400);
   if ( g_pGame == NULL )
      return FALSE;

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

   g_pBackground = new StarryBackground(500, 400);

   RECT    rcBounds = { 0, 0, 500, 400 };
   Sprite* pSprite;
   pSprite = new Sprite(g_pAsteroidBitmap, rcBounds, BA_WRAP);
   pSprite->SetNumFrames(14);
   pSprite->SetFrameDelay(1);
   pSprite->SetPosition(250, 200);
   pSprite->SetVelocity(-3, 1);
   g_pGame->AddSprite(pSprite);
   pSprite = new Sprite(g_pAsteroidBitmap, rcBounds, BA_WRAP);
   pSprite->SetNumFrames(14);
   pSprite->SetFrameDelay(2);
   pSprite->SetPosition(250, 200);
   pSprite->SetVelocity(3, -2);
   g_pGame->AddSprite(pSprite);
   pSprite = new Sprite(g_pAsteroidBitmap, rcBounds, BA_WRAP);
   pSprite->SetNumFrames(14);
   pSprite->SetFrameDelay(3);
   pSprite->SetPosition(250, 200);
   pSprite->SetVelocity(-2, -4);
   g_pGame->AddSprite(pSprite);
}

void GameEnd( )
{
   DeleteObject(g_hOffscreenBitmap);
   DeleteDC(g_hOffscreenDC);

   // Cleanup the asteroid bitmap
   delete g_pAsteroidBitmap;

   // Cleanup the background
   delete g_pBackground;

   // Cleanup the sprites
   g_pGame->CleanupSprites( );

   // Cleanup the game engine
   delete g_pGame;
}

void GameActivate(HWND hWindow)
{ }

void GameDeactivate(HWND hWindow)
{ }

void GamePaint(HDC hDC)
{
   // draw the background
   g_pBackground->Draw(hDC);

   // draw the sprites
   g_pGame->DrawSprites(hDC);
}

void GameCycle( )
{
   // update the background
   g_pBackground->Update( );

   // update the sprites
   g_pGame->UpdateSprites( );

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
