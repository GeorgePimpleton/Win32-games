# Win32 Games, Chapter 2, part 1

[![Language](https://img.shields.io/badge/Language%20-C++-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)
[![Platform](https://img.shields.io/badge/Platform%20-Win32-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)

### Background

Win9X/Me is officially dead!  Windows API ANSI encoding is dead!  Windows XP and later now use Unicode!

Windows and C/C++ have changed since 2003/2004.  Programmers need to adapt.

### Blizzard: Creating An Engine For Games

Blizzard, creates and uses the Game Engine class.  From the 2004 CD.

### Blizzard Versions

1. The bare minimum changes to work with modern C++/WinAPI x86 & x64.

### What was done
- Removed a lot of what I consider extraneous and useless comments.
- Reformatted the source to look better for easier readability.
   - Separated several class data members so they are declared seperately.
   - One part of "looks better" means adding parentheses to control statement for better readability of the code blocks.
- Increased the size of the GameEngine class data members, m_szWindowClass and m_szTitle, to 64.  A length of 32 TCHARs just seems too small to me.

The following changes had to happen to be compileable:
+ Changed two of the GameEngine ctor's parameter types from LPTSTR to LPCTSTR.
+ Changed one of the WinAPI DrawIcon parameters from (HICON) (WORD) GetClassLong(hWindow, GCL_HICON) to (HICON) GetClassLongPtr(hWindow, GCLP_HICON).

### Notes

There may be links to websites or internet sources in these pages and source code. Links can go stale. A search engine is your friend.

This is a "work in progress", there is no guarantee the entirety of the entire collection of games will ever be modernized any time soon or to what degree of modern usage.

There is also no guarantee the source files will work for you.

If you find better ways to rework the code please drop me a note and let me know.
