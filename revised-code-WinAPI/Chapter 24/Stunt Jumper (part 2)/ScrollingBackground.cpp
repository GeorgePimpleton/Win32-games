#include "ScrollingBackground.hpp"

// create a background layer from a bitmap file
BackgroundLayer::BackgroundLayer(PCWSTR fileName,
                                 int speed, SCROLLDIR direction)
   : Bitmap(fileName), m_speed(speed), m_direction(direction)
{
   // Set the viewport to the entire layer image by default
   m_viewport.left   = m_viewport.top = 0;
   m_viewport.right  = m_width;
   m_viewport.bottom = m_height;
}

// create a background layer from a bitmap resource
BackgroundLayer::BackgroundLayer(UINT resID,
                                 int speed, SCROLLDIR direction)
   : Bitmap(resID), m_speed(speed),
   m_direction(direction)
{
   // set the viewport to the entire layer image by default
   m_viewport.left   = m_viewport.top = 0;
   m_viewport.right  = m_width;
   m_viewport.bottom = m_height;
}

void BackgroundLayer::Draw(HDC dc, int x, int y, BOOL trans,
                           COLORREF transColor)
{
   // draw only the part of the layer seen through the viewport
   if ( m_viewport.top < 0 && m_viewport.left < 0 )
   {
      // draw the split viewport wrapping top to bottom and left to right
      DrawPart(dc, x, y,
               m_width + m_viewport.left, m_height + m_viewport.top,
               -m_viewport.left, -m_viewport.top,
               trans, transColor);
      DrawPart(dc, x - m_viewport.left, y,
               0, m_height + m_viewport.top,
               m_viewport.right, -m_viewport.top,
               trans, transColor);
      DrawPart(dc, x, y - m_viewport.top,
               m_width + m_viewport.left, 0,
               -m_viewport.left, m_viewport.bottom,
               trans, transColor);
      DrawPart(dc, x - m_viewport.left, y - m_viewport.top,
               0, 0,
               m_viewport.right, m_viewport.bottom,
               trans, transColor);
   }
   else if ( m_viewport.top < 0 && m_viewport.right > m_width )
   {
      // draw the split viewport wrapping top to bottom and right to left
      DrawPart(dc, x, y,
               m_viewport.left, m_height + m_viewport.top,
               m_width - m_viewport.left, -m_viewport.top,
               trans, transColor);
      DrawPart(dc, x + (m_width - m_viewport.left), y,
               0, m_height + m_viewport.top,
               m_viewport.right - m_width, -m_viewport.top,
               trans, transColor);
      DrawPart(dc, x, y - m_viewport.top,
               m_viewport.left, 0,
               m_width - m_viewport.left, m_viewport.bottom,
               trans, transColor);
      DrawPart(dc, x + (m_width - m_viewport.left), y - m_viewport.top,
               0, 0,
               m_viewport.right - m_width, m_viewport.bottom,
               trans, transColor);
   }
   else if ( m_viewport.bottom > m_height && m_viewport.left < 0 )
   {
      // draw the split viewport wrapping bottom to top and left to right
      DrawPart(dc, x, y,
               m_width + m_viewport.left, m_viewport.top,
               -m_viewport.left, m_height - m_viewport.top,
               trans, transColor);
      DrawPart(dc, x - m_viewport.left, y,
               0, m_viewport.top,
               m_viewport.right, m_height - m_viewport.top,
               trans, transColor);
      DrawPart(dc, x, y + (m_height - m_viewport.top),
               m_width + m_viewport.left, 0,
               -m_viewport.left, m_viewport.bottom - m_height,
               trans, transColor);
      DrawPart(dc, x - m_viewport.left, y + (m_height - m_viewport.top),
               0, 0,
               m_viewport.right, m_viewport.bottom - m_height,
               trans, transColor);
   }
   else if ( m_viewport.bottom > m_height && m_viewport.right > m_width )
   {
      // draw the split viewport wrapping bottom to top and right to left
      DrawPart(dc, x, y,
               m_viewport.left, m_viewport.top,
               m_width - m_viewport.left, m_height - m_viewport.top,
               trans, transColor);
      DrawPart(dc, x + (m_width - m_viewport.left), y,
               0, m_viewport.top,
               m_viewport.right - m_width, m_height - m_viewport.top,
               trans, transColor);
      DrawPart(dc, x, y + (m_height - m_viewport.top),
               m_viewport.left, 0,
               m_width - m_viewport.left, m_viewport.bottom - m_height,
               trans, transColor);
      DrawPart(dc, x + (m_width - m_viewport.left), y + (m_height - m_viewport.top),
               0, 0,
               m_viewport.right - m_width, m_viewport.bottom - m_height,
               trans, transColor);
   }
   else if ( m_viewport.top < 0 )
   {
      // draw the split viewport wrapping from top to bottom
      DrawPart(dc, x, y,
               m_viewport.left, m_height + m_viewport.top,
               m_viewport.right - m_viewport.left, -m_viewport.top,
               trans, transColor);
      DrawPart(dc, x, y - m_viewport.top,
               m_viewport.left, 0,
               m_viewport.right - m_viewport.left, m_viewport.bottom,
               trans, transColor);
   }
   else if ( m_viewport.right > m_width )
   {
      // draw the split viewport wrapping from right to left
      DrawPart(dc, x, y,
               m_viewport.left, m_viewport.top,
               m_width - m_viewport.left, m_viewport.bottom - m_viewport.top,
               trans, transColor);
      DrawPart(dc, x + (m_width - m_viewport.left), y,
               0, m_viewport.top,
               m_viewport.right - m_width, m_viewport.bottom - m_viewport.top,
               trans, transColor);
   }
   else if ( m_viewport.bottom > m_height )
   {
      // draw the split viewport wrapping from bottom to top
      DrawPart(dc, x, y,
               m_viewport.left, m_viewport.top,
               m_viewport.right - m_viewport.left, m_height - m_viewport.top,
               trans, transColor);
      DrawPart(dc, x, y + (m_height - m_viewport.top),
               m_viewport.left, 0,
               m_viewport.right - m_viewport.left, m_viewport.bottom - m_height,
               trans, transColor);
   }
   else if ( m_viewport.left < 0 )
   {
      // draw the split viewport wrapping from left to right
      DrawPart(dc, x, y,
               m_width + m_viewport.left, m_viewport.top,
               -m_viewport.left, m_viewport.bottom - m_viewport.top,
               trans, transColor);
      DrawPart(dc, x - m_viewport.left, y,
               0, m_viewport.top,
               m_viewport.right, m_viewport.bottom - m_viewport.top,
               trans, transColor);
   }
   else
   {
      // draw the entire viewport at once
      DrawPart(dc, x, y,
               m_viewport.left, m_viewport.top,
               m_viewport.right - m_viewport.left, m_viewport.bottom - m_viewport.top,
               trans, transColor);
   }
}

