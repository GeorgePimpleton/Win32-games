# Programming Windows© Games, Chapter 1a

[![Language](https://img.shields.io/badge/Language%20-C++-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)
[![Platform](https://img.shields.io/badge/Platform%20-Win32-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)

### Background

Win9X/Me is officially dead!  Windows API ANSI encoding is dead!  Windows XP and later now use Unicode!

Windows and C/C++ have changed since 2003/2004.  Programmers need to adapt.

### Ruminations on modularizing WinAPI code

Standard boiler-plate C/C++ WinAPI code is cumbersome to read and maintain  Only two functions are required for a WinAPI compliant app, WinMain and WndProc, those 2 functions do more than one simple task.

This sub-chapter shows several ways to modularize code into separate "one task only" functions.

1. **minimal Win API single file used as a template for modularizing.** *<---- this example*
2. first version of modular Win API code.
3. second version of modular Win API code split into multiple files.
4. third version of modular Win API code further split into multiple files.
5. fourth version of modular Win API code, creating functions to process WM_ messages.
6. Rewriting #5 to allow for idle-time processing.

The first step in modularizing WinAPI code, creating a template for mashing the code around.

### Notes

There may be links to websites or internet sources in these pages and source code. Links can go stale. A search engine is your friend.

This is a "work in progress", there is no guarantee the entirety of the entire collection of games will ever be modernized any time soon or to what degree of modern usage.

There is also no guarantee the source files will work for you.

If you find better ways to rework the code please drop me a note and let me know.
