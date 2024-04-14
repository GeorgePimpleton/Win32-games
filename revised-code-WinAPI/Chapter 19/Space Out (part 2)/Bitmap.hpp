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
            Bitmap( PCWSTR );
            Bitmap( UINT );
            Bitmap( HDC, int, int, COLORREF = RGB( 0, 0, 0 ) );
   virtual ~Bitmap( );

   BOOL Create( PCWSTR );
   BOOL Create( UINT );
   BOOL Create( HDC, int, int, COLORREF );

   void Draw( HDC, int, int, BOOL = FALSE,
              COLORREF = RGB( 255, 0, 255 ) );
   void DrawPart( HDC, int, int, int, int,
                  int, int, BOOL = FALSE,
                  COLORREF = RGB( 255, 0, 255 ) );

   constexpr LONG GetWidth( ) const  { return m_width; };
   constexpr LONG GetHeight( ) const { return m_height; };
};
