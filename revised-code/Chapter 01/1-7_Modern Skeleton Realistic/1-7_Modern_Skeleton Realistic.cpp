// 1-7_Skeleton Realistic.cpp - realistic Windows skeleton source

// disable the "hAccel could be zero" whinging, line 81
#pragma warning(disable : 6387)

#include "1-7_Modern_Skeleton Realistic.hpp"

int WINAPI wWinMain(_In_     HINSTANCE hInstance,
                    _In_opt_ HINSTANCE hPrevInstance,
                    _In_     PWSTR szCmdLine,
                    _In_     int iCmdShow)
{
   static std::wstring szAppName { L"C++ Modernized Realistic Windows Skeleton" };

   WNDCLASSEXW wc { };

   wc.cbSize        = sizeof(WNDCLASSEX);
   wc.style         = CS_HREDRAW | CS_VREDRAW;
   wc.lpfnWndProc   = WndProc;
   wc.cbClsExtra    = 0;
   wc.cbWndExtra    = 0;
   wc.hInstance     = hInstance;
   wc.hIcon         = (HICON)   LoadImageW(hInstance, MAKEINTRESOURCEW(IDI_ICON),    IMAGE_ICON,   0, 0, LR_DEFAULTCOLOR);
   wc.hIconSm       = (HICON)   LoadImageW(hInstance, MAKEINTRESOURCEW(IDI_ICON_SM), IMAGE_ICON,   0, 0, LR_DEFAULTCOLOR);
   wc.hCursor       = (HCURSOR) LoadImageW(hInstance, MAKEINTRESOURCEW(IDC_CURSOR),  IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR);
   wc.hbrBackground = (HBRUSH)  (COLOR_WINDOW + 1);
   wc.lpszMenuName  = MAKEINTRESOURCEW(IDR_MENU);
   wc.lpszClassName = szAppName.data();

   if (0 == RegisterClassExW(&wc))
   {
      MessageBoxW(nullptr, L"Can't Register the Window Class!", szAppName.data(), MB_OK | MB_ICONERROR);
      return E_FAIL;
   }

   static std::wstring szAppTitle { L"A C++ Monderized Realistic Windows Game Skeleton Application" };

   HWND hwnd { CreateWindowW(szAppName.data(), szAppTitle.data(),
                             WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             nullptr, nullptr, hInstance, nullptr) };

   if (nullptr == hwnd)
   {
      MessageBoxW(nullptr, L"Unable to Create the Main Window!", szAppName.data(), MB_OK | MB_ICONERROR);
      return E_FAIL;
   }

   ShowWindow(hwnd, iCmdShow);
   UpdateWindow(hwnd);

   HACCEL hAccel { LoadAcceleratorsW(hInstance, MAKEINTRESOURCEW(IDR_ACCEL)) };

   if (nullptr == hAccel)
   {
      MessageBoxW(nullptr, L"Unable to Load the Accelerators!", szAppName.data(), MB_OK | MB_ICONERROR);
   }

   static BOOL bRet;
   static MSG  msg;

   while ((bRet = GetMessageW(&msg, nullptr, 0, 0)) != 0) // 0 = WM_QUIT
   {
      if (-1 == bRet)
      {
         MessageBoxW(nullptr, L"Unable to Continue!", szAppName.data(), MB_OK | MB_ICONERROR);
         return E_FAIL;
      }
      else
      {
         if (0 == TranslateAcceleratorW(hwnd, hAccel, &msg))
         {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
         }
      }
   }

   return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   switch (message)
   {
   case WM_COMMAND:
      switch (LOWORD(wParam))
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
      HDC         hdc;
      PAINTSTRUCT ps;

      hdc = BeginPaint(hwnd, &ps);

      RECT rect;

      GetClientRect(hwnd, &rect);

      DrawTextW(hdc, L"This is a C++ modernized realistic skeletal Windows® application!", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

      EndPaint(hwnd, &ps);

      return S_OK;

   case WM_DESTROY:
      PostQuitMessage(0);

      return S_OK;
   }

   return DefWindowProcW(hwnd, message, wParam, lParam);
}

BOOL CALLBACK DlgProc(HWND hDialog, UINT message, WPARAM wParam, LPARAM lParam)
{
   switch (message)
   {
   case WM_COMMAND:
      switch (LOWORD(wParam))
      {
      case IDOK:
         EndDialog(hDialog, 0);

         return TRUE;
      }
   }

   return FALSE;
}