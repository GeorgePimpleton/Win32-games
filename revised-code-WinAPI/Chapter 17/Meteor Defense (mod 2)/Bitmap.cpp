#pragma comment(lib, "msimg32.lib")

#include "Bitmap.hpp"

Bitmap::Bitmap( )
   : m_bitmap(NULL), m_width(0), m_height(0)
{ }

Bitmap::Bitmap(HDC dc, PCWSTR fileName)
   : m_bitmap(NULL), m_width(0), m_height(0)
{
   Create(dc, fileName);
}

Bitmap::Bitmap(HDC dc, UINT resID, HINSTANCE inst)
   : m_bitmap(NULL), m_width(0), m_height(0)
{
   Create(dc, resID, inst);
}

Bitmap::Bitmap(HDC dc, int width, int height, COLORREF color)
   : m_bitmap(NULL), m_width(0), m_height(0)
{
   Create(dc, width, height, color);
}

Bitmap::~Bitmap( )
{
   Free( );
}

void Bitmap::Free( )
{
   if ( NULL != m_bitmap )
   {
      DeleteObject(m_bitmap);
      m_bitmap = NULL;
   }
}

BOOL Bitmap::Create(HDC dc, PCWSTR fileName)
{
   Free( );

   m_bitmap = (HBITMAP) LoadImageW(NULL, fileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

   if ( NULL == m_bitmap )
   {
      Free( );

      return FALSE;
   }

   BITMAP bitmap;

   GetObjectW(m_bitmap, sizeof(BITMAP), &bitmap);

   m_width  = bitmap.bmWidth;
   m_height = bitmap.bmHeight;

   return TRUE;
}

BOOL Bitmap::Create(HDC dc, UINT resID, HINSTANCE inst)
{
   Free( );

   m_bitmap = (HBITMAP) LoadImageW(GetModuleHandleW(NULL), MAKEINTRESOURCEW(resID), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

   if ( NULL == m_bitmap )
   {
      Free( );

      return FALSE;
   }

   BITMAP bitmap;

   GetObjectW(m_bitmap, sizeof(BITMAP), &bitmap);

   m_width  = bitmap.bmWidth;
   m_height = bitmap.bmHeight;

   return TRUE;
}

BOOL Bitmap::Create(HDC dc, int width, int height, COLORREF color)
{
   m_bitmap = CreateCompatibleBitmap(dc, width, height);

   if ( NULL == m_bitmap )
   {
      return FALSE;
   }

   m_width  = width;
   m_height = height;

   HDC     memDC     = CreateCompatibleDC(dc);
   HBRUSH  brush     = CreateSolidBrush(color);
   HBITMAP oldBitmap = (HBITMAP) SelectObject(memDC, m_bitmap);
   RECT    bitmap    = { 0, 0, m_width, m_height };

   FillRect(memDC, &bitmap, brush);

   SelectObject(memDC, oldBitmap);
   DeleteDC(memDC);
   DeleteObject(brush);

   return TRUE;
}

void Bitmap::Draw(HDC dc, int x, int y, BOOL trans, COLORREF transColor)
{
   DrawPart(dc, x, y, 0, 0, GetWidth( ), GetHeight( ), trans, transColor);
}

void Bitmap::DrawPart(HDC dc, int x, int y, int xPart, int yPart,
                      int wPart, int hPart, BOOL trans, COLORREF transColor)
{
   if ( NULL != m_bitmap )
   {
      HDC     memDC     = CreateCompatibleDC(dc);
      HBITMAP oldBitmap = (HBITMAP) SelectObject(memDC, m_bitmap);

      if ( trans )
      {
         TransparentBlt(dc, x, y, wPart, hPart, memDC, xPart, yPart,
                        wPart, hPart, transColor);
      }
      else
      {
         BitBlt(dc, x, y, wPart, hPart, memDC, xPart, yPart, SRCCOPY);
      }

      SelectObject(memDC, oldBitmap);
      DeleteDC(memDC);
   }
}