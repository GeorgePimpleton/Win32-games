# Programming Windows© Games, Chapter 1a

### Background

Win9X/Me is officially dead!  Windows API ANSI encoding is dead!  Windows XP and later now use Unicode!

Windows and C/C++ have changed since 2003/2004.  Programmers need to adapt.

### Ruminations on modularizing Win API code

Standard boiler-plate C/C++ Win32 API code is cumbersome to read and maintain.  Only two functions are required for a Win API compliant app those 2 functions do more than one simple task.  This sub-chapter shows several ways to  modularize the code into separate "one task only" functions.

1. minimal Win API single file used as a template for modularizing.
2. first version of modular Win API code.
3. second version of modular Win API code split into multiple files.
4. third version of modular Win API code further split into multiple files.
5. fourth version of modular Win API code, creating functions to process WM_ messages.
6. fifth version of modular Win API code, using <windowsx.h> and "message cracking" macros to process WM_ messages.
7. Rewriting #6 to allow for idle-time processing.

Not so coincidentally these Win API code modular examples show up in the game examples.  The idea of modularizing the code was not derived from the game code, though.

### Notes

There may be links to websites or internet sources in these pages and source code. Links can go stale. A search engine is your friend.

This is a "work in progress", there is no guarantee the entirety of the entire collection of games will ever be modernized any time soon.  There is also no guarantee the source files will work for you.

If you find better ways to rework the code don't hesitate to let me know.
