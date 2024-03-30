# Win32 Games, Chapter 2, part 1: Minimal Revisions
[![Language](https://img.shields.io/badge/Language%20-C++-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)
[![Platform](https://img.shields.io/badge/Platform%20-Win32-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)

### Blizzard Versions
1. The bare minimum changes to work with modern C++/WinAPI x86 & x64.

### What was done
- Removed a lot of what I consider extraneous and useless comments.
- Reformatted the source to look better for easier readability.
   - Separated several class data members so they are declared separately.
   - One part of "looks better" means adding parentheses on control statements for better readability of the code blocks.
- Increased the size of the GameEngine class data members, m_szWindowClass and m_szTitle, to 64.  A length of 32 TCHARs seems just too small to me.

The following changes had to happen to be compileable:
+ Changed two of the GameEngine ctor's parameter types from `LPTSTR` to `LPCTSTR`.
+ Changed one of the WinAPI DrawIcon parameters from `(HICON) (WORD) GetClassLong(hWindow, GCL_HICON)` to `(HICON) GetClassLongPtr(hWindow, GCLP_HICON)`.

### Notes
There may be links to websites or internet sources in these pages and source code. Links can go stale. A search engine is your friend.
