# Programming Windows© Games, Chapter 1

[![Language](https://img.shields.io/badge/Language%20-C++-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)
[![Platform](https://img.shields.io/badge/Platform%20-Win32-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)

The basics of programming using the Windows API, in a quick nutshell.  

### Background

Win9X/Me is officially dead!  Windows API ANSI encoding is dead!  Windows XP and later now use Unicode!

Windows and C/C++ have changed since 2003/2004.  Programmers need to adapt.

### Possible types of apps that can be created on Windows

1. console mode using C++
2. using a custom C++ header file to use C++11 (and later) random engine features instead of the C library functions
3. **a Windows MessageBox**  *<---- this example*
    + **a WinAPI MessageBox using modern coding** *<----- this example*
4. a minimum WinAPI app
     + a minimum WinAPI app using modern coding
5. a minimal skeletal WinAPI app
6. a realistic skeletal WinAPI app with custom assets
7. a skeletal WinAPI app using C++ wide strings using modernized C++ practices 
8. a minimum MFC app
9. modularizing WinAPI
10. modularizing WinAPI
11. modularizing WinAPI
12. modularizing WinAPI
13. modularizing WinAPI

The two examples shows the bare minimum needed to recreate the "Hello World" console example using a WinAPI message box.  The first example shows older WinAPI usages, allowing for ANSI and Unicode encodings for the WinAPI functio ns.  This is original style WinAPI coding as done by Charles Petzold back in the Win9X/Me days.  This style should be dead and buried.

The second example uses strictly Unicode/wide char functions, so you'll see a lot of W suffixes on function names.  I also use generic return code labels (S_OK, E_FAIL) for better readability. This is the style I use when creating WinAPI apps.

I also use SAL, MS source-code annotation language, on WinMain.  Visual Studio 'whinges' if it is missing.  It doesn't stop the code from compiling or the executable from running.

// https://learn.microsoft.com/en-us/cpp/code-quality/understanding-sal

Get used to it.  "This will be on 'the test'."

### Notes

There may be links to websites or internet sources in these pages and source code. Links can go stale. A search engine is your friend.

This is a "work in progress", there is no guarantee the entirety of the entire collection of games will ever be modernized any time soon.  There is also no guarantee the source files will work for you.

If you find better ways to rework the code don't hesitate to let me know.
