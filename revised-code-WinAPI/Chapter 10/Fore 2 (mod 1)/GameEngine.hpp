#pragma once

#include <windows.h>
#include <strsafe.h>
#include <vector>
#include "resource.h"
#include "Bitmap.hpp"
#include "Sprite.hpp"

const static UCHAR str_length = 64;

using JOYSTATE = WORD;
const JOYSTATE JOY_NONE  = 0x0000L;
const JOYSTATE JOY_LEFT  = 0x0001L;
const JOYSTATE JOY_RIGHT = 0x0002L;
const JOYSTATE JOY_UP    = 0x0004L;
const JOYSTATE JOY_DOWN  = 0x0008L;
const JOYSTATE JOY_FIRE1 = 0x0010L;
const JOYSTATE JOY_FIRE2 = 0x0020L;

int WINAPI       wWinMain(_In_ HINSTANCE inst, _In_opt_ HINSTANCE prevInst, _In_ PWSTR cmdLine, _In_ int cmdShow);
LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK    DlgProc(HWND dlg, UINT msg, WPARAM wParam, LPARAM lParam);

HRESULT GameInitialize(HINSTANCE inst);
void    GameStart(HWND wnd);
void    GameEnd( );
void    GameActivate(HWND wnd);
void    GameDeactivate(HWND wnd);
void    GamePaint(HDC dc);
void    GameCycle( );
void    GameMenu(WPARAM wParam);
void    HandleKeys( );
void    MouseButtonDown(LONG x, LONG y, BOOL left);
void    MouseButtonUp(LONG x, LONG y, BOOL left);
void    MouseMove(LONG x, LONG y);
void    HandleJoystick(JOYSTATE joyState);
BOOL    SpriteCollision(Sprite* spriteHitter, Sprite* spriteHittee);

class GameEngine
{
public:
            GameEngine(HINSTANCE inst, PCWSTR wndClass, PCWSTR title,
                       WORD icon, WORD smallIcon, UINT width = 640, UINT height = 480);
   virtual ~GameEngine( );

public:
   static GameEngine* GetEngine( );
   HRESULT            Initialize(int cmdShow);
   LRESULT            HandleEvent(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
   void               ErrorQuit(PCWSTR errorMsg);
   HRESULT            InitJoystick( );
   void               CaptureJoystick( );
   void               ReleaseJoystick( );
   void               CheckJoystick( );
   void               AddSprite(Sprite* sprite);
   void               DrawSprites(HDC dc);
   void               UpdateSprites( );
   void               CleanupSprites( );
   Sprite*            IsPointInSprite(LONG x, LONG y);

public:
   HINSTANCE GetInstance( ) const;
   HWND      GetWindow( ) const;
   void      SetWindow(HWND wnd);
   PCWSTR    GetTitle( );
   WORD      GetIcon( ) const;
   WORD      GetSmallIcon( ) const;
   UINT      GetWidth( ) const;
   UINT      GetHeight( ) const;
   UINT      GetFrameDelay( ) const;
   void      SetFrameRate(UINT frameRate);
   BOOL      GetSleep( ) const;
   void      SetSleep(BOOL sleep);

protected:
   BOOL      CheckSpriteCollision(Sprite* testSprite);

protected:
   static GameEngine*   m_gameEngine;
   HINSTANCE            m_inst;
   HWND                 m_wnd;
   WCHAR                m_wndClass[ str_length ];
   WCHAR                m_title[ str_length ];
   WORD                 m_icon;
   WORD                 m_smallIcon;
   UINT                 m_width;
   UINT                 m_height;
   UINT                 m_frameDelay;
   BOOL                 m_sleep;
   UINT                 m_joyID;
   RECT                 m_joyTrip;
   std::vector<Sprite*> m_sprites;
};

inline GameEngine* GameEngine::GetEngine( )
{
   return m_gameEngine;
}

inline void GameEngine::ErrorQuit(PCWSTR errorMsg)
{
   MessageBoxW(GetWindow( ), errorMsg, L"Critical Error", MB_OK | MB_ICONERROR);
   PostQuitMessage(0);
}

inline HINSTANCE GameEngine::GetInstance( ) const
{
   return m_inst;
}

inline HWND GameEngine::GetWindow( ) const
{
   return m_wnd;
}

inline void GameEngine::SetWindow(HWND wnd)
{
   m_wnd = wnd;
}

inline PCWSTR GameEngine::GetTitle( )
{
   return m_title;
}

inline WORD GameEngine::GetIcon( ) const
{
   return m_icon;
}

inline WORD GameEngine::GetSmallIcon( ) const
{
   return m_smallIcon;
}

inline UINT GameEngine::GetWidth( ) const
{
   return m_width;
}

inline UINT GameEngine::GetHeight( ) const
{
   return m_height;
}

inline UINT GameEngine::GetFrameDelay( ) const
{
   return m_frameDelay;
}

inline void GameEngine::SetFrameRate(UINT frameRate)
{
   m_frameDelay = 1000 / frameRate;
}

inline BOOL GameEngine::GetSleep( ) const
{
   return m_sleep;
}

inline void GameEngine::SetSleep(BOOL sleep)
{
   m_sleep = sleep;
}
