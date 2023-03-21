//-----------------------------------------------------------------
// Bitmap Object
// C++ Header - Bitmap.hpp
//-----------------------------------------------------------------

#pragma once

#include <windows.h>

//-----------------------------------------------------------------
// Bitmap class
//-----------------------------------------------------------------
class Bitmap
{
protected:
   // member variables
   HBITMAP m_hBitmap;
   int     m_iWidth;
   int     m_iHeight;

   // helper methods
   void Free( );

public:
   // constructor(s)/destructor
            Bitmap( );
            Bitmap(LPCTSTR szFileName);
            Bitmap(UINT uiResID, HINSTANCE hInstance);
            Bitmap(HWND hWnd, int iWidth, int iHeight, COLORREF crColor = RGB(0, 0, 0));
   virtual ~Bitmap( );

   // general methods
   BOOL Create(LPCTSTR szFileName);
   BOOL Create(UINT uiResID, HINSTANCE hInstance);
   BOOL Create(HWND hWnd, int iWidth, int iHeight, COLORREF crColor);

   void Draw(HDC hDC, int x, int y, BOOL bTrans = FALSE, COLORREF crTransColor = RGB(255, 0, 255));
   void DrawPart(HDC hDC, int x, int y, int xPart, int yPart, int wPart, int hPart,
                 BOOL bTrans = FALSE, COLORREF crTransColor = RGB(255, 0, 255));

   int  GetWidth( )  { return m_iWidth; };
   int  GetHeight( ) { return m_iHeight; };
};
