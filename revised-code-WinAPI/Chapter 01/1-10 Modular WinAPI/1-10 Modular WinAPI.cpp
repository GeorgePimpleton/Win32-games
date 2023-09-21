#include "winfuncs.hpp"

int WINAPI wWinMain(_In_ HINSTANCE inst, _In_opt_ HINSTANCE prevInst, _In_ PWSTR cmdLine, _In_ int winMode)
{
   if ( FAILED(InitApplication(inst)) )
   {
      return E_FAIL;
   }

   if ( FAILED(InitInstance(inst, winMode)) )
   {
      return E_FAIL;
   }

   return MessageLoop( );
}