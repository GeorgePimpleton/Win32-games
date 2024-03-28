#pragma once

#include <windows.h>
#include "resource.h"
#include <string>

int     WINAPI   wWinMain( _In_ HINSTANCE, _In_opt_ HINSTANCE,
                           _In_ PWSTR, _In_ int );
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );
BOOL  CALLBACK   DlgProc( HWND, UINT, WPARAM, LPARAM );

BOOL GameInitialize( HINSTANCE );
void GameStart( HWND );
void GameNew( );
void GameEnd( );
void GameActivate( HWND );
void GameDeactivate( HWND );
void GamePaint( HDC );
void GameCycle( );
void GameMenu( WPARAM );

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
   GameEngine( HINSTANCE, std::wstring, std::wstring,
               WORD, WORD, int width = 640, int height = 480 );
   virtual ~GameEngine( );

   static GameEngine* GetEngine( ) { return m_gameEngine; };
   BOOL               Initialize( int );
   LRESULT            HandleEvent( HWND, UINT, WPARAM, LPARAM );

   HINSTANCE    GetInstance( )                { return m_inst; };
   HWND         GetWindow( )                  { return m_wnd; };
   void         SetWindow( HWND wnd )         { m_wnd = wnd; };
   std::wstring GetTitle( )                   { return m_title; };
   WORD         GetIcon( )                    { return m_icon; };
   WORD         GetSmallIcon( )               { return m_smallIcon; };
   int          GetWidth( )                   { return m_width; };
   int          GetHeight( )                  { return m_height; };
   int          GetFrameDelay( )              { return m_frameDelay; };
   void         SetFrameRate( int frameRate ) { m_frameDelay = 1000 / frameRate; };
   BOOL         GetSleep( )                   { return m_asleep; };
   void         SetSleep( BOOL asleep )       { m_asleep = asleep; };
};
