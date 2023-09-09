# Programming Windows© Games, Chapter 1

[![Language](https://img.shields.io/badge/Language%20-C++-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)
[![Platform](https://img.shields.io/badge/Platform%20-Win32-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)

### Background

Win9X/Me is officially dead!  Windows API ANSI encoding is dead!  Windows XP and later now use Unicode!

Windows and C/C++ have changed since 2003/2004.  Programmers need to adapt.

### Possible types of apps that can be created on Windows

1. console mode using C++
    + console mode using C++20's modules
2. using a custom C++ header file to use C++11 (and later) random engine features instead of the C library functions
    + using a custom C\+\+20 module to use C++11 (and later) random engine features instead of the C library functions
3. a Windows API MessageBox
    + a WinAPI MessageBox using modern coding
4. a minimal WinAPI app
     + a minimal WinAPI app using modern coding
5. a minimal skeletal WinAPI app
6. a realistic skeletal WinAPI app with custom assets
7. a skeletal WinAPI app using C++ wide strings using modernized C++ practices 
8. a minimum MFC app
9. modularizing WinAPI
10. modularizing WinAPI
11. modularizing WinAPI
12. **modularizing WinAPI** *<----- this example*
13. modularizing WinAPI

### About <windowsx.h> and "message cracking"

The WndProc function is especially susceptible to bloated code.  The more functionality you add to your Win API app, the more WM_messages you have to handle, the more switch cases you create.  The more switch cases the bigger the  WndProc function grows.

Enter the idea of writing WinAPI that "cracks windows messages."

Some useful information how to use "[message cracking](https://www.codeproject.com/Articles/4948/Message-Cracker-Wizard-for-Win-SDK-Developers)" and "[Message Crackers](https://flylib.com/books/en/4.419.1.188/1/)".

A ["message cracker wizard"](https://github.com/hernandp/MessageCrackerWizard) is available at Hernán Di Pietro's GitHub repository to make using "message cracking" easier.  I used the tool to create the message cracking example.

### Notes

There may be links to websites or internet sources in these pages and source code. Links can go stale. A search engine is your friend.

This is a "work in progress", there is no guarantee the entirety of the entire collection of games will ever be modernized any time soon or to what degree of modern usage.

There is also no guarantee the source files will work for you.

If you find better ways to rework the code please drop me a note and let me know.
