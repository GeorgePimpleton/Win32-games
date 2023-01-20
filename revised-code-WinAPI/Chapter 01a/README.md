# Programming Windows© Games, Chapter 1a

[![Language](https://img.shields.io/badge/Language%20-C++-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)
[![Platform](https://img.shields.io/badge/Platform%20-Win32-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)

### Background

Win9X/Me is officially dead!  Windows API ANSI encoding is dead!  Windows XP and later now use Unicode!

Windows and C/C++ have changed since 2003/2004.  Programmers need to adapt.

### Ruminations on modularizing WinAPI code

Standard boiler-plate C/C++ WinAPI code is cumbersome to read and maintain.  Only two functions are required for a WinAPI compliant app, WinMain and WndProc. Those 2 functions do more than one simple task. Good functions should do one task as much as possible, simple and short.

This sub-chapter shows several ways to modularize code into multiple and separate "one task only" functions.

1. minimal WinAPI single file used as a template for modularizing.
2. first version of modular WinAPI code.
3. second version of modular WinAPI code split into multiple files.
4. third version of modular WinAPI code further split into multiple files.
5. fourth version of modular WinAPI code, creating functions to process WM_ messages.
6. Rewriting #5 to allow for idle-time processing.

Not so coincidentally these WinAPI code modular examples are similar to the code structuring that shows up in the game examples.  The idea of modularizing the code was not derived from the game code, though.

### About <windowsx.h> and "message cracking"

The WndProc function is especially susceptible to bloated code.  The more functionality you add to your Win API app, the more WM_messages you have to handle, the more switch cases you create.  The more switch cases the bigger the  WndProc function grows.

Enter the idea of writing WinAPI helpter that "cracks windows messages."

Some useful information how to use "[message cracking](https://www.codeproject.com/Articles/4948/Message-Cracker-Wizard-for-Win-SDK-Developers)" and "[Message Crackers](https://flylib.com/books/en/4.419.1.188/1/)".

A ["message cracker wizard"](https://github.com/hernandp/MessageCrackerWizard) is available at Hernán Di Pietro's GitHub repository to make using "message cracking" easier.  I used the tool to create the message cracking example.

### Notes

There may be links to websites or internet sources in these pages and source code. Links can go stale. A search engine is your friend.

This is a "work in progress", there is no guarantee the entirety of the entire collection of games will ever be modernized any time soon or to what degree of modern usage.

There is also no guarantee the source files will work for you.

If you find better ways to rework the code please drop me a note and let me know.