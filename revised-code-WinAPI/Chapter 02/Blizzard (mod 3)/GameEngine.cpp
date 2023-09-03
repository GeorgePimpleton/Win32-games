// GameEngine.hpp - game engine header

// include files
#include "GameEngine.hpp"

// static variable initialization
GameEngine* GameEngine::m_pGameEngine = nullptr;

// Windows functions
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR szCmdLine, _In_ int iCmdShow)
{
   if (GameInitialize(hInstance) == S_OK)
   {
      MSG msg;

      // initialize the game engine
      if (GameEngine::GetEngine()->Initialize(iCmdShow) != S_OK)
      {
         return E_FAIL;
      }

      // load the accelerators
      HACCEL hAccel { LoadAcceleratorsW(hInstance, MAKEINTRESOURCEW(IDR_ACCEL)) };

      if (hAccel == NULL)
      {
         MessageBoxW(NULL, L"Unable to Load the Accelerators!", GameEngine::GetEngine()->GetTitle(), MB_OK | MB_ICONERROR);
         return E_FAIL;
      }

      // enter the main message loop
      while (TRUE)
      {
         if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
         {
            // process the message
            if (msg.message == WM_QUIT)
            {
               break;
            }

            if (0 == TranslateAcceleratorW(GameEngine::GetEngine()->GetWindow(), hAccel, &msg))
            {
               TranslateMessage(&msg);
               DispatchMessageW(&msg);
            }
         }
         else
         {
            // make sure the game engine isn't sleeping
            if (!GameEngine::GetEngine()->GetSleep())
            {
               // check the tick count to see if a game cycle has elapsed
               static ULONGLONG iTickTrigger { };
               ULONGLONG        iTickCount   { GetTickCount64() };

               if (iTickCount > iTickTrigger)
               {
                  iTickTrigger = iTickCount + GameEngine::GetEngine()->GetFrameDelay();

                  GameCycle();
               }
            }
         }
      }

      return (int) msg.wParam;
   }

   // end the game
   GameEnd();

   return S_OK;
}

// handles messages sent by Windows to the application
LRESULT CALLBACK WndProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
   // route all windows messages to the game engine
   return GameEngine::GetEngine()->HandleEvent(hWindow, msg, wParam, lParam);
}

// handles messages sent by Windows to the about dialog box
BOOL CALLBACK DlgProc(HWND hDialog, UINT message, WPARAM wParam, LPARAM lParam)
{
   // handle about dialog box messages
   switch (message)
   {
   case WM_COMMAND:
      switch (LOWORD(wParam))
      {
      case IDOK:
         EndDialog(hDialog, 0);
         return TRUE;
      }
   }

   return FALSE;
}

GameEngine::GameEngine(HINSTANCE hInstance, PCWSTR szWindowClass, PCWSTR szTitle,
                       WORD wIcon, WORD wSmallIcon, UINT iWidth, UINT iHeight)
{
   m_pGameEngine = this;
   m_hInstance   = hInstance;
   m_hWindow     = NULL;
   m_wIcon       = wIcon;
   m_wSmallIcon  = wSmallIcon;
   m_iWidth      = iWidth;
   m_iHeight     = iHeight;
   m_iFrameDelay = 50; // 20 FPS default
   m_bSleep      = TRUE;

   size_t  pcch { };
   HRESULT hRes = StringCchLengthW(szWindowClass, str_length, &pcch);
   if (pcch > 0)
   {
      StringCchCopyW(m_szWindowClass, str_length, szWindowClass);
   }
   else
   {
      StringCchCopyW(m_szWindowClass, str_length, L"");
   }

#pragma warning(disable: 28193)

   hRes = StringCchLengthW(szTitle, str_length, &pcch);
   if (pcch > 0)
   {
      StringCchCopyW(m_szTitle, str_length, szTitle);
   }
   else
   {
      StringCchCopyW(m_szTitle, str_length, L"");
   }
}

GameEngine::~GameEngine()
{ }

HRESULT GameEngine::Initialize(int iCmdShow)
{
   WNDCLASSEXW wc { };

   wc.cbSize        = sizeof(WNDCLASSEXW);
   wc.style         = CS_HREDRAW | CS_VREDRAW;
   wc.lpfnWndProc   = WndProc;
   wc.cbClsExtra    = 0;
   wc.cbWndExtra    = 0;
   wc.hInstance     = m_hInstance;
   wc.hIcon         = (HICON)   LoadImageW(m_hInstance, MAKEINTRESOURCEW(GetIcon()), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
   wc.hIconSm       = (HICON)   LoadImageW(m_hInstance, MAKEINTRESOURCEW(GetSmallIcon()), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
   wc.hCursor       = (HCURSOR) LoadImageW(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
   wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
   wc.lpszMenuName  = MAKEINTRESOURCEW(IDR_MENU);
   wc.lpszClassName = m_szWindowClass;

   if (!RegisterClassExW(&wc))
   {
      return E_FAIL;
   }

   UINT iWindowWidth  { m_iWidth + GetSystemMetrics(SM_CXFIXEDFRAME) * 2 };
   UINT iWindowHeight { m_iHeight + GetSystemMetrics(SM_CYFIXEDFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION) };

   // Windows 10 under-calculates the width and height of the window's client area
   iWindowWidth  += 10;
   iWindowHeight += 10;

   // add the size of the main menu, if defined
   if (wc.lpszMenuName != NULL)
   {
      iWindowHeight += GetSystemMetrics(SM_CYMENU);
   }

   UINT iXWindowPos { (GetSystemMetrics(SM_CXSCREEN) - iWindowWidth) / 2 };
   UINT iYWindowPos { (GetSystemMetrics(SM_CYSCREEN) - iWindowHeight) / 2 };

   m_hWindow = CreateWindowW(m_szWindowClass, m_szTitle,
                             WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX,
                             iXWindowPos, iYWindowPos,
                             iWindowWidth, iWindowHeight,
                             NULL, NULL, m_hInstance, NULL);

   if (!m_hWindow)
   {
      return E_FAIL;
   }

   ShowWindow(m_hWindow, iCmdShow);
   UpdateWindow(m_hWindow);

   return S_OK;
}

LRESULT GameEngine::HandleEvent(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
   switch (msg)
   {
   case WM_CREATE:
      SetWindow(hWindow);
      GameStart(hWindow);
      return S_OK;

   case WM_SETFOCUS:
      GameActivate(hWindow);
      SetSleep(FALSE);
      return S_OK;

   case WM_KILLFOCUS:
      GameDeactivate(hWindow);
      SetSleep(TRUE);
      return S_OK;

   case WM_COMMAND:
      GameMenu(wParam);
      return S_OK;

   case WM_PAINT:
   {
      PAINTSTRUCT ps;

      HDC hDC = BeginPaint(hWindow, &ps);

      // paint the game
      GamePaint(hDC);

      EndPaint(hWindow, &ps);
      return S_OK;
   }

   case WM_DESTROY:
      // end the game and exit the application
      GameEnd();
      PostQuitMessage(0);
      return S_OK;
   }

   return DefWindowProcW(hWindow, msg, wParam, lParam);
}