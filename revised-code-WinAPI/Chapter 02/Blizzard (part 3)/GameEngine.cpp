#include "GameEngine.hpp"

// Static Variable Initialization
GameEngine* GameEngine::m_gameEngine = NULL;

int WINAPI WinMain(HINSTANCE inst, HINSTANCE prevInst, PSTR cmdLine, int cmdShow)
{
   MSG        msg;
   static int iTickTrigger = 0;
   int        iTickCount;

   if ( GameInitialize(inst) )
   {
      // Initialize the game engine
      if ( !GameEngine::GetEngine( )->Initialize(cmdShow) )
      {
         return FALSE;
      }

      // Enter the main message loop
      while ( TRUE )
      {
         if ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
         {
            // Process the message
            if ( msg.message == WM_QUIT )
            {
               break;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
         }
         else
         {
            // Make sure the game engine isn't sleeping
            if ( !GameEngine::GetEngine( )->GetSleep( ) )
            {
               // Check the tick count to see if a game cycle has elapsed
               iTickCount = GetTickCount( );

               if ( iTickCount > iTickTrigger )
               {
                  iTickTrigger = iTickCount + GameEngine::GetEngine( )->GetFrameDelay( );
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

// GameEngine Constructor(s)/Destructor
GameEngine::GameEngine(HINSTANCE inst, PCTSTR wndClass, PCTSTR title,
                       WORD icon, WORD smallIcon, int width, int height)
{
   // Set the member variables for the game engine
   m_gameEngine = this;
   m_inst       = inst;
   m_wnd        = NULL;

   if ( lstrlen(wndClass) > 0 )
   {
      lstrcpy(m_wndClass, wndClass);
   }

   if ( lstrlen(title) > 0 )
   {
      lstrcpy(m_title, title);
   }

   m_icon       = icon;
   m_smallIcon  = smallIcon;
   m_width      = width;
   m_height     = height;
   m_frameDelay = 50;   // 20 FPS default
   m_asleep     = TRUE;
}

GameEngine::~GameEngine( )
{ }

BOOL GameEngine::Initialize(int cmdShow)
{
   WNDCLASSEX wc;

   // Create the window class for the main window
   wc.cbSize        = sizeof(wc);
   wc.style         = CS_HREDRAW | CS_VREDRAW;
   wc.lpfnWndProc   = WndProc;
   wc.cbClsExtra    = 0;
   wc.cbWndExtra    = 0;
   wc.hInstance     = m_inst;
   wc.hIcon         = LoadIcon(m_inst, MAKEINTRESOURCE(GetIcon( )));
   wc.hIconSm       = LoadIcon(m_inst, MAKEINTRESOURCE(GetSmallIcon( )));
   wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
   wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
   wc.lpszMenuName  = NULL;
   wc.lpszClassName = m_wndClass;

   // Register the window class
   if ( !RegisterClassEx(&wc) )
   {
      return FALSE;
   }

   // Calculate the window size and position based upon the game size
   int windowWidth = m_width + GetSystemMetrics(SM_CXFIXEDFRAME) * 2;
   int windowHeight = m_height + GetSystemMetrics(SM_CYFIXEDFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION);

   if ( wc.lpszMenuName != NULL )
   {
      windowHeight += GetSystemMetrics(SM_CYMENU);
   }

   int xWindowPos = (GetSystemMetrics(SM_CXSCREEN) - windowWidth) / 2;
   int yWindowPos = (GetSystemMetrics(SM_CYSCREEN) - windowHeight) / 2;

   // Create the window
   m_wnd = CreateWindow(m_wndClass, m_title, WS_POPUPWINDOW |
                            WS_CAPTION | WS_MINIMIZEBOX, xWindowPos, yWindowPos, windowWidth,
                            windowHeight, NULL, NULL, m_inst, NULL);
   if ( !m_wnd )
   {
      return FALSE;
   }

   // Show and update the window
   ShowWindow(m_wnd, cmdShow);
   UpdateWindow(m_wnd);

   return TRUE;
}

LRESULT GameEngine::HandleEvent(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
   // Route Windows messages to game engine member functions
   switch ( msg )
   {
   case WM_CREATE:
      // Set the game window and start the game
      SetWindow(wnd);
      GameStart(wnd);
      return 0;

   case WM_SETFOCUS:
      // Activate the game and update the Sleep status
      GameActivate(wnd);
      SetSleep(FALSE);
      return 0;

   case WM_KILLFOCUS:
      // Deactivate the game and update the Sleep status
      GameDeactivate(wnd);
      SetSleep(TRUE);
      return 0;

   case WM_PAINT:
      HDC         hDC;
      PAINTSTRUCT ps;
      hDC = BeginPaint(wnd, &ps);

      // Paint the game
      GamePaint(hDC);

      EndPaint(wnd, &ps);
      return 0;

   case WM_DESTROY:
      // End the game and exit the application
      GameEnd( );
      PostQuitMessage(0);
      return 0;
   }
   return DefWindowProc(wnd, msg, wParam, lParam);
}