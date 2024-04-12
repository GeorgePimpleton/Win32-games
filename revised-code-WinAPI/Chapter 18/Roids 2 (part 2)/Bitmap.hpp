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
            Bitmap(PCWSTR);
            Bitmap(UINT);
            Bitmap(HDC, int, int, COLORREF = RGB(0, 0, 0));
   virtual ~Bitmap( );

   BOOL Create(PCWSTR);
   BOOL Create(UINT);
   BOOL Create(HDC, int, int, COLORREF);

   void Draw(HDC, int, int, BOOL = FALSE, COLORREF = RGB(255, 0, 255));
   void DrawPart(HDC, int, int, int, int, int, int,
                 BOOL = FALSE, COLORREF = RGB(255, 0, 255));

   constexpr int GetWidth( ) const  { return m_width; };
   constexpr int GetHeight( ) const { return m_height; };
};
