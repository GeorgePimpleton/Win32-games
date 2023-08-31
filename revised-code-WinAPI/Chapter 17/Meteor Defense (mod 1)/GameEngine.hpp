#pragma once

typedef WORD   JOYSTATE;
const JOYSTATE JOY_NONE  = 0x0000L;
const JOYSTATE JOY_LEFT  = 0x0001L;
const JOYSTATE JOY_RIGHT = 0x0002L;
const JOYSTATE JOY_UP    = 0x0004L;
const JOYSTATE JOY_DOWN  = 0x0008L;
const JOYSTATE JOY_FIRE1 = 0x0010L;
const JOYSTATE JOY_FIRE2 = 0x0020L;

int WINAPI        WinMain(HINSTANCE inst, HINSTANCE prevInst, PWSTR cmdLine, int cmdShow);
LRESULT CALLBACK  WndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK     DlgProc(HWND dlg, UINT msg, WPARAM wParam, LPARAM lParam);

BOOL GameInitialize(HINSTANCE inst);
void GameStart(HWND wnd);
void GameEnd();
void GameActivate(HWND wnd);
void GameDeactivate(HWND wnd);
void GamePaint(HDC hDC);
void GameCycle();
void GameMenu(WPARAM wParam);
void HandleKeys();
void MouseButtonDown(int x, int y, BOOL bLeft);
void MouseButtonUp(int x, int y, BOOL bLeft);
void MouseMove(int x, int y);
void HandleJoystick(JOYSTATE jsJoystickState);
BOOL SpriteCollision(Sprite * pSpriteHitter, Sprite * pSpriteHittee);
void NewGame();
void SpriteDying(Sprite * pSprite);

class GameEngine
{
public:
            GameEngine(HINSTANCE hInstance, PCWSTR szWindowClass, PCWSTR szTitle,
                        WORD wIcon, int iWidth = 640, int iHeight = 480);
   virtual ~GameEngine();

public:
   static GameEngine *  GetEngine();
   BOOL                 Initialize(int iCmdShow);
   LRESULT              HandleEvent(HWND wnd, UINT msg, WPARAM wParam,
                                    LPARAM lParam);
   void                 ErrorQuit(LPTSTR szErrorMsg);
   BOOL                 InitJoystick();
   void                 CaptureJoystick();
   void                 ReleaseJoystick();
   void                 CheckJoystick();
   void                 AddSprite(Sprite * pSprite);
   void                 DrawSprites(HDC hDC);
   void                 UpdateSprites();
   void                 CleanupSprites();
   Sprite *             IsPointInSprite(int x, int y);
   void                 PlayMIDISong(PCWSTR MIDIFileName = L"",
                                     BOOL bRestart = TRUE);
   void                 PauseMIDISong();
   void                 CloseMIDIPlayer();

public:
   HINSTANCE   GetInstance() const;
   HWND        GetWindow() const;
   void        SetWindow(HWND wnd);
   LPTSTR      GetTitle();
   WORD        GetIcon() const;
   int         GetWidth() const;
   int         GetHeight() const;
   int         GetFrameDelay() const;
   void        SetFrameRate(int iFrameRate);
   BOOL        GetSleep() const;
   void        SetSleep(BOOL bSleep);

protected:
   BOOL        CheckSpriteCollision(Sprite * pTestSprite);

protected:
   static GameEngine *  m_pGameEngine;
   HINSTANCE            m_hInstance;
   HWND                 m_wnd;
   TCHAR                m_szWindowClass[64];
   TCHAR                m_szTitle[64];
   WORD                 m_wIcon;
   int                  m_iWidth;
   int                  m_iHeight;
   int                  m_iFrameDelay;
   BOOL                 m_bSleep;
   UINT                 m_uiJoystickID;
   RECT                 m_rcJoystickTrip;
   vector<Sprite *>     m_vSprites;
   UINT                 m_uiMIDIPlayerID;
};

inline GameEngine * GameEngine::GetEngine()
{
   return m_pGameEngine;
}

inline void GameEngine::ErrorQuit(LPTSTR szErrorMsg)
{
   MessageBox(GetWindow(), szErrorMsg, TEXT("Critical Error"),
              MB_OK | MB_ICONERROR);
   PostQuitMessage(0);
}

inline HINSTANCE GameEngine::GetInstance() const
{
   return m_hInstance;
}

inline HWND GameEngine::GetWindow() const
{
   return m_wnd;
}

inline void GameEngine::SetWindow(HWND wnd)
{
   m_wnd = wnd;
}

inline LPTSTR GameEngine::GetTitle()
{
   return m_szTitle;
}

inline WORD GameEngine::GetIcon() const
{
   return m_wIcon;
}

inline int GameEngine::GetWidth() const
{
   return m_iWidth;
}

inline int GameEngine::GetHeight() const
{
   return m_iHeight;
}

inline int GameEngine::GetFrameDelay() const
{
   return m_iFrameDelay;
}

inline void GameEngine::SetFrameRate(int iFrameRate)
{
   m_iFrameDelay = 1000 / iFrameRate;
}

inline BOOL GameEngine::GetSleep() const
{
   return m_bSleep;
}

inline void GameEngine::SetSleep(BOOL bSleep)
{
   m_bSleep = bSleep;
}
