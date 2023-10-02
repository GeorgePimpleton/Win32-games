#pragma once

#include <windows.h>

class Bitmap
{
protected:
   HBITMAP m_bitmap;
   LONG    m_width;
   LONG    m_height;

public:
            Bitmap( );
            Bitmap(PCWSTR fileName);
            Bitmap(UINT resID, HINSTANCE inst);
            Bitmap(HWND nd, LONG width, LONG height, COLORREF color = RGB(0, 0, 0));
   virtual ~Bitmap( );

public:
   BOOL Create(PCWSTR fileName);
   BOOL Create(UINT resID, HINSTANCE inst);
   BOOL Create(HWND wnd, LONG width, LONG height, COLORREF color);

public:
   void Draw(HDC dc, int x, int y, BOOL trans = FALSE, COLORREF transColor = RGB(255, 0, 255));

public:
   constexpr LONG GetWidth( ) const { return m_width; }
   constexpr LONG GetHeight( ) const { return m_height; }

protected:
   void Free( );
};
