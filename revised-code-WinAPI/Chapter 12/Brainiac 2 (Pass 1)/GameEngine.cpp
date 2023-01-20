// "Talk to me like I'm a 3 year old!" Programming Lessons - Windows Games

// GameEngine.cpp - Game Engine Source

#include "GameEngine.hpp"

#pragma comment(lib, "winmm.lib")

GameEngine* GameEngine::m_pGameEngine = NULL;

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR szCmdLine, _In_ int iCmdShow)
{
   if (GameInitialize(hInstance) == S_OK)
   {
      if (GameEngine::GetEngine()->Initialize(iCmdShow) != S_OK)
      {
         return E_FAIL;
      }

      HACCEL hAccel = LoadAcceleratorsW(hInstance, MAKEINTRESOURCEW(IDR_ACCELERATORS));

      if (hAccel == NULL)
      {
         MessageBoxW(NULL, L"Unable to Load the Accelerators!", GameEngine::GetEngine()->GetTitle(), MB_OK | MB_ICONERROR);
         return E_FAIL;
      }

      MSG msg;

      while (TRUE)
      {
         if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE) != 0)
         {
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
            if (GameEngine::GetEngine()->GetSleep() == FALSE)
            {
               static ULONGLONG iTickTrigger = 0;
               ULONGLONG        iTickCount   = GetTickCount64();

               if (iTickCount > iTickTrigger)
               {
                  iTickTrigger = iTickCount + GameEngine::GetEngine()->GetFrameDelay();

                  HandleKeys();
                  GameEngine::GetEngine()->CheckJoystick();
                  GameCycle();
               }
            }
         }
      }
      return (int) msg.wParam;
   }

   GameEnd();
   return S_OK;
}

LRESULT CALLBACK WndProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
   return GameEngine::GetEngine()->HandleEvent(hWindow, msg, wParam, lParam);
}

BOOL CALLBACK DlgProc(HWND hDialog, UINT message, WPARAM wParam, LPARAM lParam)
{
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
   m_pGameEngine    = this;
   m_hInstance      = hInstance;
   m_hWindow        = NULL;
   m_wIcon          = wIcon;
   m_wSmallIcon     = wSmallIcon;
   m_iWidth         = iWidth;
   m_iHeight        = iHeight;
   m_iFrameDelay    = 50;
   m_bSleep         = TRUE;
   m_uiJoystickID   = 0;
   m_rcJoystickTrip = {};

   m_vSprites.reserve(50);

   size_t  pcch = 0;
   HRESULT hRes = StringCchLengthW(szWindowClass, STR_LENGTH, &pcch);

   if (pcch > 0)
   {
      StringCchCopyW(m_szWindowClass, STR_LENGTH, szWindowClass);
   }
   else
   {
      StringCchCopyW(m_szWindowClass, STR_LENGTH, L"");
   }

#pragma warning(disable : 28193)

   hRes = StringCchLengthW(szTitle, STR_LENGTH, &pcch);

   if (pcch > 0)
   {
      StringCchCopyW(m_szTitle, STR_LENGTH, szTitle);
   }
   else
   {
      StringCchCopyW(m_szTitle, STR_LENGTH, L"");
   }
}

GameEngine::~GameEngine()
{
}

