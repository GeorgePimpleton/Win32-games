# Win32 Games, Chapter 2

[![Language](https://img.shields.io/badge/Language%20-C++-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)
[![Platform](https://img.shields.io/badge/Platform%20-Win32-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)

### Background

Win9X/Me is officially dead!  Windows API ANSI encoding is dead!  Windows XP and later now use Unicode!

Windows and C/C++ have changed since 2003/2004.  Programmers need to adapt.

### Blizzard: Creating An Engine For Games

Blizzard, creates and uses the Game Engine class.  From the 2004 CD.

### Blizzard Versions

0. Original CD game files written in 2004.
1. The bare minimum changes to work with modern C++/WinAPI x86 & x64.  SAL is a minimum.
2. Reformatting, changing some file names and refining the code for Unicode and modern Windows OSes.  Added my Random Toolkit to generate random numbers the C++ way.
3. Added menus, inline the setters and getters and other misc. changes.
4. Use C++ smart pointers, shared and unique, instead of raw pointers.
5. **Changed the shared pointers to unique, any WCHAR string arrays in the Game Engine class are now std::wstring.** *<----- this example*
6. Removed the old WinAPI Hungarian Notation from variables.  Even MS recommends not using it.

### Notes

There may be links to websites or internet sources in these pages and source code. Links can go stale. A search engine is your friend.

This is a "work in progress", there is no guarantee the entirety of the entire collection of games will ever be modernized any time soon or to what degree of modern usage.

There is also no guarantee the source files will work for you.

If you find better ways to rework the code please drop me a note and let me know.
