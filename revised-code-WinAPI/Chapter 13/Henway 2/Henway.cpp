//-----------------------------------------------------------------
// Henway 2 Application
// C++ Source - Henway.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Henway.h"

//-----------------------------------------------------------------
// Game Engine Functions
//-----------------------------------------------------------------
BOOL GameInitialize(HINSTANCE hInstance)
{
  // Create the game engine
  _pGame = new GameEngine(hInstance, TEXT("Henway 2"),
    TEXT("Henway 2"), IDI_HENWAY, IDI_HENWAY_SM, 465, 400);
  if (_pGame == NULL)
    return FALSE;

  // Set the frame rate
  _pGame->SetFrameRate(30);

  // Initialize the joystick
  _pGame->InitJoystick();

  // Store the instance handle
  _hInstance = hInstance;

  return TRUE;
}

void GameStart(HWND hWindow)
{
  // Seed the random number generator
  srand(GetTickCount());

  // Create the offscreen device context and bitmap
  _hOffscreenDC = CreateCompatibleDC(GetDC(hWindow));
  _hOffscreenBitmap = CreateCompatibleBitmap(GetDC(hWindow),
    _pGame->GetWidth(), _pGame->GetHeight());
  SelectObject(_hOffscreenDC, _hOffscreenBitmap);

  // Create and load the bitmaps
  HDC hDC = GetDC(hWindow);
  _pHighwayBitmap = new Bitmap(hDC, IDB_HIGHWAY, _hInstance);
  _pChickenBitmap = new Bitmap(hDC, IDB_CHICKEN, _hInstance);
  _pCarBitmaps[0] = new Bitmap(hDC, IDB_CAR1, _hInstance);
  _pCarBitmaps[1] = new Bitmap(hDC, IDB_CAR2, _hInstance);
  _pCarBitmaps[2] = new Bitmap(hDC, IDB_CAR3, _hInstance);
  _pCarBitmaps[3] = new Bitmap(hDC, IDB_CAR4, _hInstance);
  _pChickenHeadBitmap = new Bitmap(hDC, IDB_CHICKENHEAD, _hInstance);

  // Create the chicken and car sprites
  Sprite* pSprite;
  RECT    rcBounds = { 0, 0, 465, 400 };
  _pChickenSprite = new Sprite(_pChickenBitmap, rcBounds, BA_STOP);
  _pChickenSprite->SetPosition(4, 175);
  _pChickenSprite->SetVelocity(0, 0);
  _pChickenSprite->SetZOrder(1);
  _pGame->AddSprite(_pChickenSprite);
  pSprite = new Sprite(_pCarBitmaps[0], rcBounds, BA_WRAP);
  pSprite->SetPosition(70, 0);
  pSprite->SetVelocity(0, 7);
  pSprite->SetZOrder(2);
  _pGame->AddSprite(pSprite);
  pSprite = new Sprite(_pCarBitmaps[1], rcBounds, BA_WRAP);
  pSprite->SetPosition(160, 0);
  pSprite->SetVelocity(0, 3);
  pSprite->SetZOrder(2);
  _pGame->AddSprite(pSprite);
  pSprite = new Sprite(_pCarBitmaps[2], rcBounds, BA_WRAP);
  pSprite->SetPosition(239, 400);
  pSprite->SetVelocity(0, -5);
  pSprite->SetZOrder(2);
  _pGame->AddSprite(pSprite);
  pSprite = new Sprite(_pCarBitmaps[3], rcBounds, BA_WRAP);
  pSprite->SetPosition(329, 400);
  pSprite->SetVelocity(0, -10);
  pSprite->SetZOrder(2);
  _pGame->AddSprite(pSprite);

  // Initialize the remaining global variables
  _iInputDelay = 0;
  _iNumLives = 3;
  _iScore = 0;
  _bGameOver = FALSE;

  // Play the background music
  _pGame->PlayMIDISong(TEXT("Music.mid"));
}

void GameEnd()
{
  // Close the MIDI player for the background music
  _pGame->CloseMIDIPlayer();

  // Cleanup the offscreen device context and bitmap
  DeleteObject(_hOffscreenBitmap);
  DeleteDC(_hOffscreenDC);  

  // Cleanup the bitmaps
  delete _pHighwayBitmap;
  delete _pChickenBitmap;
  for (int i = 0; i < 4; i++)
    delete _pCarBitmaps[i];
  delete _pChickenHeadBitmap;

  // Cleanup the sprites
  _pGame->CleanupSprites();

  // Cleanup the game engine
  delete _pGame;
}

void GameActivate(HWND hWindow)
{
  // Capture the joystick
  _pGame->CaptureJoystick();

  // Resume the background music
  _pGame->PlayMIDISong(TEXT(""), FALSE);
}

void GameDeactivate(HWND hWindow)
{
  // Release the joystick
  _pGame->ReleaseJoystick();

  // Pause the background music
  _pGame->PauseMIDISong();
}