HRESULT GameEngine::Initialize(int iCmdShow)
{
   WNDCLASSEXW wc;

   wc.cbSize        = sizeof(WNDCLASSEXW);
   wc.style         = CS_HREDRAW | CS_VREDRAW;
   wc.lpfnWndProc   = WndProc;
   wc.cbClsExtra    = 0;
   wc.cbWndExtra    = 0;
   wc.hInstance     = m_hInstance;
   wc.hIcon         = (HICON)   LoadImageW(m_hInstance, MAKEINTRESOURCEW(IDI_ICON), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
   wc.hIconSm       = (HICON)   LoadImageW(m_hInstance, MAKEINTRESOURCEW(IDI_ICON_SM), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);;
   wc.hCursor       = (HCURSOR) LoadImageW(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
   wc.hbrBackground = (HBRUSH)  (COLOR_WINDOW + 1);
   wc.lpszMenuName  = MAKEINTRESOURCEW(IDR_MENU);
   wc.lpszClassName = m_szWindowClass;

   if (RegisterClassExW(&wc) == 0)
   {
      MessageBoxW(NULL, L"Unable to initialize Main Window!", L"ERROR", MB_ICONERROR | MB_OK);
      return E_FAIL;
   }

   UINT iWindowWidth  = m_iWidth  + GetSystemMetrics(SM_CXFIXEDFRAME) * 2;
   UINT iWindowHeight = m_iHeight + GetSystemMetrics(SM_CYFIXEDFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION);

   iWindowWidth  += 10;
   iWindowHeight += 10;

   if (wc.lpszMenuName != NULL)
   {
      iWindowHeight += GetSystemMetrics(SM_CYMENU);
   }

   UINT iWindowPosX = (GetSystemMetrics(SM_CXSCREEN) - iWindowWidth)  / 2;
   UINT iWindowPosY = (GetSystemMetrics(SM_CYSCREEN) - iWindowHeight) / 2;

   m_hWindow = CreateWindowW(m_szWindowClass, m_szTitle,
                             WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX,
                             iWindowPosX, iWindowPosY,
                             iWindowWidth, iWindowHeight,
                             NULL, NULL, m_hInstance, NULL);

   if (m_hWindow == NULL)
   {
      MessageBoxW(NULL, L"Unable to create Main Window!", L"ERROR", MB_ICONERROR | MB_OK);
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
      return 0;

   case WM_ACTIVATE:
      if (wParam != WA_INACTIVE)
      {
         GameActivate(hWindow);
         SetSleep(FALSE);
      }
      else
      {
         GameDeactivate(hWindow);
         SetSleep(TRUE);
      }
      return 0;

   case WM_COMMAND:
      GameMenu(wParam);
      return 0;

   case WM_PAINT:
      HDC         hDC;
      PAINTSTRUCT ps;

      hDC = BeginPaint(hWindow, &ps);

      GamePaint(hDC);

      EndPaint(hWindow, &ps);
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
      GameEnd();
      PostQuitMessage(0);
      return 0;
   }

   return DefWindowProcW(hWindow, msg, wParam, lParam);
}

HRESULT GameEngine::InitJoystick()
{
   UINT uiNumJoysticks;

   if ((uiNumJoysticks = joyGetNumDevs()) == 0)
   {
      return E_FAIL;
   }

   JOYINFO jiInfo;

   if (joyGetPos(JOYSTICKID1, &jiInfo) != JOYERR_UNPLUGGED)
   {
      m_uiJoystickID = JOYSTICKID1;
   }
   else
   {
      return E_FAIL;
   }

   JOYCAPS jcCaps;

   joyGetDevCapsW(m_uiJoystickID, &jcCaps, sizeof(JOYCAPS));

   DWORD dwXCenter = ((DWORD) jcCaps.wXmin + jcCaps.wXmax) / 2;
   DWORD dwYCenter = ((DWORD) jcCaps.wYmin + jcCaps.wYmax) / 2;

   m_rcJoystickTrip.left   = (jcCaps.wXmin + (WORD) dwXCenter) / 2;
   m_rcJoystickTrip.right  = (jcCaps.wXmax + (WORD) dwXCenter) / 2;
   m_rcJoystickTrip.top    = (jcCaps.wYmin + (WORD) dwYCenter) / 2;
   m_rcJoystickTrip.bottom = (jcCaps.wYmax + (WORD) dwYCenter) / 2;

   return TRUE;
}

void GameEngine::CaptureJoystick()
{
   if (m_uiJoystickID == JOYSTICKID1)
   {
      joySetCapture(m_hWindow, m_uiJoystickID, NULL, TRUE);
   }
}

void GameEngine::ReleaseJoystick()
{
   if (m_uiJoystickID == JOYSTICKID1)
   {
      joyReleaseCapture(m_uiJoystickID);
   }
}

void GameEngine::CheckJoystick()
{
   if (m_uiJoystickID == JOYSTICKID1)
   {
      JOYINFO  jiInfo;
      JOYSTATE jsJoystickState = 0;

      if (joyGetPos(m_uiJoystickID, &jiInfo) == JOYERR_NOERROR)
      {
         if (jiInfo.wXpos < (WORD) m_rcJoystickTrip.left)
         {
            jsJoystickState |= JOY_LEFT;
         }
         else if (jiInfo.wXpos > (WORD) m_rcJoystickTrip.right)
         {
            jsJoystickState |= JOY_RIGHT;
         }

         if (jiInfo.wYpos < (WORD) m_rcJoystickTrip.top)
         {
            jsJoystickState |= JOY_UP;
         }
         else if (jiInfo.wYpos > (WORD) m_rcJoystickTrip.bottom)
         {
            jsJoystickState |= JOY_DOWN;
         }

         if (jiInfo.wButtons & JOY_BUTTON1)
         {
            jsJoystickState |= JOY_FIRE1;
         }

         if (jiInfo.wButtons & JOY_BUTTON2)
         {
            jsJoystickState |= JOY_FIRE2;
         }
      }

      HandleJoystick(jsJoystickState);
   }
}

void GameEngine::AddSprite(Sprite* pSprite)
{
   if (pSprite != NULL)
   {
      if (m_vSprites.size() > 0)
      {
         for (auto siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)
         {
            if (pSprite->GetZOrder() < (*siSprite)->GetZOrder())
            {
               m_vSprites.insert(siSprite, pSprite);
               return;
            }
         }
      }

      m_vSprites.push_back(pSprite);
   }
}

void GameEngine::DrawSprites(HDC hDC)
{
   for (auto siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)
   {
      (*siSprite)->Draw(hDC);
   }
}

void GameEngine::UpdateSprites()
{
   RECT          rcOldSpritePos;
   SPRITEACTION  saSpriteAction;

   for (auto siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)
   {
      rcOldSpritePos = (*siSprite)->GetPosition();

      saSpriteAction = (*siSprite)->Update();

      if (saSpriteAction & SA_KILL)
      {
         delete (*siSprite);
         m_vSprites.erase(siSprite);
         siSprite--;
         continue;
      }

      if (CheckSpriteCollision(*siSprite))
      {
         (*siSprite)->SetPosition(rcOldSpritePos);
      }
   }
}

void GameEngine::CleanupSprites()
{
   for (auto siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)
   {
      delete (*siSprite);
      m_vSprites.erase(siSprite);
      siSprite--;
   }
}

Sprite* GameEngine::IsPointInSprite(int x, int y)
{
   for (auto siSprite = m_vSprites.rbegin(); siSprite != m_vSprites.rend(); siSprite++)
   {
      if (!(*siSprite)->IsHidden() && (*siSprite)->IsPointInside(x, y))
      {
         return (*siSprite);
      }
   }

   return NULL;
}

BOOL GameEngine::CheckSpriteCollision(Sprite* pTestSprite)
{
   for (auto siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)
   {
      if (pTestSprite == (*siSprite))
      {
         continue;
      }

      if (pTestSprite->TestCollision(*siSprite))
      {
         return SpriteCollision((*siSprite), pTestSprite);
      }
   }

   return FALSE;
}