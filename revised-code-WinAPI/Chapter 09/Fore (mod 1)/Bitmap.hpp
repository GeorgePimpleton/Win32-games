#pragma once

#include <windows.h>

class Bitmap
{
public:
            Bitmap( );
            Bitmap(PCWSTR fileName);
            Bitmap(UINT resID, HINSTANCE inst);
            Bitmap(HWND wnd, LONG width, LONG height, COLORREF color = RGB(0, 0, 0));
   virtual ~Bitmap( );

public:
   BOOL Create(PCWSTR fileName);
   BOOL Create(UINT resID, HINSTANCE inst);
   BOOL Create(HWND wnd, LONG width, LONG height, COLORREF color);

public:
   void Draw(HDC dc, int x, int y, BOOL trans = FALSE, COLORREF transColor = RGB(255, 0, 255));

public:
   LONG GetWidth( );
   LONG GetHeight( );

protected:
   void Free();

protected:
   HBITMAP m_bitmap;
   LONG    m_width;
   LONG    m_height;
};

inline LONG Bitmap::GetWidth( )
{
   return m_width;
};

inline LONG Bitmap::GetHeight( )
{
   return m_height;
};
