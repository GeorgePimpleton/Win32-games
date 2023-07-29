//-----------------------------------------------------------------
// Game Engine Object
// C++ Source - GameEngine.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// include files
//-----------------------------------------------------------------
#include "GameEngine.hpp"

#pragma comment(lib, "winmm.lib")

//-----------------------------------------------------------------
// static variable initialization
//-----------------------------------------------------------------
GameEngine* GameEngine::m_pGameEngine = NULL;

//-----------------------------------------------------------------
// Windows functions
//-----------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
   if ( GameInitialize(hInstance) )
   {
      // initialize the game engine
      if ( !GameEngine::GetEngine( )->Initialize(iCmdShow) )
      {
         return FALSE;
      }

      MSG msg;

      // enter the main message loop
      while ( TRUE )
      {
         if ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
         {
            // process the message
            if ( msg.message == WM_QUIT )
            {
               break;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
         }
         else
         {
            // make sure the game engine isn't sleeping
            if ( !GameEngine::GetEngine( )->GetSleep( ) )
            {
               // check the tick count to see if a game cycle has elapsed
               static int iTickTrigger = 0;
               int        iTickCount   = GetTickCount( );

               if ( iTickCount > iTickTrigger )
               {
                  iTickTrigger = iTickCount + GameEngine::GetEngine( )->GetFrameDelay( );

                  HandleKeys( );
                  GameEngine::GetEngine( )->CheckJoystick( );
                  GameCycle( );
               }
            }
         }
      }

      return (int) msg.wParam;
   }

   // end the game
   GameEnd( );

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
   // Route all Windows messages to the game engine
   return GameEngine::GetEngine( )->HandleEvent(hWindow, msg, wParam, lParam);
}

//-----------------------------------------------------------------
// Game Engine Helper Methods
//-----------------------------------------------------------------
BOOL GameEngine::CheckSpriteCollision(Sprite* pTestSprite)
{
   // see if the sprite has collided with any other sprites
   for ( auto siSprite { m_vSprites.begin( ) }; siSprite != m_vSprites.end( ); siSprite++ )
   {
      // make sure not to check for collision with itself
      if ( pTestSprite == (*siSprite) )
      {
         continue;
      }

      // test the collision
      if ( pTestSprite->TestCollision(*siSprite) )
      {
         // collision detected
         return SpriteCollision((*siSprite), pTestSprite);
      }
   }

   // no collision
   return FALSE;
}

//-----------------------------------------------------------------
// GameEngine constructor(s)/destructor
//-----------------------------------------------------------------
GameEngine::GameEngine(HINSTANCE hInstance, LPCTSTR szWindowClass, LPCTSTR szTitle,
                       WORD wIcon, WORD wSmallIcon, int iWidth, int iHeight)
{
   // set the member variables for the game engine
   m_pGameEngine = this;
   m_hInstance   = hInstance;
   m_hWindow     = NULL;

   if ( lstrlen(szWindowClass) > 0 )
   {
      lstrcpy(m_szWindowClass, szWindowClass);
   }
   if ( lstrlen(szTitle) > 0 )
   {
      lstrcpy(m_szTitle, szTitle);
   }

   m_wIcon        = wIcon;
   m_wSmallIcon   = wSmallIcon;
   m_iWidth       = iWidth;
   m_iHeight      = iHeight;
   m_iFrameDelay  = 50;   // 20 FPS default
   m_bSleep       = TRUE;
   m_uiJoystickID = 0;

   m_vSprites.reserve(100);

   m_uiMIDIPlayerID = 0;
}

GameEngine::~GameEngine( )
{ }

