// "Talk to me like I'm a 3 year old!" Programming Lessons - Windows Games

// Bitmap.cpp - Bitmap Source

#pragma comment(lib, "msimg32.lib")

#include "Bitmap.hpp"

Bitmap::Bitmap()
   : m_hBitmap(NULL), m_iWidth(0), m_iHeight(0)
{ }

Bitmap::Bitmap(PCWSTR szFileName)
   : m_hBitmap(NULL), m_iWidth(0), m_iHeight(0)
{
   Create(szFileName);
}

Bitmap::Bitmap(UINT uiResID, HINSTANCE hInstance)
   : m_hBitmap(NULL), m_iWidth(0), m_iHeight(0)
{
   Create(uiResID, hInstance);
}

Bitmap::Bitmap(HWND hWindow, LONG iWidth, LONG iHeight, COLORREF crColor)
   : m_hBitmap(NULL), m_iWidth(0), m_iHeight(0)
{
   Create(hWindow, iWidth, iHeight, crColor);
}

Bitmap::~Bitmap()
{
   Free();
}

void Bitmap::Free()
{
   if (m_hBitmap != NULL)
   {
      DeleteObject(m_hBitmap);

      m_hBitmap = NULL;
   }
}

BOOL Bitmap::Create(PCWSTR szFileName)
{
   Free();

   m_hBitmap = (HBITMAP) LoadImageW(NULL, szFileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

   if (m_hBitmap == NULL)
   {
      Free();

      return FALSE;
   }

   BITMAP bitmap;

   GetObjectW(m_hBitmap, sizeof(BITMAP), &bitmap);

   m_iWidth  = bitmap.bmWidth;
   m_iHeight = bitmap.bmHeight;

   return TRUE;
}

BOOL Bitmap::Create(UINT uiResID, HINSTANCE hInstance)
{
   Free();

   m_hBitmap = (HBITMAP) LoadImageW(GetModuleHandleW(NULL), MAKEINTRESOURCEW(uiResID), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

   if (m_hBitmap == NULL)
   {
      Free();

      return FALSE;
   }

   BITMAP bitmap;

   GetObjectW(m_hBitmap, sizeof(BITMAP), &bitmap);

   m_iWidth  = bitmap.bmWidth;
   m_iHeight = bitmap.bmHeight;

   return TRUE;
}

BOOL Bitmap::Create(HWND hWindow, LONG iWidth, LONG iHeight, COLORREF crColor)
{
   HDC hDC = GetDC(hWindow);

   m_hBitmap = CreateCompatibleBitmap(hDC, iWidth, iHeight);

   if (m_hBitmap == NULL)
   {
      return FALSE;
   }

   m_iWidth  = iWidth;
   m_iHeight = iHeight;

   HDC hMemDC = CreateCompatibleDC(hDC);

   HBRUSH hBrush = CreateSolidBrush(crColor);

   HBITMAP hOldBitmap = (HBITMAP) SelectObject(hMemDC, m_hBitmap);

   RECT rcBitmap = { 0, 0, m_iWidth, m_iHeight };
   FillRect(hMemDC, &rcBitmap, hBrush);

   SelectObject(hMemDC, hOldBitmap);
   DeleteDC(hMemDC);
   DeleteObject(hBrush);

   return TRUE;
}

void Bitmap::Draw(HDC hDC, int x, int y, BOOL bTrans, COLORREF crTransColor) const
{
   DrawPart(hDC, x, y, 0, 0, GetWidth(), GetHeight(), bTrans, crTransColor);
}

void Bitmap::DrawPart(HDC hDC, int x, int y, int xPart, int yPart, int wPart, int hPart,
                       BOOL bTrans, COLORREF crTransColor) const
{
   if (m_hBitmap != NULL)
   {
   // create a memory device context for the bitmap
   HDC hMemDC = CreateCompatibleDC(hDC);

   // select the bitmap into the device context
   HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, m_hBitmap);

   // draw the bitmap to the destination device context
   if (bTrans)
   {
      TransparentBlt(hDC, x, y, wPart, hPart, hMemDC, xPart, yPart, wPart, hPart, crTransColor);
   }
   else
   {
      BitBlt(hDC, x, y, wPart, hPart, hMemDC, xPart, yPart, SRCCOPY);
   }

   // restore and delete the memory device context
   SelectObject(hMemDC, hOldBitmap);
   DeleteDC(hMemDC);
   }
}