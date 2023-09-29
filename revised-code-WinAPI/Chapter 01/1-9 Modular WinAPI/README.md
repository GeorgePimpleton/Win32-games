# Programming Windows© Games, Chapter 1
[![Language](https://img.shields.io/badge/Language%20-C++-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)
[![Platform](https://img.shields.io/badge/Platform%20-Win32-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)

The basics of programming using the Windows API, in a quick nutshell.  

### Possible types of apps that can be created on Windows

9. modularizing WinAPI

Standard boiler-plate C/C++ WinAPI code is cumbersome to read and difficult to maintain as the code's functionality increases.  Only two functions are required for a WinAPI compliant app, WinMain and WndProc. Those 2 functions do more than one simple task. Good functions should do one task as much as possible, simple and short.

The beginning modularization example, the app initialization and message loop in WinMain are placed into separate functions.  Though the functions are still contained in a single source code file.
