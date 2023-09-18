#pragma once

#include <windows.h>
#include <vector>
#include "resource.h"
#include "Bitmap.hpp"
#include "Sprite.hpp"

using JOYSTATE          = WORD;
const JOYSTATE JOY_NONE = 0x0000L,
JOY_LEFT                = 0x0001L,
JOY_RIGHT               = 0x0002L,
JOY_UP                  = 0x0004L,
JOY_DOWN                = 0x0008L,
JOY_FIRE1               = 0x0010L,
JOY_FIRE2               = 0x0020L;

int WINAPI       wWinMain(_In_ HINSTANCE inst, _In_opt_ HINSTANCE prevInst, _In_ PWSTR cmdLine, _In_ int cmdShow);
LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK    DlgProc(HWND dlg, UINT msg, WPARAM wParam, LPARAM lParam);

HRESULT GameInitialize(HINSTANCE inst);
void    GameStart(HWND wnd);
void    GameNew( );
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

void MoveChicken(int xDistance, int yDistance);

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
   BOOL               InitJoystick( );
   void               CaptureJoystick( );
   void               ReleaseJoystick( );
   void               CheckJoystick( );
   void               AddSprite(Sprite* sprite);
   void               DrawSprites(HDC dc);
   void               UpdateSprites( );
   void               CleanupSprites( );
   Sprite*            IsPointInSprite(int x, int y);
   void               PlayMIDISong(PCWSTR MIDIFileName = L"", BOOL restart = TRUE);
   void               PauseMIDISong( );
   void               CloseMIDIPlayer( );

public:
   HINSTANCE   GetInstance( ) const;
   HWND        GetWindow( ) const;
   void        SetWindow(HWND wnd);
   PCWSTR      GetTitle( );
   WORD        GetIcon( ) const;
   WORD        GetSmallIcon( ) const;
   UINT        GetWidth( ) const;
   UINT        GetHeight( ) const;
   UINT        GetFrameDelay( ) const;
   void        SetFrameRate(UINT frameRate);
   BOOL        GetSleep( ) const;
   void        SetSleep(BOOL sleep);

protected:
   BOOL        CheckSpriteCollision(Sprite* testSprite);

protected:
   static GameEngine*   m_gameEngine;
   HINSTANCE            m_inst;
   HWND                 m_wnd;
   PCWSTR               m_wndClass;
   PCWSTR               m_title;
   WORD                 m_icon;
   WORD                 m_smallIcon;
   UINT                 m_width;
   UINT                 m_height;
   UINT                 m_frameDelay;
   BOOL                 m_asleep;
   UINT                 m_joyID;
   RECT                 m_joyTrip;
   std::vector<Sprite*> m_sprites;
   UINT                 m_MIDIPlayerID;
};

inline GameEngine* GameEngine::GetEngine( )
{
   return m_gameEngine;
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
   return m_asleep;
}

inline void GameEngine::SetSleep(BOOL asleep)
{
   m_asleep = asleep;
}
