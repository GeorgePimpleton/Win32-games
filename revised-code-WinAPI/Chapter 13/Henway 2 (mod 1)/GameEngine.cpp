#include "GameEngine.hpp"

#pragma comment(lib, "winmm.lib")

GameEngine* GameEngine::m_gameEngine = NULL;

int WINAPI wWinMain(_In_ HINSTANCE inst, _In_opt_ HINSTANCE prevInst, _In_ PWSTR cmdLine, _In_ int cmdShow)
{
   if ( GameInitialize(inst) == S_OK )
   {
      if ( GameEngine::GetEngine( )->Initialize(cmdShow) != S_OK )
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
         if ( PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE) != 0 )
         {
            if ( WM_QUIT == msg.message )
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
            if ( GameEngine::GetEngine( )->GetSleep( ) == FALSE )
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

LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
   return GameEngine::GetEngine( )->HandleEvent(wnd, msg, wParam, lParam);
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
                       WORD icon, WORD smallIcoon, UINT width, UINT height)
{
   m_gameEngine   = this;
   m_inst         = inst;
   m_wnd          = NULL;
   m_icon         = icon;
   m_smallIcon    = smallIcoon;
   m_width        = width;
   m_height       = height;
   m_frameDelay   = 50;
   m_sleep        = TRUE;
   m_joyID        = 0;
   m_joyTrip      = { };
   m_MIDIPlayerID = 0;

   m_sprites.reserve(50);

   size_t  pcch = 0;
   HRESULT hRes = StringCchLengthW(wndClass, STR_LENGTH, &pcch);

   if ( pcch > 0 )
   {
      StringCchCopyW(m_wndClass, STR_LENGTH, wndClass);
   }
   else
   {
      StringCchCopyW(m_wndClass, STR_LENGTH, L"");
   }

#pragma warning(disable : 28193)

   hRes = StringCchLengthW(title, STR_LENGTH, &pcch);

   if ( pcch > 0 )
   {
      StringCchCopyW(m_title, STR_LENGTH, title);
   }
   else
   {
      StringCchCopyW(m_title, STR_LENGTH, L"");
   }
}

GameEngine::~GameEngine( )
{ }

HRESULT GameEngine::Initialize(int cmdShow)
{
   WNDCLASSEXW wc;

   wc.cbSize        = sizeof(WNDCLASSEXW);
   wc.style         = CS_HREDRAW | CS_VREDRAW;
   wc.lpfnWndProc   = WndProc;
   wc.cbClsExtra    = 0;
   wc.cbWndExtra    = 0;
   wc.hInstance     = m_inst;
   wc.hIcon         = (HICON)   LoadImageW(m_inst, MAKEINTRESOURCEW(IDI_ICON), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
   wc.hIconSm       = (HICON)   LoadImageW(m_inst, MAKEINTRESOURCEW(IDI_ICON_SM), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR); ;
   wc.hCursor       = (HCURSOR) LoadImageW(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
   wc.hbrBackground = (HBRUSH)  (COLOR_WINDOW + 1);
   wc.lpszMenuName  = MAKEINTRESOURCEW(IDR_MENU);
   wc.lpszClassName = m_wndClass;

   if ( FAILED(RegisterClassExW(&wc)) )
   {
      MessageBoxW(NULL, L"Unable to initialize Main Window!", L"ERROR", MB_ICONERROR | MB_OK);
      return E_FAIL;
   }

   UINT windowWidth  = m_width + GetSystemMetrics(SM_CXFIXEDFRAME) * 2;
   UINT windowHeight = m_height + GetSystemMetrics(SM_CYFIXEDFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION);

   windowWidth  += 10;
   windowHeight += 10;

   if ( wc.lpszMenuName != NULL )
   {
      windowHeight += GetSystemMetrics(SM_CYMENU);
   }

   UINT windowPosX = (GetSystemMetrics(SM_CXSCREEN) - windowWidth) / 2;
   UINT windowPosY = (GetSystemMetrics(SM_CYSCREEN) - windowHeight) / 2;

   m_wnd = CreateWindowW(m_wndClass, m_title,
                         WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX,
                         windowPosX, windowPosY,
                         windowWidth, windowHeight,
                         NULL, NULL, m_inst, NULL);

   if ( NULL == m_wnd )
   {
      MessageBoxW(NULL, L"Unable to create Main Window!", L"ERROR", MB_ICONERROR | MB_OK);
      return E_FAIL;
   }

   ShowWindow(m_wnd, cmdShow);
   UpdateWindow(m_wnd);

   return S_OK;
}

LRESULT GameEngine::HandleEvent(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
   switch ( msg )
   {
   case WM_CREATE:
      SetWindow(wnd);
      GameStart(wnd);
      return S_OK;

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
      return S_OK;

   case WM_COMMAND:
      GameMenu(wParam);
      return S_OK;

   case WM_PAINT:
{
      PAINTSTRUCT ps;
      HDC         dc = BeginPaint(wnd, &ps);

      EndPaint(wnd, &ps);
      return S_OK;
}

   case WM_LBUTTONDOWN:
      MouseButtonDown(LOWORD(lParam), HIWORD(lParam), TRUE);
      return S_OK;

   case WM_LBUTTONUP:
      MouseButtonUp(LOWORD(lParam), HIWORD(lParam), TRUE);
      return S_OK;

   case WM_RBUTTONDOWN:
      MouseButtonDown(LOWORD(lParam), HIWORD(lParam), FALSE);
      return S_OK;

   case WM_RBUTTONUP:
      MouseButtonUp(LOWORD(lParam), HIWORD(lParam), FALSE);
      return S_OK;

   case WM_MOUSEMOVE:
      MouseMove(LOWORD(lParam), HIWORD(lParam));
      return S_OK;

   case WM_DESTROY:
      GameEnd( );
      PostQuitMessage(0);
      return S_OK;
   }
   return DefWindowProcW(wnd, msg, wParam, lParam);
}

BOOL GameEngine::InitJoystick( )
{
   // make sure joystick driver is present
   UINT numJoysticks;

   if ( (numJoysticks = joyGetNumDevs( )) == 0 )
   {
      return FALSE;
   }

   // make sure the joystick is attached
   JOYINFO joyInfo;

   if ( joyGetPos(JOYSTICKID1, &joyInfo) != JOYERR_UNPLUGGED )
   {
      m_joyID = JOYSTICKID1;
   }
   else
   {
      return FALSE;
   }

   // calculate the trip values
   JOYCAPS joyCaps;

   joyGetDevCapsW(m_joyID, &joyCaps, sizeof(JOYCAPS));

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
      JOYINFO  joyInfo;
      JOYSTATE joyState = 0;

      if ( joyGetPos(m_joyID, &joyInfo) == JOYERR_NOERROR )
      {
         // check horizontal movement
         if ( joyInfo.wXpos < (WORD) m_joyTrip.left )
         {
            joyState |= JOY_LEFT;
         }
         else if ( joyInfo.wXpos > (WORD)m_joyTrip.right )
         {
            joyState |= JOY_RIGHT;
         }

         // check vertical movement
         if ( joyInfo.wYpos < (WORD) m_joyTrip.top )
         {
            joyState |= JOY_UP;
         }
         else if ( joyInfo.wYpos > (WORD)m_joyTrip.bottom )
         {
            joyState |= JOY_DOWN;
         }

         // check buttons
         if ( joyInfo.wButtons & JOY_BUTTON1 )
         {
            joyState |= JOY_FIRE1;
         }
         if ( joyInfo.wButtons & JOY_BUTTON2 )
         {
            joyState |= JOY_FIRE2;
         }
      }

      // allow the game to handle the joystick
      HandleJoystick(joyState);
   }
}

void GameEngine::AddSprite(Sprite* pSprite)
{
   if ( pSprite != NULL )
   {
      if ( m_sprites.size( ) > 0 )
      {
         for ( auto iterSprite = m_sprites.begin( ); iterSprite != m_sprites.end( ); iterSprite++ )
         {
            if ( pSprite->GetZOrder( ) < (*iterSprite)->GetZOrder( ) )
            {
               m_sprites.insert(iterSprite, pSprite);
               return;
            }
         }
      }

      m_sprites.push_back(pSprite);
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
   RECT          oldSpritePos;
   SPRITEACTION  spriteAction;

   for ( auto iterSprite = m_sprites.begin( ); iterSprite != m_sprites.end( ); iterSprite++ )
   {
      oldSpritePos = (*iterSprite)->GetPosition( );

      spriteAction = (*iterSprite)->Update( );

      if ( spriteAction & SA_KILL )
      {
         delete (*iterSprite);
         m_sprites.erase(iterSprite);
         iterSprite--;
         continue;
      }

      if ( CheckSpriteCollision(*iterSprite) )
      {
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
//   for ( auto iterSprite = m_sprites.rbegin( ); iterSprite != m_sprites.rend( ); iterSprite++ )
   for ( auto& iterSprite : m_sprites )
   {
      if ( !(iterSprite)->IsHidden( ) && (iterSprite)->IsPointInside(x, y) )
      {
         return (iterSprite);
      }
   }

   return NULL;
}

BOOL GameEngine::CheckSpriteCollision(Sprite* testSprite)
{
//    for ( auto iterSprite = m_sprites.begin( ); iterSprite != m_sprites.end( ); iterSprite++ )
   for ( auto& iterSprite : m_sprites )
   {
      if ( testSprite == (iterSprite) )
      {
         continue;
      }

      if ( testSprite->TestCollision(iterSprite) )
      {
         return SpriteCollision((iterSprite), testSprite);
      }
   }

   return FALSE;
}

// plays a MIDI song
void GameEngine::PlayMIDISong(PCWSTR MIDIFileName, BOOL restart)
{
   // see if the midi player needs to be opened
   if ( m_MIDIPlayerID == 0 )
   {
      // open the midi player by specifying the device and filename
      MCI_OPEN_PARMS mciOpenParms;

      mciOpenParms.lpstrDeviceType = TEXT("sequencer");
      mciOpenParms.lpstrElementName = MIDIFileName;

      if ( mciSendCommandW(NULL, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD_PTR) &mciOpenParms) == 0 )
      {
         // get the ID for the MIDI player
         m_MIDIPlayerID = mciOpenParms.wDeviceID;
      }
      else
      {
         // there was a problem, so just return
         return;
      }
   }

   // restart the MIDI song, if necessary
   if ( restart )
   {
      MCI_SEEK_PARMS mciSeekParms;

      if ( mciSendCommandW(m_MIDIPlayerID, MCI_SEEK, MCI_SEEK_TO_START, (DWORD_PTR) &mciSeekParms) != 0 )
      {
         // there was a problem, so close the MIDI player
         CloseMIDIPlayer( );
      }
   }

   // play the MIDI song
   MCI_PLAY_PARMS mciPlayParms;

   if ( mciSendCommandW(m_MIDIPlayerID, MCI_PLAY, 0, (DWORD_PTR) &mciPlayParms) != 0 )
   {
      // there was a problem, so close the MIDI player
      CloseMIDIPlayer( );
   }
}

// pauses a playing MIDI song
void GameEngine::PauseMIDISong( )
{
   // pause the currently playing song, if possible
   if ( m_MIDIPlayerID != 0 )
   {
      mciSendCommandW(m_MIDIPlayerID, MCI_PAUSE, 0, NULL);
   }
}

// closes the MIDI player
void GameEngine::CloseMIDIPlayer( )
{
   // close the MIDI player, if possible
   if ( m_MIDIPlayerID != 0 )
   {
      mciSendCommandW(m_MIDIPlayerID, MCI_CLOSE, 0, NULL);
      m_MIDIPlayerID = 0;
   }
}