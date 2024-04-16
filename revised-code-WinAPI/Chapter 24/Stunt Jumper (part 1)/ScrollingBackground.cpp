#include "ScrollingBackground.h"

BackgroundLayer::BackgroundLayer( HDC hDC, PCTSTR szFileName,
                                  int iSpeed, SCROLLDIR sdDirection )
   : Bitmap( hDC, szFileName ), m_iSpeed( iSpeed ), m_sdDirection( sdDirection )
{
   m_rcViewport.left   = m_rcViewport.top = 0;
   m_rcViewport.right  = m_iWidth;
   m_rcViewport.bottom = m_iHeight;
}

BackgroundLayer::BackgroundLayer( HDC hDC, UINT uiResID, HINSTANCE hInstance,
                                  int iSpeed, SCROLLDIR sdDirection )
   : Bitmap( hDC, uiResID, hInstance ), m_iSpeed( iSpeed ),
   m_sdDirection( sdDirection )
{
   m_rcViewport.left   = m_rcViewport.top = 0;
   m_rcViewport.right  = m_iWidth;
   m_rcViewport.bottom = m_iHeight;
}

void BackgroundLayer::Draw( HDC hDC, int x, int y, BOOL bTrans,
                            COLORREF crTransColor )
{
   if ( m_rcViewport.top < 0 && m_rcViewport.left < 0 )
   {
      DrawPart( hDC, x, y,
                m_iWidth + m_rcViewport.left, m_iHeight + m_rcViewport.top,
                -m_rcViewport.left, -m_rcViewport.top,
                bTrans, crTransColor );
      DrawPart( hDC, x - m_rcViewport.left, y,
                0, m_iHeight + m_rcViewport.top,
                m_rcViewport.right, -m_rcViewport.top,
                bTrans, crTransColor );
      DrawPart( hDC, x, y - m_rcViewport.top,
                m_iWidth + m_rcViewport.left, 0,
                -m_rcViewport.left, m_rcViewport.bottom,
                bTrans, crTransColor );
      DrawPart( hDC, x - m_rcViewport.left, y - m_rcViewport.top,
                0, 0,
                m_rcViewport.right, m_rcViewport.bottom,
                bTrans, crTransColor );
   }
   else if ( m_rcViewport.top < 0 && m_rcViewport.right > m_iWidth )
   {
      DrawPart( hDC, x, y,
                m_rcViewport.left, m_iHeight + m_rcViewport.top,
                m_iWidth - m_rcViewport.left, -m_rcViewport.top,
                bTrans, crTransColor );
      DrawPart( hDC, x + ( m_iWidth - m_rcViewport.left ), y,
                0, m_iHeight + m_rcViewport.top,
                m_rcViewport.right - m_iWidth, -m_rcViewport.top,
                bTrans, crTransColor );
      DrawPart( hDC, x, y - m_rcViewport.top,
                m_rcViewport.left, 0,
                m_iWidth - m_rcViewport.left, m_rcViewport.bottom,
                bTrans, crTransColor );
      DrawPart( hDC, x + ( m_iWidth - m_rcViewport.left ), y - m_rcViewport.top,
                0, 0,
                m_rcViewport.right - m_iWidth, m_rcViewport.bottom,
                bTrans, crTransColor );
   }
   else if ( m_rcViewport.bottom > m_iHeight && m_rcViewport.left < 0 )
   {
      DrawPart( hDC, x, y,
                m_iWidth + m_rcViewport.left, m_rcViewport.top,
                -m_rcViewport.left, m_iHeight - m_rcViewport.top,
                bTrans, crTransColor );
      DrawPart( hDC, x - m_rcViewport.left, y,
                0, m_rcViewport.top,
                m_rcViewport.right, m_iHeight - m_rcViewport.top,
                bTrans, crTransColor );
      DrawPart( hDC, x, y + ( m_iHeight - m_rcViewport.top ),
                m_iWidth + m_rcViewport.left, 0,
                -m_rcViewport.left, m_rcViewport.bottom - m_iHeight,
                bTrans, crTransColor );
      DrawPart( hDC, x - m_rcViewport.left, y + ( m_iHeight - m_rcViewport.top ),
                0, 0,
                m_rcViewport.right, m_rcViewport.bottom - m_iHeight,
                bTrans, crTransColor );
   }
   else if ( m_rcViewport.bottom > m_iHeight && m_rcViewport.right > m_iWidth )
   {
      DrawPart( hDC, x, y,
                m_rcViewport.left, m_rcViewport.top,
                m_iWidth - m_rcViewport.left, m_iHeight - m_rcViewport.top,
                bTrans, crTransColor );
      DrawPart( hDC, x + ( m_iWidth - m_rcViewport.left ), y,
                0, m_rcViewport.top,
                m_rcViewport.right - m_iWidth, m_iHeight - m_rcViewport.top,
                bTrans, crTransColor );
      DrawPart( hDC, x, y + ( m_iHeight - m_rcViewport.top ),
                m_rcViewport.left, 0,
                m_iWidth - m_rcViewport.left, m_rcViewport.bottom - m_iHeight,
                bTrans, crTransColor );
      DrawPart( hDC, x + ( m_iWidth - m_rcViewport.left ), y + ( m_iHeight - m_rcViewport.top ),
                0, 0,
                m_rcViewport.right - m_iWidth, m_rcViewport.bottom - m_iHeight,
                bTrans, crTransColor );
   }
   else if ( m_rcViewport.top < 0 )
   {
      DrawPart( hDC, x, y,
                m_rcViewport.left, m_iHeight + m_rcViewport.top,
                m_rcViewport.right - m_rcViewport.left, -m_rcViewport.top,
                bTrans, crTransColor );
      DrawPart( hDC, x, y - m_rcViewport.top,
                m_rcViewport.left, 0,
                m_rcViewport.right - m_rcViewport.left, m_rcViewport.bottom,
                bTrans, crTransColor );
   }
   else if ( m_rcViewport.right > m_iWidth )
   {
      DrawPart( hDC, x, y,
                m_rcViewport.left, m_rcViewport.top,
                m_iWidth - m_rcViewport.left, m_rcViewport.bottom - m_rcViewport.top,
                bTrans, crTransColor );
      DrawPart( hDC, x + ( m_iWidth - m_rcViewport.left ), y,
                0, m_rcViewport.top,
                m_rcViewport.right - m_iWidth, m_rcViewport.bottom - m_rcViewport.top,
                bTrans, crTransColor );
   }
   else if ( m_rcViewport.bottom > m_iHeight )
   {
      DrawPart( hDC, x, y,
                m_rcViewport.left, m_rcViewport.top,
                m_rcViewport.right - m_rcViewport.left, m_iHeight - m_rcViewport.top,
                bTrans, crTransColor );
      DrawPart( hDC, x, y + ( m_iHeight - m_rcViewport.top ),
                m_rcViewport.left, 0,
                m_rcViewport.right - m_rcViewport.left, m_rcViewport.bottom - m_iHeight,
                bTrans, crTransColor );
   }
   else if ( m_rcViewport.left < 0 )
   {
      DrawPart( hDC, x, y,
                m_iWidth + m_rcViewport.left, m_rcViewport.top,
                -m_rcViewport.left, m_rcViewport.bottom - m_rcViewport.top,
                bTrans, crTransColor );
      DrawPart( hDC, x - m_rcViewport.left, y,
                0, m_rcViewport.top,
                m_rcViewport.right, m_rcViewport.bottom - m_rcViewport.top,
                bTrans, crTransColor );
   }
   else
      DrawPart( hDC, x, y,
                m_rcViewport.left, m_rcViewport.top,
                m_rcViewport.right - m_rcViewport.left, m_rcViewport.bottom - m_rcViewport.top,
                bTrans, crTransColor );
}

