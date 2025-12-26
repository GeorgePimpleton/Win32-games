# Programming Windows Games
[![Language](https://img.shields.io/badge/Language%20-C++-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)
[![Platform](https://img.shields.io/badge/Platform%20-Win32-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)
### Background
Win9X/Me is officially dead!  Windows API ANSI encoding is dead!  Windows XP and later now use strictly Unicode!

Windows and C/C++ have changed since 2003/2004.  Programmers need to adapt.

No representation is made that the source code belongs to me, it's simply reproduced here for convenience (so you don't need to dig up the physical media of the books) and remains the copyright of Michael Morrison.

### Why this github repository exists
I own a couple of \*\****OLD***\*\* (© 2003 & 2004) books about creating 2D games using the Windows API.

No representation is made that the source code belongs to me, it remains the copyright of Michael Morrison and Samspublishing.


![Sams Teach Yourself Game Programming in 24 Hours (2003)](tygp24h.jpg) ![Sams Beginning Game Programming (2004)](bgp.jpg)

I've spent a lot of time -- years -- updating and revising this pre-C++11 code to work with modern Windows and the newer C++ language and WinAPI standards.

Why did it take so long you might ask?  I was learning C++/WinAPI fundamentals as they changed over the years as well.  Yeah, I'm a slow and methodical learner.


There are no executables included, you need to create them by compiling the sources.  Learn to program by compiling, compiling, compiling.

There are 4 directories of original and revised game code sources.  The original game code from the two CDs, the WinAPI revisions dir, and two additional dirs for revised code using GDI+ and Direct2D.  Currently the GDI+ & D2D dirs are mostly empty placeholders.

### Compiler used
I've used Visual Studio 2015, 2017, 2019, 2022 and 2026 (Community) to update and test the game code. 

The sources were adapted and compiled using Visual Studio 2022 & 2026, the Community editions. Currently I am using VS 2026, setting the C++ language standard to c++latest.

You should think about using VS 2026 as well.  The Community edition is free for non-commercial use.

[Download Visual Studio 2026 Community](https://visualstudio.microsoft.com/downloads/)

### About the revised sources
I've been working with the games source code from both books for years, plinking around and trying out newer C++ features as I've run across them.  Most of the example code is the same between the books, so I've combined the examples into one representative example for each chapter.

The main difference between the books is the orientation of the animated sprite bitmaps.  When dealing with animated sprites all the source code is from the 2004 book, except for the first animated game example.  That one uses the code from 2003.

The last two game examples are found only in the 2004 book and CD.

### Notes
There may be links to websites or internet sources in these pages and source code. Links can go stale. A search engine is your friend.

This is a "work in progress", there is no guarantee the entirety of the entire collection of games will ever be modernized any time soon.  There is also no guarantee the source files will work for you.

If you find better ways to rework the code don't hesitate to let me know.

Do the games work on Win 11?  I honestly don't know.  They should, but all 3 of my computers are unable to upgrade to Win 11.  And TBH I really don't see any advantage to upgrading from Win 10 at this time.

### Musings on Windows Games
When I originally began revising this code I was using Win98SE.  I kinda skipped over revising the game code when I upgraded to WinXP.  The real intensive work began when using Win 7 and MS released their free IDE suites with Visual Studio 2015 Community.  Now I am on Win 10 and VS 2022.

Revising the original code to work with a modern Windows compiler isn't hard, just a couple of changes and additions to get a bare-bones working version of each game.

1. Add `#pragma comment(lib, "somelibrary.lib")` in one of the source files that uses a WinAPI library -- like the Windows multi-media system library -- and you are "good to go."  No need to modify/update your project's settings to use the .lib file(s).
2. Update the WinAPI strings to be consts and usable for Unicode.  Change LPTSTR to PCTSTR/PCWSTR.  Yeah, those are Windows data types.

I made other changes such as adding menus to make playing the games more intuitive in a GUI environment.
