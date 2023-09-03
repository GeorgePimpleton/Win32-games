#pragma once

#include <windows.h>
#include <strsafe.h>

const static UCHAR str_length { 64 };

int WINAPI       wWinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
                           _In_ PWSTR     szCmdLine, _In_     int       iCmdShow );
LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

HRESULT GameInitialize(HINSTANCE hInstance);
void    GameStart(HWND hWindow);
void    GameEnd( );
void    GameActivate(HWND hWindow);
void    GameDeactivate(HWND hWindow);
void    GamePaint(HDC hDC);
void    GameCycle( );

class GameEngine
{
protected:
   static GameEngine* m_pGameEngine;
   HINSTANCE           m_hInstance;
   HWND                m_hWindow;
   WCHAR               m_szWindowClass[ str_length ];
   WCHAR               m_szTitle[ str_length ];
   WORD                m_wIcon;
   WORD                m_wSmallIcon;
   int                 m_iWidth;
   int                 m_iHeight;
   int                 m_iFrameDelay;
   BOOL                m_bSleep;

public:
   // Constructor(s)/Destructor
            GameEngine( HINSTANCE hInstance, PCWSTR szWindowClass, PCWSTR szTitle,
                        WORD wIcon, WORD wSmallIcon, int iWidth = 640, int iHeight = 480 );
   virtual ~GameEngine( );

   // General Methods
   static GameEngine* GetEngine( ) { return m_pGameEngine; };
   HRESULT            Initialize( int iCmdShow );
   LRESULT            HandleEvent( HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam );

   // Accessor Methods
   HINSTANCE GetInstance( )                 { return m_hInstance; };
   HWND      GetWindow( )                   { return m_hWindow; };
   void      SetWindow( HWND hWindow )      { m_hWindow =  hWindow; };
   PCWSTR    GetTitle( )                    { return m_szTitle; };
   WORD      GetIcon( )                     { return m_wIcon; };
   WORD      GetSmallIcon( )                { return m_wSmallIcon; };
   int       GetWidth( )                    { return m_iWidth; };
   int       GetHeight( )                   { return m_iHeight; };
   int       GetFrameDelay( )               { return m_iFrameDelay; };
   void      SetFrameRate( int iFrameRate ) { m_iFrameDelay = 1000 / iFrameRate; };
   BOOL      GetSleep( )                    { return m_bSleep; };
   void      SetSleep( BOOL bSleep )        { m_bSleep = bSleep; };
};
