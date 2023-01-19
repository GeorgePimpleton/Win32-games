# Programming Windows© Games, Chapter 1

[![Language](https://img.shields.io/badge/Language%20-C++-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)
[![Platform](https://img.shields.io/badge/Platform%20-Win32-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)

The basics of programming using the Windows API, in a quick nutshell.  

### Background

Win9X/Me is officially dead!  Windows API ANSI encoding is dead!  Windows XP and later now use Unicode!

Windows and C/C++ have changed since 2003/2004.  Programmers need to adapt.

### Possible types of apps that can be created on Windows

1. console mode using C++  *<---- This example*
2. Windows© MessageBox
3. minimum Win API
4. minimum MFC
5. minimal skeletal Win API with app icons
6. realistic skeletal Win API with icons and cursor (pointer)
7. a modernized C++ practices skeletal Win API with icons and cursor and using C++ wide strings

If you have done any C/C++ programming you've seen this example.

### Notes

There may be links to websites or internet sources in these pages and source code. Links can go stale. A search engine is your friend.

This is a "work in progress", there is no guarantee the entirety of the entire collection of games will ever be modernized any time soon.  There is also no guarantee the source files will work for you.

If you find better ways to rework the code don't hesitate to let me know.

### Musing on the Windows© API

I tried to convert the #define macros to modern C++ constexpr/const values, but the Win API refused to work that way.  Not. Cool.
