// "Talk to me like I'm a 3 year old!" Programming Lessons - Windows Games

// GameEngine.hpp - Game Engine Header

#pragma once

#include <windows.h>
#include <strsafe.h>
#include <vector>
#include "resource.h"
#include "Bitmap.hpp"
#include "Sprite.hpp"

const static UCHAR str_length = 64;

using JOYSTATE           = WORD;
const JOYSTATE JOY_NONE  = 0x0000L;
const JOYSTATE JOY_LEFT  = 0x0001L;
const JOYSTATE JOY_RIGHT = 0x0002L;
const JOYSTATE JOY_UP    = 0x0004L;
const JOYSTATE JOY_DOWN  = 0x0008L;
const JOYSTATE JOY_FIRE1 = 0x0010L;
const JOYSTATE JOY_FIRE2 = 0x0020L;

int WINAPI       wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR szCmdLine, _In_ int iCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK    DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

HRESULT GameInitialize(HINSTANCE hInstance);
void    GameStart(HWND hWindow);
void    GameEnd();
void    GameActivate(HWND hWindow);
void    GameDeactivate(HWND hWindow);
void    GamePaint(HDC hDC);
void    GameCycle();
void    GameMenu(WPARAM wParam);
void    HandleKeys();
void    MouseButtonDown(LONG x, LONG y, BOOL bLeft);
void    MouseButtonUp(LONG x, LONG y, BOOL bLeft);
void    MouseMove(LONG x, LONG y);
void    HandleJoystick(JOYSTATE jsJoystickState);
BOOL    SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee);
void    NewGame();

class GameEngine
{
public:
   GameEngine(HINSTANCE hInstance, PCWSTR szWindowClass, PCWSTR szTitle,
              WORD wIcon, WORD wSmallIcon, UINT iWidth = 640, UINT iHeight = 480);
   virtual ~GameEngine();

public:
   static GameEngine* GetEngine();
   HRESULT            Initialize(int iCmdShow);
   LRESULT            HandleEvent(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam);
   void               ErrorQuit(PCWSTR szErrorMsg);
   HRESULT            InitJoystick();
   void               CaptureJoystick();
   void               ReleaseJoystick();
   void               CheckJoystick();
   void               AddSprite(Sprite* pSprite);
   void               DrawSprites(HDC hDC);
   void               UpdateSprites();
   void               CleanupSprites();
   Sprite*            IsPointInSprite(LONG x, LONG y);

public:
   HINSTANCE GetInstance() const;
   HWND      GetWindow() const;
   void      SetWindow(HWND hWindow);
   PCWSTR    GetTitle();
   WORD      GetIcon() const;
   WORD      GetSmallIcon() const;
   UINT      GetWidth() const;
   UINT      GetHeight() const;
   UINT      GetFrameDelay() const;
   void      SetFrameRate(UINT iFrameRate);
   BOOL      GetSleep() const;
   void      SetSleep(BOOL bSleep);

protected:
   BOOL      CheckSpriteCollision(Sprite * pTestSprite);

protected:
   static GameEngine*   m_pGameEngine;
   HINSTANCE            m_hInstance;
   HWND                 m_hWindow;
   WCHAR                m_szWindowClass[str_length];
   WCHAR                m_szTitle[str_length];
   WORD                 m_wIcon;
   WORD                 m_wSmallIcon;
   UINT                 m_iWidth;
   UINT                 m_iHeight;
   UINT                 m_iFrameDelay;
   BOOL                 m_bSleep;
   UINT                 m_uiJoystickID;
   RECT                 m_rcJoystickTrip;
   std::vector<Sprite*> m_vSprites;
};

inline GameEngine* GameEngine::GetEngine()
{
   return m_pGameEngine;
}

inline void GameEngine::ErrorQuit(PCWSTR szErrorMsg)
{
   MessageBoxW(GetWindow(), szErrorMsg, L"Critical Error", MB_OK | MB_ICONERROR);
   PostQuitMessage(0);
}

inline HINSTANCE GameEngine::GetInstance() const
{
   return m_hInstance;
}

inline HWND GameEngine::GetWindow() const
{
   return m_hWindow;
}

inline void GameEngine::SetWindow(HWND hWindow)
{
   m_hWindow = hWindow;
}

inline PCWSTR GameEngine::GetTitle()
{
   return m_szTitle;
}

inline WORD GameEngine::GetIcon() const
{
   return m_wIcon;
}

inline WORD GameEngine::GetSmallIcon() const
{
   return m_wSmallIcon;
}

inline UINT GameEngine::GetWidth() const
{
   return m_iWidth;
}

inline UINT GameEngine::GetHeight() const
{
   return m_iHeight;
}

inline UINT GameEngine::GetFrameDelay() const
{
   return m_iFrameDelay;
}

inline void GameEngine::SetFrameRate(UINT iFrameRate)
{
   m_iFrameDelay = 1000 / iFrameRate;
}

inline BOOL GameEngine::GetSleep() const
{
   return m_bSleep;
}

inline void GameEngine::SetSleep(BOOL bSleep)
{
   m_bSleep = bSleep;
}
