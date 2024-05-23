#include "ScrollingBackground.hpp"

BackgroundLayer::BackgroundLayer( PCWSTR fileName,
                                  int speed, SCROLLDIR scrollDirection )
   : Bitmap( fileName ), m_speed( speed ), m_scrollDirection( scrollDirection )
{
   m_viewport.left   = m_viewport.top = 0;
   m_viewport.right  = m_width;
   m_viewport.bottom = m_height;
}

BackgroundLayer::BackgroundLayer( UINT resID,
                                  int speed, SCROLLDIR scrollDirection )
   : Bitmap( resID ), m_speed( speed ), m_scrollDirection( scrollDirection )
{
   m_viewport.left   = m_viewport.top = 0;
   m_viewport.right  = m_width;
   m_viewport.bottom = m_height;
}

void BackgroundLayer::Draw( HDC dc, int x, int y, BOOL trans,
                            COLORREF transColor ) const
{
   if ( m_viewport.top < 0 && m_viewport.left < 0 )
   {
      DrawPart( dc, x, y,
                m_width + m_viewport.left, m_height + m_viewport.top,
                -m_viewport.left, -m_viewport.top,
                trans, transColor );
      DrawPart( dc, x - m_viewport.left, y,
                0, m_height + m_viewport.top,
                m_viewport.right, -m_viewport.top,
                trans, transColor );
      DrawPart( dc, x, y - m_viewport.top,
                m_width + m_viewport.left, 0,
                -m_viewport.left, m_viewport.bottom,
                trans, transColor );
      DrawPart( dc, x - m_viewport.left, y - m_viewport.top,
                0, 0,
                m_viewport.right, m_viewport.bottom,
                trans, transColor );
   }
   else if ( m_viewport.top < 0 && m_viewport.right > m_width )
   {
      DrawPart( dc, x, y,
                m_viewport.left, m_height + m_viewport.top,
                m_width - m_viewport.left, -m_viewport.top,
                trans, transColor );
      DrawPart( dc, x + ( m_width - m_viewport.left ), y,
                0, m_height + m_viewport.top,
                m_viewport.right - m_width, -m_viewport.top,
                trans, transColor );
      DrawPart( dc, x, y - m_viewport.top,
                m_viewport.left, 0,
                m_width - m_viewport.left, m_viewport.bottom,
                trans, transColor );
      DrawPart( dc, x + ( m_width - m_viewport.left ), y - m_viewport.top,
                0, 0,
                m_viewport.right - m_width, m_viewport.bottom,
                trans, transColor );
   }
   else if ( m_viewport.bottom > m_height && m_viewport.left < 0 )
   {
      DrawPart( dc, x, y,
                m_width + m_viewport.left, m_viewport.top,
                -m_viewport.left, m_height - m_viewport.top,
                trans, transColor );
      DrawPart( dc, x - m_viewport.left, y,
                0, m_viewport.top,
                m_viewport.right, m_height - m_viewport.top,
                trans, transColor );
      DrawPart( dc, x, y + ( m_height - m_viewport.top ),
                m_width + m_viewport.left, 0,
                -m_viewport.left, m_viewport.bottom - m_height,
                trans, transColor );
      DrawPart( dc, x - m_viewport.left, y + ( m_height - m_viewport.top ),
                0, 0,
                m_viewport.right, m_viewport.bottom - m_height,
                trans, transColor );
   }
   else if ( m_viewport.bottom > m_height && m_viewport.right > m_width )
   {
      DrawPart( dc, x, y,
                m_viewport.left, m_viewport.top,
                m_width - m_viewport.left, m_height - m_viewport.top,
                trans, transColor );
      DrawPart( dc, x + ( m_width - m_viewport.left ), y,
                0, m_viewport.top,
                m_viewport.right - m_width, m_height - m_viewport.top,
                trans, transColor );
      DrawPart( dc, x, y + ( m_height - m_viewport.top ),
                m_viewport.left, 0,
                m_width - m_viewport.left, m_viewport.bottom - m_height,
                trans, transColor );
      DrawPart( dc, x + ( m_width - m_viewport.left ), y + ( m_height - m_viewport.top ),
                0, 0,
                m_viewport.right - m_width, m_viewport.bottom - m_height,
                trans, transColor );
   }
   else if ( m_viewport.top < 0 )
   {
      DrawPart( dc, x, y,
                m_viewport.left, m_height + m_viewport.top,
                m_viewport.right - m_viewport.left, -m_viewport.top,
                trans, transColor );
      DrawPart( dc, x, y - m_viewport.top,
                m_viewport.left, 0,
                m_viewport.right - m_viewport.left, m_viewport.bottom,
                trans, transColor );
   }
   else if ( m_viewport.right > m_width )
   {
      DrawPart( dc, x, y,
                m_viewport.left, m_viewport.top,
                m_width - m_viewport.left, m_viewport.bottom - m_viewport.top,
                trans, transColor );
      DrawPart( dc, x + ( m_width - m_viewport.left ), y,
                0, m_viewport.top,
                m_viewport.right - m_width, m_viewport.bottom - m_viewport.top,
                trans, transColor );
   }
   else if ( m_viewport.bottom > m_height )
   {
      DrawPart( dc, x, y,
                m_viewport.left, m_viewport.top,
                m_viewport.right - m_viewport.left, m_height - m_viewport.top,
                trans, transColor );
      DrawPart( dc, x, y + ( m_height - m_viewport.top ),
                m_viewport.left, 0,
                m_viewport.right - m_viewport.left, m_viewport.bottom - m_height,
                trans, transColor );
   }
   else if ( m_viewport.left < 0 )
   {
      DrawPart( dc, x, y,
                m_width + m_viewport.left, m_viewport.top,
                -m_viewport.left, m_viewport.bottom - m_viewport.top,
                trans, transColor );
      DrawPart( dc, x - m_viewport.left, y,
                0, m_viewport.top,
                m_viewport.right, m_viewport.bottom - m_viewport.top,
                trans, transColor );
   }
   else
      DrawPart( dc, x, y,
                m_viewport.left, m_viewport.top,
                m_viewport.right - m_viewport.left, m_viewport.bottom - m_viewport.top,
                trans, transColor );
}

