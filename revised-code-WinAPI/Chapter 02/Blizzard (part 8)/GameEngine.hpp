#pragma once

#include <windows.h>
#include "resource.h"
#include <string>

int     WINAPI   wWinMain(_In_ HINSTANCE inst, _In_opt_ HINSTANCE prevInst,
                          _In_ PWSTR cmdLine, _In_ int cmdShow);
LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL  CALLBACK   DlgProc(HWND dlg, UINT msg, WPARAM wParam, LPARAM lParam);

BOOL GameInitialize(HINSTANCE inst);
void GameStart(HWND wnd);
void GameNew( );
void GameEnd( );
void GameActivate(HWND wnd);
void GameDeactivate(HWND wnd);
void GamePaint(HDC dc);
void GameCycle( );
void GameMenu(WPARAM wParam);

class GameEngine
{
protected:
   static GameEngine* m_gameEngine;
   HINSTANCE          m_inst;
   HWND               m_wnd;
   std::wstring       m_wndClass;
   std::wstring       m_title;
   WORD               m_icon;
   WORD               m_smallIcon;
   int                m_width;
   int                m_height;
   int                m_frameDelay;
   BOOL               m_asleep;

public:
            GameEngine(HINSTANCE inst, std::wstring wndClass, std::wstring title,
                       WORD icon, WORD smallIcon, int width = 640, int height = 480);
   virtual ~GameEngine( );

   static GameEngine* GetEngine( ) { return m_gameEngine; };
   BOOL               Initialize(int cmdShow);
   LRESULT            HandleEvent(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);

   HINSTANCE    GetInstance( )              { return m_inst; };
   HWND         GetWindow( )                { return m_wnd; };
   void         SetWindow(HWND wnd)         { m_wnd = wnd; };
   std::wstring GetTitle( )                 { return m_title; };
   WORD         GetIcon( )                  { return m_icon; };
   WORD         GetSmallIcon( )             { return m_smallIcon; };
   int          GetWidth( )                 { return m_width; };
   int          GetHeight( )                { return m_height; };
   int          GetFrameDelay( )            { return m_frameDelay; };
   void         SetFrameRate(int frameRate) { m_frameDelay = 1000 / frameRate; };
   BOOL         GetSleep( )                 { return m_asleep; };
   void         SetSleep(BOOL asleep)       { m_asleep = asleep; };
};
