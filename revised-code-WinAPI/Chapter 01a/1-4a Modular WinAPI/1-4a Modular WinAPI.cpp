// a modularized Win API application skeleton, version 3

#include "winfuncs.hpp"

int WINAPI wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrevInst, _In_ PWSTR cmdLine, _In_ int winMode)
{
   if ( FAILED(InitApplication(hInst)) )
   {
      return E_FAIL;
   }

   if ( FAILED(InitInstance(hInst, winMode)) )
   {
      return E_FAIL;
   }

   return MessageLoop( );
}