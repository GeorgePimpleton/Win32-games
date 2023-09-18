#pragma comment(lib, "msimg32.lib")

#include "Bitmap.hpp"

Bitmap::Bitmap( )
   : m_bitmap(NULL), m_width(0), m_height(0)
{ }

Bitmap::Bitmap(HDC dc, PCWSTR fileName)
   : m_bitmap(NULL), m_width(0), m_height(0)
{
   Create(dc, fileName);
}

Bitmap::Bitmap(HDC dc, UINT resID, HINSTANCE inst)
   : m_bitmap(NULL), m_width(0), m_height(0)
{
   Create(dc, resID, inst);
}

Bitmap::Bitmap(HDC dc, int width, int height, COLORREF color)
   : m_bitmap(NULL), m_width(0), m_height(0)
{
   Create(dc, width, height, color);
}

Bitmap::~Bitmap( )
{
   Free( );
}

void Bitmap::Free( )
{
   if ( m_bitmap != NULL )
   {
      DeleteObject(m_bitmap);
      m_bitmap = NULL;
   }
}

BOOL Bitmap::Create(HDC dc, PCWSTR fileName)
{
   // Free any previous bitmap info
   Free( );

   // Open the bitmap file
   HANDLE hFile = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ, NULL,
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

   BITMAPINFO* bitmapInfo = (new BITMAPINFO);
   if ( bitmapInfo != NULL )
   {
      // Read the bitmap info header
      bOK = ReadFile(hFile, bitmapInfo, sizeof(BITMAPINFOHEADER),
                     &dwBytesRead, NULL);
      if ( (!bOK) || (dwBytesRead != sizeof(BITMAPINFOHEADER)) )
      {
         CloseHandle(hFile);
         Free( );
         return FALSE;
      }

      // Store the width and height of the bitmap
      m_width = (int) bitmapInfo->bmiHeader.biWidth;
      m_height = (int) bitmapInfo->bmiHeader.biHeight;

      // Get a handle to the bitmap and copy the image bits
      PBYTE bitmapBits;
      m_bitmap = CreateDIBSection(dc, bitmapInfo, DIB_RGB_COLORS,
                                   (PVOID*) &bitmapBits, NULL, 0);
      if ( (m_bitmap != NULL) && (bitmapBits != NULL) )
      {
         SetFilePointer(hFile, bmfHeader.bfOffBits, NULL, FILE_BEGIN);
         bOK = ReadFile(hFile, bitmapBits, bitmapInfo->bmiHeader.biSizeImage,
                        &dwBytesRead, NULL);
         if ( bOK )
            return TRUE;
      }
   }

   // Something went wrong, so cleanup everything
   Free( );
   return FALSE;
}

BOOL Bitmap::Create(HDC dc, UINT resID, HINSTANCE inst)
{
   Free( );

   // Find the bitmap resource
   HRSRC hResInfo = FindResource(inst, MAKEINTRESOURCE(resID), RT_BITMAP);
   if ( hResInfo == NULL )
      return FALSE;

   // Load the bitmap resource
   HGLOBAL hMemBitmap = LoadResource(inst, hResInfo);
   if ( hMemBitmap == NULL )
      return FALSE;

   // Lock the resource and access the entire bitmap image
   PBYTE bitmapImage = (BYTE*) LockResource(hMemBitmap);
   if ( bitmapImage == NULL )
   {
      FreeResource(hMemBitmap);
      return FALSE;
   }

   // Store the width and height of the bitmap
   BITMAPINFO* bitmapInfo = (BITMAPINFO*) bitmapImage;
   m_width = (int) bitmapInfo->bmiHeader.biWidth;
   m_height = (int) bitmapInfo->bmiHeader.biHeight;

   // Get a handle to the bitmap and copy the image bits
   PBYTE bitmapBits;
   m_bitmap = CreateDIBSection(dc, bitmapInfo, DIB_RGB_COLORS,
                                (PVOID*) &bitmapBits, NULL, 0);
   if ( (m_bitmap != NULL) && (bitmapBits != NULL) )
   {
      const PBYTE pTempBits = bitmapImage + bitmapInfo->bmiHeader.biSize +
         bitmapInfo->bmiHeader.biClrUsed * sizeof(RGBQUAD);
      CopyMemory(bitmapBits, pTempBits, bitmapInfo->bmiHeader.biSizeImage);

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

BOOL Bitmap::Create(HDC dc, int width, int height, COLORREF color)
{
   m_bitmap = CreateCompatibleBitmap(dc, width, height);

   if ( m_bitmap == NULL )
   {
      return FALSE;
   }

   m_width = width;
   m_height = height;

   HDC hMemDC = CreateCompatibleDC(dc);
   HBRUSH hBrush = CreateSolidBrush(color);

   HBITMAP hOldBitmap = (HBITMAP) SelectObject(hMemDC, m_bitmap);

   RECT rcBitmap = { 0, 0, m_width, m_height };
   FillRect(hMemDC, &rcBitmap, hBrush);

   SelectObject(hMemDC, hOldBitmap);
   DeleteDC(hMemDC);
   DeleteObject(hBrush);

   return TRUE;
}

void Bitmap::Draw(HDC dc, int x, int y, BOOL trans, COLORREF transColor)
{
   DrawPart(dc, x, y, 0, 0, GetWidth( ), GetHeight( ), trans, transColor);
}

void Bitmap::DrawPart(HDC dc, int x, int y, int xPart, int yPart,
                      int wPart, int hPart, BOOL trans, COLORREF transColor)
{
   if ( m_bitmap != NULL )
   {
      HDC hMemDC = CreateCompatibleDC(dc);

      HBITMAP hOldBitmap = (HBITMAP) SelectObject(hMemDC, m_bitmap);

      if ( trans )
      {
         TransparentBlt(dc, x, y, wPart, hPart, hMemDC, xPart, yPart,
                        wPart, hPart, transColor);
      }
      else
      {
         BitBlt(dc, x, y, wPart, hPart, hMemDC, xPart, yPart, SRCCOPY);
      }

      SelectObject(hMemDC, hOldBitmap);
      DeleteDC(hMemDC);
   }
}