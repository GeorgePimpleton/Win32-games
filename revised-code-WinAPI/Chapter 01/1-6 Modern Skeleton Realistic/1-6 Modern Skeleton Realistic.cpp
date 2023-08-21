#pragma warning(disable : 6387)

#include "1-6 Modern Skeleton Realistic.hpp"

int WINAPI wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrevInst, _In_ PWSTR cmdLine, _In_ int cmdShow)
{
   static std::wstring appName { L"C++ Modernized Realistic Windows Skeleton" };

   WNDCLASSEXW wc { };

   wc.cbSize        = sizeof(WNDCLASSEX);
   wc.style         = CS_HREDRAW | CS_VREDRAW;
   wc.lpfnWndProc   = WndProc;
   wc.cbClsExtra    = 0;
   wc.cbWndExtra    = 0;
   wc.hInstance     = hInst;
   wc.hIcon         = (HICON)   LoadImageW(hInst, MAKEINTRESOURCEW(IDI_ICON), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
   wc.hIconSm       = (HICON)   LoadImageW(hInst, MAKEINTRESOURCEW(IDI_ICON_SM), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
   wc.hCursor       = (HCURSOR) LoadImageW(hInst, MAKEINTRESOURCEW(IDC_CURSOR), IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR);
   wc.hbrBackground = (HBRUSH)  (COLOR_WINDOW + 1);
   wc.lpszMenuName  = MAKEINTRESOURCEW(IDR_MENU);
   wc.lpszClassName = appName.data( );

   if ( FAILED(RegisterClassExW(&wc)) )
   {
      MessageBoxW(nullptr, L"Can't Register the Window Class!", appName.data( ), MB_OK | MB_ICONERROR);
      return E_FAIL;
   }

   static std::wstring appTitle { L"A C++ Monderized Realistic Windows Skeleton Application" };

   HWND hwnd { CreateWindowW(appName.data( ), appTitle.data( ),
                             WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             nullptr, nullptr, hInst, nullptr) };

   if ( nullptr == hwnd )
   {
      MessageBoxW(nullptr, L"Unable to Create the Main Window!", appName.data( ), MB_OK | MB_ICONERROR);
      return E_FAIL;
   }

   ShowWindow(hwnd, cmdShow);
   UpdateWindow(hwnd);

   HACCEL hAccel { LoadAcceleratorsW(hInst, MAKEINTRESOURCEW(IDR_ACCEL)) };

   if ( nullptr == hAccel )
   {
      MessageBoxW(nullptr, L"Unable to Load the Accelerators!", appName.data( ), MB_OK | MB_ICONERROR);
   }

   static BOOL bRet;
   static MSG  msg;

   while ( (bRet = GetMessageW(&msg, nullptr, 0, 0)) != 0 ) // 0 = WM_QUIT
   {
      if ( -1 == bRet )
      {
         MessageBoxW(nullptr, L"Unable to Continue!", appName.data( ), MB_OK | MB_ICONERROR);
         return E_FAIL;
      }
      else
      {
         if (SUCCEEDED(TranslateAcceleratorW(hwnd, hAccel, &msg)) )
         {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
         }
      }
   }

   return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
   switch ( msg )
   {
   case WM_COMMAND:
      switch ( LOWORD(wParam) )
      {
      case IDM_GAME_EXIT:
         PostQuitMessage(0);

         return S_OK;

      case IDM_HELP_ABOUT:
         DialogBoxW((HINSTANCE) GetWindowLongPtrW(hwnd, GWLP_HINSTANCE), MAKEINTRESOURCEW(IDD_ABOUT), hwnd, (DLGPROC) DlgProc);

         return S_OK;
      }
      break;

   case WM_CREATE:
      SetClassLongPtrW(hwnd, GCLP_HCURSOR,
                       (LONG64) LoadImageW(GetModuleHandleW(nullptr), MAKEINTRESOURCEW(IDC_CURSOR), IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR));

      return S_OK;

   case WM_PAINT:
   {
      PAINTSTRUCT ps;
      HDC         hdc = BeginPaint(hwnd, &ps);

      RECT rect;

      GetClientRect(hwnd, &rect);

      DrawTextW(hdc, L"This is a C++ modernized realistic skeletal Windows® application!", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

      EndPaint(hwnd, &ps);

      return S_OK;
   }

   case WM_DESTROY:
      PostQuitMessage(0);

      return S_OK;
   }

   return DefWindowProcW(hwnd, msg, wParam, lParam);
}

BOOL CALLBACK DlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
   switch ( msg )
   {
   case WM_COMMAND:
      switch ( LOWORD(wParam) )
      {
      case IDOK:
         EndDialog(hDlg, 0);

         return TRUE;
      }
   }

   return FALSE;
}