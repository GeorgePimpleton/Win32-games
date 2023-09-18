#include "SpaceOut.hpp"

BOOL GameInitialize(HINSTANCE inst)
{
  g_game = new GameEngine(inst, L"Space Out", L"Example Game: Space Out",
                           IDI_ICON, IDI_ICON_SM, 600, 450);

  if ( g_game == NULL )
  {
     return FALSE;
  }

  g_game->SetFrameRate(30);

  g_inst = inst;

  return TRUE;
}

void GameStart(HWND wnd)
{
  srand((UINT)GetTickCount64());

  g_hOffscreenDC = CreateCompatibleDC(GetDC(wnd));
  g_hOffscreenBitmap = CreateCompatibleBitmap(GetDC(wnd),
    g_game->GetWidth(), g_game->GetHeight());

  SelectObject(g_hOffscreenDC, g_hOffscreenBitmap);

  HDC dc = GetDC(wnd);

  g_pDesertBitmap      = new Bitmap(dc, IDB_DESERT, g_inst);
  g_pCarBitmap         = new Bitmap(dc, IDB_CAR, g_inst);
  g_pSmCarBitmap       = new Bitmap(dc, IDB_SMCAR, g_inst);
  g_pMissileBitmap     = new Bitmap(dc, IDB_MISSILE, g_inst);
  g_pBlobboBitmap      = new Bitmap(dc, IDB_BLOBBO, g_inst);
  g_pBMissileBitmap    = new Bitmap(dc, IDB_BMISSILE, g_inst);
  g_pJellyBitmap       = new Bitmap(dc, IDB_JELLY, g_inst);
  g_pJMissileBitmap    = new Bitmap(dc, IDB_JMISSILE, g_inst);
  g_pTimmyBitmap       = new Bitmap(dc, IDB_TIMMY, g_inst);
  g_pTMissileBitmap    = new Bitmap(dc, IDB_TMISSILE, g_inst);
  g_pSmExplosionBitmap = new Bitmap(dc, IDB_SMEXPLOSION, g_inst);
  g_pLgExplosionBitmap = new Bitmap(dc, IDB_LGEXPLOSION, g_inst);
  g_gameOverBitmap    = new Bitmap(dc, IDB_GAMEOVER, g_inst);

  g_pBackground = new StarryBackground(600, 450);

  g_game->PlayMIDISong(L"Music.mid");

  NewGame();
}

void GameEnd()
{
  g_game->CloseMIDIPlayer();

  DeleteObject(g_hOffscreenBitmap);
  DeleteDC(g_hOffscreenDC);

  delete g_pDesertBitmap;
  delete g_pCarBitmap;
  delete g_pSmCarBitmap;
  delete g_pMissileBitmap;
  delete g_pBlobboBitmap;
  delete g_pBMissileBitmap;
  delete g_pJellyBitmap;
  delete g_pJMissileBitmap;
  delete g_pTimmyBitmap;
  delete g_pTMissileBitmap;
  delete g_pSmExplosionBitmap;
  delete g_pLgExplosionBitmap;
  delete g_gameOverBitmap;

  delete g_pBackground;

  g_game->CleanupSprites();

  delete g_game;
}

void GameActivate(HWND wnd)
{
  g_game->PlayMIDISong(L"", FALSE);
}

void GameDeactivate(HWND wnd)
{
  g_game->PauseMIDISong();
}

