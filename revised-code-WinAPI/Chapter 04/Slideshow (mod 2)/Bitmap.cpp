//-----------------------------------------------------------------
// Bitmap Object
// C++ Source - Bitmap.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Bitmap.h"

//-----------------------------------------------------------------
// Bitmap Constructor(s)/Destructor
//-----------------------------------------------------------------
Bitmap::Bitmap( )
   : m_hBitmap(NULL), m_iWidth(0), m_iHeight(0)
{ }

// Create a bitmap from a file
Bitmap::Bitmap(HDC hDC, PCTSTR szFileName)
   : m_hBitmap(NULL), m_iWidth(0), m_iHeight(0)
{
   Create(hDC, szFileName);
}

// Create a bitmap from a resource
Bitmap::Bitmap(HDC hDC, UINT uiResID, HINSTANCE hInstance)
   : m_hBitmap(NULL), m_iWidth(0), m_iHeight(0)
{
   Create(hDC, uiResID, hInstance);
}

// Create a blank bitmap from scratch
Bitmap::Bitmap(HDC hDC, int iWidth, int iHeight, COLORREF crColor)
   : m_hBitmap(NULL), m_iWidth(0), m_iHeight(0)
{
   Create(hDC, iWidth, iHeight, crColor);
}

Bitmap::~Bitmap( )
{
   Free( );
}

//-----------------------------------------------------------------
// Bitmap Helper Methods
//-----------------------------------------------------------------
void Bitmap::Free( )
{
   // Delete the bitmap graphics object
   if ( m_hBitmap != NULL )
   {
      DeleteObject(m_hBitmap);
      m_hBitmap = NULL;
   }
}

//-----------------------------------------------------------------
// Bitmap General Methods
//-----------------------------------------------------------------
BOOL Bitmap::Create(HDC hDC, PCTSTR szFileName)
{
   // free any previous bitmap info
   Free( );

   // load the bitmap file
   m_hBitmap = (HBITMAP) LoadImageW(NULL, szFileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

   if ( m_hBitmap == NULL )
   {
      // something went wrong, so cleanup everything
      Free( );

      return FALSE;
   }

   BITMAP bitmap;

   GetObjectW(m_hBitmap, sizeof(BITMAP), &bitmap);

   // store the width and height of the bitmap
   m_iWidth = bitmap.bmWidth;
   m_iHeight = bitmap.bmHeight;

   return TRUE;
}

BOOL Bitmap::Create(HDC hDC, UINT uiResID, HINSTANCE hInstance)
{
   // free any previous bitmap info
   Free( );

   // load the bitmap resource
   m_hBitmap = (HBITMAP) LoadImageW(GetModuleHandleW(NULL), MAKEINTRESOURCEW(uiResID), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

   if ( m_hBitmap == NULL )
   {
      // something went wrong, so cleanup everything
      Free( );

      return FALSE;
   }

   BITMAP bitmap;

   GetObjectW(m_hBitmap, sizeof(BITMAP), &bitmap);

   // store the width and height of the bitmap
   m_iWidth = bitmap.bmWidth;
   m_iHeight = bitmap.bmHeight;

   return TRUE;
}

BOOL Bitmap::Create(HDC hDC, int iWidth, int iHeight, COLORREF crColor)
{
   // Create a blank bitmap
   m_hBitmap = CreateCompatibleBitmap(hDC, iWidth, iHeight);
   if ( m_hBitmap == NULL )
      return FALSE;

   // Set the width and height
   m_iWidth = iWidth;
   m_iHeight = iHeight;

   // Create a memory device context to draw on the bitmap
   HDC hMemDC = CreateCompatibleDC(hDC);

   // Create a solid brush to fill the bitmap
   HBRUSH hBrush = CreateSolidBrush(crColor);

   // Select the bitmap into the device context
   HBITMAP hOldBitmap = (HBITMAP) SelectObject(hMemDC, m_hBitmap);

   // Fill the bitmap with a solid color
   RECT rcBitmap = { 0, 0, m_iWidth, m_iHeight };
   FillRect(hMemDC, &rcBitmap, hBrush);

   // Cleanup
   SelectObject(hMemDC, hOldBitmap);
   DeleteDC(hMemDC);
   DeleteObject(hBrush);

   return TRUE;
}

void Bitmap::Draw(HDC hDC, int x, int y)
{
   if ( m_hBitmap != NULL )
   {
      // Create a memory device context for the bitmap
      HDC hMemDC = CreateCompatibleDC(hDC);

      // Select the bitmap into the device context
      HBITMAP hOldBitmap = (HBITMAP) SelectObject(hMemDC, m_hBitmap);

      // Draw the bitmap to the destination device context
      BitBlt(hDC, x, y, GetWidth( ), GetHeight( ), hMemDC, 0, 0, SRCCOPY);

      // Restore and delete the memory device context
      SelectObject(hMemDC, hOldBitmap);
      DeleteDC(hMemDC);
   }
}