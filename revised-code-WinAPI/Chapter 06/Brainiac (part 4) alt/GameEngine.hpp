#pragma once

#include <windows.h>
#include "resource.h"

int     WINAPI   wWinMain( _In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int );
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );
BOOL    CALLBACK DlgProc( HWND, UINT, WPARAM, LPARAM );

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
void    MouseMove( LONG, LONG );
void    GameNew( );

class GameEngine
{
public:
   GameEngine( HINSTANCE, PCWSTR, PCWSTR,
               WORD, WORD, UINT width = 640, UINT height = 480 );
   virtual ~GameEngine( );

public:
   static GameEngine* GetEngine( ) { return m_gameEngine; }

   HRESULT Initialize( int );
   LRESULT HandleEvent( HWND, UINT, WPARAM, LPARAM );

public:
   HINSTANCE GetInstance( ) const          { return m_inst; }
   HWND      GetWindow( ) const            { return m_wnd; }
   void      SetWindow( HWND wnd )         { m_wnd = wnd; }
   PCWSTR    GetTitle( )                   { return m_title; }
   WORD      GetIcon( ) const              { return m_icon; }
   WORD      GetSmallIcon( ) const         { return m_smallIcon; }
   UINT      GetWidth( ) const             { return m_width; }
   UINT      GetHeight( ) const            { return m_height; }
   UINT      GetFrameDelay( ) const        { return m_frameDelay; }
   void      SetFrameRate( int frameRate ) { m_frameDelay = 1000 / frameRate; }
   BOOL      GetSleep( ) const             { return m_asleep; }
   void      SetSleep( BOOL asleep )       { m_asleep = asleep; }

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
