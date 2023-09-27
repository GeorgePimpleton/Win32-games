#include "ScrollingBackground.hpp"

// Create a background layer from a bitmap file
BackgroundLayer::BackgroundLayer(HDC dc, PCWSTR fileName,
                                 int iSpeed, SCROLLDIR sdDirection)
   : Bitmap(dc, fileName), m_iSpeed(iSpeed), m_sdDirection(sdDirection)
{
   // Set the viewport to the entire layer image by default
   m_rcViewport.left = m_rcViewport.top = 0;
   m_rcViewport.right = m_width;
   m_rcViewport.bottom = m_height;
}

// Create a background layer from a bitmap resource
BackgroundLayer::BackgroundLayer(HDC dc, UINT resID, HINSTANCE inst,
                                 int iSpeed, SCROLLDIR sdDirection)
   : Bitmap(dc, resID, inst), m_iSpeed(iSpeed),
   m_sdDirection(sdDirection)
{
   // Set the viewport to the entire layer image by default
   m_rcViewport.left = m_rcViewport.top = 0;
   m_rcViewport.right = m_width;
   m_rcViewport.bottom = m_height;
}

void BackgroundLayer::Draw(HDC dc, int x, int y, BOOL trans,
                           COLORREF transColor)
{
   // Draw only the part of the layer seen through the viewport
   if ( m_rcViewport.top < 0 && m_rcViewport.left < 0 )
   {
      // Draw the split viewport wrapping top to bottom AND left to right
      DrawPart(dc, x, y,
               m_width + m_rcViewport.left, m_height + m_rcViewport.top,
               -m_rcViewport.left, -m_rcViewport.top,
               trans, transColor);
      DrawPart(dc, x - m_rcViewport.left, y,
               0, m_height + m_rcViewport.top,
               m_rcViewport.right, -m_rcViewport.top,
               trans, transColor);
      DrawPart(dc, x, y - m_rcViewport.top,
               m_width + m_rcViewport.left, 0,
               -m_rcViewport.left, m_rcViewport.bottom,
               trans, transColor);
      DrawPart(dc, x - m_rcViewport.left, y - m_rcViewport.top,
               0, 0,
               m_rcViewport.right, m_rcViewport.bottom,
               trans, transColor);
   }
   else if ( m_rcViewport.top < 0 && m_rcViewport.right > m_width )
   {
      // Draw the split viewport wrapping top to bottom AND right to left
      DrawPart(dc, x, y,
               m_rcViewport.left, m_height + m_rcViewport.top,
               m_width - m_rcViewport.left, -m_rcViewport.top,
               trans, transColor);
      DrawPart(dc, x + (m_width - m_rcViewport.left), y,
               0, m_height + m_rcViewport.top,
               m_rcViewport.right - m_width, -m_rcViewport.top,
               trans, transColor);
      DrawPart(dc, x, y - m_rcViewport.top,
               m_rcViewport.left, 0,
               m_width - m_rcViewport.left, m_rcViewport.bottom,
               trans, transColor);
      DrawPart(dc, x + (m_width - m_rcViewport.left), y - m_rcViewport.top,
               0, 0,
               m_rcViewport.right - m_width, m_rcViewport.bottom,
               trans, transColor);
   }
   else if ( m_rcViewport.bottom > m_height && m_rcViewport.left < 0 )
   {
      // Draw the split viewport wrapping bottom to top AND left to right
      DrawPart(dc, x, y,
               m_width + m_rcViewport.left, m_rcViewport.top,
               -m_rcViewport.left, m_height - m_rcViewport.top,
               trans, transColor);
      DrawPart(dc, x - m_rcViewport.left, y,
               0, m_rcViewport.top,
               m_rcViewport.right, m_height - m_rcViewport.top,
               trans, transColor);
      DrawPart(dc, x, y + (m_height - m_rcViewport.top),
               m_width + m_rcViewport.left, 0,
               -m_rcViewport.left, m_rcViewport.bottom - m_height,
               trans, transColor);
      DrawPart(dc, x - m_rcViewport.left, y + (m_height - m_rcViewport.top),
               0, 0,
               m_rcViewport.right, m_rcViewport.bottom - m_height,
               trans, transColor);
   }
   else if ( m_rcViewport.bottom > m_height && m_rcViewport.right > m_width )
   {
      // Draw the split viewport wrapping bottom to top AND right to left
      DrawPart(dc, x, y,
               m_rcViewport.left, m_rcViewport.top,
               m_width - m_rcViewport.left, m_height - m_rcViewport.top,
               trans, transColor);
      DrawPart(dc, x + (m_width - m_rcViewport.left), y,
               0, m_rcViewport.top,
               m_rcViewport.right - m_width, m_height - m_rcViewport.top,
               trans, transColor);
      DrawPart(dc, x, y + (m_height - m_rcViewport.top),
               m_rcViewport.left, 0,
               m_width - m_rcViewport.left, m_rcViewport.bottom - m_height,
               trans, transColor);
      DrawPart(dc, x + (m_width - m_rcViewport.left), y + (m_height - m_rcViewport.top),
               0, 0,
               m_rcViewport.right - m_width, m_rcViewport.bottom - m_height,
               trans, transColor);
   }
   else if ( m_rcViewport.top < 0 )
   {
      // Draw the split viewport wrapping from top to bottom
      DrawPart(dc, x, y,
               m_rcViewport.left, m_height + m_rcViewport.top,
               m_rcViewport.right - m_rcViewport.left, -m_rcViewport.top,
               trans, transColor);
      DrawPart(dc, x, y - m_rcViewport.top,
               m_rcViewport.left, 0,
               m_rcViewport.right - m_rcViewport.left, m_rcViewport.bottom,
               trans, transColor);
   }
   else if ( m_rcViewport.right > m_width )
   {
      // Draw the split viewport wrapping from right to left
      DrawPart(dc, x, y,
               m_rcViewport.left, m_rcViewport.top,
               m_width - m_rcViewport.left, m_rcViewport.bottom - m_rcViewport.top,
               trans, transColor);
      DrawPart(dc, x + (m_width - m_rcViewport.left), y,
               0, m_rcViewport.top,
               m_rcViewport.right - m_width, m_rcViewport.bottom - m_rcViewport.top,
               trans, transColor);
   }
   else if ( m_rcViewport.bottom > m_height )
   {
      // Draw the split viewport wrapping from bottom to top
      DrawPart(dc, x, y,
               m_rcViewport.left, m_rcViewport.top,
               m_rcViewport.right - m_rcViewport.left, m_height - m_rcViewport.top,
               trans, transColor);
      DrawPart(dc, x, y + (m_height - m_rcViewport.top),
               m_rcViewport.left, 0,
               m_rcViewport.right - m_rcViewport.left, m_rcViewport.bottom - m_height,
               trans, transColor);
   }
   else if ( m_rcViewport.left < 0 )
   {
      // Draw the split viewport wrapping from left to right
      DrawPart(dc, x, y,
               m_width + m_rcViewport.left, m_rcViewport.top,
               -m_rcViewport.left, m_rcViewport.bottom - m_rcViewport.top,
               trans, transColor);
      DrawPart(dc, x - m_rcViewport.left, y,
               0, m_rcViewport.top,
               m_rcViewport.right, m_rcViewport.bottom - m_rcViewport.top,
               trans, transColor);
   }
   else
      // Draw the entire viewport at once
      DrawPart(dc, x, y,
               m_rcViewport.left, m_rcViewport.top,
               m_rcViewport.right - m_rcViewport.left, m_rcViewport.bottom - m_rcViewport.top,
               trans, transColor);
}

