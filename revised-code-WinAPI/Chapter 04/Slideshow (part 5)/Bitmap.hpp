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
            Bitmap( PCWSTR fileName );
            Bitmap( UINT resID );
            Bitmap( HDC dc, int width, int height, COLORREF color = RGB( 0, 0, 0 ) );
   virtual ~Bitmap( );

   BOOL Create( PCWSTR fileName );
   BOOL Create( UINT resID );
   BOOL Create( HDC dc, int width, int height, COLORREF color );

   void Draw( HDC dc, int x, int y );

   constexpr int GetWidth( ) const  { return m_width; };
   constexpr int GetHeight( ) const { return m_height; };
};
