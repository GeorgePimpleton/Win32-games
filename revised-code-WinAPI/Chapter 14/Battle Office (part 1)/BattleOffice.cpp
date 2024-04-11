#include "BattleOffice.h"

BOOL GameInitialize(HINSTANCE hInstance)
{
   _pGame = new GameEngine(hInstance, TEXT("Battle Office"), TEXT("Battle Office"),
                           IDI_BATTLEOFFICE, IDI_BATTLEOFFICE_SM, 500, 400);
   if ( _pGame == NULL )
      return FALSE;

   _pGame->SetFrameRate(30);

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
   _pOfficeBitmap = new Bitmap(hDC, IDB_OFFICE, _hInstance);
   _pTargetBitmap = new Bitmap(hDC, IDB_TARGET, _hInstance);
   _pPowBitmap = new Bitmap(hDC, IDB_POW, _hInstance);
   _pGuyBitmaps[ 0 ] = new Bitmap(hDC, IDB_GUY1, _hInstance);
   _pGuyBitmaps[ 1 ] = new Bitmap(hDC, IDB_GUY2, _hInstance);
   _pGuyBitmaps[ 2 ] = new Bitmap(hDC, IDB_GUY3, _hInstance);
   _pGuyBitmaps[ 3 ] = new Bitmap(hDC, IDB_GUY4, _hInstance);
   _pGuyBitmaps[ 4 ] = new Bitmap(hDC, IDB_GUY5, _hInstance);
   _pSmallGuyBitmap = new Bitmap(hDC, IDB_SMALLGUY, _hInstance);
   _pGameOverBitmap = new Bitmap(hDC, IDB_GAMEOVER, _hInstance);

   RECT    rcBounds = { 0, 0, 500, 400 };
   _pTargetSprite = new Sprite(_pTargetBitmap, rcBounds, BA_STOP);
   _pTargetSprite->SetZOrder(4);
   _pGame->AddSprite(_pTargetSprite);
   _pPowSprite = new Sprite(_pPowBitmap, rcBounds, BA_STOP);
   _pPowSprite->SetZOrder(3);
   _pPowSprite->SetHidden(TRUE);
   _pGame->AddSprite(_pPowSprite);
   _pGuySprites[ 0 ] = new Sprite(_pGuyBitmaps[ 0 ], rcBounds);
   _pGuySprites[ 0 ]->SetPosition(92, 175);
   _pGuySprites[ 0 ]->SetZOrder(2);
   _pGuySprites[ 0 ]->SetHidden(TRUE);
   _pGame->AddSprite(_pGuySprites[ 0 ]);
   _pGuySprites[ 1 ] = new Sprite(_pGuyBitmaps[ 1 ], rcBounds);
   _pGuySprites[ 1 ]->SetPosition(301, 184);
   _pGuySprites[ 1 ]->SetZOrder(2);
   _pGuySprites[ 1 ]->SetHidden(TRUE);
   _pGame->AddSprite(_pGuySprites[ 1 ]);
   _pGuySprites[ 2 ] = new Sprite(_pGuyBitmaps[ 2 ], rcBounds);
   _pGuySprites[ 2 ]->SetPosition(394, 61);
   _pGuySprites[ 2 ]->SetZOrder(2);
   _pGuySprites[ 2 ]->SetHidden(TRUE);
   _pGame->AddSprite(_pGuySprites[ 2 ]);
   rcBounds.left = 340;
   _pGuySprites[ 3 ] = new Sprite(_pGuyBitmaps[ 3 ], rcBounds, BA_WRAP);
   _pGuySprites[ 3 ]->SetPosition(500, 10);
   _pGuySprites[ 3 ]->SetVelocity(-3, 0);
   _pGuySprites[ 3 ]->SetZOrder(1);
   _pGuySprites[ 3 ]->SetHidden(TRUE);
   _pGame->AddSprite(_pGuySprites[ 3 ]);
   rcBounds.left = 385;
   _pGuySprites[ 4 ] = new Sprite(_pGuyBitmaps[ 4 ], rcBounds, BA_WRAP);
   _pGuySprites[ 4 ]->SetPosition(260, 60);
   _pGuySprites[ 4 ]->SetVelocity(5, 0);
   _pGuySprites[ 4 ]->SetZOrder(1);
   _pGuySprites[ 4 ]->SetHidden(TRUE);
   _pGame->AddSprite(_pGuySprites[ 4 ]);

   _iGuyMasterDelay = 50;
   _iHits = 0;
   _iMisses = 0;
   _bGameOver = FALSE;

   _pGame->PlayMIDISong(TEXT("Music.mid"));
}

