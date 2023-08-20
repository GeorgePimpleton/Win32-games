# Programming Windows© Games, Chapter 1

[![Language](https://img.shields.io/badge/Language%20-C++-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)
[![Platform](https://img.shields.io/badge/Platform%20-Win32-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)

### Background

Win9X/Me is officially dead!  Windows API ANSI encoding is dead!  Windows XP and later now use Unicode!

Windows and C/C++ have changed since 2003/2004.  Programmers need to adapt.

### Possible types of apps that can be created on Windows

1. console mode using C++
2. Windows© MessageBox
3. **minimum Win API**  *<----- this example*
4. minimal skeletal Win API app
5. realistic skeletal Win API app with custom assets
6. a modernized C++ practices skeletal Win API using C++ wide strings
7. minimum MFC app
8. using a custom C++ header file to use C++11 (and later) random engine features instead of the C library

The two examples shows the minimum needed to recreate create a fully formed and functional WinAPI app.  That doesn't "do" anything.

The first example shows older WinAPI usages, allowing for ANSI and Unicode encodings for the WinAPI functio ns.  This is original style WinAPI coding as done by Charles Petzold back in the Win9X/Me days.  This style should be dead and buries.

The second example uses strictly Unicode/wide char functions, so you'll see a lot of W suffixes on function names.  I also use generic return code labels (S_OK, E_FAIL) for better readability. This is the style I use when creating WinAPI apps.

I also use SAL, MS source-code annotation language, on WinMain.  Visual Studio 'whinges' if it is missing.  It doesn't stop the code from compiling or the executable from running.

// https://learn.microsoft.com/en-us/cpp/code-quality/understanding-sal

Get used to it.  "This will be on 'the test'."
This is the last example that shows the two different coding methods, from now on it will be strictly Unicode.

### Notes

There may be links to websites or internet sources in these pages and source code. Links can go stale. A search engine is your friend.

This is a "work in progress", there is no guarantee the entirety of the entire collection of games will ever be modernized any time soon.  There is also no guarantee the source files will work for you.

If you find better ways to rework the code don't hesitate to let me know.
