// 1-1 Starting Up and Shutting Down GDI+

#include <windows.h>
#include <WindowsX.h>
#include <GDIPlus.h>

using namespace Gdiplus;

// tell VS to include the GDI+ libraries
#pragma comment(lib, "Gdiplus.lib" )

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void OnPaint(HWND hwnd);
void OnDestroy(HWND hwnd);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
                    _In_ PWSTR     szCmdLine, _In_     int       nWinMode)
{
   static const WCHAR szAppName[ ] { L"GDI+" };

   WNDCLASSEXW wc { };

   // GDI+ variables
   GdiplusStartupInput  gdiplusStartupInput;
   ULONG_PTR            gdiplusToken;
   Status               status;

   MessageBoxW(NULL, L"Initializing the GDI+ System", szAppName, MB_OK);

   // initialize GDI+
   status = GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

   if ( Ok != status )
   {
      MessageBoxW(NULL, L"Unable to Initialize the GDI+ System!", L"ERROR",
                  MB_OK | MB_ICONERROR);
      return 0;
   }

   wc.cbSize        = sizeof(WNDCLASSEX);
   wc.style         = CS_HREDRAW | CS_VREDRAW;
   wc.lpfnWndProc   = WndProc;
   wc.cbClsExtra    = 0;
   wc.cbWndExtra    = 0;
   wc.hInstance     = hInstance;
   wc.hIcon         = (HICON)   LoadImageW(NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
   wc.hIconSm       = (HICON)   LoadImageW(NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
   wc.hCursor       = (HCURSOR) LoadImageW(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
   wc.hbrBackground = (HBRUSH)  (COLOR_WINDOW + 1);
   wc.lpszMenuName  = NULL;
   wc.lpszClassName = szAppName;

   if ( 0 == RegisterClassExW(&wc) )
   {
      MessageBoxW(NULL, L"Can't Register the Window Class!", szAppName, MB_OK | MB_ICONERROR);
      return E_FAIL;
   }

   static const WCHAR szAppTitle[ ] { L"Using GDI+: Getting Started" };

   HWND hwnd { CreateWindowW(szAppName, szAppTitle,
                             WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             NULL, NULL, hInstance, NULL) };

   if ( NULL == hwnd )
   {
      MessageBoxW(NULL, L"Unable to Create the Main Window!", szAppName, MB_OK | MB_ICONERROR);
      return E_FAIL;
   }

   ShowWindow(hwnd, nWinMode);
   UpdateWindow(hwnd);

   MSG msg;

   while ( GetMessageW(&msg, NULL, 0, 0) )
   {
      TranslateMessage(&msg);
      DispatchMessageW(&msg);
   }

   MessageBoxW(NULL, L"Shutting-down the GDI+ System", szAppName, MB_OK);

   // shutdown GDI+
   GdiplusShutdown(gdiplusToken);

   return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   switch ( message )
   {
      HANDLE_MSG(hwnd, WM_PAINT, OnPaint);
      HANDLE_MSG(hwnd, WM_DESTROY, OnDestroy);
   }

   return DefWindowProcW(hwnd, message, wParam, lParam);
}

void OnDestroy(HWND hwnd)
{
   PostQuitMessage(0);
}

void OnPaint(HWND hwnd)
{
   HDC         hdc;
   PAINTSTRUCT ps;

   hdc = BeginPaint(hwnd, &ps);

   // GDI+ drawing goes here

   EndPaint(hwnd, &ps);
}