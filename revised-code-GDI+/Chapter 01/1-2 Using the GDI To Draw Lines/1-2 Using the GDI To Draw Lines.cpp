// 1-2 Using the GDI To Draw Lines

#include <windows.h>
#include <windowsx.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void OnDestroy(HWND);

void OnPaint(HWND hwnd)
{
   PAINTSTRUCT ps;

   HDC hdc = BeginPaint(hwnd, &ps);

   HPEN hRedPen    = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
   HPEN hGreenPen  = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
   HPEN hBluePen   = CreatePen(PS_SOLID, 3, RGB(0, 0, 255));
   HPEN hYellowPen = CreatePen(PS_SOLID, 4, RGB(255, 255, 0));

   LineTo(hdc, 100, 50);
   MoveToEx(hdc, 100, 50, NULL);

   HPEN hOldPen = (HPEN) SelectObject(hdc, hGreenPen);
   LineTo(hdc, 200, 100);

   SelectObject(hdc, hYellowPen);
   LineTo(hdc, 0, 200);

   SelectObject(hdc, hBluePen);
   LineTo(hdc, 200, 200);

   SelectObject(hdc, hRedPen);
   LineTo(hdc, 0, 0);

   SelectObject(hdc, hOldPen);

   EndPaint(hwnd, &ps);
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
                    _In_ PWSTR     szCmdLine, _In_     int       nWinMode)
{
   static const WCHAR szAppName[ ] { L"GDIDrawLines" };

   WNDCLASSEXW wc { };

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

   static const WCHAR szAppTitle[ ] { L"Using the GDI to Draw Lines" };

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