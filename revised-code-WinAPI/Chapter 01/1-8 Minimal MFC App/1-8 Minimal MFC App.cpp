// a minimal MFC program

#include "1-7 Minimal MFC App.hpp"

// instantiate the application
CTheApp theApp;

// construct a window
CMainWnd::CMainWnd( )
{
   Create(NULL, L"An MFC Application Skeleton");
}

// initalize the application
BOOL CTheApp::InitInstance( )
{
   m_pMainWnd = new CMainWnd;

   m_pMainWnd->ShowWindow(m_nCmdShow);
   m_pMainWnd->UpdateWindow( );

   return TRUE;
}

// application's message map
BEGIN_MESSAGE_MAP(CMainWnd, CFrameWnd)
   ON_WM_PAINT( )
END_MESSAGE_MAP( )

// draw some text centered in the client area
void CMainWnd::OnPaint( )
{
   CRect rect;

   GetClientRect(&rect);

   CPaintDC dc(this);

   dc.DrawTextW(L"Hello, MFC!", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}