#include <windows.h>

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow )
{
   MessageBox( NULL, TEXT( "Hello World!" ), TEXT( "Hello Message" ), MB_OK );

   return 0;
}
