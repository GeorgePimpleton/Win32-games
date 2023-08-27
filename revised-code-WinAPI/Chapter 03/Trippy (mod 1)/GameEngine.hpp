// GameEngine.hpp - game engine header

#pragma once

#include <windows.h>
#include <strsafe.h>
#include <memory>
#include <string>
#include "resource.h"
#include "random_toolkit.hpp"

// const static UCHAR str_length = 64;

int WINAPI       wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrevInst, _In_ PWSTR cmdLine, _In_ int cmdShow);
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK    DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

HRESULT GameInitialize(HINSTANCE hInst);
void    GameStart(HWND hwnd);
void    GameEnd( );
void    GameActivate(HWND hwnd);
void    GameDeactivate(HWND hwnd);
void    GamePaint(HDC hDC);
void    GameCycle( );
void    GameMenu(WPARAM wParam);

class GameEngine
{
public:
            GameEngine(HINSTANCE hInst, PCWSTR wndClass, PCWSTR title,
                       WORD icon, WORD smallIcon, UINT width = 640, UINT height = 480);
   virtual ~GameEngine();

public:
   static GameEngine* GetEngine( );
   HRESULT            Initialize(int cmdShow);
   LRESULT            HandleEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
   HINSTANCE GetInstance( ) const;
   HWND      GetWindow( ) const;
   void      SetWindow(HWND hWindow);
   PWSTR     GetTitle( );
   WORD      GetIcon( ) const;
   WORD      GetSmallIcon( ) const;
   UINT      GetWidth( ) const;
   UINT      GetHeight( ) const;
   UINT      GetFrameDelay( ) const;
   void      SetFrameRate(UINT iFrameRate);
   BOOL      GetSleep( ) const;
   void      SetSleep(BOOL bSleep);

protected:
   static std::shared_ptr<GameEngine> m_gameEngine;
   HINSTANCE                          m_hInst;
   HWND                               m_hwnd;
   std::wstring                       m_wndClass;
   std::wstring                       m_title;
   WORD                               m_icon;
   WORD                               m_smallIcon;
   UINT                               m_width;
   UINT                               m_height;
   UINT                               m_frameDelay;
   BOOL                               m_sleep;
};

inline GameEngine* GameEngine::GetEngine( )          { return m_gameEngine.get(); }

inline HINSTANCE GameEngine::GetInstance( ) const    { return m_hInst; }

inline HWND GameEngine::GetWindow( ) const           { return m_hwnd; }

inline void GameEngine::SetWindow(HWND hwnd)         { m_hwnd = hwnd; }

inline PWSTR GameEngine::GetTitle( )                 { return m_title.data(); }

inline WORD GameEngine::GetIcon( ) const             { return m_icon; }

inline WORD GameEngine::GetSmallIcon( ) const        { return m_icon; }

inline UINT GameEngine::GetWidth( ) const            { return m_width; }

inline UINT GameEngine::GetHeight( ) const           { return m_height; }

inline UINT GameEngine::GetFrameDelay( ) const       { return m_frameDelay; }

inline void GameEngine::SetFrameRate(UINT frameRate) { m_frameDelay = 1000 / frameRate; }

inline BOOL GameEngine::GetSleep( ) const            { return m_sleep; }

inline void GameEngine::SetSleep(BOOL sleep)         { m_sleep = sleep; }
