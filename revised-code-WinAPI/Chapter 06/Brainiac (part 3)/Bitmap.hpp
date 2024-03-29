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
            Bitmap( PCWSTR );
            Bitmap( UINT );
            Bitmap( HWND, LONG, LONG, COLORREF = RGB( 0, 0, 0 ) );
   virtual ~Bitmap( );

public:
   BOOL Create( PCWSTR );
   BOOL Create( UINT );
   BOOL Create( HWND, LONG, LONG, COLORREF );

public:
   void Draw( HDC, int, int, BOOL = FALSE, COLORREF = RGB( 255, 0, 255 ) );

public:
   constexpr LONG GetWidth( ) const;
   constexpr LONG GetHeight( )const;

protected:
   void Free( );
};

inline constexpr LONG Bitmap::GetWidth( ) const  { return m_width; };
inline constexpr LONG Bitmap::GetHeight( ) const { return m_height; };
