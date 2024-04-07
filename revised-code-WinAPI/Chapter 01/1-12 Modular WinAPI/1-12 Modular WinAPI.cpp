// a modularized Win API application skeleton, version 4

#include "winfuncs.hpp"

int WINAPI wWinMain( _In_ HINSTANCE inst, _In_opt_ HINSTANCE prevInst, _In_ PWSTR cmdLine, _In_ int winMode )
{
   if ( FAILED( InitApplication( inst ) ) )
   {
      return S_OK;
   }

   if ( FAILED( InitInstance( inst, winMode ) ) )
   {
      return S_OK;
   }

   return MessageLoop( );
}
