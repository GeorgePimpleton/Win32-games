#pragma once

#include <windows.h>

int WINAPI        WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                          PSTR szCmdLine, int iCmdShow);
LRESULT CALLBACK  WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Game Engine Function Declarations
BOOL GameInitialize(HINSTANCE hInstance);
void GameStart(HWND hWindow);
void GameEnd( );
void GameActivate(HWND hWindow);
void GameDeactivate(HWND hWindow);
void GamePaint(HDC hDC);
void GameCycle( );

// GameEngine Class
class GameEngine
{
protected:
   static GameEngine* m_pGameEngine;
   HINSTANCE          m_hInstance;
   HWND               m_hWindow;
   TCHAR              m_szWindowClass[ 64 ];
   TCHAR              m_szTitle[ 64 ];
   WORD               m_wIcon;
   WORD               m_wSmallIcon;
   int                m_iWidth;
   int                m_iHeight;
   int                m_iFrameDelay;
   BOOL               m_bSleep;

public:
            GameEngine(HINSTANCE hInstance, PCTSTR szWindowClass, PCTSTR szTitle,
                       WORD wIcon, WORD wSmallIcon, int iWidth = 640, int iHeight = 480);
   virtual ~GameEngine( );

   static GameEngine* GetEngine( ) { return m_pGameEngine; };
   BOOL               Initialize(int iCmdShow);
   LRESULT            HandleEvent(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam);

   HINSTANCE GetInstance( )               { return m_hInstance; };
   HWND      GetWindow( )                 { return m_hWindow; };
   void      SetWindow(HWND hWindow)      { m_hWindow = hWindow; };
   LPTSTR    GetTitle( )                  { return m_szTitle; };
   WORD      GetIcon( )                   { return m_wIcon; };
   WORD      GetSmallIcon( )              { return m_wSmallIcon; };
   int       GetWidth( )                  { return m_iWidth; };
   int       GetHeight( )                 { return m_iHeight; };
   int       GetFrameDelay( )             { return m_iFrameDelay; };
   void      SetFrameRate(int iFrameRate) { m_iFrameDelay = 1000 / iFrameRate; };
   BOOL      GetSleep( )                  { return m_bSleep; };
   void      SetSleep(BOOL bSleep)        { m_bSleep = bSleep; };
};
