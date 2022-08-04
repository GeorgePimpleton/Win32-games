// "Talk to me like I'm a 3 year old!" Programming Lessons - Windows Games

// Bitmap.cpp - Bitmap Source

#include "Bitmap.hpp"

// default bitmap constructor
Bitmap::Bitmap() :
   m_hBitmap(NULL), m_iWidth(0), m_iHeight(0)
{
}

// bitmap filename constructor
Bitmap::Bitmap(PCWSTR szFileName) :
   m_hBitmap(NULL), m_iWidth(0), m_iHeight(0)
{
   Create(szFileName);
}

// bitmap resource constructor
Bitmap::Bitmap(UINT uiResID, HINSTANCE hInstance) :
   m_hBitmap(NULL), m_iWidth(0), m_iHeight(0)
{
   Create(uiResID, hInstance);
}

// bitmap blank constructor
Bitmap::Bitmap(HWND hWindow, LONG iWidth, LONG iHeight, COLORREF crColor)
   : m_hBitmap(NULL), m_iWidth(0), m_iHeight(0)
{
   Create(hWindow, iWidth, iHeight, crColor);
}

// bitmap destructor
Bitmap::~Bitmap()
{
   Free();
}

// creates a bitmap from a file
BOOL Bitmap::Create(PCWSTR szFileName)
{
   // free any previous bitmap info
   Free();

   // load the bitmap file
   m_hBitmap = (HBITMAP) LoadImageW(NULL, szFileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

   if (m_hBitmap == NULL)
   {
      // something went wrong, so cleanup everything
      Free();

      return FALSE;
   }

   BITMAP bitmap;

   GetObjectW(m_hBitmap, sizeof(BITMAP), &bitmap);

   // store the width and height of the bitmap
   m_iWidth  = bitmap.bmWidth;
   m_iHeight = bitmap.bmHeight;

   return TRUE;
}

// creates a bitmap from a resource
BOOL Bitmap::Create(UINT uiResID, HINSTANCE hInstance)
{
   // free any previous bitmap info
   Free();

   // load the bitmap resource
   m_hBitmap = (HBITMAP) LoadImageW(GetModuleHandleW(NULL), MAKEINTRESOURCEW(uiResID), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

   if (m_hBitmap == NULL)
   {
      // something went wrong, so cleanup everything
      Free();

      return FALSE;
   }

   BITMAP bitmap;

   GetObjectW(m_hBitmap, sizeof(BITMAP), &bitmap);

   // store the width and height of the bitmap
   m_iWidth  = bitmap.bmWidth;
   m_iHeight = bitmap.bmHeight;

   return TRUE;
}

// creates a blank bitmap with the included color
BOOL Bitmap::Create(HWND hWindow, LONG iWidth, LONG iHeight, COLORREF crColor)
{
   HDC hDC = GetDC(hWindow);

   // create a blank bitmap
   m_hBitmap = CreateCompatibleBitmap(hDC, iWidth, iHeight);

   if (m_hBitmap == NULL)
   {
      return FALSE;
   }

   // set the width and height
   m_iWidth  = iWidth;
   m_iHeight = iHeight;

   // create a memory device context to draw on the bitmap
   HDC hMemDC = CreateCompatibleDC(hDC);

   // create a solid brush to fill the bitmap
   HBRUSH hBrush = CreateSolidBrush(crColor);

   // select the bitmap into the device context
   HBITMAP hOldBitmap = (HBITMAP) SelectObject(hMemDC, m_hBitmap);

   // fill the bitmap with a solid color
   RECT rcBitmap = { 0, 0, m_iWidth, m_iHeight };
   FillRect(hMemDC, &rcBitmap, hBrush);

   // cleanup
   SelectObject(hMemDC, hOldBitmap);
   DeleteDC(hMemDC);
   DeleteObject(hBrush);

   return TRUE;
}

// draws a bitmap
void Bitmap::Draw(HDC hDC, int x, int y) const
{
   if (m_hBitmap != NULL)
   {
      // create a memory device context for the bitmap
      HDC hMemDC = CreateCompatibleDC(hDC);

      // select the bitmap into the device context
      HBITMAP hOldBitmap = (HBITMAP) SelectObject(hMemDC, m_hBitmap);

      // draw the bitmap to the destination device context
      BitBlt(hDC, x, y, GetWidth(), GetHeight(), hMemDC, 0, 0, SRCCOPY);

      // restore and delete the memory device context
      SelectObject(hMemDC, hOldBitmap);
      DeleteDC(hMemDC);
   }
}

// deletes a bitmap
void Bitmap::Free()
{
   // delete the bitmap graphics object
   if (m_hBitmap != NULL)
   {
       DeleteObject(m_hBitmap);
       m_hBitmap = NULL;
   }
}