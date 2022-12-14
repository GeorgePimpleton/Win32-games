#pragma once

#include <windows.h>

class Bitmap
{
public:
            Bitmap();
            Bitmap(PCWSTR szFileName);
            Bitmap(UINT uiResID, HINSTANCE hInstance);
            Bitmap(HWND hWnd, LONG iWidth, LONG iHeight, COLORREF crColor = RGB(0, 0, 0));
   virtual ~Bitmap();

public:
   BOOL Create(PCWSTR szFileName);
   BOOL Create(UINT uiResID, HINSTANCE hInstance);
   BOOL Create(HWND hWnd, LONG iWidth, LONG iHeight, COLORREF crColor);

public:
   void Draw(HDC hDC, int x, int y) const;

public:
   LONG GetWidth() const;
   LONG GetHeight() const;

protected:
   void Free();

protected:
   HBITMAP m_hBitmap;
   LONG    m_iWidth;
   LONG    m_iHeight;
};

inline LONG Bitmap::GetWidth() const
{
   return m_iWidth;
}

inline LONG Bitmap::GetHeight() const
{
   return m_iHeight;
}
