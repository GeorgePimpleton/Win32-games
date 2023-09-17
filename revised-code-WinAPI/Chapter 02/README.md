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
1. The bare minimum changes to work with modern C++/WinAPI x86 & x64.
2. Removed the old WinAPI Hungarian Notation from variables.  Even MS recommends not using it.
3. Changed file names.
4. Added my Random Toolkit header to help generate random numbers the C++ way.
5. Refined the code for Unicode only and modern Windows OSes.
6. Added menus, inline the setters and getters and other misc. changes.
7. Use C++ smart pointers, shared and unique, instead of raw pointers.
8. Changed the shared pointers to unique, any WCHAR string arrays in the Game Engine class are now std::wstring.
9. Changed the wide strings to PCWSTRs.

### Notes
There may be links to websites or internet sources in these pages and source code. Links can go stale. A search engine is your friend.
