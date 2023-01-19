// GameEngine.hpp - game engine header

#pragma once

#include <windows.h>
#include <strsafe.h>
#include <memory>
#include <string>
#include "resource.h"
#include "random_toolkit.hpp"

const static UCHAR str_length = 64;

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

public:
   HINSTANCE GetInstance() const;
   HWND      GetWindow() const;
   void      SetWindow(HWND hWindow);
   PWSTR     GetTitle();
   WORD      GetIcon() const;
   WORD      GetSmallIcon() const;
   UINT      GetWidth() const;
   UINT      GetHeight() const;
   UINT      GetFrameDelay() const;
   void      SetFrameRate(UINT iFrameRate);
   BOOL      GetSleep() const;
   void      SetSleep(BOOL bSleep);

protected:
   static std::shared_ptr<GameEngine> m_pGameEngine;
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
};

inline GameEngine* GameEngine::GetEngine()            { return m_pGameEngine.get(); }

inline HINSTANCE GameEngine::GetInstance() const      { return m_hInstance; }

inline HWND GameEngine::GetWindow() const             { return m_hWindow; }

inline void GameEngine::SetWindow(HWND hWindow)       { m_hWindow = hWindow; }

inline PWSTR GameEngine::GetTitle()                   { return m_szTitle.data(); }

inline WORD GameEngine::GetIcon() const               { return m_wIcon; }

inline WORD GameEngine::GetSmallIcon() const          { return m_wIcon; }

inline UINT GameEngine::GetWidth() const              { return m_iWidth; }

inline UINT GameEngine::GetHeight() const             { return m_iHeight; }

inline UINT GameEngine::GetFrameDelay() const         { return m_iFrameDelay; }

inline void GameEngine::SetFrameRate(UINT iFrameRate) { m_iFrameDelay = 1000 / iFrameRate; }

inline BOOL GameEngine::GetSleep() const              { return m_bSleep; }

inline void GameEngine::SetSleep(BOOL bSleep)         { m_bSleep = bSleep; }
