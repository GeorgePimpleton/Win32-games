# Programming Windows Games
[![Language](https://img.shields.io/badge/Language%20-C++-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)
[![Platform](https://img.shields.io/badge/Platform%20-Win32-blue.svg)](https://github.com/GeorgePimpleton/Win32-games/)

## Background
Win9X/Me is officially dead!  Windows API ANSI encoding is dead!  Windows XP and later now use Unicode!

Windows and C/C++ have changed since 2003/2004.  Programmers need to adapt.

## The Game Code in 24 (or so) Chapters
1. A Basic Programming Primer (Not just WinAPI)
2. Blizzard: Creating An Engine For Games
3. Trippy: Learning to Draw Basic Graphics
4. Slideshow: Drawing Graphical Images
5. UFO: Controlling Games with the Keyboard and Mouse
6. Example Game: Brainiac
7. UFO 2: Improving Input with Joysticks
8. Example Game: Light Cycles
9. Fore: Making Things Move with Sprites

![under construction](construction.gif)  \*\* ***under construction*** \*\*  ![under construction](construction.gif)

10. Fore 2: Managing a World of Sprites

![queue](queue.jpg)  \*\* ***in the queue*** \*\*  ![queue](queue.jpg)

10. Fore 2: Managing a World of Sprites
11. Example Game: Henway
12. Brainiac 2: Playing Digital Sound Effects/Wave Files
13. Henway 2: Playing MIDI Music
14. Example Game: Battle Office
15. Battle Office 2: Animating the Appearance of Sprites
16. Roids: Creating Backgrounds for Your Sprites
17. Example Game: Meteor Defense
18. Roids 2: Teaching Games to Think
19. Example Game: Space Out
20. Space Out 2: Adding Pizzazz to Your Game with a Splash Screen
21. Space Out 3: Showing Off Your Game with Demo Mode
22. Space Out 4: Keeping Track of High Scores
23. Wanderer: Changing Perspective with Scrolling Backgrounds
24. Example Game: Stunt Jumper

----
### Caveat on the original 2003 & 2004 source code
Since the original source code for the Win Games was written in 2003/2004 the game examples aren't likely to compile with a modern compiler such as Visual Studio 2022. The WinAPI has changed a lot to work for x64 processors as well as other changes for the 21st Century.  The first pass in each chapter will be the bare minimum needed to work now for both x86 (32-bit) and x64, with some minor reformatting and excessive comment cleanup.

There is a difference in the frame layout for the animated sprites between the two books.  I used the 2003 source code for the first animated sprite example, for all the rest of the examples that have versions in both books I use the source from the 2004 book.  The last two examples are only in the 2004 book

----
### Notes
There may be links to websites or internet sources in these pages and source code. Links can go stale. A search engine is your friend.

This is a "work in progress", there is no guarantee the entirety of the entire collection of games will ever be modernized any time soon or to what degree of modern usage.

There is also no guarantee the source files, orginal or modernized, will work for you.  All I can guarantee is the source(s) work for me.

If you find better ways to rework the code please drop me a note and let me know.

### Musings on Windows Games
The game engine as created may be old, but still is (mostly) workable for modern WinAPI practices.

The WinAPI/Visual Studio/Windows 10 doesn't like some C++ stdlib assets.  Smart pointers as class data member types really don't work, raw pointers are still needed so the apps don't "crash" on exit.
