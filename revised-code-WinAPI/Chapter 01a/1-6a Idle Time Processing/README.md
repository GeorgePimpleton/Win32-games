# Programming Windows© Games, Chapter 1a

[![Language](https://img.shields.io/badge/Language%20-C++-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)
[![Platform](https://img.shields.io/badge/Platform%20-Win32-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)

### Background

Win9X/Me is officially dead!  Windows API ANSI encoding is dead!  Windows XP and later now use Unicode!

Windows and C/C++ have changed since 2003/2004.  Programmers need to adapt.

### Ruminations on modularizing Win API code

Standard boiler-plate C/C++ WinAPI code is cumbersome to read and difficult to maintain as the code's functionality increases.  Only two functions are required for a WinAPI compliant app, WinMain and WndProc. Those 2 functions do more than one simple task. Good functions should do one task as much as possible, simple and short.

This sub-chapter shows several ways to modularize code into separate "one task only" functions.

1. minimal WinAPI single file used as a template for modularizing.
2. first version of modular WinAPI code.
3. second version of modular WinAPI code split into multiple files.
4. third version of modular WinAPI code further split into multiple files.
5. fourth version of modular WinAPI code, creating functions to process WM_ messages.
6. **rewriting #5 to allow for idle-time processing.** *<----- this example*
7. a simple toolkit for using the C++ <random> library written as a C++20 module interface file.

Normal WinAPI apps spend a lot of time idle, waiting for user input.  Games can use "idle-time" processing to do their magic.

### Notes

There may be links to websites or internet sources in these pages and source code. Links can go stale. A search engine is your friend.

This is a "work in progress", there is no guarantee the entirety of the entire collection of games will ever be modernized any time soon or to what degree of modern usage.

There is also no guarantee the source files will work for you.

If you find better ways to rework the code please drop me a note and let me know.