#include "Henway.h"

BOOL GameInitialize(HINSTANCE hInstance)
{
   _pGame = new GameEngine(hInstance, TEXT("Henway 2"), TEXT("Henway 2"),
                           IDI_HENWAY, IDI_HENWAY_SM, 465, 400);
   if ( _pGame == NULL )
      return FALSE;

   _pGame->SetFrameRate(30);

   _pGame->InitJoystick( );

   _hInstance = hInstance;

   return TRUE;
}

void GameStart(HWND hWindow)
{
   srand(GetTickCount( ));

   _hOffscreenDC = CreateCompatibleDC(GetDC(hWindow));
   _hOffscreenBitmap = CreateCompatibleBitmap(GetDC(hWindow), _pGame->GetWidth( ), _pGame->GetHeight( ));
   SelectObject(_hOffscreenDC, _hOffscreenBitmap);

   HDC hDC = GetDC(hWindow);
   _pHighwayBitmap = new Bitmap(hDC, IDB_HIGHWAY, _hInstance);
   _pChickenBitmap = new Bitmap(hDC, IDB_CHICKEN, _hInstance);
   _pCarBitmaps[ 0 ] = new Bitmap(hDC, IDB_CAR1, _hInstance);
   _pCarBitmaps[ 1 ] = new Bitmap(hDC, IDB_CAR2, _hInstance);
   _pCarBitmaps[ 2 ] = new Bitmap(hDC, IDB_CAR3, _hInstance);
   _pCarBitmaps[ 3 ] = new Bitmap(hDC, IDB_CAR4, _hInstance);
   _pChickenHeadBitmap = new Bitmap(hDC, IDB_CHICKENHEAD, _hInstance);

   Sprite* pSprite;
   RECT    rcBounds = { 0, 0, 465, 400 };
   _pChickenSprite = new Sprite(_pChickenBitmap, rcBounds, BA_STOP);
   _pChickenSprite->SetPosition(4, 175);
   _pChickenSprite->SetVelocity(0, 0);
   _pChickenSprite->SetZOrder(1);
   _pGame->AddSprite(_pChickenSprite);
   pSprite = new Sprite(_pCarBitmaps[ 0 ], rcBounds, BA_WRAP);
   pSprite->SetPosition(70, 0);
   pSprite->SetVelocity(0, 7);
   pSprite->SetZOrder(2);
   _pGame->AddSprite(pSprite);
   pSprite = new Sprite(_pCarBitmaps[ 1 ], rcBounds, BA_WRAP);
   pSprite->SetPosition(160, 0);
   pSprite->SetVelocity(0, 3);
   pSprite->SetZOrder(2);
   _pGame->AddSprite(pSprite);
   pSprite = new Sprite(_pCarBitmaps[ 2 ], rcBounds, BA_WRAP);
   pSprite->SetPosition(239, 400);
   pSprite->SetVelocity(0, -5);
   pSprite->SetZOrder(2);
   _pGame->AddSprite(pSprite);
   pSprite = new Sprite(_pCarBitmaps[ 3 ], rcBounds, BA_WRAP);
   pSprite->SetPosition(329, 400);
   pSprite->SetVelocity(0, -10);
   pSprite->SetZOrder(2);
   _pGame->AddSprite(pSprite);

   _iInputDelay = 0;
   _iNumLives = 3;
   _iScore = 0;
   _bGameOver = FALSE;

   _pGame->PlayMIDISong(TEXT("Music.mid"));
}

void GameEnd( )
{
   _pGame->CloseMIDIPlayer( );

   DeleteObject(_hOffscreenBitmap);
   DeleteDC(_hOffscreenDC);

   // Cleanup the bitmaps
   delete _pHighwayBitmap;
   delete _pChickenBitmap;
   for ( int i = 0; i < 4; i++ )
      delete _pCarBitmaps[ i ];
   delete _pChickenHeadBitmap;

   _pGame->CleanupSprites( );

   delete _pGame;
}

void GameActivate(HWND hWindow)
{
   _pGame->CaptureJoystick( );

   _pGame->PlayMIDISong(TEXT(""), FALSE);
}

void GameDeactivate(HWND hWindow)
{
   _pGame->ReleaseJoystick( );

   _pGame->PauseMIDISong( );
}

