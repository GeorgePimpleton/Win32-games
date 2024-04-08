#include "Bitmap.h"

Bitmap::Bitmap( )
   : m_hBitmap( NULL ), m_iWidth( 0 ), m_iHeight( 0 )
{ }

Bitmap::Bitmap( PCTSTR szFileName )
   : m_hBitmap( NULL ), m_iWidth( 0 ), m_iHeight( 0 )
{
   Create( szFileName );
}

Bitmap::Bitmap( UINT uiResID )
   : m_hBitmap( NULL ), m_iWidth( 0 ), m_iHeight( 0 )
{
   Create( uiResID );
}

Bitmap::Bitmap( HDC hDC, int iWidth, int iHeight, COLORREF crColor )
   : m_hBitmap( NULL ), m_iWidth( 0 ), m_iHeight( 0 )
{
   Create( hDC, iWidth, iHeight, crColor );
}

Bitmap::~Bitmap( )
{
   Free( );
}

void Bitmap::Free( )
{
   if ( m_hBitmap != NULL )
   {
      DeleteObject( m_hBitmap );
      m_hBitmap = NULL;
   }
}

BOOL Bitmap::Create( PCTSTR szFileName )
{
   Free( );

   m_hBitmap = ( HBITMAP ) LoadImageW( NULL, szFileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE );

   if ( m_hBitmap == NULL )
   {
      Free( );

      return FALSE;
   }

   BITMAP bitmap;

   GetObjectW( m_hBitmap, sizeof( BITMAP ), &bitmap );

   m_iWidth  = bitmap.bmWidth;
   m_iHeight = bitmap.bmHeight;

   return TRUE;
}

BOOL Bitmap::Create( UINT uiResID )
{
   Free( );

   m_hBitmap = ( HBITMAP ) LoadImageW( GetModuleHandleW( NULL ), MAKEINTRESOURCEW( uiResID ), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION );

   if ( m_hBitmap == NULL )
   {
      Free( );

      return FALSE;
   }

   BITMAP bitmap;

   GetObjectW( m_hBitmap, sizeof( BITMAP ), &bitmap );

   m_iWidth  = bitmap.bmWidth;
   m_iHeight = bitmap.bmHeight;

   return TRUE;
}

BOOL Bitmap::Create( HDC hDC, int iWidth, int iHeight, COLORREF crColor )
{
   m_hBitmap = CreateCompatibleBitmap( hDC, iWidth, iHeight );
   if ( m_hBitmap == NULL )
      return FALSE;

   m_iWidth  = iWidth;
   m_iHeight = iHeight;

   HDC hMemDC = CreateCompatibleDC( hDC );

   HBRUSH hBrush = CreateSolidBrush( crColor );

   HBITMAP hOldBitmap = ( HBITMAP ) SelectObject( hMemDC, m_hBitmap );

   RECT rcBitmap = { 0, 0, m_iWidth, m_iHeight };
   FillRect( hMemDC, &rcBitmap, hBrush );

   SelectObject( hMemDC, hOldBitmap );
   DeleteDC( hMemDC );
   DeleteObject( hBrush );

   return TRUE;
}

void Bitmap::Draw( HDC hDC, int x, int y )
{
   if ( m_hBitmap != NULL )
   {
      HDC hMemDC = CreateCompatibleDC( hDC );

      HBITMAP hOldBitmap = ( HBITMAP ) SelectObject( hMemDC, m_hBitmap );

      BitBlt( hDC, x, y, GetWidth( ), GetHeight( ), hMemDC, 0, 0, SRCCOPY );

      SelectObject( hMemDC, hOldBitmap );
      DeleteDC( hMemDC );
   }
}