void BackgroundLayer::Update( )
{
   switch ( m_sdDirection )
   {
   case SD_UP:
      m_rcViewport.top += m_iSpeed;
      m_rcViewport.bottom += m_iSpeed;
      if ( m_rcViewport.top > m_iHeight )
      {
         m_rcViewport.bottom = m_rcViewport.bottom - m_rcViewport.top;
         m_rcViewport.top = 0;
      }
      break;

   case SD_RIGHT:
      m_rcViewport.left -= m_iSpeed;
      m_rcViewport.right -= m_iSpeed;
      if ( m_rcViewport.right < 0 )
      {
         m_rcViewport.left = m_iWidth - ( m_rcViewport.right - m_rcViewport.left );
         m_rcViewport.right = m_iWidth;
      }
      break;

   case SD_DOWN:
      m_rcViewport.top -= m_iSpeed;
      m_rcViewport.bottom -= m_iSpeed;
      if ( m_rcViewport.bottom < 0 )
      {
         m_rcViewport.top = m_iHeight - ( m_rcViewport.bottom - m_rcViewport.top );
         m_rcViewport.bottom = m_iHeight;
      }
      break;

   case SD_LEFT:
      m_rcViewport.left += m_iSpeed;
      m_rcViewport.right += m_iSpeed;
      if ( m_rcViewport.left > m_iWidth )
      {
         m_rcViewport.right = ( m_rcViewport.right - m_rcViewport.left );
         m_rcViewport.left = 0;
      }
      break;
   }
}

ScrollingBackground::ScrollingBackground( int iWidth, int iHeight )
   : Background( iWidth, iHeight, 0 ), m_iNumLayers( 0 )
{ }

ScrollingBackground::~ScrollingBackground( )
{ }

void ScrollingBackground::Update( )
{
   for ( int i = 0; i < m_iNumLayers; i++ )
   {
      m_pLayers[i]->Update( );
   }
}

void ScrollingBackground::Draw( HDC hDC, BOOL bTrans,
                                COLORREF crTransColor )
{
   for ( int i = 0; i < m_iNumLayers; i++ )
   {
      m_pLayers[i]->Draw( hDC, 0, 0, bTrans, crTransColor );
   }
}

void ScrollingBackground::AddLayer( BackgroundLayer* pLayer )
{
   if ( m_iNumLayers < 10 )
   {
      m_pLayers[m_iNumLayers++] = pLayer;
   }
}
