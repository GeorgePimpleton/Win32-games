// GameEngine.hpp - game engine header

#pragma once

#include <windows.h>
#include <strsafe.h>
#include <memory>
#include <string>
#include "resource.h"

// a global for setting the size of the game engine's WCHAR strings
//  no longer needed with std::wstring
// const static UCHAR str_length { 64 };

//-----------------------------------------------------------------
// Windows© function declarations
//-----------------------------------------------------------------
int WINAPI       wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR szCmdLine, _In_ int iCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK    DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

//-----------------------------------------------------------------
// game engine function declarations
//-----------------------------------------------------------------
HRESULT GameInitialize(HINSTANCE hInstance);
void    GameStart(HWND hWindow);
void    GameEnd();
void    GameActivate(HWND hWindow);
void    GameDeactivate(HWND hWindow);
void    GamePaint(HDC hDC);
void    GameCycle();
void    GameMenu(WPARAM wParam);

//-----------------------------------------------------------------
// GameEngine class
//-----------------------------------------------------------------
class GameEngine
{
protected:
   // member variables
   static std::unique_ptr<GameEngine> m_pGameEngine;
   HINSTANCE                          m_hInstance;
   HWND                               m_hWindow;
   std::wstring                       m_szWindowClass;
   std::wstring                       m_szTitle;
   WORD                               m_wIcon;
   WORD                               m_wSmallIcon;
   UINT                               m_iWidth;
   UINT                               m_iHeight;
   UINT                               m_iFrameDelay;
   BOOL                               m_bSleep;

public:
            GameEngine(HINSTANCE hInstance, PCWSTR szWindowClass, PCWSTR szTitle,
                       WORD wIcon, WORD wSmallIcon, UINT iWidth = 640, UINT iHeight = 480);
   virtual ~GameEngine();

   // general methods
   static GameEngine* GetEngine();

   HRESULT Initialize(int iCmdShow);
   LRESULT  HandleEvent(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam);

   // accessor methods
   HINSTANCE GetInstance() const;
   HWND      GetWindow() const;
   void      SetWindow(HWND hWindow);
   PCWSTR    GetTitle() const;
   WORD      GetIcon() const;
   WORD      GetSmallIcon() const;
   UINT      GetWidth() const;
   UINT      GetHeight() const;
   UINT      GetFrameDelay() const;
   void      SetFrameRate(UINT iFrameRate);
   BOOL      GetSleep() const;
   void      SetSleep(BOOL bSleep);
};

// inline class methods

// returns the pointer to the game engine
inline GameEngine* GameEngine::GetEngine()            { return m_pGameEngine.get(); }

// return the current game instance
inline HINSTANCE GameEngine::GetInstance() const      { return m_hInstance; }

// returns the current game window
inline HWND GameEngine::GetWindow() const             { return m_hWindow; }

// sets the current game window
inline void GameEngine::SetWindow(HWND hWindow)       { m_hWindow = hWindow; }

// returns the current game title
inline PCWSTR GameEngine::GetTitle() const            { return m_szTitle.c_str(); }

// gets the current game icon
inline WORD GameEngine::GetIcon() const               { return m_wIcon; }

// gets the current small game icon
inline WORD GameEngine::GetSmallIcon() const          { return m_wSmallIcon; }

// gets the current game window width
inline UINT GameEngine::GetWidth() const              { return m_iWidth; }

// gets the current game window height
inline UINT GameEngine::GetHeight() const             { return m_iHeight; }

// gets the current game frame delay
inline UINT GameEngine::GetFrameDelay() const         { return m_iFrameDelay; }

// sets the current game frame rate
inline void GameEngine::SetFrameRate(UINT iFrameRate) { m_iFrameDelay = 1000 / iFrameRate; }

// gets the current game sleep status
inline BOOL GameEngine::GetSleep() const              { return m_bSleep; }

// sets the current game sleep status
inline void GameEngine::SetSleep(BOOL bSleep)         { m_bSleep = bSleep; }
