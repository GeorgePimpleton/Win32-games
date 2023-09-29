# Programming Windows© Games, Chapter 1
[![Language](https://img.shields.io/badge/Language%20-C++-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)
[![Platform](https://img.shields.io/badge/Platform%20-Win32-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)

The basics of programming using the Windows API, in a quick nutshell.  

### Possible types of apps that can be created on Windows

12. modularizing WinAPI

### About <windowsx.h> and "message cracking"
The WndProc function is especially susceptible to bloated code.  The more functionality you add to your Win API app, the more WM_messages you have to handle, the more switch cases you create.  The more switch cases the bigger the  WndProc function grows.

Enter the idea of writing WinAPI that "cracks windows messages."

Some useful information how to use "[message cracking](https://www.codeproject.com/Articles/4948/Message-Cracker-Wizard-for-Win-SDK-Developers)" and "[Message Crackers](https://flylib.com/books/en/4.419.1.188/1/)".

A ["message cracker wizard"](https://github.com/hernandp/MessageCrackerWizard) is available at Hernán Di Pietro's GitHub repository to make using "message cracking" easier.  I used the tool to create the message cracking example.
