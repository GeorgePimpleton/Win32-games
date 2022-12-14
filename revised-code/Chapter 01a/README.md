# Programming Windows© Games, Chapter 1a

[![Language](https://img.shields.io/badge/Language%20-C++-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)
[![Platform](https://img.shields.io/badge/Platform%20-Win32-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)

### Background

Win9X/Me is officially dead!  Windows API ANSI encoding is dead!  Windows XP and later now use Unicode!

Windows and C/C++ have changed since 2003/2004.  Programmers need to adapt.

### Ruminations on modularizing Win API code

Standard boiler-plate C/C++ Win32 API code is cumbersome to read and maintain  Only two functions are required for a Win API compliant app, WinMain and WndProc, those 2 functions do more than one simple task.

This sub-chapter shows several ways to  modularize the code into separate "one task only" functions.

1. minimal Win API single file used as a template for modularizing.
2. first version of modular Win API code.
3. second version of modular Win API code split into multiple files.
4. third version of modular Win API code further split into multiple files.
5. fourth version of modular Win API code, creating functions to process WM_ messages.
6. fifth version of modular Win API code, using <windowsx.h> and "message cracking" macros to process WM_ messages.
7. Rewriting #6 to allow for idle-time processing.

Not so coincidentally these Win API code modular examples show up in the game examples.  The idea of modularizing the code was not derived from the game code, though.

### About <windowsx.h> and "message cracking"

The WndProc function is especially susceptible to bloated code.  The more functionality you add to your Win API app, the more WM_messages you have to handle, the more switch cases you create.  The more switch cases the bigger the  WndProc function grows.

Enter the idea of writing Win API that "cracks windows messages."

Some useful information how to use "[message cracking](https://www.codeproject.com/Articles/4948/Message-Cracker-Wizard-for-Win-SDK-Developers)" and "[Message Crackers](https://flylib.com/books/en/4.419.1.188/1/)".

A ["message cracker wizard"](https://github.com/hernandp/MessageCrackerWizard) is available at Hernán Di Pietro's GitHub repository to make using "message cracking" easier.  I used the tool to create the message cracking example.

### Notes

There may be links to websites or internet sources in these pages and source code. Links can go stale. A search engine is your friend.

This is a "work in progress", there is no guarantee the entirety of the entire collection of games will ever be modernized any time soon.  There is also no guarantee the source files will work for you.

If you find better ways to rework the code don't hesitate to let me know.




