# Win32 Games, Chapter 4

[![Language](https://img.shields.io/badge/Language%20-C++-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)
[![Platform](https://img.shields.io/badge/Platform%20-Win32-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)

### Background

Win9X/Me is officially dead!  Windows API ANSI encoding is dead!  Windows XP and later now use Unicode!

Windows and C/C++ have changed since 2003/2004.  Programmers need to adapt.

### Slideshow: Drawing Graphical Images

Slideshow demonstrates how to load bitmap files from a file or as a Win32 resource.  Introduces the Bitmap class.

### Slideshow Versions

0. Chapter 5 on 2003 CD.  The program files are also on Chapter 4 2004 CD.  Each version of Slideshow has different pictures.

1. Replaced all the gawd-awful C-style "load a bitmap" code to use the Win32 LoadImage function instead.  Why reinvent a flat wheel when the Win API, even the antique GDI, has built-in support.

Is this the final version?  Probably, but maybe not.  Depends on later game code that is updated from the original.

Possible updated version(s), use GDI+ and/or DirectX so more than bitmap files can be used.

### Notes

There may be links to websites or internet sources in these pages and source code. Links can go stale. A search engine is your friend.

This is a "work in progress", there is no guarantee the entirety of the entire collection of games will ever be modernized any time soon.  There is also no guarantee the source files will work for you.

If you find better ways to rework the code don't hesitate to let me know.

### Musings on Windows© Games

To be honest the game engine created is hard to convert to modern C++ practices.  More and more it is looking like the game engine needs to be rewritten from top to bottom from scratch.
