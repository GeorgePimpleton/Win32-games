#include "Bitmap.hpp"

Bitmap::Bitmap( )
   : m_hBitmap(NULL), m_width(0), m_height(0)
{ }

Bitmap::Bitmap(PCWSTR fileName)
   : m_hBitmap(NULL), m_width(0), m_height(0)
{
   Create(fileName);
}

Bitmap::Bitmap(UINT resID, HINSTANCE inst)
   : m_hBitmap(NULL), m_width(0), m_height(0)
{
   Create(resID, inst);
}

Bitmap::Bitmap(HDC dc, int width, int height, COLORREF color)
   : m_hBitmap(NULL), m_width(0), m_height(0)
{
   Create(dc, width, height, color);
}

Bitmap::~Bitmap( )
{
   Free( );
}

void Bitmap::Free( )
{
   if ( m_hBitmap != NULL )
   {
      DeleteObject(m_hBitmap);
      m_hBitmap = NULL;
   }
}

BOOL Bitmap::Create(PCWSTR fileName)
{
   Free( );

   m_hBitmap = (HBITMAP) LoadImageW(NULL, fileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

   if ( m_hBitmap == NULL )
   {
      Free( );

      return FALSE;
   }

   BITMAP bitmap = { };

   GetObjectW(m_hBitmap, sizeof(BITMAP), &bitmap);

   m_width  = bitmap.bmWidth;
   m_height = bitmap.bmHeight;

   return TRUE;
}

BOOL Bitmap::Create(UINT resID, HINSTANCE inst)
{
   Free( );

   m_hBitmap = (HBITMAP) LoadImageW(GetModuleHandleW(NULL), MAKEINTRESOURCEW(resID), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

   if ( m_hBitmap == NULL )
   {
      Free( );

      return FALSE;
   }

   BITMAP bitmap = { };

   GetObjectW(m_hBitmap, sizeof(BITMAP), &bitmap);

   m_width  = bitmap.bmWidth;
   m_height = bitmap.bmHeight;

   return TRUE;
}

BOOL Bitmap::Create(HDC dc, int width, int height, COLORREF color)
{
   m_hBitmap = CreateCompatibleBitmap(dc, width, height);

   if ( m_hBitmap == NULL )
   {
      return FALSE;
   }

   m_width  = width;
   m_height = height;

   HDC     memDC     = CreateCompatibleDC(dc);
   HBRUSH  brush     = CreateSolidBrush(color);
   HBITMAP oldBitmap = (HBITMAP) SelectObject(memDC, m_hBitmap);
   RECT    bitmap    = { 0, 0, m_width, m_height };

   FillRect(memDC, &bitmap, brush);

   SelectObject(memDC, oldBitmap);
   DeleteDC(memDC);
   DeleteObject(brush);

   return TRUE;
}

void Bitmap::Draw(HDC dc, int x, int y)
{
   if ( m_hBitmap != NULL )
   {
      HDC     memDC     = CreateCompatibleDC(dc);
      HBITMAP oldBitmap = (HBITMAP) SelectObject(memDC, m_hBitmap);

      BitBlt(dc, x, y, GetWidth( ), GetHeight( ), memDC, 0, 0, SRCCOPY);

      SelectObject(memDC, oldBitmap);
      DeleteDC(memDC);
   }
}