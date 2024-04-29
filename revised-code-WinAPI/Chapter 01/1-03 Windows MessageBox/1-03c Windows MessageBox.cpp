// exclude  some of the less common WinAPI declarations

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow )
{
   MessageBox( NULL, TEXT( "Hello World!" ), TEXT( "Hello Message" ), MB_OK );

   return 0;
}
