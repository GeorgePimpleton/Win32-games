// a modularized Win API application skeleton, version 4

#include "winfuncs.hpp"

int WINAPI wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrevInst, _In_ PWSTR cmdLine, _In_ int winMode)
{
   if ( FAILED(InitApplication(hInst)) )
   {
      return S_OK;
   }

   if ( FAILED(InitInstance(hInst, winMode)) )
   {
      return S_OK;
   }

   return MessageLoop( );
}