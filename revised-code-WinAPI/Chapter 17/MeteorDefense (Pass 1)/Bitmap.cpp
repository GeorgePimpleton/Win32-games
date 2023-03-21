//-----------------------------------------------------------------
// Bitmap Object
// C++ Source - Bitmap.cpp
//-----------------------------------------------------------------

#include "Bitmap.hpp"

#pragma comment (lib, "msimg32.lib")

//-----------------------------------------------------------------
// Bitmap constructor(s)/destructor
//-----------------------------------------------------------------
Bitmap::Bitmap( )
   : m_hBitmap(NULL), m_iWidth(0), m_iHeight(0)
{ }

// create a bitmap from a file
Bitmap::Bitmap(LPCTSTR szFileName)
   : m_hBitmap(NULL), m_iWidth(0), m_iHeight(0)
{
   Create(szFileName);
}

// create a bitmap from a resource
Bitmap::Bitmap(UINT uiResID, HINSTANCE hInstance)
   : m_hBitmap(NULL), m_iWidth(0), m_iHeight(0)
{
   Create(uiResID, hInstance);
}

// create a blank bitmap from scratch
Bitmap::Bitmap(HWND hWindow, int iWidth, int iHeight, COLORREF crColor)
   : m_hBitmap(NULL), m_iWidth(0), m_iHeight(0)
{
   Create(hWindow, iWidth, iHeight, crColor);
}

Bitmap::~Bitmap( )
{
   Free( );
}

//-----------------------------------------------------------------
// Bitmap helper methods
//-----------------------------------------------------------------
void Bitmap::Free( )
{
   // delete the bitmap graphics object
   if ( m_hBitmap != NULL )
   {
      DeleteObject(m_hBitmap);

      m_hBitmap = NULL;
   }
}

//-----------------------------------------------------------------
// Bitmap general methods
//-----------------------------------------------------------------
BOOL Bitmap::Create(LPCTSTR szFileName)
{
   // free any previous bitmap info
   Free( );

   // load the bitmap file
   m_hBitmap = (HBITMAP) LoadImage(NULL, szFileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

   if ( m_hBitmap == NULL )
   {
      // something went wrong, so cleanup everything
      Free( );

      return FALSE;
   }

   BITMAP bitmap;

   GetObject(m_hBitmap, sizeof(BITMAP), &bitmap);

   // store the width and height of the bitmap
   m_iWidth = (int) bitmap.bmWidth;
   m_iHeight = (int) bitmap.bmHeight;

   return TRUE;
}

BOOL Bitmap::Create(UINT uiResID, HINSTANCE hInstance)
{
   // free any previous bitmap info
   Free( );

   // load the bitmap resource
   m_hBitmap = (HBITMAP) LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(uiResID), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

   if ( m_hBitmap == NULL )
   {
      // something went wrong, so cleanup everything
      Free( );

      return FALSE;
   }

   BITMAP bitmap;

   GetObject(m_hBitmap, sizeof(BITMAP), &bitmap);

   // store the width and height of the bitmap
   m_iWidth = (int) bitmap.bmWidth;
   m_iHeight = (int) bitmap.bmHeight;

   return TRUE;
}

BOOL Bitmap::Create(HWND hWindow, int iWidth, int iHeight, COLORREF crColor)
{
   HDC hDC = GetDC(hWindow);

   // create a blank bitmap
   m_hBitmap = CreateCompatibleBitmap(hDC, iWidth, iHeight);

   if ( m_hBitmap == NULL )
   {
      return FALSE;
   }

   // set the width and height
   m_iWidth = iWidth;
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

void Bitmap::Draw(HDC hDC, int x, int y, BOOL bTrans, COLORREF crTransColor)
{
   DrawPart(hDC, x, y, 0, 0, GetWidth( ), GetHeight( ), bTrans, crTransColor);
}

void Bitmap::DrawPart(HDC hDC, int x, int y, int xPart, int yPart, int wPart, int hPart,
                      BOOL bTrans, COLORREF crTransColor)
{
   if ( m_hBitmap != NULL )
   {
      // create a memory device context for the bitmap
      HDC hMemDC = CreateCompatibleDC(hDC);

      // select the bitmap into the device context
      HBITMAP hOldBitmap = (HBITMAP) SelectObject(hMemDC, m_hBitmap);

      // draw the bitmap to the destination device context
      if ( bTrans )
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