void BackgroundLayer::Update( )
{
   switch ( m_sdDirection )
   {
   case SD_UP:
      // Move the layer up (slide the viewport down)
      m_rcViewport.top += m_iSpeed;
      m_rcViewport.bottom += m_iSpeed;
      if ( m_rcViewport.top > m_height )
      {
         m_rcViewport.bottom = m_rcViewport.bottom - m_rcViewport.top;
         m_rcViewport.top = 0;
      }
      break;

   case SD_RIGHT:
      // Move the layer right (slide the viewport left)
      m_rcViewport.left -= m_iSpeed;
      m_rcViewport.right -= m_iSpeed;
      if ( m_rcViewport.right < 0 )
      {
         m_rcViewport.left = m_width - (m_rcViewport.right - m_rcViewport.left);
         m_rcViewport.right = m_width;
      }
      break;

   case SD_DOWN:
      // Move the layer down (slide the viewport up)
      m_rcViewport.top -= m_iSpeed;
      m_rcViewport.bottom -= m_iSpeed;
      if ( m_rcViewport.bottom < 0 )
      {
         m_rcViewport.top = m_height - (m_rcViewport.bottom - m_rcViewport.top);
         m_rcViewport.bottom = m_height;
      }
      break;

   case SD_LEFT:
      // Move the layer left (slide the viewport right)
      m_rcViewport.left += m_iSpeed;
      m_rcViewport.right += m_iSpeed;
      if ( m_rcViewport.left > m_width )
      {
         m_rcViewport.right = (m_rcViewport.right - m_rcViewport.left);
         m_rcViewport.left = 0;
      }
      break;
   }
}

ScrollingBackground::ScrollingBackground(int width, int height)
   : Background(width, height, 0), m_iNumLayers(0)
{ }

ScrollingBackground::~ScrollingBackground( )
{ }

void ScrollingBackground::Update( )
{
   // Update the layers
   for ( int i = 0; i < m_iNumLayers; i++ )
   {
      m_pLayers[ i ]->Update( );
   }
}

void ScrollingBackground::Draw(HDC dc, BOOL trans,
                               COLORREF transColor)
{
   // Draw the layers
   for ( int i = 0; i < m_iNumLayers; i++ )
   {
      m_pLayers[ i ]->Draw(dc, 0, 0, trans, transColor);
   }
}

void ScrollingBackground::AddLayer(BackgroundLayer* pLayer)
{
   // Add the new layer (maximum of 10)
   if ( m_iNumLayers < 10 )
   {
      m_pLayers[ m_iNumLayers++ ] = pLayer;
   }
}