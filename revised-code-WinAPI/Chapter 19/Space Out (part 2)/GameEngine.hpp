#pragma once

#include <windows.h>
#include <mmsystem.h>
#include <vector>
using namespace std;
#include "Sprite.hpp"
#include "resource.h"

typedef WORD    JOYSTATE;
const JOYSTATE  JOY_NONE  = 0x0000L,
                JOY_LEFT  = 0x0001L,
                JOY_RIGHT = 0x0002L,
                JOY_UP    = 0x0004L,
                JOY_DOWN  = 0x0008L,
                JOY_FIRE1 = 0x0010L,
                JOY_FIRE2 = 0x0020L;

int WINAPI       wWinMain(_In_ HINSTANCE inst, _In_opt_ HINSTANCE hPrevInstance,
                          _In_ PWSTR szCmdLine, _In_ int iCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL    CALLBACK DlgProc(HWND dlg, UINT msg, WPARAM wParam, LPARAM lParam);

BOOL GameInitialize(HINSTANCE inst);
void GameStart(HWND hWindow);
void GameEnd( );
void GameActivate(HWND hWindow);
void GameDeactivate(HWND hWindow);
void GamePaint(HDC dc);
void GameCycle( );
void GameMenu(WPARAM wParam);
void HandleKeys( );
void MouseButtonDown(int x, int y, BOOL bLeft);
void MouseButtonUp(int x, int y, BOOL bLeft);
void MouseMove(int x, int y);
void HandleJoystick(JOYSTATE jsJoystickState);
BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee);
void SpriteDying(Sprite* pSpriteDying);

class GameEngine
{
protected:
   static GameEngine* m_pGameEngine;
   HINSTANCE          m_inst;
   HWND               m_hWindow;
   PCWSTR             m_szWindowClass;
   PCWSTR             m_szTitle;
   WORD               m_wIcon;
   WORD               m_wSmallIcon;
   int                m_width;
   int                m_height;
   int                m_iFrameDelay;
   BOOL               m_bSleep;
   UINT               m_uiJoystickID;
   RECT               m_rcJoystickTrip;
   vector<Sprite*>    m_vSprites;
   UINT               m_uiMIDIPlayerID;

   BOOL CheckSpriteCollision(Sprite* pTestSprite);

public:
         GameEngine(HINSTANCE inst, PCWSTR szWindowClass, PCWSTR szTitle,
                    WORD wIcon, WORD wSmallIcon, int width = 640, int height = 480);
   virtual ~GameEngine( );

   static GameEngine* GetEngine( )                       { return m_pGameEngine; };
   BOOL               Initialize(int iCmdShow);
   LRESULT            HandleEvent(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam);
   BOOL               InitJoystick( );
   void               CaptureJoystick( );
   void               ReleaseJoystick( );
   void               CheckJoystick( );
   void               AddSprite(Sprite* pSprite);
   void               DrawSprites(HDC dc);
   void               UpdateSprites( );
   void               CleanupSprites( );
   Sprite*            IsPointInSprite(int x, int y);
   void               PlayMIDISong(PCWSTR szMIDIFileName = TEXT(""), BOOL bRestart = TRUE);
   void               PauseMIDISong( );
   void               CloseMIDIPlayer( );

   HINSTANCE GetInstance( )               { return m_inst; };
   HWND      GetWindow( )                 { return m_hWindow; };
   void      SetWindow(HWND hWindow)      { m_hWindow = hWindow; };
   PCWSTR    GetTitle( )                  { return m_szTitle; };
   WORD      GetIcon( )                   { return m_wIcon; };
   WORD      GetSmallIcon( )              { return m_wSmallIcon; };
   int       GetWidth( )                  { return m_width; };
   int       GetHeight( )                 { return m_height; };
   int       GetFrameDelay( )             { return m_iFrameDelay; };
   void      SetFrameRate(int iFrameRate) { m_iFrameDelay = 1000 / iFrameRate; }; BOOL GetSleep( ) { return m_bSleep; };
   void      SetSleep(BOOL bSleep)        { m_bSleep = bSleep; };
};
