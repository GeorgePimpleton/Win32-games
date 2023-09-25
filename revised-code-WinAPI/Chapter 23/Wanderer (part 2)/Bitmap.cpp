#pragma comment(lib, "msimg32.lib")

#include "Bitmap.hpp"

Bitmap::Bitmap( )
   : m_hBitmap(NULL), m_width(0), m_height(0)
{ }

Bitmap::Bitmap(HDC dc, PCWSTR szFileName)
   : m_hBitmap(NULL), m_width(0), m_height(0)
{
   Create(dc, szFileName);
}

Bitmap::Bitmap(HDC dc, UINT uiResID, HINSTANCE inst)
   : m_hBitmap(NULL), m_width(0), m_height(0)
{
   Create(dc, uiResID, inst);
}

Bitmap::Bitmap(HDC dc, int width, int height, COLORREF crColor)
   : m_hBitmap(NULL), m_width(0), m_height(0)
{
   Create(dc, width, height, crColor);
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

BOOL Bitmap::Create(HDC dc, PCWSTR szFileName)
{
   Free( );

   // Open the bitmap file
   HANDLE hFile = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, NULL,
                             OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
   if ( hFile == INVALID_HANDLE_VALUE )
      return FALSE;

   // Read the bitmap file header
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

   BITMAPINFO* pBitmapInfo = (new BITMAPINFO);
   if ( pBitmapInfo != NULL )
   {
      // Read the bitmap info header
      bOK = ReadFile(hFile, pBitmapInfo, sizeof(BITMAPINFOHEADER),
                     &dwBytesRead, NULL);
      if ( (!bOK) || (dwBytesRead != sizeof(BITMAPINFOHEADER)) )
      {
         CloseHandle(hFile);
         Free( );
         return FALSE;
      }

      // Store the width and height of the bitmap
      m_width = (int) pBitmapInfo->bmiHeader.biWidth;
      m_height = (int) pBitmapInfo->bmiHeader.biHeight;

      // Get a handle to the bitmap and copy the image bits
      PBYTE pBitmapBits;
      m_hBitmap = CreateDIBSection(dc, pBitmapInfo, DIB_RGB_COLORS,
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

   // Something went wrong, so cleanup everything
   Free( );
   return FALSE;
}

BOOL Bitmap::Create(HDC dc, UINT uiResID, HINSTANCE inst)
{
   // Free any previous DIB info
   Free( );

   // Find the bitmap resource
   HRSRC hResInfo = FindResource(inst, MAKEINTRESOURCE(uiResID), RT_BITMAP);
   if ( hResInfo == NULL )
      return FALSE;

   // Load the bitmap resource
   HGLOBAL hMemBitmap = LoadResource(inst, hResInfo);
   if ( hMemBitmap == NULL )
      return FALSE;

   // Lock the resource and access the entire bitmap image
   PBYTE pBitmapImage = (BYTE*) LockResource(hMemBitmap);
   if ( pBitmapImage == NULL )
   {
      FreeResource(hMemBitmap);
      return FALSE;
   }

   // Store the width and height of the bitmap
   BITMAPINFO* pBitmapInfo = (BITMAPINFO*) pBitmapImage;
   m_width = (int) pBitmapInfo->bmiHeader.biWidth;
   m_height = (int) pBitmapInfo->bmiHeader.biHeight;

   // Get a handle to the bitmap and copy the image bits
   PBYTE pBitmapBits;
   m_hBitmap = CreateDIBSection(dc, pBitmapInfo, DIB_RGB_COLORS,
                                (PVOID*) &pBitmapBits, NULL, 0);
   if ( (m_hBitmap != NULL) && (pBitmapBits != NULL) )
   {
      const PBYTE pTempBits = pBitmapImage + pBitmapInfo->bmiHeader.biSize +
         pBitmapInfo->bmiHeader.biClrUsed * sizeof(RGBQUAD);
      CopyMemory(pBitmapBits, pTempBits, pBitmapInfo->bmiHeader.biSizeImage);

      // Unlock and free the bitmap graphics object
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

BOOL Bitmap::Create(HDC dc, int width, int height, COLORREF crColor)
{
   m_hBitmap = CreateCompatibleBitmap(dc, width, height);

   if ( m_hBitmap == NULL )
   {
      return FALSE;
   }

   m_width = width;
   m_height = height;

   HDC hMemDC = CreateCompatibleDC(dc);
   HBRUSH hBrush = CreateSolidBrush(crColor);
   HBITMAP hOldBitmap = (HBITMAP) SelectObject(hMemDC, m_hBitmap);
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
   if ( m_hBitmap != NULL )
   {
      HDC hMemDC = CreateCompatibleDC(dc);

      HBITMAP hOldBitmap = (HBITMAP) SelectObject(hMemDC, m_hBitmap);

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