void GamePaint(HDC dc)
{
  g_pBackground->Draw(dc);

  g_pDesertBitmap->Draw(dc, 0, 371);

  g_game->DrawSprites(dc);

  TCHAR szText[64];
  RECT  rect = { 460, 0, 510, 30 };

  wsprintf(szText, L"%d", g_iScore);

  SetBkMode(dc, TRANSPARENT);
  SetTextColor(dc, RGB(255, 255, 255));

  DrawText(dc, szText, -1, &rect, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

  for ( int i = 0; i < g_iNumLives; i++ )
  {
     g_pSmCarBitmap->Draw(dc, 520 + (g_pSmCarBitmap->GetWidth( ) * i),
                          10, TRUE);
  }

  if ( g_bGameOver )
  {
     g_gameOverBitmap->Draw(dc, 190, 149, TRUE);
  }
}

void GameCycle()
{
  if (!g_bGameOver)
  {
     if ( (rand( ) % g_iDifficulty) == 0 )
     {
        AddAlien( );
     }

    g_pBackground->Update();

    g_game->UpdateSprites();

    HWND  wnd = g_game->GetWindow();
    HDC   dc = GetDC(wnd);

    GamePaint(g_hOffscreenDC);

    BitBlt(dc, 0, 0, g_game->GetWidth(), g_game->GetHeight(),
      g_hOffscreenDC, 0, 0, SRCCOPY);

    ReleaseDC(wnd, dc);
  }
}

void GameMenu(WPARAM wParam)
{
   switch ( LOWORD(wParam) )
   {
   case IDM_GAME_NEW:
      NewGame( );
      return;

   case IDM_GAME_EXIT:
      GameEnd( );
      PostQuitMessage(0);
      return;

   case IDM_HELP_ABOUT:
      DialogBoxW(g_game->GetInstance( ), MAKEINTRESOURCEW(IDD_ABOUT), g_game->GetWindow( ), (DLGPROC) DlgProc);
      return;
   }
}

void HandleKeys()
{
  if (!g_bGameOver)
  {
    POINT velocity = g_pCarSprite->GetVelocity();

    if (GetAsyncKeyState(VK_LEFT) < 0)
    {
      velocity.x = max(velocity.x - 1, -4);
      g_pCarSprite->SetVelocity(velocity);
    }
    else if (GetAsyncKeyState(VK_RIGHT) < 0)
    {
      velocity.x = min(velocity.x + 2, 6);
      g_pCarSprite->SetVelocity(velocity);
    }

    if ((++g_iFireInputDelay > 6) && GetAsyncKeyState(VK_SPACE) < 0)
    {
      RECT  bounds = { 0, 0, 600, 450 };
      RECT  rcPos = g_pCarSprite->GetPosition();
      Sprite* pSprite = new Sprite(g_pMissileBitmap, bounds, BA_DIE);
      pSprite->SetPosition(rcPos.left + 15, 400);
      pSprite->SetVelocity(0, -7);
      g_game->AddSprite(pSprite);

      PlaySound((PCWSTR)IDW_MISSILE, g_inst, SND_ASYNC |
        SND_RESOURCE | SND_NOSTOP);

      g_iFireInputDelay = 0;
    }
  }

  if ( g_bGameOver && (GetAsyncKeyState(VK_RETURN) < 0) )
  {
     NewGame( );
  }
}

void MouseButtonDown(int x, int y, BOOL left)
{ }

void MouseButtonUp(int x, int y, BOOL left)
{ }

void MouseMove(int x, int y)
{ }

void HandleJoystick(JOYSTATE joyState)
{ }

BOOL SpriteCollision(Sprite* spriteHitter, Sprite* spriteHittee)
{
  Bitmap* pHitter = spriteHitter->GetBitmap();
  Bitmap* pHittee = spriteHittee->GetBitmap();

  if ((pHitter == g_pMissileBitmap && (pHittee == g_pBlobboBitmap ||
    pHittee == g_pJellyBitmap || pHittee == g_pTimmyBitmap)) ||
    (pHittee == g_pMissileBitmap && (pHitter == g_pBlobboBitmap ||
    pHitter == g_pJellyBitmap || pHitter == g_pTimmyBitmap)))
  {
    PlaySound((PCWSTR)IDW_LGEXPLODE, g_inst, SND_ASYNC |
      SND_RESOURCE);

    spriteHitter->Kill();
    spriteHittee->Kill();

    RECT bounds = { 0, 0, 600, 450 };
    RECT rcPos;

    if ( pHitter == g_pMissileBitmap )
    {
       rcPos = spriteHittee->GetPosition( );
    }
    else
    {
       rcPos = spriteHitter->GetPosition( );
    }

    Sprite* pSprite = new Sprite(g_pLgExplosionBitmap, bounds);
    pSprite->SetNumFrames(8, TRUE);
    pSprite->SetPosition(rcPos.left, rcPos.top);
    g_game->AddSprite(pSprite);

    g_iScore += 25;
    g_iDifficulty = max(80 - (g_iScore / 20), 20);
  }

  if ((pHitter == g_pCarBitmap && (pHittee == g_pBMissileBitmap ||
    pHittee == g_pJMissileBitmap || pHittee == g_pTMissileBitmap)) ||
    (pHittee == g_pCarBitmap && (pHitter == g_pBMissileBitmap ||
    pHitter == g_pJMissileBitmap || pHitter == g_pTMissileBitmap)))
  {
    PlaySound((PCWSTR)IDW_LGEXPLODE, g_inst, SND_ASYNC |
      SND_RESOURCE);

    if ( pHitter == g_pCarBitmap )
    {
       spriteHittee->Kill( );
    }
    else
    {
       spriteHitter->Kill( );
    }

    RECT bounds = { 0, 0, 600, 480 };
    RECT rcPos;

    if ( pHitter == g_pCarBitmap )
    {
       rcPos = spriteHitter->GetPosition( );
    }
    else
    {
       rcPos = spriteHittee->GetPosition( );
    }

    Sprite* pSprite = new Sprite(g_pLgExplosionBitmap, bounds);
    pSprite->SetNumFrames(8, TRUE);
    pSprite->SetPosition(rcPos.left, rcPos.top);
    g_game->AddSprite(pSprite);

    g_pCarSprite->SetPosition(300, 405);

    if (--g_iNumLives == 0)
    {
      PlaySound((PCWSTR)IDW_GAMEOVER, g_inst, SND_ASYNC |
        SND_RESOURCE);
      g_bGameOver = TRUE;
    }
  }

  return FALSE;
}

void SpriteDying(Sprite* spriteDying)
{
  if (spriteDying->GetBitmap() == g_pBMissileBitmap ||
    spriteDying->GetBitmap() == g_pJMissileBitmap ||
    spriteDying->GetBitmap() == g_pTMissileBitmap)
  {
    PlaySound((PCWSTR)IDW_SMEXPLODE, g_inst, SND_ASYNC |
      SND_RESOURCE | SND_NOSTOP);

    RECT bounds = { 0, 0, 600, 450 };
    RECT rcPos = spriteDying->GetPosition();
    Sprite* pSprite = new Sprite(g_pSmExplosionBitmap, bounds);
    pSprite->SetNumFrames(8, TRUE);
    pSprite->SetPosition(rcPos.left, rcPos.top);
    g_game->AddSprite(pSprite);
  }
}

void NewGame()
{
  g_game->CleanupSprites();

  RECT bounds = { 0, 0, 600, 450 };
  g_pCarSprite = new Sprite(g_pCarBitmap, bounds, BA_WRAP);
  g_pCarSprite->SetPosition(300, 405);
  g_game->AddSprite(g_pCarSprite);

  g_iFireInputDelay = 0;
  g_iScore = 0;
  g_iNumLives = 3;
  g_iDifficulty = 80;
  g_bGameOver = FALSE;

  g_game->PlayMIDISong();
}

void AddAlien()
{
  RECT         bounds = { 0, 0, 600, 410 };
  AlienSprite* pSprite  = NULL;

  switch(rand() % 3)
  {
  case 0:
    // Blobbo
    pSprite = new AlienSprite(g_pBlobboBitmap, bounds, BA_BOUNCE);
    pSprite->SetNumFrames(8);
    pSprite->SetPosition(((rand() % 2) == 0) ? 0 : 600, rand() % 370);
    pSprite->SetVelocity((rand() % 7) - 2, (rand() % 7) - 2);
    break;

  case 1:
    // Jelly
    pSprite = new AlienSprite(g_pJellyBitmap, bounds, BA_BOUNCE);
    pSprite->SetNumFrames(8);
    pSprite->SetPosition(rand() % 600, rand() % 370);
    pSprite->SetVelocity((rand() % 5) - 2, (rand() % 5) + 3);
    break;

  case 2:
    // Timmy
    pSprite = new AlienSprite(g_pTimmyBitmap, bounds, BA_WRAP);
    pSprite->SetNumFrames(8);
    pSprite->SetPosition(rand() % 600, rand() % 370);
    pSprite->SetVelocity((rand() % 7) + 3, 0);
    break;
  }

  g_game->AddSprite(pSprite);
}