void BackgroundLayer::Update( )
{
   switch ( m_scrollDirection )
   {
   case SD_UP:
      m_viewport.top    += m_speed;
      m_viewport.bottom += m_speed;

      if ( m_viewport.top > m_height )
      {
         m_viewport.bottom = m_viewport.bottom - m_viewport.top;
         m_viewport.top    = 0;
      }
      break;

   case SD_RIGHT:
      m_viewport.left  -= m_speed;
      m_viewport.right -= m_speed;

      if ( m_viewport.right < 0 )
      {
         m_viewport.left  = m_width - ( m_viewport.right - m_viewport.left );
         m_viewport.right = m_width;
      }
      break;

   case SD_DOWN:
      m_viewport.top    -= m_speed;
      m_viewport.bottom -= m_speed;

      if ( m_viewport.bottom < 0 )
      {
         m_viewport.top    = m_height - ( m_viewport.bottom - m_viewport.top );
         m_viewport.bottom = m_height;
      }
      break;

   case SD_LEFT:
      m_viewport.left  += m_speed;
      m_viewport.right += m_speed;

      if ( m_viewport.left > m_width )
      {
         m_viewport.right = ( m_viewport.right - m_viewport.left );
         m_viewport.left  = 0;
      }
      break;
   }
}

ScrollingBackground::ScrollingBackground( int width, int height )
   : Background( width, height, 0 ), m_numLayers( 0 )
{ }

ScrollingBackground::~ScrollingBackground( )
{ }

void ScrollingBackground::Update( )
{
   for ( int i = 0; i < m_numLayers; i++ )
   {
      m_layers[i]->Update( );
   }
}

void ScrollingBackground::Draw( HDC dc, BOOL trans,
                                COLORREF transColor ) const
{
   for ( int i = 0; i < m_numLayers; i++ )
   {
      m_layers[i]->Draw( dc, 0, 0, trans, transColor );
   }
}

void ScrollingBackground::AddLayer( BackgroundLayer* layer )
{
   if ( m_numLayers < 10 )
   {
      m_layers[m_numLayers++] = layer;
   }
}
