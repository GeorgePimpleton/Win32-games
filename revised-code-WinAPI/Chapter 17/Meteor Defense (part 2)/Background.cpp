#include "Background.hpp"

Background::Background( LONG width, LONG height, COLORREF color )
{
   m_bitmap = NULL;
   m_color  = color;
   m_width  = width;
   m_height = height;
}

Background::Background( Bitmap* bitmap )
{
   m_bitmap = bitmap;
   m_color  = 0;
   m_width  = bitmap->GetWidth( );
   m_height = bitmap->GetHeight( );
}

Background::~Background( )
{ }

void Background::Update( )
{
}

void Background::Draw( HDC dc )
{
   if ( NULL != m_bitmap )
   {
      m_bitmap->Draw( dc, 0, 0 );
   }
   else
   {
      RECT   rect  = { 0, 0, m_width, m_height };
      HBRUSH brush = CreateSolidBrush( m_color );

      FillRect( dc, &rect, brush );

      DeleteObject( brush );
   }
}

StarryBackground::StarryBackground( LONG width, LONG height,
                                    LONG numStars, LONG twinkleDelay )
   : Background( width, height, 0 )
{
   m_numStars     = min( numStars, 100 );
   m_twinkleDelay = twinkleDelay;

   for ( LONG i = 0; i < m_numStars; i++ )
   {
      m_stars[i].x    = rtk::rand( 0, width );
      m_stars[i].y    = rtk::rand( 0, height );
      m_starColors[i] = RGB( 128, 128, 128 );

      m_sizeStars[i] = SS_SMALL;

      if ( rtk::rand( 0, 100 ) < 20 )
      {
         m_sizeStars[i] = SS_MEDIUM;
      }
      else if ( rtk::rand( 0, 100 ) < 5 )
      {
         m_sizeStars[i] = SS_LARGE;
      }
   }
}

StarryBackground::~StarryBackground( )
{ }

void StarryBackground::Update( )
{
   for ( LONG i = 0; i < m_numStars; i++ )
   {
      if ( 0 == ( rtk::rand( 0, m_twinkleDelay ) ) )
      {
         LONG iRGB = rtk::rand( 0, 255 );

         m_starColors[i] = RGB( iRGB, iRGB, iRGB );

         m_sizeStars[i] = SS_SMALL;

         if ( rtk::rand( 0, 100 ) < 20 )
         {
            m_sizeStars[i] = SS_MEDIUM;
         }
         else if ( rtk::rand( 0, 100 ) < 5 )
         {
            m_sizeStars[i] = SS_LARGE;
         }
      }
   }
}

void StarryBackground::Draw( HDC dc )
{
   RECT   rect  = { 0, 0, m_width, m_height };
   HBRUSH brush = CreateSolidBrush( RGB( 0, 0, 0 ) );

   FillRect( dc, &rect, brush );
   DeleteObject( brush );

   for ( LONG i = 0; i < m_numStars; i++ )
   {
      SetPixel( dc, m_stars[i].x, m_stars[i].y, m_starColors[i] );

      if ( m_sizeStars[i] == SS_MEDIUM )
      {
         for ( LONG x = -1; x < 2; x++ )
         {
            for ( LONG y = -1; y < 2; y++ )
            {
               SetPixel( dc, m_stars[i].x + x, m_stars[i].y + y, m_starColors[i] );
            }
         }
      }
      else if ( m_sizeStars[i] == SS_LARGE )
      {
         for ( LONG x = -2; x < 3; x++ )
         {
            for ( LONG y = -2; y < 3; y++ )
            {
               if ( ( abs( x ) == 2 ) && ( abs( y ) == 2 ) )
               {
                  break;
               }

               SetPixel( dc, m_stars[i].x + x, m_stars[i].y + y, m_starColors[i] );
            }
         }
      }
   }
}