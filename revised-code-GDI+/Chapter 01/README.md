# Programming Windows© Games Using GDI+

[![Language](https://img.shields.io/badge/Language%20-C++-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)
[![Platform](https://img.shields.io/badge/Platform%20-Win32-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)

### Background

Win9X/Me is officially dead!  Windows API ANSI encoding is dead!  Windows XP and later now use Unicode!

Windows and C/C++ have changed since 2003/2004.  Programmers need to adapt.

### Using GDI+: The Basics

1. Starting Up and Shutting Down the GDI+ System.
2. Using the GDI to Draw Lines.
3. Using GDI+ to Draw Lines.
4. Using the GDI to Draw Strings.
5. Using GDI+ to Draw Strings.
6. Using the GDI to Draw Images.
7. Using GDI+ to Draw Images.
8. GDI+ and Error Checking.

The examples in this section use <windowsx.h> message cracking for the WNDPROC, so the relevant GDI+ functionality other than intializating and shutting down the system is used in a separate function located at the top of the source file for ease of maintenance.

### GDI+ vs. GDI

GDI+ was an enhancement to the original GDI system that was core to Windows.  GDI+ is a C++ class-based API.

Because it is newer technology than the GDI using GDI+ requires initializing and shutting down the system in WinAPI code.

I show the basics of common drawing usages for both the GDI and GDI+ systems.  Drawing l,ine

### Notes

There may be links to websites or internet sources in these pages and source code. Links can go stale. A search engine is your friend.

This is a "work in progress", there is no guarantee the entirety of the entire collection of games will ever be modernized any time soon or to what degree of modern usage.

There is also no guarantee the source files will work for you.

If you find better ways to rework the code please drop me a note and let me know.