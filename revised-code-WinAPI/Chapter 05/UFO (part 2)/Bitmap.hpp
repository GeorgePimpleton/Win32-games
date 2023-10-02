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
            Bitmap(UINT resID);
            Bitmap(HDC dc, int width, int height, COLORREF color = RGB(0, 0, 0));
   virtual ~Bitmap( );

   BOOL Create(PCWSTR fileName);
   BOOL Create(UINT resID);
   BOOL Create(HDC dc, int width, int height, COLORREF color);

   void Draw(HDC dc, int x, int y, BOOL trans = FALSE,
             COLORREF transColor = RGB(255, 0, 255));

   constexpr LONG GetWidth( ) const  { return m_width; };
   constexpr LONG GetHeight( ) const { return m_height; };
};
