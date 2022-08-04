# Win32 Games, Chapter 4: Slideshow

Win9X/Me is officially dead!

Slideshow demonstrates how to load bitmap files from a file or as a Win32 resource.

Versions:

Blizzard, orginal: Chapter 5 on 2003 CD.  The program files are also on Chapter 4 2004 CD.  Each version of Slideshow has different pictures.

Blizzard, pass 1: Replaced all the gawd-awful C-style "load a bitmap" code to use the Win32 LoadImage function instead.  Why reinvent a flat wheel when the Win API has built-in support.

Is this the final version?  Probably, but maybe not.  Depends on later game code that is updated from the original.

Possible updated version(s), use GDI+ and/or DirectX so more than bitmap files can be used.