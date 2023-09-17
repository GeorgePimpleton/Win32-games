# Win32 Games, Chapter 2

[![Language](https://img.shields.io/badge/Language%20-C++-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)
[![Platform](https://img.shields.io/badge/Platform%20-Win32-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)

### Background

Win9X/Me is officially dead!  Windows API ANSI encoding is dead!  Windows XP and later now use Unicode!

Windows and C/C++ have changed since 2003/2004.  Programmers need to adapt.

### Blizzard: Creating An Engine For Games

Blizzard, creates and uses the Game Engine class.  From the 2004 CD.

### Blizzard Versions

4. Refined the code for Unicode only and modern Windows OSes.

- Changed WinMain to wWinMain, the Unicode entry point for a WinAPI program.  Have to change the command-line parameter type from PSTR to PWSTR.
- Added SAL notation to WinMain since Visual Studio 2022 "whinges" about "[i]nconsistent annotation for 'WinMain'".

[Using SAL Annotations to Reduce C/C++ Code Defects](https://learn.microsoft.com/en-us/cpp/code-quality/using-sal-annotations-to-reduce-c-cpp-code-defects)

### Notes

There may be links to websites or internet sources in these pages and source code. Links can go stale. A search engine is your friend.

This is a "work in progress", there is no guarantee the entirety of the entire collection of games will ever be modernized any time soon or to what degree of modern usage.

There is also no guarantee the source files will work for you.

If you find better ways to rework the code please drop me a note and let me know.
