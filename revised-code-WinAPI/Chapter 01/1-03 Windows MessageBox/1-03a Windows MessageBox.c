// include commonly used Windows libraries
#include <windows.h>

// the entry point for a WinAPI app
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow )
{
   // displays a message box
   MessageBox( NULL, TEXT( "Hello World!" ), TEXT( "Hello Message" ), MB_OK );

   // return control back to Windows and exit
   return 0;
}
