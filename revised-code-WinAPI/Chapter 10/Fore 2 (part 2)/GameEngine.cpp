#pragma comment(lib, "winmm.lib")

#include "GameEngine.hpp"

std::unique_ptr<GameEngine> GameEngine::m_gameEngine = NULL;

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
                       WORD icon, WORD smallIcon, UINT width, UINT height)
{
   m_gameEngine.reset(this);

   m_inst       = inst;
   m_wnd        = NULL;
   m_icon       = icon;
   m_smallIcon  = smallIcon;
   m_width      = width;
   m_height     = height;
   m_frameDelay = 50;
   m_asleep     = TRUE;
   m_joyID      = 0;
   m_joyTrip    = { };
   m_wndClass   = wndClass;
   m_title      = title;

   m_sprites.reserve(50);
}

GameEngine::~GameEngine( )
{ }

HRESULT GameEngine::Initialize(int cmdShow)
{
   WNDCLASSEXW wc = { };

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

   if ( FAILED(RegisterClassEx(&wc)) )
   {
      MessageBoxW(NULL, L"Unable to initialize Main Window!", L"ERROR", MB_ICONERROR | MB_OK);
      return E_FAIL;
   }

   UINT windowWidth  = m_width + GetSystemMetrics(SM_CXFIXEDFRAME) * 2 + 10;
   UINT windowHeight = m_height + GetSystemMetrics(SM_CYFIXEDFRAME) * 2
                                + GetSystemMetrics(SM_CYCAPTION) + 10;

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

      GamePaint(dc);

      EndPaint(wnd, &ps);
   }
   return S_OK;

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

HRESULT GameEngine::InitJoystick( )
{
   UINT numJoysticks = { };

   if ( 0 == (numJoysticks = joyGetNumDevs( )) )
   {
      return E_FAIL;
   }

   JOYINFO joyInfo = { };

   if ( joyGetPos(JOYSTICKID1, &joyInfo) != JOYERR_UNPLUGGED )
   {
      m_joyID = JOYSTICKID1;
   }
   else
   {
      return E_FAIL;
   }

   JOYCAPSW joyCaps = { };

   joyGetDevCapsW(m_joyID, &joyCaps, sizeof(JOYCAPSW));

   WORD xCenter = ((WORD) joyCaps.wXmin + joyCaps.wXmax) / 2;
   WORD yCenter = ((WORD) joyCaps.wYmin + joyCaps.wYmax) / 2;

   m_joyTrip.left   = (joyCaps.wXmin + (WORD) xCenter) / 2;
   m_joyTrip.right  = (joyCaps.wXmax + (WORD) xCenter) / 2;
   m_joyTrip.top    = (joyCaps.wYmin + (WORD) yCenter) / 2;
   m_joyTrip.bottom = (joyCaps.wYmax + (WORD) yCenter) / 2;

   return S_OK;
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
      JOYSTATE joyState = 0L;

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
   // add a sprite to the sprite vector
   if ( sprite != NULL )
   {
      // see if there are sprites already in the sprite vector
      if ( m_sprites.size( ) > 0 )
      {
         // find a spot in the sprite vector to insert the sprite by its z-order
         for ( auto iterSprite = m_sprites.begin( ); iterSprite != m_sprites.end( ); iterSprite++ )
         {
            if ( sprite->GetZOrder( ) < (*iterSprite)->GetZOrder( ) )
            {
               // insert the sprite into the sprite vector
               m_sprites.insert(iterSprite, sprite);
               return;
            }
         }
      }

      // the sprite's z-order is highest, so add it to the end of the vector
      m_sprites.push_back(sprite);
   }
}

void GameEngine::DrawSprites(HDC dc)
{
   // draw the sprites in the sprite vector
   for ( auto iterSprite = m_sprites.begin( ); iterSprite != m_sprites.end( ); iterSprite++ )
   {
      (*iterSprite)->Draw(dc);
   }
}

void GameEngine::UpdateSprites( )
{
   RECT         oldSpritePos = { };
   SPRITEACTION spriteAction = { };

   for ( auto iterSprite = m_sprites.begin( ); iterSprite != m_sprites.end( ); iterSprite++ )
   {
      // save the old sprite position in case we need to restore it
      oldSpritePos = (*iterSprite)->GetPosition( );

      // update the sprite
      spriteAction = (*iterSprite)->Update( );

      // handle the sa_kill sprite action
      if ( spriteAction & SA_KILL )
      {
         delete (*iterSprite);
         m_sprites.erase(iterSprite);
         iterSprite--;
         continue;
      }

      // see if the sprite collided with any others
      if ( CheckSpriteCollision(*iterSprite) )
      {
         // restore the old sprite position
         (*iterSprite)->SetPosition(oldSpritePos);
      }
   }
}

void GameEngine::CleanupSprites( )
{
   // delete and remove the sprites in the sprite vector
   for ( auto iterSprite = m_sprites.begin( ); iterSprite != m_sprites.end( ); iterSprite++ )
   {
      delete (*iterSprite);
      m_sprites.erase(iterSprite);
      iterSprite--;
   }
}

Sprite* GameEngine::IsPointInSprite(LONG x, LONG y)
{
   // see if the point is in a sprite in the sprite vector
   for ( auto iterSprite = m_sprites.rbegin( ); iterSprite != m_sprites.rend( ); iterSprite++ )
   {
      if ( !(*iterSprite)->IsHidden( ) && (*iterSprite)->IsPointInside(x, y) )
      {
         return (*iterSprite);
      }
   }

   // the point is not in a sprite
   return NULL;
}

BOOL GameEngine::CheckSpriteCollision(Sprite* testSprite)
{
   // see if the sprite has collided with any other sprites
   for ( auto iterSprite = m_sprites.begin( ); iterSprite != m_sprites.end( ); iterSprite++ )
   {
      // make sure not to check for collision with itself
      if ( testSprite == (*iterSprite) )
      {
         continue;
      }

      // test the collision
      if ( testSprite->TestCollision(*iterSprite) )
      {
         // collision detected
         return SpriteCollision((*iterSprite), testSprite);
      }
   }

   // no collision
   return FALSE;
}