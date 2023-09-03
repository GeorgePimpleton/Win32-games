#pragma once

#include <windows.h>
#include <memory>
#include "resource.h"

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

class GameEngine
{
public:
            GameEngine(HINSTANCE inst, PCWSTR wndClass, PCWSTR title,
                       WORD icon, WORD smallIcon, UINT width = 640, UINT height = 480);
   virtual ~GameEngine( );

public:
   static GameEngine* GetEngine( );
   HRESULT            Initialize(int cmdShow);
   LRESULT            HandleEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

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
   void      SetSleep(BOOL asleep);

protected:
   static std::unique_ptr<GameEngine> m_gameEngine;
   HINSTANCE                          m_inst;
   HWND                               m_wnd;
   PCWSTR                             m_wndClass;
   PCWSTR                             m_title;
   WORD                               m_icon;
   WORD                               m_smallIcon;
   UINT                               m_width;
   UINT                               m_height;
   UINT                               m_frameDelay;
   BOOL                               m_asleep;
};

inline GameEngine* GameEngine::GetEngine( )          { return m_gameEngine.get( ); }

inline HINSTANCE GameEngine::GetInstance( ) const    { return m_inst; }

inline HWND GameEngine::GetWindow( ) const           { return m_wnd; }

inline void GameEngine::SetWindow(HWND hwnd)         { m_wnd = hwnd; }

inline PCWSTR GameEngine::GetTitle( )                { return m_title; }

inline WORD GameEngine::GetIcon( ) const             { return m_icon; }

inline WORD GameEngine::GetSmallIcon( ) const        { return m_icon; }

inline UINT GameEngine::GetWidth( ) const            { return m_width; }

inline UINT GameEngine::GetHeight( ) const           { return m_height; }

inline UINT GameEngine::GetFrameDelay( ) const       { return m_frameDelay; }

inline void GameEngine::SetFrameRate(UINT frameRate) { m_frameDelay = 1000 / frameRate; }

inline BOOL GameEngine::GetSleep( ) const            { return m_asleep; }

inline void GameEngine::SetSleep(BOOL asleep)        { m_asleep = asleep; }
