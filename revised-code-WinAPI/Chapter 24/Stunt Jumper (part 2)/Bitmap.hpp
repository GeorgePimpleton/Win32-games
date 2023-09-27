#pragma once

#include <windows.h>
#include "resource.h"

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
            Bitmap(HDC dc, int width, int height, COLORREF crColor = RGB(0, 0, 0));
   virtual ~Bitmap( );

   BOOL Create(HDC dc, PCWSTR fileName);
   BOOL Create(HDC dc, UINT resID, HINSTANCE inst);
   BOOL Create(HDC dc, int width, int height, COLORREF crColor);

   virtual void Draw(HDC dc, int x, int y, BOOL bTrans = FALSE,
                     COLORREF crTransColor = RGB(255, 0, 255));
   void DrawPart(HDC dc, int x, int y, int xPart, int yPart,
                 int wPart, int hPart, BOOL bTrans = FALSE,
                 COLORREF crTransColor = RGB(255, 0, 255));

   int GetWidth( )  { return m_width; };
   int GetHeight( ) { return m_height; };
};
