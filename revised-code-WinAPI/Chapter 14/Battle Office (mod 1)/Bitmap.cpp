#pragma comment(lib, "msimg32.lib")

#include "Bitmap.hpp"

Bitmap::Bitmap( )
   : m_bitmap(NULL), m_width(0), m_height(0)
{ }

Bitmap::Bitmap(PCWSTR fileName)
   : m_bitmap(NULL), m_width(0), m_height(0)
{
   Create(fileName);
}

Bitmap::Bitmap(UINT resID, HINSTANCE inst)
   : m_bitmap(NULL), m_width(0), m_height(0)
{
   Create(resID, inst);
}

Bitmap::Bitmap(HWND wnd, LONG iWidth, LONG height, COLORREF color)
   : m_bitmap(NULL), m_width(0), m_height(0)
{
   Create(wnd, iWidth, height, color);
}

Bitmap::~Bitmap( )
{
   Free( );
}

void Bitmap::Free( )
{
   if ( m_bitmap != NULL )
   {
      DeleteObject(m_bitmap);

      m_bitmap = NULL;
   }
}

BOOL Bitmap::Create(PCWSTR fileName)
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

BOOL Bitmap::Create(UINT resID, HINSTANCE inst)
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

BOOL Bitmap::Create(HWND wnd, LONG width, LONG height, COLORREF color)
{
   HDC dc = GetDC(wnd);

   m_bitmap = CreateCompatibleBitmap(dc, width, height);

   if ( NULL == m_bitmap )
   {
      return FALSE;
   }

   m_width  = width;
   m_height = height;

   HDC memDC = CreateCompatibleDC(dc);

   HBRUSH brush = CreateSolidBrush(color);

   HBITMAP oldBitmap = (HBITMAP) SelectObject(memDC, m_bitmap);

   RECT bitmap = { 0, 0, m_width, m_height };
   FillRect(memDC, &bitmap, brush);

   SelectObject(memDC, oldBitmap);
   DeleteDC(memDC);
   DeleteObject(brush);

   return TRUE;
}

void Bitmap::Draw(HDC dc, int x, int y, BOOL trans, COLORREF transColor)
{
   if ( m_bitmap != NULL )
   {
      HDC memDC = CreateCompatibleDC(dc);

      HBITMAP oldBitmap = (HBITMAP) SelectObject(memDC, m_bitmap);

      if ( trans )
      {
         TransparentBlt(dc, x, y, GetWidth( ), GetHeight( ), memDC, 0, 0, GetWidth( ), GetHeight( ), transColor);
      }
      else
      {
         BitBlt(dc, x, y, GetWidth( ), GetHeight( ), memDC, 0, 0, SRCCOPY);
      }

      SelectObject(memDC, oldBitmap);
      DeleteDC(memDC);
   }
}