#include "Bitmap.hpp"

Bitmap::Bitmap() :
   m_hBitmap(nullptr), m_iWidth(0), m_iHeight(0)
{
}

Bitmap::Bitmap(PCWSTR szFileName) :
   m_hBitmap(nullptr), m_iWidth(0), m_iHeight(0)
{
   Create(szFileName);
}

Bitmap::Bitmap(UINT uiResID, HINSTANCE hInstance) :
   m_hBitmap(nullptr), m_iWidth(0), m_iHeight(0)
{
   Create(uiResID, hInstance);
}

Bitmap::Bitmap(HWND hWindow, LONG iWidth, LONG iHeight, COLORREF crColor)
   : m_hBitmap(nullptr), m_iWidth(0), m_iHeight(0)
{
   Create(hWindow, iWidth, iHeight, crColor);
}

Bitmap::~Bitmap()
{
   Free();
}

BOOL Bitmap::Create(PCWSTR szFileName)
{
   Free();

   m_hBitmap = (HBITMAP) LoadImageW(nullptr, szFileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

   if (m_hBitmap == nullptr)
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

   m_hBitmap = (HBITMAP) LoadImageW(GetModuleHandleW(nullptr), MAKEINTRESOURCEW(uiResID), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

   if (m_hBitmap == nullptr)
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

   if (m_hBitmap == nullptr)
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

void Bitmap::Draw(HDC hDC, int x, int y) const
{
   if (m_hBitmap != nullptr)
   {
      HDC hMemDC = CreateCompatibleDC(hDC);

      HBITMAP hOldBitmap = (HBITMAP) SelectObject(hMemDC, m_hBitmap);

      BitBlt(hDC, x, y, GetWidth(), GetHeight(), hMemDC, 0, 0, SRCCOPY);

      SelectObject(hMemDC, hOldBitmap);
      DeleteDC(hMemDC);
   }
}

void Bitmap::Free()
{
   if (m_hBitmap != nullptr)
   {
       DeleteObject(m_hBitmap);
       m_hBitmap = nullptr;
   }
}