# Win32 Games, Chapter 4: Slideshow, Drawing Graphical Images
[![Language](https://img.shields.io/badge/Language%20-C++-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)
[![Platform](https://img.shields.io/badge/Platform%20-Win32-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)

### Background
Win9X/Me is officially dead!  Windows API ANSI encoding is dead!  Windows XP and later now use Unicode!

Windows and C/C++ have changed since 2003/2004.  Programmers need to adapt.

### Slideshow: Drawing Graphical Images
Slideshow demonstrates how to load bitmap files from a file or as a Win32 resource.  Introduces the Bitmap class.

### Slideshow Versions
0. Chapter 4 on 2004 CD.  The program files are also on Chapter 5 2003 CD.  Each CD version of Slideshow has different pictures.
1. Bare minimum rewrite to get the source to compile.
2. Replaced all the gawd-awful C-style WinAPI "load a bitmap" code to use the WinAPI LoadImage function instead.  Why reinvent a flat wheel when the Win API, even the antique GDI, has built-in support.  Even the books' author  admits he had to get help to write that bitmap loading code.  \**Ouch!*\*
3. A minimal rewrite to use the image files from the 2003 CD.
4. Modern Windows compliant

I personally like the bitmap images on the 2003 CD.

### Notes
There may be links to websites or internet sources in these pages and source code. Links can go stale. A search engine is your friend.
