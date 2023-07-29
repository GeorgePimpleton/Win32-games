//-----------------------------------------------------------------
// Game Engine Object
// C++ Header - GameEngine.hPP
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// include files
//-----------------------------------------------------------------
#include <windows.h>
#include <mmsystem.h>
#include <vector>
#include "Sprite.hpp"

//-----------------------------------------------------------------
// joystick flags
//-----------------------------------------------------------------
using JOYSTATE           = WORD;
const JOYSTATE JOY_NONE  = 0x0000L,
               JOY_LEFT  = 0x0001L,
               JOY_RIGHT = 0x0002L,
               JOY_UP    = 0x0004L,
               JOY_DOWN  = 0x0008L,
               JOY_FIRE1 = 0x0010L,
               JOY_FIRE2 = 0x0020L;

//-----------------------------------------------------------------
// Windows function declarations
//-----------------------------------------------------------------
int WINAPI       WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//-----------------------------------------------------------------
// game engine function declarations
//-----------------------------------------------------------------
BOOL GameInitialize(HINSTANCE hInstance);
void GameStart(HWND hWindow);
void GameEnd( );
void GameActivate(HWND hWindow);
void GameDeactivate(HWND hWindow);
void GamePaint(HDC hDC);
void GameCycle( );
void HandleKeys( );
void MouseButtonDown(int x, int y, BOOL bLeft);
void MouseButtonUp(int x, int y, BOOL bLeft);
void MouseMove(int x, int y);
void HandleJoystick(JOYSTATE jsJoystickState);
BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee);
void SpriteDying(Sprite* pSpriteDying);

//-----------------------------------------------------------------
// GameEngine class
//-----------------------------------------------------------------
class GameEngine
{
protected:
   // member variables
   static GameEngine*   m_pGameEngine;
   HINSTANCE            m_hInstance;
   HWND                 m_hWindow;
   TCHAR                m_szWindowClass[ 64 ];
   TCHAR                m_szTitle[ 64 ];
   WORD                 m_wIcon;
   WORD                 m_wSmallIcon;
   int                  m_iWidth;
   int                  m_iHeight;
   int                  m_iFrameDelay;
   BOOL                 m_bSleep;
   UINT                 m_uiJoystickID;
   RECT                 m_rcJoystickTrip;
   std::vector<Sprite*> m_vSprites;
   UINT                 m_uiMIDIPlayerID;

   // helper methods
   BOOL                 CheckSpriteCollision(Sprite* pTestSprite);

public:
   // constructor(s)/destructor
   GameEngine(HINSTANCE hInstance, LPCTSTR szWindowClass, LPCTSTR szTitle,
              WORD wIcon, WORD wSmallIcon, int iWidth = 640, int iHeight = 480);
   virtual ~GameEngine( );

   // General Methods
   static GameEngine* GetEngine( ) { return m_pGameEngine; };

   BOOL    Initialize(int iCmdShow);
   LRESULT HandleEvent(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam);
   void    ErrorQuit(LPCTSTR szErrorMsg);
   BOOL    InitJoystick( );
   void    CaptureJoystick( );
   void    ReleaseJoystick( );
   void    CheckJoystick( );
   void    AddSprite(Sprite* pSprite);
   void    DrawSprites(HDC hDC);
   void    UpdateSprites( );
   void    CleanupSprites( );
   Sprite* IsPointInSprite(int x, int y);
   void    PlayMIDISong(LPCTSTR szMIDIFileName = TEXT(""), BOOL bRestart = TRUE);
   void    PauseMIDISong( );
   void    CloseMIDIPlayer( );

   // accessor methods
   HINSTANCE GetInstance( )               { return m_hInstance; };
   HWND      GetWindow( )                 { return m_hWindow; };
   void      SetWindow(HWND hWindow)      { m_hWindow = hWindow; };
   LPCTSTR   GetTitle( )                  { return m_szTitle; };
   WORD      GetIcon( )                   { return m_wIcon; };
   WORD      GetSmallIcon( )              { return m_wSmallIcon; };
   int       GetWidth( )                  { return m_iWidth; };
   int       GetHeight( )                 { return m_iHeight; };
   int       GetFrameDelay( )             { return m_iFrameDelay; };
   void      SetFrameRate(int iFrameRate) { m_iFrameDelay = 1000 / iFrameRate; };
   BOOL      GetSleep( )                  { return m_bSleep; };
   void      SetSleep(BOOL bSleep)        { m_bSleep = bSleep; };
};
