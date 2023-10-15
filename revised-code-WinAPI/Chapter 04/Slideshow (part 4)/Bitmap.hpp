#pragma once

#include <windows.h>

class Bitmap
{
protected:
   HBITMAP m_hBitmap;
   int     m_width;
   int     m_height;

   void Free( );

public:
            Bitmap( );
            Bitmap(PCWSTR);
            Bitmap(UINT resID);
            Bitmap(HDC, int, int, COLORREF color = RGB(0, 0, 0));
   virtual ~Bitmap( );

   BOOL Create(PCWSTR);
   BOOL Create(UINT resID);
   BOOL Create(HDC, int width, int height, COLORREF color);

   void Draw(HDC, int, int);

   constexpr int GetWidth( ) const  { return m_width; };
   constexpr int GetHeight( ) const { return m_height; };
};