void GamePaint(HDC hDC)
{
  // Draw the background highway
  _pHighwayBitmap->Draw(hDC, 0, 0);

  // Draw the sprites
  _pGame->DrawSprites(hDC);

  // Draw the number of remaining chicken lives
  for (int i = 0; i < _iNumLives; i++)
    _pChickenHeadBitmap->Draw(hDC,
      406 + (_pChickenHeadBitmap->GetWidth() * i), 382, TRUE);
}

void GameCycle()
{
  if (!_bGameOver)
  {
    // Play a random car sound randomly
    if (rand() % 100 == 0)
      if (rand() % 2 == 0)
        PlaySound((LPCSTR)IDW_CARHORN1, _hInstance, SND_ASYNC | SND_RESOURCE);
      else
        PlaySound((LPCSTR)IDW_CARHORN2, _hInstance, SND_ASYNC | SND_RESOURCE);

    // Update the sprites
    _pGame->UpdateSprites();

    // Obtain a device context for repainting the game
    HWND  hWindow = _pGame->GetWindow();
    HDC   hDC = GetDC(hWindow);

    // Paint the game to the offscreen device context
    GamePaint(_hOffscreenDC);

    // Blit the offscreen bitmap to the game screen
    BitBlt(hDC, 0, 0, _pGame->GetWidth(), _pGame->GetHeight(),
      _hOffscreenDC, 0, 0, SRCCOPY);

    // Cleanup
    ReleaseDC(hWindow, hDC);
  }
}

void HandleKeys()
{
  if (!_bGameOver && (++_iInputDelay > 2))
  {
    // Move the chicken based upon key presses
    if (GetAsyncKeyState(VK_LEFT) < 0)
      MoveChicken(-20, 0);
    else if (GetAsyncKeyState(VK_RIGHT) < 0)
      MoveChicken(20, 0);
    if (GetAsyncKeyState(VK_UP) < 0)
      MoveChicken(0, -20);
    else if (GetAsyncKeyState(VK_DOWN) < 0)
      MoveChicken(0, 20);

    // Reset the input delay
    _iInputDelay = 0;
  }
}

void MouseButtonDown(int x, int y, BOOL bLeft)
{
  // Start a new game, if necessary
  if (_bGameOver)
  {
    // Restart the background music
    _pGame->PlayMIDISong();

    // Initialize the game variables
    _iNumLives = 3;
    _iScore = 0;
    _bGameOver = FALSE;
  }
}

void MouseButtonUp(int x, int y, BOOL bLeft)
{
}

void MouseMove(int x, int y)
{
}

void HandleJoystick(JOYSTATE jsJoystickState)
{
  if (!_bGameOver && (++_iInputDelay > 2))
  {
    // Check horizontal movement
    if (jsJoystickState & JOY_LEFT)
        MoveChicken(-20, 0);
    else if (jsJoystickState & JOY_RIGHT)
        MoveChicken(20, 0);

    // Check vertical movement
    if (jsJoystickState & JOY_UP)
        MoveChicken(0, -20);
    else if (jsJoystickState & JOY_DOWN)
        MoveChicken(0, 20);

    // Reset the input delay
    _iInputDelay = 0;
  }

  // Check the joystick button and start a new game, if necessary
  if (_bGameOver && (jsJoystickState & JOY_FIRE1))
  {
    // Restart the background music
    _pGame->PlayMIDISong();

    // Initialize the game variables
    _iNumLives = 3;
    _iScore = 0;
    _bGameOver = FALSE;
  }
}

BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee)
{
  // See if the chicken was hit
  if (pSpriteHittee == _pChickenSprite)
  {
    // Move the chicken back to the start
    _pChickenSprite->SetPosition(4, 175);

    // See if the game is over
    if (--_iNumLives > 0)
      // Play a sound for the chicken getting hit
      PlaySound((LPCSTR)IDW_SQUISH, _hInstance, SND_ASYNC | SND_RESOURCE);
    else
    {
      // Play a sound for the game ending
      PlaySound((LPCSTR)IDW_GAMEOVER, _hInstance, SND_ASYNC | SND_RESOURCE);

      // Display game over message
      TCHAR szText[64];
      wsprintf(szText, "Game Over! You scored %d points.", _iScore);
      MessageBox(_pGame->GetWindow(), szText, TEXT("Henway 2"), MB_OK);
      _bGameOver = TRUE;

      // Pause the background music
      _pGame->PauseMIDISong();
    }

    return FALSE;
  }

  return TRUE;
}

//-----------------------------------------------------------------
// Functions
//-----------------------------------------------------------------
void MoveChicken(int iXDistance, int iYDistance)
{
  // Move the chicken to its new position
  _pChickenSprite->OffsetPosition(iXDistance, iYDistance);

  // See if the chicken made it across
  if (_pChickenSprite->GetPosition().left > 400)
  {
    // Play a sound for the chicken making it safely across
    PlaySound((LPCSTR)IDW_CELEBRATE, _hInstance, SND_ASYNC | SND_RESOURCE);

    // Move the chicken back to the start and add to the score
    _pChickenSprite->SetPosition(4, 175);
    _iScore += 150;
  }
}
