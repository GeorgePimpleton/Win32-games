// 1-2 Windows MessageBox Updated.cpp - displays "Hello World!" in a GUI message box

// the updates are explictly using the Unicode/wide char Windows API functions

#include <windows.h>

// using SAL (MS source-code annotation language)
// https://learn.microsoft.com/en-us/cpp/code-quality/understanding-sal
int WINAPI wWinMain(_In_     HINSTANCE hInst,
                    _In_opt_ HINSTANCE hPrevInst,
                    _In_     PWSTR     cmdLine,
                    _In_     int       cmdShow)
{
   MessageBoxW(NULL, L"Hello World!", L"Hello Message", MB_OK);

   // using generic return values for readability
   // https://learn.microsoft.com/en-us/windows/win32/seccrypto/common-hresult-values
   return S_OK;
}