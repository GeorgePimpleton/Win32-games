# Programming Windows© Games, Chapter 1

[![Language](https://img.shields.io/badge/Language%20-C++-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)
[![Platform](https://img.shields.io/badge/Platform%20-Win32-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)

### Background

Win9X/Me is officially dead!  Windows API ANSI encoding is dead!  Windows XP and later now use Unicode!

Windows and C/C++ have changed since 2003/2004.  Programmers need to adapt.

### Possible types of apps that can be created on Windows

1. console mode using C++
2. Windows© MessageBox
3. minimum Win API  *<----- this example*
4. minimum MFC
5. minimal skeletal Win API with app icons
6. realistic skeletal Win API with icons and cursor (pointer)
7. a modernized C++ practices skeletal Win API with icons and cursor and using C++ wide strings

The two examples shows the minimum needed to recreate the minimum needed to create a fully formed and functional WinAPI app.  The first example shows older WinAPI usages, allowing for ANSI and Unicode encodings for the WinAPI functio ns.  This is original style WinAPI coding as done by Charles Petzold back in the Win9X/Me days.  This style should be dead and buries.

The second example uses strictly Unicode, so you'll see a lot of W suffixes on function names.  This is the style I use when creating WinAPI apps.

Get used to it.  "This will be on 'the test'."

I also use SAL, the WinAPI function annotation on WinMain.  Visual Studio 'whinges' if it is missing.  It doesn't stop the code from compiling or the executable from running.

This is the last example that shows the two different coding methods, from now on it will be strictly Unicode.

### Notes

There may be links to websites or internet sources in these pages and source code. Links can go stale. A search engine is your friend.

This is a "work in progress", there is no guarantee the entirety of the entire collection of games will ever be modernized any time soon.  There is also no guarantee the source files will work for you.

If you find better ways to rework the code don't hesitate to let me know.

### Musings on Windows© Games

To be honest the game engine created is hard to convert to modern C++ practices.  More and more it is looking like the game engine needs to be rewritten from top to bottom from scratch.

### Musing on the Windows© API

I tried to convert the #define macros to modern C++ constexpr/const values, but the Win API refused to work that way.  Not. Cool.
