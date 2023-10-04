#pragma once

#include <windows.h>
#include "resource.h"
#include <mmsystem.h>
#include <vector>
#include <memory>
#include "Sprite.hpp"

typedef WORD    JOYSTATE;
const JOYSTATE  JOY_NONE  = 0x0000L,
                JOY_LEFT  = 0x0001L,
                JOY_RIGHT = 0x0002L,
                JOY_UP    = 0x0004L,
                JOY_DOWN  = 0x0008L,
                JOY_FIRE1 = 0x0010L,
                JOY_FIRE2 = 0x0020L;

int WINAPI      wWinMain(_In_ HINSTANCE inst, _In_opt_ HINSTANCE prevInst,
                         _In_ PWSTR cmdLine, _In_ int cmdShow);
LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL    CALLBACK DlgProc(HWND dlg, UINT msg, WPARAM wParam, LPARAM lParam);

BOOL GameInitialize(HINSTANCE inst);
void GameStart(HWND wnd);
void GameNew( );
void GameEnd( );
void GameActivate(HWND wnd);
void GameDeactivate(HWND wnd);
void GamePaint(HDC dc);
void GameCycle( );
void GameMenu(WPARAM wParam);
void HandleKeys( );
void MouseButtonDown(int x, int y, BOOL left);
void MouseButtonUp(int x, int y, BOOL left);
void MouseMove(int x, int y);
void HandleJoystick(JOYSTATE joyState);
BOOL SpriteCollision(Sprite* spriteHitter, Sprite* spriteHittee);
void SpriteDying(Sprite* spriteDying);

class GameEngine
{
protected:
   static std::unique_ptr<GameEngine> m_gameEngine;
   HINSTANCE                          m_inst;
   HWND                               m_wnd;
   PCWSTR                             m_wndClass;
   PCWSTR                             m_title;
   WORD                               m_icon;
   WORD                               m_smallIcon;
   LONG                               m_width;
   LONG                               m_height;
   LONG                               m_frameDelay;
   BOOL                               m_asleep;
   UINT                               m_joyID;
   RECT                               m_joyTrip;
   std::vector<Sprite*>               m_sprites;
   UINT                               m_MIDIPlayerID;

   BOOL CheckSpriteCollision(Sprite* testSprite);

public:
            GameEngine(HINSTANCE inst, PCWSTR wndClass, PCWSTR title,
                       WORD icon, WORD smallIcon, int width = 640, int height = 480);
   virtual ~GameEngine( );

   static GameEngine* GetEngine( )                       { return m_gameEngine.get( ); };
   BOOL               Initialize(int cmdShow);
   LRESULT            HandleEvent(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
   BOOL               InitJoystick( );
   void               CaptureJoystick( );
   void               ReleaseJoystick( );
   void               CheckJoystick( );
   void               AddSprite(Sprite* sprite);
   void               DrawSprites(HDC dc);
   void               UpdateSprites( );
   void               CleanupSprites( );
   Sprite*            IsPointInSprite(int x, int y);
   void               PlayMIDISong(PCWSTR MIDIFileName = L"", BOOL bRestart = TRUE);
   void               PauseMIDISong( );
   void               CloseMIDIPlayer( );

   HINSTANCE GetInstance( ) const          { return m_inst; };
   HWND      GetWindow( ) const            { return m_wnd; };
   void      SetWindow(HWND wnd)           { m_wnd = wnd; };
   PCWSTR    GetTitle( ) const             { return m_title; };
   WORD      GetIcon( ) const              { return m_icon; };
   WORD      GetSmallIcon( ) const         { return m_smallIcon; };
   LONG      GetWidth( ) const             { return m_width; };
   LONG      GetHeight( ) const            { return m_height; };
   LONG      GetFrameDelay( ) const        { return m_frameDelay; };
   void      SetFrameRate(LONG iFrameRate) { m_frameDelay = 1000 / iFrameRate; };
   BOOL      GetSleep( ) const             { return m_asleep; };
   void      SetSleep(BOOL asleep)         { m_asleep = asleep; };
};
