#pragma once

#include <windows.h>
#include "resource.h"

int WINAPI       wWinMain( _In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int );
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );
BOOL CALLBACK    DlgProc( HWND, UINT, WPARAM, LPARAM );

HRESULT GameInitialize( HINSTANCE );
void    GameStart( HWND );
void    GameEnd( );
void    GameActivate( HWND );
void    GameDeactivate( HWND );
void    GamePaint( HDC );
void    GameCycle( );
void    GameMenu( WPARAM );
void    HandleKeys( );
void    MouseButtonDown( LONG, LONG, BOOL );
void    MouseButtonUp( LONG, LONG, BOOL );
void    MouseMove( LONG , LONG );

class GameEngine
{
public:
            GameEngine( HINSTANCE, PCWSTR, PCWSTR,
                        WORD, WORD, UINT = 640, UINT = 480 );
   virtual ~GameEngine( );

public:
   static GameEngine* GetEngine( ) { return m_gameEngine; };

   HRESULT            Initialize( int );
   LRESULT            HandleEvent( HWND, UINT, WPARAM, LPARAM );

public:
   HINSTANCE GetInstance( ) const;
   HWND      GetWindow( ) const;
   void      SetWindow( HWND );
   PCWSTR    GetTitle( );
   WORD      GetIcon( ) const;
   WORD      GetSmallIcon( ) const;
   UINT      GetWidth( ) const;
   UINT      GetHeight( ) const;
   UINT      GetFrameDelay( ) const;
   void      SetFrameRate( int );
   BOOL      GetSleep( ) const;
   void      SetSleep( BOOL );

protected:
   static GameEngine* m_gameEngine;
   HINSTANCE          m_inst;
   HWND               m_wnd;
   PCWSTR             m_wndClass;
   PCWSTR             m_title;
   WORD               m_icon;
   WORD               m_smallIcon;
   UINT               m_width;
   UINT               m_height;
   UINT               m_frameDelay;
   BOOL               m_asleep;
};

inline HINSTANCE GameEngine::GetInstance( ) const     { return m_inst; }
inline HWND GameEngine::GetWindow( ) const            { return m_wnd; }
inline void GameEngine::SetWindow( HWND wnd )         { m_wnd = wnd; }
inline PCWSTR GameEngine::GetTitle( )                 { return m_title; }
inline WORD GameEngine::GetIcon( ) const              { return m_icon; }
inline WORD GameEngine::GetSmallIcon( ) const         { return m_smallIcon; }
inline UINT GameEngine::GetWidth( ) const             { return m_width; }
inline UINT GameEngine::GetHeight( ) const            { return m_height; }
inline UINT GameEngine::GetFrameDelay( ) const        { return m_frameDelay; }
inline void GameEngine::SetFrameRate( int frameRate ) { m_frameDelay = 1000 / frameRate; }
inline BOOL GameEngine::GetSleep( ) const             { return m_asleep; }
inline void GameEngine::SetSleep( BOOL asleep )       { m_asleep = asleep; }
