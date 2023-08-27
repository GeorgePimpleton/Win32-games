#pragma comment(lib, "winmm.lib")

#include "GameEngine.hpp"

std::unique_ptr<GameEngine> GameEngine::m_gameEngine = nullptr;

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
         if ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0 )
         {
            if ( msg.message == WM_QUIT )
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
               static ULONGLONG iTickTrigger { };
               ULONGLONG        iTickCount { GetTickCount64( ) };

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

   GameEnd( );
   return S_OK;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
   return GameEngine::GetEngine( )->HandleEvent(hwnd, msg, wParam, lParam);
}

BOOL CALLBACK DlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
   switch ( msg )
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

   m_hInst        = hInst;
   m_hwnd      = NULL;
   m_icon         = icon;
   m_smallIcon    = smallIcon;
   m_width        = width;
   m_height       = height;
   m_frameDelay   = 50;
   m_sleep        = TRUE;
   m_joyID   = 0;
   m_joyTrip = { };

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

#pragma warning(disable : 28193)

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
   return DefWindowProc(hwnd, msg, wParam, lParam);
}

// initializes the joystick
HRESULT GameEngine::InitJoystick( )
{
   // make sure joystick driver is present
   UINT numJoysticks;

   if ( (numJoysticks = joyGetNumDevs( )) == 0 )
   {
      return E_FAIL;
   }

   // make sure the joystick is attached
   JOYINFO joyInfo;

   if ( joyGetPos(JOYSTICKID1, &joyInfo) != JOYERR_UNPLUGGED )
   {
      m_joyID = JOYSTICKID1;
   }
   else
   {
      return E_FAIL;
   }

   // calculate the trip values
   JOYCAPS joyCaps;

   joyGetDevCaps(m_joyID, &joyCaps, sizeof(JOYCAPS));

   WORD xCenter { (WORD) (((WORD) joyCaps.wXmin + joyCaps.wXmax) / 2) };
   WORD yCenter { (WORD) (((WORD) joyCaps.wYmin + joyCaps.wYmax) / 2) };

   m_joyTrip.left   = (joyCaps.wXmin + (WORD) xCenter) / 2;
   m_joyTrip.right  = (joyCaps.wXmax + (WORD) xCenter) / 2;
   m_joyTrip.top    = (joyCaps.wYmin + (WORD) yCenter) / 2;
   m_joyTrip.bottom = (joyCaps.wYmax + (WORD) yCenter) / 2;

   return S_OK;
}

// captures the joystick
void GameEngine::CaptureJoystick( )
{
   // capture the joystick
   if ( m_joyID == JOYSTICKID1 )
   {
      joySetCapture(m_hwnd, m_joyID, NULL, TRUE);
   }
}

// releases the joystick
void GameEngine::ReleaseJoystick( )
{
   // release the joystick
   if ( m_joyID == JOYSTICKID1 )
   {
      joyReleaseCapture(m_joyID);
   }
}

// handles joystick changes
void GameEngine::CheckJoystick( )
{
   if ( m_joyID == JOYSTICKID1 )
   {
      JOYINFO  joyInfo;
      JOYSTATE joyState { 0 };

      if ( joyGetPos(m_joyID, &joyInfo) == JOYERR_NOERROR )
      {
         // check horizontal movement
         if ( joyInfo.wXpos < (WORD) m_joyTrip.left )
         {
            joyState |= JOY_LEFT;
         }
         else if ( joyInfo.wXpos > (WORD) m_joyTrip.right )
         {
            joyState |= JOY_RIGHT;
         }

         // check vertical movement
         if ( joyInfo.wYpos < (WORD) m_joyTrip.top )
         {
            joyState |= JOY_UP;
         }
         else if ( joyInfo.wYpos > (WORD) m_joyTrip.bottom )
         {
            joyState |= JOY_DOWN;
         }

         // check if 1st and 2nd buttons were pressed
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