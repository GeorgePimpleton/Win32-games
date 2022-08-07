# Win32 Games, Chapter 2: Blizzard

### Background

Win9X/Me is officially dead!  Windows API ANSI encoding is dead!  Windows XP and later now use Unicode!

Windows and C/C++ have changed since 2003/2004.  Programmers need to adapt.

### Creating An Engine For Games

Blizzard, creates and uses the Game Engine class.  From the 2004 CD.

### Versions

Blizzard, orginal: Original CD game files written in 2004.

Blizzard, pass 1: The bare minimum changes to work with modern C++/Win32 x86 & x64.  SAL is a minimum.

Blizzard, pass 2: Reformatting, changing some file names and refining the code for Unicode and modern Windows OSes.  Added my Random Toolkit to generate random numbers the C++ way.

Blizzard, pass 3: Added menus, inline the setters and getters and other misc. changes.

Blizzard, pass 4: Use C++ smart pointers, shared and unique, instead of raw pointers.

Blizzard, pass 5: Changed the shared pointers to unique, any WCHAR string arrays in the Game Engine class are now std::wstring.

Is pass 5 the final version?  Probably, but maybe not.  Depends on later game code that is updated to use smart pointers.