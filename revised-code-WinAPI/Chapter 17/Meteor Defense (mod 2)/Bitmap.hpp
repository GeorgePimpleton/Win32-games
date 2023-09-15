#pragma once

#include <windows.h>

class Bitmap
{
protected:
   HBITMAP m_bitmap;
   int     m_width;
   int     m_height;

   void Free( );

public:
            Bitmap( );
            Bitmap(HDC dc, PCWSTR fileName);
            Bitmap(HDC dc, UINT resID, HINSTANCE inst);
            Bitmap(HDC dc, int width, int height, COLORREF color = RGB(0, 0, 0));
   virtual ~Bitmap( );

   BOOL Create(HDC dc, PCWSTR fileName);
   BOOL Create(HDC dc, UINT resID, HINSTANCE inst);
   BOOL Create(HDC dc, int width, int height, COLORREF color);

   void Draw(HDC dc, int x, int y, BOOL trans = FALSE,
             COLORREF transColor = RGB(255, 0, 255));
   void DrawPart(HDC dc, int x, int y, int xPart, int yPart,
                 int wPart, int hPart, BOOL trans = FALSE, COLORREF transColor = RGB(255, 0, 255));

   int GetWidth( ) const  { return m_width; };
   int GetHeight( ) const { return m_height; };
};
