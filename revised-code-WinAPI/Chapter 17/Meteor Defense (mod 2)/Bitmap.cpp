#pragma comment(lib, "msimg32.lib")

#include "Bitmap.hpp"

Bitmap::Bitmap( )
   : m_hBitmap(NULL), m_iWidth(0), m_iHeight(0)
{ }

Bitmap::Bitmap(HDC hDC, PCTSTR szFileName)
   : m_hBitmap(NULL), m_iWidth(0), m_iHeight(0)
{
   Create(hDC, szFileName);
}

Bitmap::Bitmap(HDC hDC, UINT uiResID, HINSTANCE hInstance)
   : m_hBitmap(NULL), m_iWidth(0), m_iHeight(0)
{
   Create(hDC, uiResID, hInstance);
}

Bitmap::Bitmap(HDC hDC, int iWidth, int iHeight, COLORREF crColor)
   : m_hBitmap(NULL), m_iWidth(0), m_iHeight(0)
{
   Create(hDC, iWidth, iHeight, crColor);
}

Bitmap::~Bitmap( )
{
   Free( );
}

void Bitmap::Free( )
{
   // Delete the bitmap graphics object
   if ( m_hBitmap != NULL )
   {
      DeleteObject(m_hBitmap);
      m_hBitmap = NULL;
   }
}

BOOL Bitmap::Create(HDC hDC, PCTSTR szFileName)
{
   Free( );

   m_hBitmap = (HBITMAP) LoadImageW(NULL, szFileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

   if ( NULL == m_hBitmap )
   {
      Free( );

      return FALSE;
   }

   BITMAP bitmap;

   GetObjectW(m_hBitmap, sizeof(BITMAP), &bitmap);

   m_iWidth  = bitmap.bmWidth;
   m_iHeight = bitmap.bmHeight;

   return TRUE;
}

BOOL Bitmap::Create(HDC hDC, UINT uiResID, HINSTANCE hInstance)
{
   Free( );

   m_hBitmap = (HBITMAP) LoadImageW(GetModuleHandleW(NULL), MAKEINTRESOURCEW(uiResID), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

   if ( NULL == m_hBitmap )
   {
      Free( );

      return FALSE;
   }

   BITMAP bitmap;

   GetObjectW(m_hBitmap, sizeof(BITMAP), &bitmap);

   m_iWidth  = bitmap.bmWidth;
   m_iHeight = bitmap.bmHeight;

   return TRUE;
}

BOOL Bitmap::Create(HDC hDC, int iWidth, int iHeight, COLORREF crColor)
{
   m_hBitmap = CreateCompatibleBitmap(hDC, iWidth, iHeight);

   if ( m_hBitmap == NULL )
   {
      return FALSE;
   }

   m_iWidth  = iWidth;
   m_iHeight = iHeight;

   HDC     hMemDC     = CreateCompatibleDC(hDC);
   HBRUSH  hBrush     = CreateSolidBrush(crColor);
   HBITMAP hOldBitmap = (HBITMAP) SelectObject(hMemDC, m_hBitmap);

   RECT rcBitmap = { 0, 0, m_iWidth, m_iHeight };

   FillRect(hMemDC, &rcBitmap, hBrush);

   SelectObject(hMemDC, hOldBitmap);
   DeleteDC(hMemDC);
   DeleteObject(hBrush);

   return TRUE;
}

void Bitmap::Draw(HDC hDC, int x, int y, BOOL bTrans, COLORREF crTransColor)
{
   DrawPart(hDC, x, y, 0, 0, GetWidth( ), GetHeight( ), bTrans, crTransColor);
}

void Bitmap::DrawPart(HDC hDC, int x, int y, int xPart, int yPart,
                      int wPart, int hPart, BOOL bTrans, COLORREF crTransColor)
{
   if ( m_hBitmap != NULL )
   {
      HDC hMemDC = CreateCompatibleDC(hDC);

      HBITMAP hOldBitmap = (HBITMAP) SelectObject(hMemDC, m_hBitmap);

      if ( bTrans )
      {
         TransparentBlt(hDC, x, y, wPart, hPart, hMemDC, xPart, yPart,
                        wPart, hPart, crTransColor);
      }
      else
      {
         BitBlt(hDC, x, y, wPart, hPart, hMemDC, xPart, yPart, SRCCOPY);
      }

      SelectObject(hMemDC, hOldBitmap);
      DeleteDC(hMemDC);
   }
}