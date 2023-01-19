// 1-2_Windows MessageBox Updated.cpp - displays "Hello World!" in a GUI message box

#include <windows.h>

int WINAPI wWinMain(_In_     HINSTANCE hInstance,
                    _In_opt_ HINSTANCE hPrevInstance,
                    _In_     PWSTR szCmdLine,
                    _In_     int iCmdShow)
{
   MessageBoxW(NULL, L"Hello World!", L"Hello Message", MB_OK);

   return 0;
}