void BackgroundLayer::Update( )
{
   switch ( m_direction )
   {
   case SD_UP:
      // move the layer up (slide the viewport down)
      m_viewport.top    += m_speed;
      m_viewport.bottom += m_speed;

      if ( m_viewport.top > m_height )
      {
         m_viewport.bottom = m_viewport.bottom - m_viewport.top;
         m_viewport.top    = 0;
      }
      break;

   case SD_RIGHT:
      // move the layer right (slide the viewport left)
      m_viewport.left  -= m_speed;
      m_viewport.right -= m_speed;

      if ( m_viewport.right < 0 )
      {
         m_viewport.left  = m_width - (m_viewport.right - m_viewport.left);
         m_viewport.right = m_width;
      }
      break;

   case SD_DOWN:
      // move the layer down (slide the viewport up)
      m_viewport.top    -= m_speed;
      m_viewport.bottom -= m_speed;

      if ( m_viewport.bottom < 0 )
      {
         m_viewport.top    = m_height - (m_viewport.bottom - m_viewport.top);
         m_viewport.bottom = m_height;
      }
      break;

   case SD_LEFT:
      // move the layer left (slide the viewport right)
      m_viewport.left  += m_speed;
      m_viewport.right += m_speed;

      if ( m_viewport.left > m_width )
      {
         m_viewport.right = (m_viewport.right - m_viewport.left);
         m_viewport.left  = 0;
      }
      break;
   }
}

ScrollingBackground::ScrollingBackground(int width, int height)
   : Background(width, height, 0), m_numLayers(0)
{ }

ScrollingBackground::~ScrollingBackground( )
{ }

void ScrollingBackground::Update( )
{
   // update the layers
   for ( int i = 0; i < m_numLayers; i++ )
   {
      m_layers[ i ]->Update( );
   }
}

void ScrollingBackground::Draw(HDC dc, BOOL trans,
                               COLORREF transColor)
{
   // draw the layers
   for ( int i = 0; i < m_numLayers; i++ )
   {
      m_layers[ i ]->Draw(dc, 0, 0, trans, transColor);
   }
}

void ScrollingBackground::AddLayer(BackgroundLayer* layer)
{
   // add the new layer (maximum of 10)
   if ( m_numLayers < 10 )
   {
      m_layers[ m_numLayers++ ] = layer;
   }
}