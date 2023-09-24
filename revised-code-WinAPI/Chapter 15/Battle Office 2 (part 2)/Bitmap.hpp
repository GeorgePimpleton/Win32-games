#pragma once

#include <windows.h>

class Bitmap
{
protected:
   HBITMAP m_bitmap;
   LONG    m_width;
   LONG    m_height;

   void Free( );

public:
            Bitmap( );
            Bitmap(PCWSTR fileName);
            Bitmap(UINT resID, HINSTANCE inst);
            Bitmap(HDC dc, LONG width, LONG height, COLORREF color = RGB(0, 0, 0));
   virtual ~Bitmap( );

   BOOL Create(PCWSTR fileName);
   BOOL Create(UINT resID, HINSTANCE inst);
   BOOL Create(HDC dc, LONG width, LONG height, COLORREF color);

   void Draw(HDC dc, int x, int y, BOOL trans = FALSE,
             COLORREF transColor = RGB(255, 0, 255));
   void DrawPart(HDC dc, int x, int y, int xPart, int yPart,
                 int wPart, int hPart, BOOL trans = FALSE,
                 COLORREF transColor = RGB(255, 0, 255));

   int GetWidth( )  { return m_width; };
   int GetHeight( ) { return m_height; };
};
