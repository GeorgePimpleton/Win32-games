// 1-5_Skeleton Realistic.cpp - realistic Windows skeleton source

// disable the "hAccel could be zero" whinging, line 81
#pragma warning(disable : 6387)

#include "1-5_Skeleton Realistic.hpp"

int WINAPI wWinMain(_In_     HINSTANCE hInstance,
                    _In_opt_ HINSTANCE hPrevInstance,
                    _In_     PWSTR szCmdLine,
                    _In_     int iCmdShow)
{
   static WCHAR szAppName[] { L"Realistic Windows Skeleton" };

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
   wc.lpszClassName = szAppName;

   if (0 == RegisterClassExW(&wc))
   {
      MessageBoxW(NULL, L"Can't Register the Window Class!", szAppName, MB_OK | MB_ICONERROR);
      return E_FAIL;
   }

   static WCHAR szAppTitle[] { L"A Realistic Windows Game Skeleton Application" };

   HWND hwnd { CreateWindowW(szAppName, szAppTitle,
                             WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             NULL, NULL, hInstance, NULL) };

   if (NULL == hwnd)
   {
      MessageBoxW(NULL, L"Unable to Create the Main Window!", szAppName, MB_OK | MB_ICONERROR);
      return E_FAIL;
   }

   ShowWindow(hwnd, iCmdShow);
   UpdateWindow(hwnd);

   // load the accelerators
   HACCEL hAccel { LoadAcceleratorsW(hInstance, MAKEINTRESOURCEW(IDR_ACCEL)) };

   // check accelerators are loaded
   // the app can still run without accelerators
   if (NULL == hAccel)
   {
      MessageBoxW(NULL, L"Unable to Load the Accelerators!", szAppName, MB_OK | MB_ICONERROR);
   }

   // enter the main message loop with a bit of sanity checking
   static BOOL bRet;
   static MSG  msg;

   while ((bRet = GetMessageW(&msg, NULL, 0, 0)) != 0) // 0 = WM_QUIT
   {
      // check for error
      if (-1 == bRet)
      {
         // handle the error and possibly exit

         // for this app simply report error and exit
         MessageBoxW(NULL, L"Unable to Continue!", szAppName, MB_OK | MB_ICONERROR);
         return E_FAIL;
      }
      else
      {
         // check for accelerator keystrokes
         if (0 == TranslateAcceleratorW(hwnd, hAccel, &msg))
         {
            // no accelerator used so process the message
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
   // process menu items
   case WM_COMMAND:
      switch (LOWORD(wParam))
      {
      case IDM_GAME_EXIT:
         PostQuitMessage(0);

         return S_OK;

      case IDM_HELP_ABOUT:
         // the About... menu item was selected, display the app's pre-defined dialog box
         DialogBoxW((HINSTANCE) GetWindowLongPtrW(hwnd, GWLP_HINSTANCE), MAKEINTRESOURCEW(IDD_ABOUT), hwnd, (DLGPROC) DlgProc);

         return S_OK;
      }
      break;

   case WM_CREATE:
      // set the custom cursor as the class cursor
      SetClassLongPtrW(hwnd, GCLP_HCURSOR,
                      (LONG64) LoadImageW(GetModuleHandleW(NULL), MAKEINTRESOURCEW(IDC_CURSOR), IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR));

      return S_OK;

   case WM_PAINT:
      HDC         hdc;
      PAINTSTRUCT ps;

      hdc = BeginPaint(hwnd, &ps);

      RECT rect;

      GetClientRect(hwnd, &rect);

      DrawTextW(hdc, L"This is a realistic skeletal Windows® application!", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

      EndPaint(hwnd, &ps);

      return S_OK;

   case WM_DESTROY:
      PostQuitMessage(0);

      return S_OK;
   }

   return DefWindowProcW(hwnd, message, wParam, lParam);
}

// handle dialog box messages
BOOL CALLBACK DlgProc(HWND hDialog, UINT message, WPARAM wParam, LPARAM lParam)
{
   // process dialog box messages
   switch (message)
   {
   // choose the message(s) to process
   case WM_COMMAND:
      switch (LOWORD(wParam))
      {
      case IDOK:
         // the OK button was pushed, exit the dialog box
         EndDialog(hDialog, 0);

         return TRUE;
      }
   }

   // no dialog box message processed
   return FALSE;
}