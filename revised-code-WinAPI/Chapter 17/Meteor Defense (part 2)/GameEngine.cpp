#pragma comment(lib, "winmm.lib")

#include "GameEngine.hpp"

GameEngine* GameEngine::m_gameEngine = NULL;

int WINAPI wWinMain(_In_ HINSTANCE inst, _In_opt_ HINSTANCE prevInst, _In_ PWSTR cmdLine, _In_ int cmdShow)
{
   if ( GameInitialize(inst) )
   {
      if ( !GameEngine::GetEngine( )->Initialize(cmdShow) )
      {
         return E_FAIL;
      }

      HACCEL accel = LoadAcceleratorsW(inst, MAKEINTRESOURCEW(IDR_ACCELERATORS));

      if ( NULL == accel )
      {
         MessageBoxW(NULL, L"Unable to Load the Accelerators!", GameEngine::GetEngine( )->GetTitle( ), MB_OK | MB_ICONERROR);
         return E_FAIL;
      }

      MSG msg;

      while ( TRUE )
      {
         if ( PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE) )
         {
            if ( msg.message == WM_QUIT )
            {
               break;
            }

            if ( 0 == TranslateAcceleratorW(GameEngine::GetEngine( )->GetWindow( ), accel, &msg) )
            {
               TranslateMessage(&msg);
               DispatchMessageW(&msg);
            }
         }
         else
         {
            if ( !GameEngine::GetEngine( )->GetSleep( ) )
            {
               static ULONGLONG tickTrigger = 0;
               ULONGLONG        tickCount   = GetTickCount64( );

               if ( tickCount > tickTrigger )
               {
                  tickTrigger = tickCount + GameEngine::GetEngine( )->GetFrameDelay( );

                  HandleKeys( );
                  GameEngine::GetEngine( )->CheckJoystick( );
                  GameCycle( );
               }
            }
         }
      }
      return (int) msg.wParam;
   }

   GameEnd( );

   return S_OK;
}

LRESULT CALLBACK WndProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
   return GameEngine::GetEngine( )->HandleEvent(hWindow, msg, wParam, lParam);
}

BOOL CALLBACK DlgProc(HWND dlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
   switch ( msg )
   {
   case WM_COMMAND:
      switch ( LOWORD(wParam) )
      {
      case IDOK:
         EndDialog(dlg, 0);
         return TRUE;
      }
   }

   return FALSE;
}

GameEngine::GameEngine(HINSTANCE inst, PCWSTR wndClass, PCWSTR title,
                       WORD icon, WORD smallIcon, int width, int height)
{
   m_gameEngine    = this;
   m_inst          = inst;
   m_wnd           = NULL;
   m_wndClass      = wndClass;
   m_title         = title;
   m_icon          = icon;
   m_smallIcon     = smallIcon;
   m_width         = width;
   m_height        = height;
   m_frameDelay    = 50;   // 20 FPS default
   m_asleep        = TRUE;
   m_joyID         = 0;
   m_joyTrip       = { };
   m_MIDIPlayerID  = 0;

   m_sprites.reserve(100);
}

GameEngine::~GameEngine( )
{ }

