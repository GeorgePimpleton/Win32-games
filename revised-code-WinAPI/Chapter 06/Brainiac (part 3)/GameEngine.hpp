#pragma once

#include <windows.h>
#include <memory>
#include "resource.h"

int WINAPI       wWinMain(_In_ HINSTANCE inst, _In_opt_ HINSTANCE prevInst,
                          _In_ PWSTR cmdLine, _In_ int cmdShow);
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
void    GameNew( );

class GameEngine
{
public:
            GameEngine(HINSTANCE inst, PCWSTR wndClass, PCWSTR title,
                       WORD icon, WORD smallIcon, UINT width = 640, UINT height = 480);
   virtual ~GameEngine( );

public:
   static GameEngine* GetEngine( ) { return m_gameEngine.get( ); }
   HRESULT            Initialize(int cmdShow);
   LRESULT            HandleEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
   HINSTANCE GetInstance( ) const        { return m_inst; }
   HWND      GetWindow( ) const          { return m_wnd; }
   void      SetWindow(HWND wnd)         { m_wnd = wnd; }
   PCWSTR    GetTitle( )                 { return m_title; }
   WORD      GetIcon( ) const            { return m_icon; }
   WORD      GetSmallIcon( ) const       { return m_smallIcon; }
   UINT      GetWidth( ) const           { return m_width; }
   UINT      GetHeight( ) const          { return m_height; }
   UINT      GetFrameDelay( ) const      { return m_frameDelay; }
   void      SetFrameRate(int frameRate) { m_frameDelay = 1000 / frameRate; }
   BOOL      GetSleep( ) const           { return m_asleep; }
   void      SetSleep(BOOL asleep)       { m_asleep = asleep; }

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
