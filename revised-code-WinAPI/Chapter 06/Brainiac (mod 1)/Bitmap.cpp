#pragma comment(lib, "msimg32.lib")

#include "Bitmap.hpp"

Bitmap::Bitmap()
   : m_bitmap(NULL), m_width(0), m_height(0)
{ }

Bitmap::Bitmap(PCWSTR fileName)
   : m_bitmap(NULL), m_width(0), m_height(0)
{
   Create(fileName);
}

Bitmap::Bitmap(UINT resID, HINSTANCE hInst)
   : m_bitmap(NULL), m_width(0), m_height(0)
{
   Create(resID, hInst);
}

Bitmap::Bitmap(HWND hwnd, LONG width, LONG height, COLORREF color)
   : m_bitmap(NULL), m_width(0), m_height(0)
{
   Create(hwnd, width, height, color);
}

Bitmap::~Bitmap()
{
   Free();
}

void Bitmap::Free()
{
   if (m_bitmap != NULL)
   {
      DeleteObject(m_bitmap);

      m_bitmap = NULL;
   }
}

BOOL Bitmap::Create(PCWSTR fileName)
{
   Free( );

   m_bitmap = (HBITMAP) LoadImageW(NULL, fileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

   if (m_bitmap == NULL)
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

BOOL Bitmap::Create(UINT resID, HINSTANCE hInst)
{
   Free( );

   m_bitmap = (HBITMAP) LoadImageW(GetModuleHandleW(NULL), MAKEINTRESOURCEW(resID), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

   if (m_bitmap == NULL)
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

BOOL Bitmap::Create(HWND hwnd, LONG width, LONG height, COLORREF color)
{
   HDC hDC = GetDC(hwnd);

   m_bitmap = CreateCompatibleBitmap(hDC, width, height);

   if (m_bitmap == NULL)
   {
      return FALSE;
   }

   m_width  = width;
   m_height = height;

   HDC     memDC     { CreateCompatibleDC(hDC) };
   HBRUSH  brush     { CreateSolidBrush(color) };
   HBITMAP oldBitmap { (HBITMAP) SelectObject(memDC, m_bitmap) };
   RECT    bitmap    { 0, 0, m_width, m_height };

   FillRect(memDC, &bitmap, brush);

   SelectObject(memDC, oldBitmap);
   DeleteDC(memDC);
   DeleteObject(brush);

   return TRUE;
}

void Bitmap::Draw(HDC hDC, int x, int y, BOOL trans, COLORREF transColor)
{
   if (m_bitmap != NULL)
   {
      HDC     memDC     { CreateCompatibleDC(hDC) };
      HBITMAP oldBitmap { (HBITMAP) SelectObject(memDC, m_bitmap) };

      if (trans)
      {
         TransparentBlt(hDC, x, y, GetWidth(), GetHeight(), memDC, 0, 0, GetWidth(), GetHeight(), transColor);
      }
      else
      {
         BitBlt(hDC, x, y, GetWidth(), GetHeight(), memDC, 0, 0, SRCCOPY);
      }

      SelectObject(memDC, oldBitmap);
      DeleteDC(memDC);
   }
}