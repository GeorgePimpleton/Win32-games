# Programming Windows© Games, Chapter 1

### Background

Win9X/Me is officially dead!  Windows API ANSI encoding is dead!  Windows XP and later now use Unicode!

Windows and C/C++ have changed since 2003/2004.  Programmers need to adapt.

### Possible types of apps that can be created on Windows

1. console mode using C++
2. Windows© MessageBox
3. minimum Win API
4. minimum MFC
5. minimal skeletal Win API with app icons
6. realistic skeletal Win API with icons and cursor (pointer)
7. a modernized C++ practices skeletal Win API with icons and cursor and using C++ wide strings

### Notes

There may be links to websites or internet sources in these pages and source code. Links can go stale. A search engine is your friend.

This is a "work in progress", there is no guarantee the entirety of the entire collection of games will ever be modernized any time soon.  There is also no guarantee the source files will work for you.

If you find better ways to rework the code don't hesitate to let me know.

### Musings on Windows© Games

To be honest the game engine created is hard to convert to modern C++ practices.  More and more it is looking like the game engine needs to be rewritten from top to bottom from scratch.

### Musing on the Windows© API

I tried to convert the #define macros to modern C++ constexpr/const values, but the Win API refused to work that way.  Not. Cool.
