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
            Bitmap( HDC, LONG, LONG, COLORREF = RGB( 0, 0, 0 ) );
   virtual ~Bitmap( );

   BOOL Create( PCWSTR );
   BOOL Create( UINT );
   BOOL Create( HDC, LONG, LONG, COLORREF );

   virtual void Draw( HDC, int, int, BOOL = FALSE, COLORREF = RGB( 255, 0, 255 ) ) const;
   void DrawPart( HDC, int, int, int, int,
                  int, int, BOOL = FALSE, COLORREF = RGB( 255, 0, 255 ) ) const;

   constexpr LONG GetWidth( )  const { return m_width; };
   constexpr LONG GetHeight( ) const { return m_height; };
};
