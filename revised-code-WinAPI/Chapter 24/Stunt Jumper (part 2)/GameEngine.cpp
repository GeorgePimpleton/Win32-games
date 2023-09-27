#pragma comment(lib, "winmm.lib")

#include "GameEngine.hpp"

GameEngine* GameEngine::m_gameEngine = NULL;

int WINAPI wWinMain(_In_ HINSTANCE inst, _In_opt_ HINSTANCE prevInst,
                    _In_ PWSTR cmdLine, _In_ int cmdShow)
{
   MSG         msg;
   static int  iTickTrigger = 0;
   int         iTickCount;

   if ( GameInitialize(inst) )
   {
      if ( !GameEngine::GetEngine( )->Initialize(cmdShow) )
      {
         return FALSE;
      }

      while ( TRUE )
      {
         if ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
         {
            if ( msg.message == WM_QUIT )
            {
               break;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
         }
         else
         {
            if ( !GameEngine::GetEngine( )->GetSleep( ) )
            {
               iTickCount = GetTickCount( );

               if ( iTickCount > iTickTrigger )
               {
                  iTickTrigger = iTickCount + GameEngine::GetEngine( )-> GetFrameDelay( );

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

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
   return GameEngine::GetEngine( )->HandleEvent(wnd, msg, wParam, lParam);
}

BOOL GameEngine::CheckSpriteCollision(Sprite* testSprite)
{
   for ( auto iterSprite = m_sprites.begin( ); iterSprite != m_sprites.end( ); iterSprite++ )
   {
      if ( testSprite == (*iterSprite) )
      {
         continue;
      }

      if ( testSprite->TestCollision(*iterSprite) )
      {
         return SpriteCollision((*iterSprite), testSprite);
      }
   }

   return FALSE;
}

GameEngine::GameEngine(HINSTANCE inst, PCWSTR wndClass, PCWSTR title,
                       WORD icon, WORD smallIcon, int width, int height)
{
   m_gameEngine = this;
   m_inst = inst;
   m_wnd = NULL;
   m_wndClass = wndClass;
   m_title = title;
   m_icon = icon;
   m_smallIcon = smallIcon;
   m_width = width;
   m_height = height;
   m_frameDelay = 50;   // 20 FPS default
   m_asleep = TRUE;
   m_joyID = 0;
   m_joyTrip = { };
   m_MIDIPlayerID = 0;

   m_sprites.reserve(100);
}

GameEngine::~GameEngine( )
{ }

BOOL GameEngine::Initialize(int cmdShow)
{
   WNDCLASSEX wndclass;

   wndclass.cbSize = sizeof(wndclass);
   wndclass.style = CS_HREDRAW | CS_VREDRAW;
   wndclass.lpfnWndProc = WndProc;
   wndclass.cbClsExtra = 0;
   wndclass.cbWndExtra = 0;
   wndclass.hInstance = m_inst;
   wndclass.hIcon = LoadIcon(m_inst, MAKEINTRESOURCE(GetIcon( )));
   wndclass.hIconSm = LoadIcon(m_inst, MAKEINTRESOURCE(GetSmallIcon( )));
   wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
   wndclass.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
   wndclass.lpszMenuName = NULL;
   wndclass.lpszClassName = m_wndClass;

   if ( !RegisterClassEx(&wndclass) )
   {
      return FALSE;
   }

   int iWindowWidth = m_width + GetSystemMetrics(SM_CXFIXEDFRAME) * 2,
      iWindowHeight = m_height + GetSystemMetrics(SM_CYFIXEDFRAME) * 2 +
                                  GetSystemMetrics(SM_CYCAPTION);

   iWindowWidth += 10;
   iWindowHeight += 10;

   if ( wndclass.lpszMenuName != NULL )
   {
      iWindowHeight += GetSystemMetrics(SM_CYMENU);
   }

   int iXWindowPos = (GetSystemMetrics(SM_CXSCREEN) - iWindowWidth) / 2,
      iYWindowPos = (GetSystemMetrics(SM_CYSCREEN) - iWindowHeight) / 2;

   m_wnd = CreateWindow(m_wndClass, m_title, WS_POPUPWINDOW |
                            WS_CAPTION | WS_MINIMIZEBOX, iXWindowPos, iYWindowPos, iWindowWidth,
                            iWindowHeight, NULL, NULL, m_inst, NULL);

   if ( !m_wnd )
   {
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

   case WM_SETFOCUS:
      GameActivate(wnd);
      SetSleep(FALSE);
      return 0;

   case WM_KILLFOCUS:
      GameDeactivate(wnd);
      SetSleep(TRUE);
      return 0;

   case WM_PAINT:
      HDC         dc;
      PAINTSTRUCT ps;
      dc = BeginPaint(wnd, &ps);

      GamePaint(dc);

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
   return DefWindowProc(wnd, msg, wParam, lParam);
}

BOOL GameEngine::InitJoystick( )
{
   UINT uiNumJoysticks;

   if ( (uiNumJoysticks = joyGetNumDevs( )) == 0 )
   {
      return FALSE;
   }

   JOYINFO jiInfo;

   if ( joyGetPos(JOYSTICKID1, &jiInfo) != JOYERR_UNPLUGGED )
   {
      m_joyID = JOYSTICKID1;
   }
   else
   {
      return FALSE;
   }

   JOYCAPS jcCaps;

   joyGetDevCaps(m_joyID, &jcCaps, sizeof(JOYCAPS));

   DWORD dwXCenter = ((DWORD) jcCaps.wXmin + jcCaps.wXmax) / 2;
   DWORD dwYCenter = ((DWORD) jcCaps.wYmin + jcCaps.wYmax) / 2;

   m_joyTrip.left = (jcCaps.wXmin + (WORD) dwXCenter) / 2;
   m_joyTrip.right = (jcCaps.wXmax + (WORD) dwXCenter) / 2;
   m_joyTrip.top = (jcCaps.wYmin + (WORD) dwYCenter) / 2;
   m_joyTrip.bottom = (jcCaps.wYmax + (WORD) dwYCenter) / 2;

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
      JOYINFO jiInfo;
      JOYSTATE joyState = 0;

      if ( joyGetPos(m_joyID, &jiInfo) == JOYERR_NOERROR )
      {
         if ( jiInfo.wXpos < (WORD) m_joyTrip.left )
         {
            joyState |= JOY_LEFT;
         }
         else if ( jiInfo.wXpos > (WORD)m_joyTrip.right )
         {
            joyState |= JOY_RIGHT;
         }

         if ( jiInfo.wYpos < (WORD) m_joyTrip.top )
         {
            joyState |= JOY_UP;
         }
         else if ( jiInfo.wYpos > (WORD)m_joyTrip.bottom )
         {
            joyState |= JOY_DOWN;
         }

         if ( jiInfo.wButtons & JOY_BUTTON1 )
         {
            joyState |= JOY_FIRE1;
         }

         if ( jiInfo.wButtons & JOY_BUTTON2 )
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

   RECT          rcOldSpritePos;
   SPRITEACTION  saSpriteAction;

   for ( auto iterSprite = m_sprites.begin( ); iterSprite != m_sprites.end( ); iterSprite++ )
   {
      rcOldSpritePos = (*iterSprite)->GetPosition( );

      saSpriteAction = (*iterSprite)->Update( );

      if ( saSpriteAction & SA_ADDSPRITE )
      {
         AddSprite((*iterSprite)->AddSprite( ));
      }

      if ( saSpriteAction & SA_KILL )
      {
         SpriteDying(*iterSprite);

         delete (*iterSprite);
         m_sprites.erase(iterSprite);
         iterSprite--;
         continue;
      }

      if ( CheckSpriteCollision(*iterSprite) )
      {
         (*iterSprite)->SetPosition(rcOldSpritePos);
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

void GameEngine::PlayMIDISong(PCWSTR MIDIFileName, BOOL restart)
{
   if ( m_MIDIPlayerID == 0 )
   {
      MCI_OPEN_PARMS mciOpenParms;

      mciOpenParms.lpstrDeviceType = L"sequencer";
      mciOpenParms.lpstrElementName = MIDIFileName;

      if ( mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT,
                          (DWORD_PTR) &mciOpenParms) == 0 )
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
      MCI_SEEK_PARMS mciSeekParms;

      if ( mciSendCommand(m_MIDIPlayerID, MCI_SEEK, MCI_SEEK_TO_START,
                          (DWORD_PTR) &mciSeekParms) != 0 )
      {
         CloseMIDIPlayer( );
      }
   }

   MCI_PLAY_PARMS mciPlayParms;

   if ( mciSendCommand(m_MIDIPlayerID, MCI_PLAY, 0,
                       (DWORD_PTR) &mciPlayParms) != 0 )
   {
      CloseMIDIPlayer( );
   }
}

void GameEngine::PauseMIDISong( )
{
   if ( m_MIDIPlayerID != 0 )
   {
      mciSendCommand(m_MIDIPlayerID, MCI_PAUSE, 0, NULL);
   }
}

void GameEngine::CloseMIDIPlayer( )
{
   if ( m_MIDIPlayerID != 0 )
   {
      mciSendCommand(m_MIDIPlayerID, MCI_CLOSE, 0, NULL);
      m_MIDIPlayerID = 0;
   }
}