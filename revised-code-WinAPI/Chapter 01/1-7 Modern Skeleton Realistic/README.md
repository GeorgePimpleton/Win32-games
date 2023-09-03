# Programming Windows© Games, Chapter 1

[![Language](https://img.shields.io/badge/Language%20-C++-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)
[![Platform](https://img.shields.io/badge/Platform%20-Win32-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)

### Background

Win9X/Me is officially dead!  Windows API ANSI encoding is dead!  Windows XP and later now use Unicode!

Windows and C/C++ have changed since 2003/2004.  Programmers need to adapt.

### Possible types of apps that can be created on Windows

1. console mode using C++
2. using a custom C++ header file to use C++11 (and later) random engine features instead of the C library functions
3. a Windows© MessageBox
4. a minimum Win API app
5. a minimal skeletal Win API app
6. a realistic skeletal Win API app with custom assets
7. **a skeletal Win API app using C++ wide strings using modernized C++ practices** *<----- this example*
8. a minimum MFC app

This was more of an experiment with C++ than anything really useful when using the WinAPI.  The Win API already has wide string data types, PCWSTR for example.  A pointer to a constant wide string array.  I will be using the Win API data types. 

### Notes

There may be links to websites or internet sources in these pages and source code. Links can go stale. A search engine is your friend.

This is a "work in progress", there is no guarantee the entirety of the entire collection of games will ever be modernized any time soon.  There is also no guarantee the source files will work for you.

If you find better ways to rework the code don't hesitate to let me know.
