#include "1-7a Modern Skeleton Realistic.hpp"

int WINAPI wWinMain(_In_ HINSTANCE inst, _In_opt_ HINSTANCE prevInst, _In_ PWSTR cmdLine, _In_ int cmdShow)
{
   static std::wstring appName = L"C++ Modernized Realistic Windows Skeleton";

   WNDCLASSEXW wc { };

   wc.cbSize        = sizeof(WNDCLASSEX);
   wc.style         = CS_HREDRAW | CS_VREDRAW;
   wc.lpfnWndProc   = WndProc;
   wc.cbClsExtra    = 0;
   wc.cbWndExtra    = 0;
   wc.hInstance     = inst;
   wc.hIcon         = (HICON)   LoadImageW(inst, MAKEINTRESOURCEW(IDI_ICON), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
   wc.hIconSm       = (HICON)   LoadImageW(inst, MAKEINTRESOURCEW(IDI_ICON_SM), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
   wc.hCursor       = (HCURSOR) LoadImageW(inst, MAKEINTRESOURCEW(IDC_CURSOR), IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR);
   wc.hbrBackground = (HBRUSH)  (COLOR_WINDOW + 1);
   wc.lpszMenuName  = MAKEINTRESOURCEW(IDR_MENU);
   wc.lpszClassName = appName.data( );

   if ( FAILED(RegisterClassExW(&wc)) )
   {
      MessageBoxW(nullptr, L"Can't Register the Window Class!", appName.data( ), MB_OK | MB_ICONERROR);
      return E_FAIL;
   }

   static std::wstring appTitle = L"A C++ Monderized Realistic Windows Skeleton Application";

   HWND wnd { CreateWindowW(appName.data( ), appTitle.data( ),
                            WS_OVERLAPPEDWINDOW,
                            CW_USEDEFAULT, CW_USEDEFAULT,
                            CW_USEDEFAULT, CW_USEDEFAULT,
                            nullptr, nullptr, inst, nullptr) };

   if ( nullptr == wnd )
   {
      MessageBoxW(nullptr, L"Unable to Create the Main Window!", appName.data( ), MB_OK | MB_ICONERROR);
      return E_FAIL;
   }

   ShowWindow(wnd, cmdShow);
   UpdateWindow(wnd);

   HACCEL accel = LoadAcceleratorsW(inst, MAKEINTRESOURCEW(IDR_ACCEL));

   if ( nullptr == accel )
   {
      MessageBoxW(nullptr, L"Unable to Load the Accelerators!", appName.data( ), MB_OK | MB_ICONERROR);
   }

   static BOOL ret;
   static MSG  msg;

   while ( (ret = GetMessageW(&msg, nullptr, 0, 0)) != 0 ) // 0 = WM_QUIT
   {
      if ( -1 == ret )
      {
         MessageBoxW(nullptr, L"Unable to Continue!", appName.data( ), MB_OK | MB_ICONERROR);
         return E_FAIL;
      }
      else
      {
         if (SUCCEEDED(TranslateAcceleratorW(wnd, accel, &msg)) )
         {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
         }
      }
   }

   return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
   switch ( msg )
   {
   case WM_COMMAND:
      switch ( LOWORD(wParam) )
      {
      case IDM_APP_EXIT:
         PostQuitMessage(0);
         return S_OK;

      case IDM_HELP_ABOUT:
         DialogBoxParamW((HINSTANCE) GetWindowInstance(wnd), MAKEINTRESOURCEW(IDD_ABOUT), wnd, (DLGPROC) DlgProc, 0L);
         return S_OK;
      }
      break;

   case WM_CREATE:
      SetClassLongPtrW(wnd, GCLP_HCURSOR,
                       (LONG64) LoadImageW(GetModuleHandleW(nullptr), MAKEINTRESOURCEW(IDC_CURSOR), IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR));
      return S_OK;

   case WM_PAINT:
   {
      PAINTSTRUCT ps;
      HDC         dc = BeginPaint(wnd, &ps);
      RECT        rect;

      GetClientRect(wnd, &rect);
      DrawTextW(dc, L"This is a C++ modernized realistic skeletal Windows application!", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
      EndPaint(wnd, &ps);
      return S_OK;
   }

   case WM_DESTROY:
      PostQuitMessage(0);
      return S_OK;
   }

   return DefWindowProcW(wnd, msg, wParam, lParam);
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