//-----------------------------------------------------------------
// Game Engine General Methods
//-----------------------------------------------------------------
BOOL GameEngine::Initialize(int iCmdShow)
{
   WNDCLASSEX wndclass;

   // create the window class for the main window
   wndclass.cbSize        = sizeof(wndclass);
   wndclass.style         = CS_HREDRAW | CS_VREDRAW;
   wndclass.lpfnWndProc   = WndProc;
   wndclass.cbClsExtra    = 0;
   wndclass.cbWndExtra    = 0;
   wndclass.hInstance     = m_hInstance;
   wndclass.hIcon         = (HICON) LoadImage(m_hInstance, MAKEINTRESOURCE(GetIcon( )), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
   wndclass.hIconSm       = (HICON) LoadImage(m_hInstance, MAKEINTRESOURCE(GetSmallIcon( )), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
   wndclass.hCursor       = (HCURSOR) LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
   wndclass.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
   wndclass.lpszMenuName  = NULL;
   wndclass.lpszClassName = m_szWindowClass;

   // register the window class
   if ( !RegisterClassEx(&wndclass) )
   {
      return FALSE;
   }

   // calculate the window size and position based upon the game size
   int iWindowWidth  = m_iWidth + GetSystemMetrics(SM_CXFIXEDFRAME) * 2;
   int iWindowHeight = m_iHeight + GetSystemMetrics(SM_CYFIXEDFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION);

   // Windows 10 under-calculates the width and height of the window's client area
   iWindowWidth += 10;
   iWindowHeight += 10;

   // add the size of the main menu, if defined
   if ( wndclass.lpszMenuName != NULL )
   {
      iWindowHeight += GetSystemMetrics(SM_CYMENU);
   }

   // position the window in the center of the primary display
   int iXWindowPos = (GetSystemMetrics(SM_CXSCREEN) - iWindowWidth) / 2;
   int iYWindowPos = (GetSystemMetrics(SM_CYSCREEN) - iWindowHeight) / 2;

   // create the window
   m_hWindow = CreateWindow(m_szWindowClass, m_szTitle, WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX,
                            iXWindowPos, iYWindowPos,
                            iWindowWidth, iWindowHeight,
                            NULL, NULL, m_hInstance, NULL);

   if ( !m_hWindow )
   {
      return FALSE;
   }

   // show and update the window
   ShowWindow(m_hWindow, iCmdShow);
   UpdateWindow(m_hWindow);

   return TRUE;
}

LRESULT GameEngine::HandleEvent(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
   // route windows messages to game engine member functions
   switch ( msg )
   {
   case WM_CREATE:
      // set the game window and start the game
      SetWindow(hWindow);
      GameStart(hWindow);
      return 0;

   case WM_SETFOCUS:
      // activate the game and update the sleep status
      GameActivate(hWindow);
      SetSleep(FALSE);
      return 0;

   case WM_KILLFOCUS:
      // deactivate the game and update the sleep status
      GameDeactivate(hWindow);
      SetSleep(TRUE);
      return 0;

   case WM_PAINT:
      HDC         hDC;
      PAINTSTRUCT ps;

      hDC = BeginPaint(hWindow, &ps);

      // paint the game
      GamePaint(hDC);

      EndPaint(hWindow, &ps);
      return 0;

   case WM_LBUTTONDOWN:
      // handle left mouse button press
      MouseButtonDown(LOWORD(lParam), HIWORD(lParam), TRUE);
      return 0;

   case WM_LBUTTONUP:
      // handle left mouse button release
      MouseButtonUp(LOWORD(lParam), HIWORD(lParam), TRUE);
      return 0;

   case WM_RBUTTONDOWN:
      // handle right mouse button press
      MouseButtonDown(LOWORD(lParam), HIWORD(lParam), FALSE);
      return 0;

   case WM_RBUTTONUP:
      // handle right mouse button release
      MouseButtonUp(LOWORD(lParam), HIWORD(lParam), FALSE);
      return 0;

   case WM_MOUSEMOVE:
      // handle mouse movement
      MouseMove(LOWORD(lParam), HIWORD(lParam));
      return 0;

   case WM_DESTROY:
      // end the game and exit the application
      GameEnd( );
      PostQuitMessage(0);
      return 0;
   }

   return DefWindowProc(hWindow, msg, wParam, lParam);
}

void GameEngine::ErrorQuit(LPCTSTR szErrorMsg)
{
   MessageBox(GetWindow( ), szErrorMsg, TEXT("Critical Error"), MB_OK | MB_ICONERROR);
   PostQuitMessage(0);
}

BOOL GameEngine::InitJoystick( )
{
   // make sure joystick driver is present
   UINT uiNumJoysticks;

   if ( (uiNumJoysticks = joyGetNumDevs( )) == 0 )
   {
      return FALSE;
   }

   // make sure the joystick is attached
   JOYINFO jiInfo;

   if ( joyGetPos(JOYSTICKID1, &jiInfo) != JOYERR_UNPLUGGED )
   {
      m_uiJoystickID = JOYSTICKID1;
   }
   else
   {
      return FALSE;
   }

   // calculate the trip values
   JOYCAPS jcCaps;

   joyGetDevCaps(m_uiJoystickID, &jcCaps, sizeof(JOYCAPS));

   DWORD dwXCenter = ((DWORD) jcCaps.wXmin + jcCaps.wXmax) / 2;
   DWORD dwYCenter = ((DWORD) jcCaps.wYmin + jcCaps.wYmax) / 2;

   m_rcJoystickTrip.left   = (jcCaps.wXmin + (WORD) dwXCenter) / 2;
   m_rcJoystickTrip.right  = (jcCaps.wXmax + (WORD) dwXCenter) / 2;
   m_rcJoystickTrip.top    = (jcCaps.wYmin + (WORD) dwYCenter) / 2;
   m_rcJoystickTrip.bottom = (jcCaps.wYmax + (WORD) dwYCenter) / 2;

   return TRUE;
}

void GameEngine::CaptureJoystick( )
{
   // capture the joystick
   if ( m_uiJoystickID == JOYSTICKID1 )
   {
      joySetCapture(m_hWindow, m_uiJoystickID, NULL, TRUE);
   }
}

void GameEngine::ReleaseJoystick( )
{
   // release the joystick
   if ( m_uiJoystickID == JOYSTICKID1 )
   {
      joyReleaseCapture(m_uiJoystickID);
   }
}

void GameEngine::CheckJoystick( )
{
   if ( m_uiJoystickID == JOYSTICKID1 )
   {
      JOYINFO  jiInfo;
      JOYSTATE jsJoystickState = 0;

      if ( joyGetPos(m_uiJoystickID, &jiInfo) == JOYERR_NOERROR )
      {
         // check horizontal movement
         if ( jiInfo.wXpos < (WORD) m_rcJoystickTrip.left )
         {
            jsJoystickState |= JOY_LEFT;
         }
         else if ( jiInfo.wXpos > (WORD)m_rcJoystickTrip.right )
         {
            jsJoystickState |= JOY_RIGHT;
         }

         // check vertical movement
         if ( jiInfo.wYpos < (WORD) m_rcJoystickTrip.top )
         {
            jsJoystickState |= JOY_UP;
         }
         else if ( jiInfo.wYpos > (WORD)m_rcJoystickTrip.bottom )
         {
            jsJoystickState |= JOY_DOWN;
         }

         // check buttons
         if ( jiInfo.wButtons & JOY_BUTTON1 )
         {
            jsJoystickState |= JOY_FIRE1;
         }
         if ( jiInfo.wButtons & JOY_BUTTON2 )
         {
            jsJoystickState |= JOY_FIRE2;
         }
      }

      // allow the game to handle the joystick
      HandleJoystick(jsJoystickState);
   }
}

void GameEngine::AddSprite(Sprite* pSprite)
{
   // add a sprite to the sprite vector
   if ( pSprite != NULL )
   {
      // see if there are sprites already in the sprite vector
      if ( m_vSprites.size( ) > 0 )
      {
         // find a spot in the sprite vector to insert the sprite by its z-order
         for ( auto siSprite { m_vSprites.begin( ) }; siSprite != m_vSprites.end( ); siSprite++ )
         {
            if ( pSprite->GetZOrder( ) < (*siSprite)->GetZOrder( ) )
            {
               // insert the sprite into the sprite vector
               m_vSprites.insert(siSprite, pSprite);
               return;
            }
         }
      }

      // the sprite's z-order is highest, so add it to the end of the vector
      m_vSprites.push_back(pSprite);
   }
}

void GameEngine::DrawSprites(HDC hDC)
{
   // draw the sprites in the sprite vector
   for ( auto siSprite { m_vSprites.begin( ) }; siSprite != m_vSprites.end( ); siSprite++ )
   {
      (*siSprite)->Draw(hDC);
   }
}

void GameEngine::UpdateSprites( )
{
   // check to see if the sprite vector needs to grow
   if ( m_vSprites.size( ) >= (m_vSprites.capacity( ) / 2) )
   {
      m_vSprites.reserve(m_vSprites.capacity( ) * 2);
   }

   // update the sprites in the sprite vector
   RECT         rcOldSpritePos;
   SPRITEACTION saSpriteAction;

   for ( auto siSprite = m_vSprites.begin( ); siSprite != m_vSprites.end( ); siSprite++ )
   {
      // save the old sprite position in case we need to restore it
      rcOldSpritePos = (*siSprite)->GetPosition( );

      // update the sprite
      saSpriteAction = (*siSprite)->Update( );

      // handle the sa_kill sprite action
      if ( saSpriteAction & SA_KILL )
      {
         // notify the game that the sprite is dying
         SpriteDying(*siSprite);

         // kill the sprite
         delete (*siSprite);
         m_vSprites.erase(siSprite);
         siSprite--;
         continue;
      }

      // see if the sprite collided with any others
      if ( CheckSpriteCollision(*siSprite) )
      {
         // restore the old sprite position
         (*siSprite)->SetPosition(rcOldSpritePos);
      }
   }
}

void GameEngine::CleanupSprites( )
{
   // delete and remove the sprites in the sprite vector
   for ( auto siSprite { m_vSprites.begin( ) }; siSprite != m_vSprites.end( ); siSprite++ )
   {
      delete (*siSprite);
      m_vSprites.erase(siSprite);
      siSprite--;
   }
}

Sprite* GameEngine::IsPointInSprite(int x, int y)
{
   // see if the point is in a sprite in the sprite vector
   for ( auto siSprite { m_vSprites.rbegin( ) }; siSprite != m_vSprites.rend( ); siSprite++ )
   {
      if ( !(*siSprite)->IsHidden( ) && (*siSprite)->IsPointInside(x, y) )
      {
         return (*siSprite);
      }
   }

   // the point is not in a sprite
   return NULL;
}

void GameEngine::PlayMIDISong(LPCTSTR szMIDIFileName, BOOL bRestart)
{
   // see if the MIDI player needs to be opened
   if ( m_uiMIDIPlayerID == 0 )
   {
      // open the MIDI player by specifying the device and filename
      MCI_OPEN_PARMS mciOpenParms;

      mciOpenParms.lpstrDeviceType  = TEXT("sequencer");
      mciOpenParms.lpstrElementName = szMIDIFileName;

      if ( mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD_PTR) &mciOpenParms) == 0 )
      {
         // get the ID for the MIDI player
         m_uiMIDIPlayerID = mciOpenParms.wDeviceID;
      }
      else
      {
         // there was a problem, so just return
         return;
      }
   }

   // restart the MIDI song, if necessary
   if ( bRestart )
   {
      MCI_SEEK_PARMS mciSeekParms;

      if ( mciSendCommand(m_uiMIDIPlayerID, MCI_SEEK, MCI_SEEK_TO_START, (DWORD_PTR) &mciSeekParms) != 0 )
      {
         // there was a problem, so close the MIDI player
         CloseMIDIPlayer( );
      }
   }

   // play the MIDI song
   MCI_PLAY_PARMS mciPlayParms;

   if ( mciSendCommand(m_uiMIDIPlayerID, MCI_PLAY, 0, (DWORD_PTR) &mciPlayParms) != 0 )
   {
      // there was a problem, so close the MIDI player
      CloseMIDIPlayer( );
   }
}

void GameEngine::PauseMIDISong( )
{
   // pause the currently playing song, if possible
   if ( m_uiMIDIPlayerID != 0 )
   {
      mciSendCommand(m_uiMIDIPlayerID, MCI_PAUSE, 0, NULL);
   }
}

void GameEngine::CloseMIDIPlayer( )
{
   // close the MIDI player, if possible
   if ( m_uiMIDIPlayerID != 0 )
   {
      mciSendCommand(m_uiMIDIPlayerID, MCI_CLOSE, 0, NULL);

      m_uiMIDIPlayerID = 0;
   }
}