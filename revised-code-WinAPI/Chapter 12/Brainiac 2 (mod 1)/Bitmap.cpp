#pragma comment(lib, "msimg32.lib")

#include "Bitmap.h"

Bitmap::Bitmap( )
   : m_hBitmap(NULL), m_iWidth(0), m_iHeight(0)
{
}

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
   if ( m_hBitmap != NULL )
   {
      DeleteObject(m_hBitmap);
      m_hBitmap = NULL;
   }
}

BOOL Bitmap::Create(HDC hDC, PCTSTR szFileName)
{
   Free( );

   HANDLE hFile = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, NULL,
                             OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
   if ( hFile == INVALID_HANDLE_VALUE )
      return FALSE;

   BITMAPFILEHEADER  bmfHeader;
   DWORD             dwBytesRead;
   BOOL bOK = ReadFile(hFile, &bmfHeader, sizeof(BITMAPFILEHEADER),
                       &dwBytesRead, NULL);
   if ( (!bOK) || (dwBytesRead != sizeof(BITMAPFILEHEADER)) ||
       (bmfHeader.bfType != 0x4D42) )
   {
      CloseHandle(hFile);
      return FALSE;
   }

   BITMAPINFO* pBitmapInfo = (BITMAPINFO*) (new BITMAPINFO_256);
   if ( pBitmapInfo != NULL )
   {
      bOK = ReadFile(hFile, pBitmapInfo, sizeof(BITMAPINFOHEADER),
                     &dwBytesRead, NULL);
      if ( (!bOK) || (dwBytesRead != sizeof(BITMAPINFOHEADER)) )
      {
         CloseHandle(hFile);
         Free( );
         return FALSE;
      }

      m_iWidth = (int) pBitmapInfo->bmiHeader.biWidth;
      m_iHeight = (int) pBitmapInfo->bmiHeader.biHeight;

      if ( pBitmapInfo->bmiHeader.biSize != sizeof(BITMAPINFOHEADER) )
         SetFilePointer(hFile, pBitmapInfo->bmiHeader.biSize - sizeof
         (BITMAPINFOHEADER), NULL, FILE_CURRENT);

      bOK = ReadFile(hFile, pBitmapInfo->bmiColors,
                     pBitmapInfo->bmiHeader.biClrUsed * sizeof(RGBQUAD), &dwBytesRead,
                     NULL);

      PBYTE pBitmapBits;
      m_hBitmap = CreateDIBSection(hDC, pBitmapInfo, DIB_RGB_COLORS,
                                   (PVOID*) &pBitmapBits, NULL, 0);
      if ( (m_hBitmap != NULL) && (pBitmapBits != NULL) )
      {
         SetFilePointer(hFile, bmfHeader.bfOffBits, NULL, FILE_BEGIN);
         bOK = ReadFile(hFile, pBitmapBits, pBitmapInfo->bmiHeader.biSizeImage,
                        &dwBytesRead, NULL);
         if ( bOK )
            return TRUE;
      }
   }

   Free( );
   return FALSE;
}

BOOL Bitmap::Create(HDC hDC, UINT uiResID, HINSTANCE hInstance)
{
   Free( );

   HRSRC hResInfo = FindResource(hInstance, MAKEINTRESOURCE(uiResID), RT_BITMAP);
   if ( hResInfo == NULL )
      return FALSE;

   HGLOBAL hMemBitmap = LoadResource(hInstance, hResInfo);
   if ( hMemBitmap == NULL )
      return FALSE;

   PBYTE pBitmapImage = (BYTE*) LockResource(hMemBitmap);
   if ( pBitmapImage == NULL )
   {
      FreeResource(hMemBitmap);
      return FALSE;
   }

   BITMAPINFO* pBitmapInfo = (BITMAPINFO*) pBitmapImage;
   m_iWidth = (int) pBitmapInfo->bmiHeader.biWidth;
   m_iHeight = (int) pBitmapInfo->bmiHeader.biHeight;

   PBYTE pBitmapBits;
   m_hBitmap = CreateDIBSection(hDC, pBitmapInfo, DIB_RGB_COLORS,
                                (PVOID*) &pBitmapBits, NULL, 0);
   if ( (m_hBitmap != NULL) && (pBitmapBits != NULL) )
   {
      const PBYTE pTempBits = pBitmapImage + pBitmapInfo->bmiHeader.biSize +
         pBitmapInfo->bmiHeader.biClrUsed * sizeof(RGBQUAD);
      CopyMemory(pBitmapBits, pTempBits, pBitmapInfo->bmiHeader.biSizeImage);

      UnlockResource(hMemBitmap);
      FreeResource(hMemBitmap);
      return TRUE;
   }

   // Something went wrong, so cleanup everything
   UnlockResource(hMemBitmap);
   FreeResource(hMemBitmap);
   Free( );
   return FALSE;
}

BOOL Bitmap::Create(HDC hDC, int iWidth, int iHeight, COLORREF crColor)
{
   m_hBitmap = CreateCompatibleBitmap(hDC, iWidth, iHeight);
   if ( m_hBitmap == NULL )
      return FALSE;

   m_iWidth = iWidth;
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

void Bitmap::Draw(HDC hDC, int x, int y, BOOL bTrans, COLORREF crTransColor)
{
   if ( m_hBitmap != NULL )
   {
      HDC hMemDC = CreateCompatibleDC(hDC);

      HBITMAP hOldBitmap = (HBITMAP) SelectObject(hMemDC, m_hBitmap);

      if ( bTrans )
         TransparentBlt(hDC, x, y, GetWidth( ), GetHeight( ), hMemDC, 0, 0,
                        GetWidth( ), GetHeight( ), crTransColor);
      else
         BitBlt(hDC, x, y, GetWidth( ), GetHeight( ), hMemDC, 0, 0, SRCCOPY);

      // Restore and delete the memory device context
      SelectObject(hMemDC, hOldBitmap);
      DeleteDC(hMemDC);
   }
}