void GameEnd( )
{
   _pGame->CloseMIDIPlayer( );

   DeleteObject(_hOffscreenBitmap);
   DeleteDC(_hOffscreenDC);

   delete _pOfficeBitmap;
   delete _pTargetBitmap;
   delete _pPowBitmap;
   for ( int i = 0; i < 5; i++ )
      delete _pGuyBitmaps[ i ];
   delete _pSmallGuyBitmap;
   delete _pGameOverBitmap;

   _pGame->CleanupSprites( );

   delete _pGame;
}

void GameActivate(HWND hWindow)
{
   _pGame->PlayMIDISong(TEXT(""), FALSE);
}

void GameDeactivate(HWND hWindow)
{
   _pGame->PauseMIDISong( );
}

void GamePaint(HDC hDC)
{
   _pOfficeBitmap->Draw(hDC, 0, 0);

   _pGame->DrawSprites(hDC);

  TCHAR szText[ 64 ];
   RECT  rect = { 237, 360, 301, 390 };
   wsprintf(szText, TEXT("%d"), _iHits);
   DrawText(hDC, szText, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

   for ( int i = 0; i < _iMisses; i++ )
      _pSmallGuyBitmap->Draw(hDC, 389 + (_pSmallGuyBitmap->GetWidth( ) * i), 359, TRUE);

   if ( _bGameOver )
      _pGameOverBitmap->Draw(hDC, 120, 110, TRUE);
}

void GameCycle( )
{
   if ( !_bGameOver )
   {
      for ( int i = 0; i < 5; i++ )
         if ( _pGuySprites[ i ]->IsHidden( ) )
         {
            if ( rand( ) % 60 == 0 )
            {
               _pGuySprites[ i ]->SetHidden(FALSE);

               if ( i == 3 )
               {
                  _iGuyDelay[ i ] = 80;
                  _pGuySprites[ i ]->SetPosition(500, 10);
               }
               else if ( i == 4 )
               {
                  _iGuyDelay[ i ] = 45;
                  _pGuySprites[ i ]->SetPosition(260, 60);
               }
               else
                  _iGuyDelay[ i ] = 20 + (rand( ) % _iGuyMasterDelay);
            }
         }
         else
         {
            if ( --_iGuyDelay[ i ] == 0 )
            {
               PlaySound((PCTSTR) IDW_TAUNT, _hInstance, SND_ASYNC | SND_RESOURCE);

               _pGuySprites[ i ]->SetHidden(TRUE);

               if ( ++_iMisses == 5 )
               {
                  PlaySound((PCTSTR) IDW_BOO, _hInstance, SND_ASYNC | SND_RESOURCE);

                  for ( int i = 0; i < 5; i++ )
                     _pGuySprites[ i ]->SetHidden(TRUE);
                  _bGameOver = TRUE;

                  _pGame->PauseMIDISong( );
               }
            }
         }

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
{ }

void MouseButtonDown(int x, int y, BOOL bLeft)
{
   if ( !_bGameOver && bLeft )
   {
      _pTargetSprite->SetHidden(TRUE);
      _pPowSprite->SetHidden(TRUE);

      Sprite* pSprite;
      if ( (pSprite = _pGame->IsPointInSprite(x, y)) != NULL )
      {
         PlaySound((PCTSTR) IDW_WHACK, _hInstance, SND_ASYNC | SND_RESOURCE);

         _pPowSprite->SetPosition(x - (_pPowSprite->GetWidth( ) / 2),
                                  y - (_pPowSprite->GetHeight( ) / 2));
         _pPowSprite->SetHidden(FALSE);

         pSprite->SetHidden(TRUE);

         if ( (++_iHits % 5) == 0 )
            if ( --_iGuyMasterDelay == 0 )
               _iGuyMasterDelay = 1;
      }

      _pTargetSprite->SetHidden(FALSE);
   }
   else if ( _bGameOver && !bLeft )
   {
      _bGameOver = FALSE;
      _iHits = 0;
      _iMisses = 0;

      _pGame->PlayMIDISong( );
   }
}

void MouseButtonUp(int x, int y, BOOL bLeft)
{ }

void MouseMove(int x, int y)
{
   _pTargetSprite->SetPosition(x - (_pTargetSprite->GetWidth( ) / 2),
                               y - (_pTargetSprite->GetHeight( ) / 2));
}

void HandleJoystick(JOYSTATE jsJoystickState)
{ }

BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee)
{
   return FALSE;
}