// derive essential MFC classes

#pragma once

// compiling for Windows 10
// #define WINVER       0x0A00
// #define _WIN32_WINNT 0x0A00

// compiling for latest Win version
#include <sdkddkver.h>

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
