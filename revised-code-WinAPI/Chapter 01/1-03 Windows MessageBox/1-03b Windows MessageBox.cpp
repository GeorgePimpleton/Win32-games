// explictly using the Unicode/wide char Windows© API functions and data types

#include <windows.h>

// NOT using Hungarian notation on parameter/variable identifiers
// https://learn.microsoft.com/en-us/windows/win32/learnwin32/windows-coding-conventions#hungarian-notation

// using SAL (MS source-code annotation language)
// https://learn.microsoft.com/en-us/cpp/code-quality/understanding-sal

int WINAPI wWinMain( _In_     HINSTANCE inst,
                     _In_opt_ HINSTANCE prevInst,
                     _In_     PWSTR     cmdLine,
                     _In_     int       cmdShow )
{
   MessageBoxW( NULL, L"Hello World!", L"Hello Message", MB_OK );

   // using generic return values for readability
   // https://learn.microsoft.com/en-us/windows/win32/seccrypto/common-hresult-values

   return S_OK;
}
