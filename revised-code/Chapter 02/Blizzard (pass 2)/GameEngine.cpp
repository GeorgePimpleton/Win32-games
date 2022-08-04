// "Talk to me like I'm a 3 year old!" Programming Lessons - Windows Games

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

               TranslateMessage(&msg);
               DispatchMessageW(&msg);
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

// GameEngine constructor(s)/destructor
GameEngine::GameEngine(HINSTANCE hInstance, PCWSTR szWindowClass, PCWSTR szTitle,
                       WORD wIcon, WORD wSmallIcon, int iWidth, int iHeight)
{
   // set the member variables for the game engine
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

// game engine general methods
HRESULT GameEngine::Initialize(int iCmdShow)
{
   WNDCLASSEXW wndclass { };

   // create the window class for the main window
   wndclass.cbSize        = sizeof(WNDCLASSEXW);
   wndclass.style         = CS_HREDRAW | CS_VREDRAW;
   wndclass.lpfnWndProc   = WndProc;
   wndclass.cbClsExtra    = 0;
   wndclass.cbWndExtra    = 0;
   wndclass.hInstance     = m_hInstance;
   wndclass.hIcon         = (HICON)   LoadImageW(m_hInstance, MAKEINTRESOURCEW(GetIcon()), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
   wndclass.hIconSm       = (HICON)   LoadImageW(m_hInstance, MAKEINTRESOURCEW(GetSmallIcon()), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
   wndclass.hCursor       = (HCURSOR) LoadImageW(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
   wndclass.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
   wndclass.lpszMenuName  = NULL;
   wndclass.lpszClassName = m_szWindowClass;

   // register the window class
   if (!RegisterClassExW(&wndclass))
   {
      return E_FAIL;
   }

   // calculate the window size and position based upon the game size
   int iWindowWidth  { m_iWidth + GetSystemMetrics(SM_CXFIXEDFRAME) * 2 };
   int iWindowHeight { m_iHeight + GetSystemMetrics(SM_CYFIXEDFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION) };

   // position the window in the center of the primary display
   int iXWindowPos { (GetSystemMetrics(SM_CXSCREEN) - iWindowWidth) / 2 };
   int iYWindowPos { (GetSystemMetrics(SM_CYSCREEN) - iWindowHeight) / 2 };

   // create the window
   m_hWindow = CreateWindowW(m_szWindowClass, m_szTitle,
                             WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX,
                             iXWindowPos, iYWindowPos,
                             iWindowWidth, iWindowHeight,
                             NULL, NULL, m_hInstance, NULL);

   if (!m_hWindow)
   {
      return E_FAIL;
   }

   // show and update the window
   ShowWindow(m_hWindow, iCmdShow);
   UpdateWindow(m_hWindow);

   return S_OK;
}

LRESULT GameEngine::HandleEvent(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
   // route windows messages to game engine member functions
   switch (msg)
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

   case WM_DESTROY:
      // end the game and exit the application
      GameEnd();
      PostQuitMessage(0);
      return 0;
   }

   return DefWindowProcW(hWindow, msg, wParam, lParam);
}