// idle time processing

#include "winfuncs.hpp"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
                    _In_ PWSTR szCmdLine, _In_ int nWinMode)
{
   if ( FAILED(InitApplication(hInstance)) )
   {
      return 0;
   }

   if ( FAILED(InitInstance(hInstance, nWinMode)) )
   {
      return 0;
   }

   return MessageLoop();
}