void GamePaint(HDC hDC)
{
   _pHighwayBitmap->Draw(hDC, 0, 0);

   _pGame->DrawSprites(hDC);

   for ( int i = 0; i < _iNumLives; i++ )
      _pChickenHeadBitmap->Draw(hDC,
                                406 + (_pChickenHeadBitmap->GetWidth( ) * i), 382, TRUE);
}

void GameCycle( )
{
   if ( !_bGameOver )
   {
      if ( rand( ) % 100 == 0 )
         if ( rand( ) % 2 == 0 )
            PlaySound((PCTSTR) IDW_CARHORN1, _hInstance, SND_ASYNC | SND_RESOURCE);
         else
            PlaySound((PCTSTR) IDW_CARHORN2, _hInstance, SND_ASYNC | SND_RESOURCE);

      _pGame->UpdateSprites( );

      HWND  hWindow = _pGame->GetWindow( );
      HDC   hDC = GetDC(hWindow);

      GamePaint(_hOffscreenDC);

      BitBlt(hDC, 0, 0, _pGame->GetWidth( ), _pGame->GetHeight( ),
             _hOffscreenDC, 0, 0, SRCCOPY);

      ReleaseDC(hWindow, hDC);
   }
}

void HandleKeys( )
{
   if ( !_bGameOver && (++_iInputDelay > 2) )
   {
      if ( GetAsyncKeyState(VK_LEFT) < 0 )
         MoveChicken(-20, 0);
      else if ( GetAsyncKeyState(VK_RIGHT) < 0 )
         MoveChicken(20, 0);
      if ( GetAsyncKeyState(VK_UP) < 0 )
         MoveChicken(0, -20);
      else if ( GetAsyncKeyState(VK_DOWN) < 0 )
         MoveChicken(0, 20);

      _iInputDelay = 0;
   }
}

void MouseButtonDown(int x, int y, BOOL bLeft)
{
   if ( _bGameOver )
   {
      _pGame->PlayMIDISong( );

      _iNumLives = 3;
      _iScore = 0;
      _bGameOver = FALSE;
   }
}

void MouseButtonUp(int x, int y, BOOL bLeft)
{ }

void MouseMove(int x, int y)
{ }

void HandleJoystick(JOYSTATE jsJoystickState)
{
   if ( !_bGameOver && (++_iInputDelay > 2) )
   {
      if ( jsJoystickState & JOY_LEFT )
         MoveChicken(-20, 0);
      else if ( jsJoystickState & JOY_RIGHT )
         MoveChicken(20, 0);

      if ( jsJoystickState & JOY_UP )
         MoveChicken(0, -20);
      else if ( jsJoystickState & JOY_DOWN )
         MoveChicken(0, 20);

      _iInputDelay = 0;
   }

   if ( _bGameOver && (jsJoystickState & JOY_FIRE1) )
   {
      _pGame->PlayMIDISong( );

      _iNumLives = 3;
      _iScore = 0;
      _bGameOver = FALSE;
   }
}

BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee)
{
   if ( pSpriteHittee == _pChickenSprite )
   {
      _pChickenSprite->SetPosition(4, 175);

      if ( --_iNumLives > 0 )
         PlaySound((PCTSTR) IDW_SQUISH, _hInstance, SND_ASYNC | SND_RESOURCE);
      else
      {
         PlaySound((PCTSTR) IDW_GAMEOVER, _hInstance, SND_ASYNC | SND_RESOURCE);

         TCHAR szText[ 64 ];
         wsprintf(szText, TEXT("Game Over! You scored %d points."), _iScore);
         MessageBox(_pGame->GetWindow( ), szText, TEXT("Henway 2"), MB_OK);
         _bGameOver = TRUE;

         _pGame->PauseMIDISong( );
      }

      return FALSE;
   }

   return TRUE;
}

void MoveChicken(int iXDistance, int iYDistance)
{
   _pChickenSprite->OffsetPosition(iXDistance, iYDistance);

   if ( _pChickenSprite->GetPosition( ).left > 400 )
   {
      PlaySound((PCTSTR) IDW_CELEBRATE, _hInstance, SND_ASYNC | SND_RESOURCE);

      _pChickenSprite->SetPosition(4, 175);
      _iScore += 150;
   }
}