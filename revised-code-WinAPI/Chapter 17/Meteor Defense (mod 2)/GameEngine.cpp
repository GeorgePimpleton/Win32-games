#pragma comment(lib, "winmm.lib")

#include "GameEngine.hpp"

GameEngine* GameEngine::m_gameEngine = NULL;

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR szCmdLine, _In_ int iCmdShow)
{

   if ( GameInitialize(hInstance) )
   {
      // Initialize the game engine
      if ( !GameEngine::GetEngine( )->Initialize(iCmdShow) )
         return FALSE;

      MSG msg;

      // Enter the main message loop
      while ( TRUE )
      {
         if ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
         {
            // Process the message
            if ( msg.message == WM_QUIT )
               break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
         }
         else
         {
            // Make sure the game engine isn't sleeping
            if ( !GameEngine::GetEngine( )->GetSleep( ) )
            {
               // Check the tick count to see if a game cycle has elapsed
               static ULONGLONG iTickTrigger = 0;
               ULONGLONG        iTickCount   = GetTickCount64( );

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

   // End the game
   GameEnd( );

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
   // Route all Windows messages to the game engine
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

GameEngine::GameEngine(HINSTANCE hInstance, PCWSTR szWindowClass, PCWSTR szTitle,
                       WORD wIcon, WORD wSmallIcon, int iWidth, int iHeight)
{
   // Set the member variables for the game engine
   m_gameEngine    = this;
   m_inst      = hInstance;
   m_wnd        = NULL;
   m_wndClass  = szWindowClass;
   m_title        = szTitle;
   m_icon          = wIcon;
   m_smallIcon     = wSmallIcon;
   m_width         = iWidth;
   m_height        = iHeight;
   m_frameDelay    = 50;   // 20 FPS default
   m_asleep         = TRUE;
   m_joyID   = 0;
   m_joyTrip = { };
   m_MIDIPlayerID = 0;

   m_sprites.reserve(100);
}

GameEngine::~GameEngine( )
{ }

BOOL GameEngine::Initialize(int iCmdShow)
{
   WNDCLASSEX    wndclass;

   // Create the window class for the main window
   wndclass.cbSize        = sizeof(wndclass);
   wndclass.style         = CS_HREDRAW | CS_VREDRAW;
   wndclass.lpfnWndProc   = WndProc;
   wndclass.cbClsExtra    = 0;
   wndclass.cbWndExtra    = 0;
   wndclass.hInstance     = m_inst;
   wndclass.hIcon         = LoadIcon(m_inst, MAKEINTRESOURCE(GetIcon( )));
   wndclass.hIconSm       = LoadIcon(m_inst, MAKEINTRESOURCE(GetSmallIcon( )));
   wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
   wndclass.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
   wndclass.lpszMenuName  = MAKEINTRESOURCEW(IDR_MENU);
   wndclass.lpszClassName = m_wndClass;

   // Register the window class
   if ( !RegisterClassEx(&wndclass) )
   {
      return FALSE;
   }

   // Calculate the window size and position based upon the game size
   int iWindowWidth = m_width + GetSystemMetrics(SM_CXFIXEDFRAME) * 2,
       iWindowHeight = m_height + GetSystemMetrics(SM_CYFIXEDFRAME) * 2 +
                       GetSystemMetrics(SM_CYCAPTION);

   if ( wndclass.lpszMenuName != NULL )
   {
      iWindowHeight += GetSystemMetrics(SM_CYMENU);
   }

   iWindowWidth += 10;
   iWindowHeight += 10;

 int iXWindowPos = (GetSystemMetrics(SM_CXSCREEN) - iWindowWidth) / 2,
     iYWindowPos = (GetSystemMetrics(SM_CYSCREEN) - iWindowHeight) / 2;

   // Create the window
   m_wnd = CreateWindow(m_wndClass, m_title, WS_POPUPWINDOW |
                            WS_CAPTION | WS_MINIMIZEBOX, iXWindowPos, iYWindowPos, iWindowWidth,
                            iWindowHeight, NULL, NULL, m_inst, NULL);
   if ( !m_wnd )
   {
      return FALSE;
   }

   // Show and update the window
   ShowWindow(m_wnd, iCmdShow);
   UpdateWindow(m_wnd);

   return TRUE;
}

LRESULT GameEngine::HandleEvent(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
   // Route Windows messages to game engine member functions
   switch ( msg )
   {
   case WM_CREATE:
      // Set the game window and start the game
      SetWindow(hWindow);
      GameStart(hWindow);
      return 0;

   case WM_SETFOCUS:
      // Activate the game and update the Sleep status
      GameActivate(hWindow);
      SetSleep(FALSE);
      return 0;

   case WM_KILLFOCUS:
      // Deactivate the game and update the Sleep status
      GameDeactivate(hWindow);
      SetSleep(TRUE);
      return 0;

   case WM_PAINT:
      HDC         hDC;
      PAINTSTRUCT ps;
      hDC = BeginPaint(hWindow, &ps);

      // Paint the game
      GamePaint(hDC);

      EndPaint(hWindow, &ps);
      return 0;

   case WM_LBUTTONDOWN:
      // Handle left mouse button press
      MouseButtonDown(LOWORD(lParam), HIWORD(lParam), TRUE);
      return 0;

   case WM_LBUTTONUP:
      // Handle left mouse button release
      MouseButtonUp(LOWORD(lParam), HIWORD(lParam), TRUE);
      return 0;

   case WM_RBUTTONDOWN:
      // Handle right mouse button press
      MouseButtonDown(LOWORD(lParam), HIWORD(lParam), FALSE);
      return 0;

   case WM_RBUTTONUP:
      // Handle right mouse button release
      MouseButtonUp(LOWORD(lParam), HIWORD(lParam), FALSE);
      return 0;

   case WM_MOUSEMOVE:
      // Handle mouse movement
      MouseMove(LOWORD(lParam), HIWORD(lParam));
      return 0;

   case WM_DESTROY:
      // End the game and exit the application
      GameEnd( );
      PostQuitMessage(0);
      return 0;
   }
   return DefWindowProc(hWindow, msg, wParam, lParam);
}

BOOL GameEngine::InitJoystick( )
{
   // Make sure joystick driver is present
   UINT uiNumJoysticks;

   if ( (uiNumJoysticks = joyGetNumDevs( )) == 0 )
   {
      return FALSE;
   }

   // Make sure the joystick is attached
   JOYINFO jiInfo;

   if ( joyGetPos(JOYSTICKID1, &jiInfo) != JOYERR_UNPLUGGED )
   {
      m_joyID = JOYSTICKID1;
   }
   else
   {
      return FALSE;
   }

   // Calculate the trip values
   JOYCAPS jcCaps;

   joyGetDevCaps(m_joyID, &jcCaps, sizeof(JOYCAPS));

   DWORD dwXCenter = ((DWORD) jcCaps.wXmin + jcCaps.wXmax) / 2;
   DWORD dwYCenter = ((DWORD) jcCaps.wYmin + jcCaps.wYmax) / 2;

   m_joyTrip.left   = (jcCaps.wXmin + (WORD) dwXCenter) / 2;
   m_joyTrip.right  = (jcCaps.wXmax + (WORD) dwXCenter) / 2;
   m_joyTrip.top    = (jcCaps.wYmin + (WORD) dwYCenter) / 2;
   m_joyTrip.bottom = (jcCaps.wYmax + (WORD) dwYCenter) / 2;

   return TRUE;
}

void GameEngine::CaptureJoystick( )
{
   // Capture the joystick
   if ( m_joyID == JOYSTICKID1 )
   {
      joySetCapture(m_wnd, m_joyID, NULL, TRUE);
   }
}

void GameEngine::ReleaseJoystick( )
{
   // Release the joystick
   if ( m_joyID == JOYSTICKID1 )
{      joyReleaseCapture(m_joyID);
}}

void GameEngine::CheckJoystick( )
{
   if ( m_joyID == JOYSTICKID1 )
   {
      JOYINFO jiInfo;
      JOYSTATE jsJoystickState = 0;

      if ( joyGetPos(m_joyID, &jiInfo) == JOYERR_NOERROR )
      {
         // Check horizontal movement
         if ( jiInfo.wXpos < (WORD) m_joyTrip.left )
         {
            jsJoystickState |= JOY_LEFT;
         }
         else if ( jiInfo.wXpos > (WORD)m_joyTrip.right )
         {
            jsJoystickState |= JOY_RIGHT;
         }

         // Check vertical movement
         if ( jiInfo.wYpos < (WORD) m_joyTrip.top )
         {
            jsJoystickState |= JOY_UP;
         }
         else if ( jiInfo.wYpos > (WORD)m_joyTrip.bottom )
         {
            jsJoystickState |= JOY_DOWN;
         }

         // Check buttons
         if ( jiInfo.wButtons & JOY_BUTTON1 )
         {
            jsJoystickState |= JOY_FIRE1;
         }

         if ( jiInfo.wButtons & JOY_BUTTON2 )
         {
            jsJoystickState |= JOY_FIRE2;
         }
      }

      // Allow the game to handle the joystick
      HandleJoystick(jsJoystickState);
   }
}

void GameEngine::AddSprite(Sprite* pSprite)
{
   // Add a sprite to the sprite vector
   if ( pSprite != NULL )
   {
      // See if there are sprites already in the sprite vector
      if ( m_sprites.size( ) > 0 )
      {
         // Find a spot in the sprite vector to insert the sprite by its z-order
         for ( auto siSprite = m_sprites.begin( ); siSprite != m_sprites.end( ); siSprite++ )
         {
            if ( pSprite->GetZOrder( ) < (*siSprite)->GetZOrder( ) )
            {
               // Insert the sprite into the sprite vector
               m_sprites.insert(siSprite, pSprite);
               return;
            }
         }
      }

      // The sprite's z-order is highest, so add it to the end of the vector
      m_sprites.push_back(pSprite);
   }
}

void GameEngine::DrawSprites(HDC hDC)
{
   // Draw the sprites in the sprite vector
   for ( auto siSprite = m_sprites.begin( ); siSprite != m_sprites.end( ); siSprite++ )
   {
      (*siSprite)->Draw(hDC);
   }
}

void GameEngine::UpdateSprites( )
{
   // Check to see if the sprite vector needs to grow
   if ( m_sprites.size( ) >= (m_sprites.capacity( ) / 2) )
   {
      m_sprites.reserve(m_sprites.capacity( ) * 2);
   }

   // Update the sprites in the sprite vector
   RECT         rcOldSpritePos;
   SPRITEACTION saSpriteAction;

   for ( auto siSprite = m_sprites.begin( ); siSprite != m_sprites.end( ); siSprite++ )
   {
      // Save the old sprite position in case we need to restore it
      rcOldSpritePos = (*siSprite)->GetPosition( );

      // Update the sprite
      saSpriteAction = (*siSprite)->Update( );

      // Handle the SA_KILL sprite action
      if ( saSpriteAction & SA_KILL )
      {
         // Notify the game that the sprite is dying
         SpriteDying(*siSprite);

         // Kill the sprite
         delete (*siSprite);
         m_sprites.erase(siSprite);
         siSprite--;
         continue;
      }

      // See if the sprite collided with any others
      if ( CheckSpriteCollision(*siSprite) )
      {         // Restore the old sprite position
         (*siSprite)->SetPosition(rcOldSpritePos);
      }
   }
}

void GameEngine::CleanupSprites( )
{
   // Delete and remove the sprites in the sprite vector
   for ( auto siSprite = m_sprites.begin( ); siSprite != m_sprites.end( ); siSprite++ )
   {
      delete (*siSprite);
      m_sprites.erase(siSprite);
      siSprite--;
   }
}

Sprite* GameEngine::IsPointInSprite(int x, int y)
{
   // See if the point is in a sprite in the sprite vector
   for ( auto siSprite = m_sprites.rbegin( ); siSprite != m_sprites.rend( ); siSprite++ )
   {
      if ( !(*siSprite)->IsHidden( ) && (*siSprite)->IsPointInside(x, y) )
      {
         return (*siSprite);
      }
   }

   // The point is not in a sprite
   return NULL;
}

BOOL GameEngine::CheckSpriteCollision(Sprite* pTestSprite)
{
   // See if the sprite has collided with any other sprites
   for ( auto siSprite = m_sprites.begin( ); siSprite != m_sprites.end( ); siSprite++ )
   {
      // Make sure not to check for collision with itself
      if ( pTestSprite == (*siSprite) )
      {
         continue;
      }

      // Test the collision
      if ( pTestSprite->TestCollision(*siSprite) )
      {        // Collision detected
         return SpriteCollision((*siSprite), pTestSprite);
      }
   }

   // No collision
   return FALSE;
}

void GameEngine::PlayMIDISong(PCWSTR MIDIFileName, BOOL restart)
{
   // See if the MIDI player needs to be opened
   if ( m_MIDIPlayerID == 0 )
   {
      // Open the MIDI player by specifying the device and filename
      MCI_OPEN_PARMSW mciOpenParms;

      mciOpenParms.lpstrDeviceType = L"sequencer";
      mciOpenParms.lpstrElementName = MIDIFileName;

      if ( mciSendCommandW(NULL, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT,
                           (DWORD_PTR) &mciOpenParms) == 0 )
      {
         // Get the ID for the MIDI player
         m_MIDIPlayerID = mciOpenParms.wDeviceID;
      }
      else
      {
         // There was a problem, so just return
         return;
      }
   }

   // Restart the MIDI song, if necessary
   if ( restart )
   {
      MCI_SEEK_PARMS mciSeekParms;

      if ( mciSendCommandW(m_MIDIPlayerID, MCI_SEEK, MCI_SEEK_TO_START,
                          (DWORD_PTR) &mciSeekParms) != 0 )
      {
         // There was a problem, so close the MIDI player
         CloseMIDIPlayer( );
      }
   }

   // Play the MIDI song
   MCI_PLAY_PARMS mciPlayParms;

   if ( mciSendCommandW(m_MIDIPlayerID, MCI_PLAY, 0,
                       (DWORD_PTR) &mciPlayParms) != 0 )
   {
      // There was a problem, so close the MIDI player
      CloseMIDIPlayer( );
   }
}

void GameEngine::PauseMIDISong( )
{
   // Pause the currently playing song, if possible
   if ( m_MIDIPlayerID != 0 )
   {
      mciSendCommandW(m_MIDIPlayerID, MCI_PAUSE, 0, NULL);
   }
}

void GameEngine::CloseMIDIPlayer( )
{
   // Close the MIDI player, if possible
   if ( m_MIDIPlayerID != 0 )
   {
      mciSendCommandW(m_MIDIPlayerID, MCI_CLOSE, 0, NULL);
      m_MIDIPlayerID = 0;
   }
}