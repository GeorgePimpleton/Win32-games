# Programming Windows Games, Chapter 1, Minimal WinAPI App
[![Language](https://img.shields.io/badge/Language%20-C++-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)
[![Platform](https://img.shields.io/badge/Platform%20-Win32-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)

The basics of programming using the Windows API, in a quick nutshell.  

### Possible types of apps that can be created on Windows
4. a minimal WinAPI app
   + a minimal WinAPI app using modern coding
   + a minimal WinAPI app using modern coding with different WinAPI data types and structures

The three examples shows the minimum needed to recreate create a fully formed and functional WinAPI app.  That doesn't "do" anything except be a very minimal Windows© compliant app.

The first example shows older WinAPI usages, allowing for ANSI and Unicode encodings for the WinAPI functio ns.  This is original style WinAPI coding as done by Charles Petzold back in the Win9X/Me days.  This style should be dead and buries.

The second example uses strictly Unicode/wide char functions, so you'll see a lot of W suffixes on function names.  I also use generic return code labels (S_OK, E_FAIL) for better readability.

I also use SAL, MS source-code annotation language, on WinMain.  Visual Studio 'whinges' if it is missing.  It doesn't stop the code from compiling or the executable from running.

https://learn.microsoft.com/en-us/cpp/code-quality/understanding-sal

The third example uses a few different WinAPI data types and structures from the second example.  This is the style I use when creating WinAPI apps.

Get used to it.  "This will be on 'the test'."
