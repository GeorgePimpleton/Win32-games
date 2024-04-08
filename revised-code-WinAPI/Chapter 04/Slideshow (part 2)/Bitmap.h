#pragma once

#include <windows.h>

class Bitmap
{
protected:
   HBITMAP m_hBitmap;
   int     m_iWidth, m_iHeight;

   void Free( );

public:
            Bitmap( );
            Bitmap( PCTSTR szFileName );
            Bitmap( UINT uiResID );
            Bitmap( HDC hDC, int iWidth, int iHeight, COLORREF crColor = RGB( 0, 0, 0 ) );
   virtual ~Bitmap( );

   BOOL Create( PCTSTR szFileName );
   BOOL Create( UINT uiResID );
   BOOL Create( HDC hDC, int iWidth, int iHeight, COLORREF crColor );

   void Draw( HDC hDC, int x, int y );

   int GetWidth( )  { return m_iWidth; };
   int GetHeight( ) { return m_iHeight; };
};
