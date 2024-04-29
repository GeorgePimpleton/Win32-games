# Programming Windows Games, Chapter 1, WinAPI MessageBox
[![Language](https://img.shields.io/badge/Language%20-C++-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)
[![Platform](https://img.shields.io/badge/Platform%20-Win32-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)

The basics of programming using the Windows API, in a quick nutshell.  

### Possible types of apps that can be created on Windows
3. a Windows API MessageBox using a .c file
   + a WinAPI MessageBox as a .cpp file
   + exclude  some of the less common WinAPI declarations
   + a WinAPI MessageBox using modern coding practices

The four examples shows the bare minimum needed to recreate the "Hello World" console example using a WinAPI message box.  The first two examples show older WinAPI usages, allowing for ANSI and Unicode encodings for the WinAPI functions.  This is original style WinAPI coding as done by Charles Petzold back in the Win9X/Me days.  This style should be dead and buried.

The third example removes the uncommon WinAPI libraries.

The fourth example uses strictly Unicode/wide char functions, so you'll see a lot of W suffixes on function names.  I also use generic return code labels (S_OK, E_FAIL) for better readability. This is the style I use when creating WinAPI apps.

I also use SAL, MS source-code annotation language, on WinMain.  Visual Studio 'whinges' if it is missing.  It doesn't stop the code from compiling or the executable from running.

https://learn.microsoft.com/en-us/cpp/code-quality/understanding-sal

Get used to it.  "This will be on 'the test'."
