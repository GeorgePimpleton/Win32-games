#pragma comment(lib, "winmm.lib")

#include "GameEngine.hpp"

std::unique_ptr<GameEngine> GameEngine::m_gameEngine = NULL;

int WINAPI wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrevInst, _In_ PWSTR cmdLine, _In_ int cmdShow)
{
   if ( GameInitialize(hInst) == S_OK )
   {
      if ( GameEngine::GetEngine( )->Initialize(cmdShow) != S_OK )
      {
         return E_FAIL;
      }

      HACCEL hAccel { LoadAcceleratorsW(hInst, MAKEINTRESOURCEW(IDR_ACCELERATORS)) };

      if ( NULL == hAccel )
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

            if ( 0 == TranslateAcceleratorW(GameEngine::GetEngine( )->GetWindow( ), hAccel, &msg) )
            {
               TranslateMessage(&msg);
               DispatchMessageW(&msg);
            }
         }
         else
         {
            if ( GameEngine::GetEngine( )->GetSleep( ) == FALSE )
            {
               static ULONGLONG tickTrigger { };
               ULONGLONG        tickCount   { GetTickCount64( ) };

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

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
   return GameEngine::GetEngine( )->HandleEvent(hwnd, msg, wParam, lParam);
}

BOOL CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
   switch ( message )
   {
   case WM_COMMAND:
      switch ( LOWORD(wParam) )
      {
      case IDOK:
         EndDialog(hDlg, 0);
         return TRUE;
      }
   }

   return FALSE;
}

GameEngine::GameEngine(HINSTANCE hInst, PCWSTR wndClass, PCWSTR title,
                       WORD icon, WORD smallIcon, UINT width, UINT height)
{
   m_gameEngine.reset(this);

   m_hInst      = hInst;
   m_hwnd       = NULL;
   m_icon       = icon;
   m_smallIcon  = smallIcon;
   m_width      = width;
   m_height     = height;
   m_frameDelay = 50;
   m_sleep      = TRUE;
   m_joyID      = 0;
   m_joyTrip    = { };

   size_t  pcch { };
   HRESULT hRes { StringCchLengthW(wndClass, str_length, &pcch) };

   if ( pcch > 0 )
   {
      StringCchCopyW(m_wndClass, str_length, wndClass);
   }
   else
   {
      StringCchCopyW(m_wndClass, str_length, L"");
   }

#pragma warning (disable : 28193)

   hRes = StringCchLengthW(title, str_length, &pcch);

   if ( pcch > 0 )
   {
      StringCchCopyW(m_title, str_length, title);
   }
   else
   {
      StringCchCopyW(m_title, str_length, L"");
   }
}

GameEngine::~GameEngine( )
{ }

HRESULT GameEngine::Initialize(int cmdShow)
{
   WNDCLASSEXW wc { };

   wc.cbSize        = sizeof(WNDCLASSEXW);
   wc.style         = CS_HREDRAW | CS_VREDRAW;
   wc.lpfnWndProc   = WndProc;
   wc.cbClsExtra    = 0;
   wc.cbWndExtra    = 0;
   wc.hInstance     = m_hInst;
   wc.hIcon         = (HICON)   LoadImageW(m_hInst, MAKEINTRESOURCEW(IDI_ICON), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
   wc.hIconSm       = (HICON)   LoadImageW(m_hInst, MAKEINTRESOURCEW(IDI_ICON_SM), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR); ;
   wc.hCursor       = (HCURSOR) LoadImageW(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
   wc.hbrBackground = (HBRUSH)  (COLOR_WINDOW + 1);
   wc.lpszMenuName  = MAKEINTRESOURCEW(IDR_MENU);
   wc.lpszClassName = m_wndClass;

   if ( FAILED(RegisterClassExW(&wc)) )
   {
      MessageBoxW(NULL, L"Unable to initialize Main Window!", L"ERROR", MB_ICONERROR | MB_OK);
      return E_FAIL;
   }

   UINT windowWidth  { m_width + GetSystemMetrics(SM_CXFIXEDFRAME) * 2 };
   UINT windowHeight { m_height + GetSystemMetrics(SM_CYFIXEDFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION) };

   windowWidth  += 10;
   windowHeight += 10;

   if ( wc.lpszMenuName != NULL )
   {
      windowHeight += GetSystemMetrics(SM_CYMENU);
   }

   UINT windowPosX { (GetSystemMetrics(SM_CXSCREEN) - windowWidth) / 2 };
   UINT windowPosY { (GetSystemMetrics(SM_CYSCREEN) - windowHeight) / 2 };

   m_hwnd = CreateWindowW(m_wndClass, m_title,
                          WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX,
                          windowPosX, windowPosY,
                          windowWidth, windowHeight,
                          NULL, NULL, m_hInst, NULL);

   if ( NULL == m_hwnd )
   {
      MessageBoxW(NULL, L"Unable to create Main Window!", L"ERROR", MB_ICONERROR | MB_OK);
      return E_FAIL;
   }

   ShowWindow(m_hwnd, cmdShow);
   UpdateWindow(m_hwnd);

   return S_OK;
}

LRESULT GameEngine::HandleEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
   switch ( msg )
   {
   case WM_CREATE:
      SetWindow(hwnd);
      GameStart(hwnd);
      return S_OK;

   case WM_ACTIVATE:
      if ( wParam != WA_INACTIVE )
      {
         GameActivate(hwnd);
         SetSleep(FALSE);
      }
      else
      {
         GameDeactivate(hwnd);
         SetSleep(TRUE);
      }
      return S_OK;

   case WM_COMMAND:
      GameMenu(wParam);
      return S_OK;

   case WM_PAINT:
   {
      PAINTSTRUCT ps;
      HDC         hDC = BeginPaint(hwnd, &ps);

      GamePaint(hDC);

      EndPaint(hwnd, &ps);
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
   return DefWindowProcW(hwnd, msg, wParam, lParam);
}

HRESULT GameEngine::InitJoystick( )
{
   UINT numJoysticks;

   if ( (numJoysticks = joyGetNumDevs( )) == 0 )
   {
      return E_FAIL;
   }

   JOYINFO jiInfo;

   if ( joyGetPos(JOYSTICKID1, &jiInfo) != JOYERR_UNPLUGGED )
   {
      m_joyID = JOYSTICKID1;
   }
   else
   {
      return E_FAIL;
   }

   JOYCAPS jcCaps;

   joyGetDevCaps(m_joyID, &jcCaps, sizeof(JOYCAPS));

   WORD dwXCenter { (WORD) (((WORD) jcCaps.wXmin + jcCaps.wXmax) / 2) };
   WORD dwYCenter { (WORD) (((WORD) jcCaps.wYmin + jcCaps.wYmax) / 2) };

   m_joyTrip.left = (jcCaps.wXmin + (WORD) dwXCenter) / 2;
   m_joyTrip.right = (jcCaps.wXmax + (WORD) dwXCenter) / 2;
   m_joyTrip.top = (jcCaps.wYmin + (WORD) dwYCenter) / 2;
   m_joyTrip.bottom = (jcCaps.wYmax + (WORD) dwYCenter) / 2;

   return S_OK;
}

void GameEngine::CaptureJoystick( )
{
   if ( m_joyID == JOYSTICKID1 )
   {
      joySetCapture(m_hwnd, m_joyID, NULL, TRUE);
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
      JOYINFO  jiInfo;
      JOYSTATE jsJoystickState { 0 };

      if ( joyGetPos(m_joyID, &jiInfo) == JOYERR_NOERROR )
      {
         if ( jiInfo.wXpos < (WORD) m_joyTrip.left )
         {
            jsJoystickState |= JOY_LEFT;
         }
         else if ( jiInfo.wXpos > (WORD) m_joyTrip.right )
         {
            jsJoystickState |= JOY_RIGHT;
         }

         if ( jiInfo.wYpos < (WORD) m_joyTrip.top )
         {
            jsJoystickState |= JOY_UP;
         }
         else if ( jiInfo.wYpos > (WORD) m_joyTrip.bottom )
         {
            jsJoystickState |= JOY_DOWN;
         }

         if ( jiInfo.wButtons & JOY_BUTTON1 )
         {
            jsJoystickState |= JOY_FIRE1;
         }

         if ( jiInfo.wButtons & JOY_BUTTON2 )
         {
            jsJoystickState |= JOY_FIRE2;
         }
      }

      HandleJoystick(jsJoystickState);
   }
}