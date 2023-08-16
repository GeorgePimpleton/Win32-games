// 1-7 Minimal MFC App.hpp - derive essential MFC classes

#pragma once

// compile for Windows 7 or higher
// #define WINVER       0x0601
// #define _WIN32_WINNT 0x0601

// compiling for Windows 10 (recommended)
#define WINVER       0x0A00
#define _WIN32_WINNT 0x0A00

#include <afxwin.h>

// application class
class CTheApp : public CWinApp
{
public:
   BOOL InitInstance( );
};

// main window class
class CMainWnd : public CFrameWnd
{
public:
   CMainWnd( );

protected:
   afx_msg void OnPaint( );

   DECLARE_MESSAGE_MAP( )
};
