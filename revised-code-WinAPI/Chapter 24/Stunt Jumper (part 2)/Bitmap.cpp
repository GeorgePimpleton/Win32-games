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

Bitmap::Bitmap(HDC dc, int width, int height, COLORREF crColor)
   : m_bitmap(NULL), m_width(0), m_height(0)
{
   Create(dc, width, height, crColor);
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

BOOL Bitmap::Create(HDC dc, PCWSTR fileName)
{
   Free( );

   m_bitmap = (HBITMAP) LoadImageW(NULL, fileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

   if ( NULL == m_bitmap )
   {
      Free( );

      return FALSE;
   }

   BITMAP bitmap = { };

   GetObjectW(m_bitmap, sizeof(BITMAP), &bitmap);

   m_width = bitmap.bmWidth;
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

   BITMAP bitmap = { };

   GetObjectW(m_bitmap, sizeof(BITMAP), &bitmap);

   m_width = bitmap.bmWidth;
   m_height = bitmap.bmHeight;

   return TRUE;
}

BOOL Bitmap::Create(HDC dc, int width, int height, COLORREF crColor)
{
   m_bitmap = CreateCompatibleBitmap(dc, width, height);

   if ( m_bitmap == NULL )
   {
      return FALSE;
   }

   m_width = width;
   m_height = height;

   HDC hMemDC = CreateCompatibleDC(dc);
   HBRUSH hBrush = CreateSolidBrush(crColor);
   HBITMAP hOldBitmap = (HBITMAP) SelectObject(hMemDC, m_bitmap);
   RECT rcBitmap = { 0, 0, m_width, m_height };

   FillRect(hMemDC, &rcBitmap, hBrush);

   SelectObject(hMemDC, hOldBitmap);
   DeleteDC(hMemDC);
   DeleteObject(hBrush);

   return TRUE;
}

void Bitmap::Draw(HDC dc, int x, int y, BOOL bTrans, COLORREF crTransColor)
{
   DrawPart(dc, x, y, 0, 0, GetWidth( ), GetHeight( ), bTrans, crTransColor);
}

void Bitmap::DrawPart(HDC dc, int x, int y, int xPart, int yPart,
                      int wPart, int hPart, BOOL bTrans, COLORREF crTransColor)
{
   if ( m_bitmap != NULL )
   {
      HDC hMemDC = CreateCompatibleDC(dc);
      HBITMAP hOldBitmap = (HBITMAP) SelectObject(hMemDC, m_bitmap);

      if ( bTrans )
      {
         TransparentBlt(dc, x, y, wPart, hPart, hMemDC, xPart, yPart,
                        wPart, hPart, crTransColor);
      }
      else
      {
         BitBlt(dc, x, y, wPart, hPart, hMemDC, xPart, yPart, SRCCOPY);
      }

      SelectObject(hMemDC, hOldBitmap);
      DeleteDC(hMemDC);
   }
}