#pragma once

#include <windows.h>

class Bitmap
{
protected:
   HBITMAP m_hBitmap;
   int     m_iWidth;
   int     m_iHeight;

   void Free( );

public:
            Bitmap( );
            Bitmap(HDC hDC, PCWSTR szFileName);
            Bitmap(HDC hDC, UINT uiResID, HINSTANCE hInstance);
            Bitmap(HDC hDC, int iWidth, int iHeight, COLORREF crColor = RGB(0, 0, 0));
   virtual ~Bitmap( );

   BOOL Create(HDC hDC, PCWSTR szFileName);
   BOOL Create(HDC hDC, UINT uiResID, HINSTANCE hInstance);
   BOOL Create(HDC hDC, int iWidth, int iHeight, COLORREF crColor);

   void Draw(HDC hDC, int x, int y, BOOL bTrans = FALSE, COLORREF crTransColor = RGB(255, 0, 255));
   void DrawPart(HDC hDC, int x, int y, int xPart, int yPart,
                 int wPart, int hPart, BOOL bTrans = FALSE, COLORREF crTransColor = RGB(255, 0, 255));

   int GetWidth( )  { return m_iWidth; };
   int GetHeight( ) { return m_iHeight; };
};
