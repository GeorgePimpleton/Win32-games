# Programming Windows© Games, Chapter 1

[![Language](https://img.shields.io/badge/Language%20-C++-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)
[![Platform](https://img.shields.io/badge/Platform%20-Win32-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)

The basics of programming using the Windows API, in a quick nutshell.  

### Background

Win9X/Me is officially dead!  Windows API ANSI encoding is dead!  Windows XP and later now use Unicode!

Windows and C/C++ have changed since 2003/2004.  Programmers need to adapt.

### Possible types of apps that can be created on Windows

1. console mode using C++
2. Windows© MessageBox   *<----- this example*
3. minimum Win API
4. minimum MFC
5. minimal skeletal Win API with app icons
6. realistic skeletal Win API with icons and cursor (pointer)
7. a modernized C++ practices skeletal Win API with icons and cursor and using C++ wide strings

The two examples shows the bare minimum needed to recreate the "Hello World" console example using a WinAPI message box.  The first example shows older WinAPI usages, allowing for ANSI and Unicode encodings for the WinAPI functio ns.  This is original style WinAPI coding as done by Charles Petzold back in the Win9X/Me days.  This style should be dead and buries.

The second example uses strictly Unicode, so you'll see a lot of W suffixes on function names.  This is the style I use when creating WinAPI apps.

Get used to it.  "This will be on 'the test'."

I also use SAL, the WinAPI function annotation on WinMain.  Visual Studio 'whinges' if it is missing.  It doesn't stop the code from compiling or the executable from running.

https://learn.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-2013/ms182032(v=vs.120)

### Notes

There may be links to websites or internet sources in these pages and source code. Links can go stale. A search engine is your friend.

This is a "work in progress", there is no guarantee the entirety of the entire collection of games will ever be modernized any time soon.  There is also no guarantee the source files will work for you.