BOOL GameEngine::Initialize(int cmdShow)
{
   WNDCLASSEXW wc = { };

   wc.cbSize        = sizeof(wc);
   wc.style         = CS_HREDRAW | CS_VREDRAW;
   wc.lpfnWndProc   = WndProc;
   wc.cbClsExtra    = 0;
   wc.cbWndExtra    = 0;
   wc.hInstance     = m_inst;
   wc.hIcon         = (HICON)   LoadImageW(m_inst, MAKEINTRESOURCEW(IDI_ICON), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
   wc.hIconSm       = (HICON)   LoadImageW(m_inst, MAKEINTRESOURCEW(IDI_ICON_SM), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR); ;
   wc.hCursor       = (HCURSOR) LoadImageW(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
   wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
   wc.lpszMenuName  = MAKEINTRESOURCEW(IDR_MENU);
   wc.lpszClassName = m_wndClass;

   if ( FAILED(RegisterClassExW(&wc)) )
   {
      MessageBoxW(NULL, L"Unable to initialize Main Window!", L"ERROR", MB_ICONERROR | MB_OK);
      return FALSE;
   }

   int windowWidth  = m_width + GetSystemMetrics(SM_CXFIXEDFRAME) * 2;
   int windowHeight = m_height + GetSystemMetrics(SM_CYFIXEDFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION);

   if ( wc.lpszMenuName != NULL )
   {
      windowHeight += GetSystemMetrics(SM_CYMENU);
   }

   windowWidth  += 10;
   windowHeight += 10;

 int iXWindowPos = (GetSystemMetrics(SM_CXSCREEN) - windowWidth) / 2;
 int iYWindowPos = (GetSystemMetrics(SM_CYSCREEN) - windowHeight) / 2;

   m_wnd = CreateWindowW(m_wndClass, m_title, WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX,
                         iXWindowPos, iYWindowPos, windowWidth, windowHeight,
                         NULL, NULL, m_inst, NULL);

   if ( NULL == m_wnd )
   {
      MessageBoxW(NULL, L"Unable to create Main Window!", L"ERROR", MB_ICONERROR | MB_OK);
      return FALSE;
   }

   ShowWindow(m_wnd, cmdShow);
   UpdateWindow(m_wnd);

   return TRUE;
}

LRESULT GameEngine::HandleEvent(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
   switch ( msg )
   {
   case WM_CREATE:
      SetWindow(wnd);
      GameStart(wnd);
      return 0;

   case WM_ACTIVATE:
      if ( wParam != WA_INACTIVE )
      {
         GameActivate(wnd);
         SetSleep(FALSE);
      }
      else
      {
         GameDeactivate(wnd);
         SetSleep(TRUE);
      }
      return 0;

   case WM_COMMAND:
      GameMenu(wParam);
      return 0;

   case WM_PAINT:
      HDC         hDC;
      PAINTSTRUCT ps;
      hDC = BeginPaint(wnd, &ps);

      GamePaint(hDC);

      EndPaint(wnd, &ps);
      return 0;

   case WM_LBUTTONDOWN:
      MouseButtonDown(LOWORD(lParam), HIWORD(lParam), TRUE);
      return 0;

   case WM_LBUTTONUP:
      MouseButtonUp(LOWORD(lParam), HIWORD(lParam), TRUE);
      return 0;

   case WM_RBUTTONDOWN:
      MouseButtonDown(LOWORD(lParam), HIWORD(lParam), FALSE);
      return 0;

   case WM_RBUTTONUP:
      MouseButtonUp(LOWORD(lParam), HIWORD(lParam), FALSE);
      return 0;

   case WM_MOUSEMOVE:
      MouseMove(LOWORD(lParam), HIWORD(lParam));
      return 0;

   case WM_DESTROY:
      GameEnd( );
      PostQuitMessage(0);
      return 0;
   }
   return DefWindowProcW(wnd, msg, wParam, lParam);
}

BOOL GameEngine::InitJoystick( )
{
   UINT numJoysticks;

   if ( (numJoysticks = joyGetNumDevs( )) == 0 )
   {
      return FALSE;
   }

   JOYINFO joyInfo;

   if ( joyGetPos(JOYSTICKID1, &joyInfo) != JOYERR_UNPLUGGED )
   {
      m_joyID = JOYSTICKID1;
   }
   else
   {
      return FALSE;
   }

   JOYCAPS joyCaps;

   joyGetDevCaps(m_joyID, &joyCaps, sizeof(JOYCAPS));

   DWORD xCenter = ((DWORD) joyCaps.wXmin + joyCaps.wXmax) / 2;
   DWORD yCenter = ((DWORD) joyCaps.wYmin + joyCaps.wYmax) / 2;

   m_joyTrip.left   = (joyCaps.wXmin + (WORD) xCenter) / 2;
   m_joyTrip.right  = (joyCaps.wXmax + (WORD) xCenter) / 2;
   m_joyTrip.top    = (joyCaps.wYmin + (WORD) yCenter) / 2;
   m_joyTrip.bottom = (joyCaps.wYmax + (WORD) yCenter) / 2;

   return TRUE;
}

void GameEngine::CaptureJoystick( )
{
   if ( m_joyID == JOYSTICKID1 )
   {
      joySetCapture(m_wnd, m_joyID, NULL, TRUE);
   }
}

void GameEngine::ReleaseJoystick( )
{
   if ( m_joyID == JOYSTICKID1 )
   {
      joyReleaseCapture(m_joyID);
   }
}

void GameEngine::CheckJoystick( )
{
   if ( m_joyID == JOYSTICKID1 )
   {
      JOYINFO  joyInfo  = { };
      JOYSTATE joyState = 0;

      if ( joyGetPos(m_joyID, &joyInfo) == JOYERR_NOERROR )
      {
         if ( joyInfo.wXpos < (WORD) m_joyTrip.left )
         {
            joyState |= JOY_LEFT;
         }
         else if ( joyInfo.wXpos > (WORD)m_joyTrip.right )
         {
            joyState |= JOY_RIGHT;
         }

         if ( joyInfo.wYpos < (WORD) m_joyTrip.top )
         {
            joyState |= JOY_UP;
         }
         else if ( joyInfo.wYpos > (WORD)m_joyTrip.bottom )
         {
            joyState |= JOY_DOWN;
         }

         if ( joyInfo.wButtons & JOY_BUTTON1 )
         {
            joyState |= JOY_FIRE1;
         }

         if ( joyInfo.wButtons & JOY_BUTTON2 )
         {
            joyState |= JOY_FIRE2;
         }
      }

      HandleJoystick(joyState);
   }
}

void GameEngine::AddSprite(Sprite* sprite)
{
   if ( sprite != NULL )
   {
      if ( m_sprites.size( ) > 0 )
      {
         for ( auto iterSprite = m_sprites.begin( ); iterSprite != m_sprites.end( ); iterSprite++ )
         {
            if ( sprite->GetZOrder( ) < (*iterSprite)->GetZOrder( ) )
            {
               m_sprites.insert(iterSprite, sprite);
               return;
            }
         }
      }

      m_sprites.push_back(sprite);
   }
}

void GameEngine::DrawSprites(HDC dc)
{
   for ( auto iterSprite = m_sprites.begin( ); iterSprite != m_sprites.end( ); iterSprite++ )
   {
      (*iterSprite)->Draw(dc);
   }
}

void GameEngine::UpdateSprites( )
{
   if ( m_sprites.size( ) >= (m_sprites.capacity( ) / 2) )
   {
      m_sprites.reserve(m_sprites.capacity( ) * 2);
   }

   RECT         oldSpritePos;
   SPRITEACTION spriteAction;

   for ( auto iterSprite = m_sprites.begin( ); iterSprite != m_sprites.end( ); iterSprite++ )
   {
      oldSpritePos = (*iterSprite)->GetPosition( );

      spriteAction = (*iterSprite)->Update( );

      // Handle the SA_KILL sprite action
      if ( spriteAction & SA_KILL )
      {
         // Notify the game that the sprite is dying
         SpriteDying(*iterSprite);

         // Kill the sprite
         delete (*iterSprite);
         m_sprites.erase(iterSprite);
         iterSprite--;
         continue;
      }

      // See if the sprite collided with any others
      if ( CheckSpriteCollision(*iterSprite) )
      {
         // Restore the old sprite position
         (*iterSprite)->SetPosition(oldSpritePos);
      }
   }
}

void GameEngine::CleanupSprites( )
{
   for ( auto iterSprite = m_sprites.begin( ); iterSprite != m_sprites.end( ); iterSprite++ )
   {
      delete (*iterSprite);
      m_sprites.erase(iterSprite);
      iterSprite--;
   }
}

Sprite* GameEngine::IsPointInSprite(int x, int y)
{
   for ( auto iterSprite = m_sprites.rbegin( ); iterSprite != m_sprites.rend( ); iterSprite++ )
   {
      if ( !(*iterSprite)->IsHidden( ) && (*iterSprite)->IsPointInside(x, y) )
      {
         return (*iterSprite);
      }
   }

   return NULL;
}

BOOL GameEngine::CheckSpriteCollision(Sprite* testSprite)
{
   for ( auto iterSprite = m_sprites.begin( ); iterSprite != m_sprites.end( ); iterSprite++ )
   {
      // Make sure not to check for collision with itself
      if ( testSprite == (*iterSprite) )
      {
         continue;
      }

      // Test the collision
      if ( testSprite->TestCollision(*iterSprite) )
      {
         // Collision detected
         return SpriteCollision((*iterSprite), testSprite);
      }
   }

   // No collision
   return FALSE;
}

void GameEngine::PlayMIDISong(PCWSTR MIDIFileName, BOOL restart)
{
   if ( 0 == m_MIDIPlayerID )
   {
      MCI_OPEN_PARMSW mciOpenParms = { };

      mciOpenParms.lpstrDeviceType = L"sequencer";
      mciOpenParms.lpstrElementName = MIDIFileName;

      if ( 0 == mciSendCommandW(NULL, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT,
                                (DWORD_PTR) &mciOpenParms) )
      {
         m_MIDIPlayerID = mciOpenParms.wDeviceID;
      }
      else
      {
         return;
      }
   }

   if ( restart )
   {
      MCI_SEEK_PARMS mciSeekParms = { };

      if ( 0 != mciSendCommandW(m_MIDIPlayerID, MCI_SEEK, MCI_SEEK_TO_START,
                                (DWORD_PTR) &mciSeekParms) )
      {
         CloseMIDIPlayer( );
      }
   }

   MCI_PLAY_PARMS mciPlayParms = { };

   if ( 0 != mciSendCommandW(m_MIDIPlayerID, MCI_PLAY, 0,
                             (DWORD_PTR) &mciPlayParms) )
   {
      CloseMIDIPlayer( );
   }
}

void GameEngine::PauseMIDISong( )
{
   if ( m_MIDIPlayerID != 0 )
   {
      mciSendCommandW(m_MIDIPlayerID, MCI_PAUSE, 0, NULL);
   }
}

void GameEngine::CloseMIDIPlayer( )
{
   if ( 0 != m_MIDIPlayerID )
   {
      mciSendCommandW(m_MIDIPlayerID, MCI_CLOSE, 0, NULL);
      m_MIDIPlayerID = 0;
   }
}